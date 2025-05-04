#include "PixelpartEffectRuntime.h"
#include "PixelpartSystem.h"
#include "node/PixelpartNodeFactory.h"
#include "node/PixelpartParticleEmitter.h"
#include "util/PixelpartUtil.h"
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <pixelpart-runtime/common/Math.h>
#include <pixelpart-runtime/common/Id.h>
#include <pixelpart-runtime/engine/SingleThreadedEffectEngine.h>
#include <pixelpart-runtime/engine/MultiThreadedEffectEngine.h>
#include <algorithm>

namespace godot {
PixelpartEffectRuntime::PixelpartEffectRuntime() {
	ProjectSettings* settings = ProjectSettings::get_singleton();

	if(settings->has_setting("pixelpart/particle_capacity")) {
		particleCapacity = static_cast<uint32_t>(
			std::max(static_cast<int>(settings->get_setting("pixelpart/particle_capacity")), 1));
	}
}

void PixelpartEffectRuntime::set_effect(const pixelpart::Effect& eff) {
	reset_effect();

	effect = eff;
	effect.applyInputs();

#ifdef PIXELPART_RUNTIME_MULTITHREADING
	effectEngine = std::unique_ptr<pixelpart::MultiThreadedEffectEngine>(
		new pixelpart::MultiThreadedEffectEngine(effect, particleCapacity, PixelpartSystem::get_instance()->get_thread_pool()));
#else
	effectEngine = std::unique_ptr<pixelpart::SingleThreadedEffectEngine>(
		new pixelpart::SingleThreadedEffectEngine(effect, particleCapacity));
#endif

	for(const auto& node : effect.sceneGraph()) {
		nodeRefs[node->name()] = create_pixelpart_node(node.get(), effectEngine.get());
	}

	for(auto& particleType : effect.particleTypes()) {
		Ref<PixelpartParticleType> particleTypeRef;
		particleTypeRef.instantiate();
		particleTypeRef->init(&particleType, effectEngine.get());

		particleTypeRefs[particleType.name()] = particleTypeRef;
	}

	for(const auto& inputEntry : effect.inputs()) {
		StringName inputName = StringName(inputEntry.second.name().c_str());
		inputValues[inputName] = pxpt_to_gd(inputEntry.second.value());
	}
}
void PixelpartEffectRuntime::reset_effect() {
	effectEngine = nullptr;
	simulationTime = 0.0f;

	nodeRefs.clear();
	particleTypeRefs.clear();
	inputValues.clear();
}
const pixelpart::Effect& PixelpartEffectRuntime::get_effect() const {
	return effect;
}
const pixelpart::EffectEngine* PixelpartEffectRuntime::get_effect_engine() const {
	return effectEngine.get();
}

void PixelpartEffectRuntime::advance(double dt) {
	if(!effectEngine || !playing) {
		return;
	}

	dt = static_cast<float>(glm::clamp(dt, 0.0, 1.0));
	simulationTime += dt * speed;

	while(simulationTime > timeStep) {
		simulationTime -= timeStep;
		effectEngine->advance(timeStep);

		if(loop && effectEngine->runtimeContext().time() > loopTime) {
			effectEngine->restart(false);
		}
	}
}

void PixelpartEffectRuntime::play(bool mode) {
	playing = mode;
}
void PixelpartEffectRuntime::restart(bool reset) {
	if(!effectEngine) {
		return;
	}

	effectEngine->restart(reset);
}
bool PixelpartEffectRuntime::is_playing() const {
	return playing;
}
float PixelpartEffectRuntime::get_time() const {
	return effectEngine ? static_cast<float>(effectEngine->runtimeContext().time()) : 0.0f;
}

void PixelpartEffectRuntime::set_loop(bool mode) {
	loop = mode;
}
bool PixelpartEffectRuntime::get_loop() const {
	return loop;
}

void PixelpartEffectRuntime::set_loop_time(float time) {
	loopTime = std::max(time, 0.0f);
}
float PixelpartEffectRuntime::get_loop_time() const {
	return loopTime;
}

void PixelpartEffectRuntime::set_speed(float sp) {
	speed = std::max(sp, 0.0f);
}
float PixelpartEffectRuntime::get_speed() const {
	return speed;
}

void PixelpartEffectRuntime::set_frame_rate(float rate) {
	timeStep = 1.0f / glm::clamp(rate, 1.0f, 100.0f);
}
float PixelpartEffectRuntime::get_frame_rate() const {
	return 1.0f / timeStep;
}

void PixelpartEffectRuntime::set_inputs(Dictionary inputs) {
	inputValues = inputs;
	apply_inputs();
}
void PixelpartEffectRuntime::apply_inputs() {
	for(auto& inputEntry : effect.inputs()) {
		StringName inputName = StringName(inputEntry.second.name().c_str());
		if(!inputValues.has(inputName)) {
			continue;
		}

		inputEntry.second.value(gd_to_pxpt(inputValues.get(inputName, Variant())));
	}

	effect.applyInputs();
}
Dictionary PixelpartEffectRuntime::get_inputs() const {
	return inputValues;
}

void PixelpartEffectRuntime::set_input(String name, const pixelpart::VariantValue& value) {
	std::string inputName = std::string(name.utf8().get_data());
	pixelpart::EffectInputCollection::iterator inputIt = std::find_if(
		effect.inputs().begin(), effect.inputs().end(),
		[&inputName](const std::pair<pixelpart::id_t, pixelpart::EffectInput>& entry) {
			return entry.second.name() == inputName;
		});

	if(inputIt == effect.inputs().end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return;
	}

	inputIt->second.value(value);
	inputValues[name] = pxpt_to_gd(value);

	effect.applyInputs();
}
pixelpart::VariantValue PixelpartEffectRuntime::get_input(String name) const {
	std::string inputName = std::string(name.utf8().get_data());
	pixelpart::EffectInputCollection::const_iterator inputIt = std::find_if(
		effect.inputs().begin(), effect.inputs().end(),
		[&inputName](const std::pair<pixelpart::id_t, pixelpart::EffectInput>& entry) {
			return entry.second.name() == inputName;
		});

	if(inputIt == effect.inputs().end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return pixelpart::VariantValue();
	}

	return inputIt->second.value();
}
TypedArray<String> PixelpartEffectRuntime::get_input_names() const {
	TypedArray<String> names;
	for(const auto& entry : effect.inputs()) {
		names.append(String(entry.second.name().c_str()));
	}

	return names;
}

void PixelpartEffectRuntime::activate_trigger(String name) {
	if(!effectEngine) {
		return;
	}

	CharString nameCharString = name.utf8();
	std::string triggerName = std::string(nameCharString.get_data(), nameCharString.length());

	auto triggerIt = std::find_if(effect.triggers().begin(), effect.triggers().end(),
		[&triggerName](const std::pair<pixelpart::id_t, pixelpart::Trigger>& entry) {
			return entry.second.name() == triggerName;
		});

	if(triggerIt == effect.triggers().end()) {
		UtilityFunctions::push_warning("Unknown trigger \"", name, "\"");
		return;
	}

	effectEngine->activateTrigger(triggerIt->first);
}
bool PixelpartEffectRuntime::is_trigger_activated(String name) const {
	CharString nameCharString = name.utf8();
	std::string triggerName = std::string(nameCharString.get_data(), nameCharString.length());

	auto triggerIt = std::find_if(effect.triggers().begin(), effect.triggers().end(),
		[&triggerName](const std::pair<pixelpart::id_t, pixelpart::Trigger>& entry) {
			return entry.second.name() == triggerName;
		});

	if(triggerIt == effect.triggers().end()) {
		UtilityFunctions::push_warning("Unknown trigger \"", name, "\"");
		return false;
	}

	return effectEngine->runtimeContext().triggerActivated(triggerIt->first);
}

void PixelpartEffectRuntime::spawn_particles(String particleEmitterName, String particleTypeName, int count) {
	if(!effectEngine || count <= 0) {
		return;
	}

	Ref<PixelpartParticleEmitter> particleEmitter = find_node(particleEmitterName);
	Ref<PixelpartParticleType> particleType = find_particle_type(particleTypeName);

	if(!particleEmitter.is_valid()) {
		UtilityFunctions::push_warning("Unknown particle emitter \"", particleEmitterName, "\"");
		return;
	}
	else if(!particleType.is_valid()) {
		UtilityFunctions::push_warning("Unknown particle type \"", particleTypeName, "\"");
		return;
	}

	effectEngine->spawnParticles(
		pixelpart::id_t(static_cast<uint32_t>(particleEmitter->get_id())),
		pixelpart::id_t(static_cast<uint32_t>(particleType->get_id())),
		static_cast<uint32_t>(count));
}

Ref<PixelpartNode> PixelpartEffectRuntime::find_node(String name) const {
	CharString nameCharString = name.utf8();
	std::string nameStdString = std::string(nameCharString.get_data(), nameCharString.length());

	if(nodeRefs.count(nameStdString) == 0) {
		return Ref<PixelpartNode>();
	}

	return nodeRefs.at(nameStdString);
}
Ref<PixelpartNode> PixelpartEffectRuntime::get_node(int id) const {
	if(id < 0 || !effect.sceneGraph().contains(pixelpart::id_t(static_cast<uint32_t>(id)))) {
		return Ref<PixelpartNode>();
	}

	std::string name = effect.sceneGraph().at(pixelpart::id_t(static_cast<uint32_t>(id))).name();
	if(nodeRefs.count(name) == 0) {
		return Ref<PixelpartNode>();
	}

	return nodeRefs.at(name);
}
Ref<PixelpartNode> PixelpartEffectRuntime::get_node_at_index(int index) const {
	if(index < 0 || !effect.sceneGraph().containsIndex(static_cast<uint32_t>(index))) {
		return Ref<PixelpartNode>();
	}

	std::string name = effect.sceneGraph().atIndex(static_cast<uint32_t>(index)).name();
	if(nodeRefs.count(name) == 0) {
		return Ref<PixelpartNode>();
	}

	return nodeRefs.at(name);
}

Ref<PixelpartParticleType> PixelpartEffectRuntime::find_particle_type(String name) const {
	CharString nameCharString = name.utf8();
	std::string nameStdString = std::string(nameCharString.get_data(), nameCharString.length());

	if(particleTypeRefs.count(nameStdString) == 0) {
		return Ref<PixelpartParticleType>();
	}

	return particleTypeRefs.at(nameStdString);
}
Ref<PixelpartParticleType> PixelpartEffectRuntime::get_particle_type(int id) const {
	if(id < 0 || !effect.particleTypes().contains(pixelpart::id_t(static_cast<uint32_t>(id)))) {
		return Ref<PixelpartParticleType>();
	}

	std::string name = effect.particleTypes().at(pixelpart::id_t(static_cast<uint32_t>(id))).name();
	if(particleTypeRefs.count(name) == 0) {
		return Ref<PixelpartParticleType>();
	}

	return particleTypeRefs.at(name);
}
Ref<PixelpartParticleType> PixelpartEffectRuntime::get_particle_type_at_index(int index) const {
	if(index < 0 || !effect.particleTypes().containsIndex(static_cast<uint32_t>(index))) {
		return Ref<PixelpartParticleType>();
	}

	std::string name = effect.particleTypes().atIndex(static_cast<uint32_t>(index)).name();
	if(particleTypeRefs.count(name) == 0) {
		return Ref<PixelpartParticleType>();
	}

	return particleTypeRefs.at(name);
}
}