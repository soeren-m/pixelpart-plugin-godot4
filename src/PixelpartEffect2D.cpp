#include "PixelpartEffect2D.h"
#include "PixelpartSystem.h"
#include "util/PixelpartUtil.h"
#include <engine/CPUParticleEngine.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <algorithm>

namespace godot {
void PixelpartEffect2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_effect", "effect_res"), &PixelpartEffect2D::set_effect);
	ClassDB::bind_method(D_METHOD("get_effect"), &PixelpartEffect2D::get_effect);
	ClassDB::bind_method(D_METHOD("play", "p"), &PixelpartEffect2D::play);
	ClassDB::bind_method(D_METHOD("pause"), &PixelpartEffect2D::pause);
	ClassDB::bind_method(D_METHOD("restart"), &PixelpartEffect2D::restart);
	ClassDB::bind_method(D_METHOD("reset"), &PixelpartEffect2D::reset);
	ClassDB::bind_method(D_METHOD("is_playing"), &PixelpartEffect2D::is_playing);
	ClassDB::bind_method(D_METHOD("get_time"), &PixelpartEffect2D::get_time);
	ClassDB::bind_method(D_METHOD("set_loop", "l"), &PixelpartEffect2D::set_loop);
	ClassDB::bind_method(D_METHOD("get_loop"), &PixelpartEffect2D::get_loop);
	ClassDB::bind_method(D_METHOD("set_loop_time", "l"), &PixelpartEffect2D::set_loop_time);
	ClassDB::bind_method(D_METHOD("get_loop_time"), &PixelpartEffect2D::get_loop_time);
	ClassDB::bind_method(D_METHOD("set_speed", "s"), &PixelpartEffect2D::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &PixelpartEffect2D::get_speed);
	ClassDB::bind_method(D_METHOD("set_frame_rate", "r"), &PixelpartEffect2D::set_frame_rate);
	ClassDB::bind_method(D_METHOD("get_frame_rate"), &PixelpartEffect2D::get_frame_rate);
	ClassDB::bind_method(D_METHOD("set_inputs", "in"), &PixelpartEffect2D::set_inputs);
	ClassDB::bind_method(D_METHOD("get_inputs"), &PixelpartEffect2D::get_inputs);
	ClassDB::bind_method(D_METHOD("set_flip_h", "flip"), &PixelpartEffect2D::set_flip_h);
	ClassDB::bind_method(D_METHOD("set_flip_v", "flip"), &PixelpartEffect2D::set_flip_v);
	ClassDB::bind_method(D_METHOD("get_flip_h"), &PixelpartEffect2D::get_flip_h);
	ClassDB::bind_method(D_METHOD("get_flip_v"), &PixelpartEffect2D::get_flip_v);
	ClassDB::bind_method(D_METHOD("set_input_bool", "name", "value"), &PixelpartEffect2D::set_input_bool);
	ClassDB::bind_method(D_METHOD("set_input_int", "name", "value"), &PixelpartEffect2D::set_input_int);
	ClassDB::bind_method(D_METHOD("set_input_float", "name", "value"), &PixelpartEffect2D::set_input_float);
	ClassDB::bind_method(D_METHOD("set_input_float2", "name", "value"), &PixelpartEffect2D::set_input_float2);
	ClassDB::bind_method(D_METHOD("set_input_float3", "name", "value"), &PixelpartEffect2D::set_input_float3);
	ClassDB::bind_method(D_METHOD("set_input_float4", "name", "value"), &PixelpartEffect2D::set_input_float4);
	ClassDB::bind_method(D_METHOD("get_input_bool", "name"), &PixelpartEffect2D::get_input_bool);
	ClassDB::bind_method(D_METHOD("get_input_int", "name"), &PixelpartEffect2D::get_input_int);
	ClassDB::bind_method(D_METHOD("get_input_float", "name"), &PixelpartEffect2D::get_input_float);
	ClassDB::bind_method(D_METHOD("get_input_float2", "name"), &PixelpartEffect2D::get_input_float2);
	ClassDB::bind_method(D_METHOD("get_input_float3", "name"), &PixelpartEffect2D::get_input_float3);
	ClassDB::bind_method(D_METHOD("get_input_float4", "name"), &PixelpartEffect2D::get_input_float4);
	ClassDB::bind_method(D_METHOD("get_input_type", "name"), &PixelpartEffect2D::get_input_type);
	ClassDB::bind_method(D_METHOD("get_input_names"), &PixelpartEffect2D::get_input_names);
	ClassDB::bind_method(D_METHOD("spawn_particles", "particleTypeName", "count"), &PixelpartEffect2D::spawn_particles);
	ClassDB::bind_method(D_METHOD("get_import_scale"), &PixelpartEffect2D::get_import_scale);
	ClassDB::bind_method(D_METHOD("find_particle_emitter"), &PixelpartEffect2D::find_particle_emitter);
	ClassDB::bind_method(D_METHOD("find_particle_type"), &PixelpartEffect2D::find_particle_type);
	ClassDB::bind_method(D_METHOD("find_force_field"), &PixelpartEffect2D::find_force_field);
	ClassDB::bind_method(D_METHOD("find_collider"), &PixelpartEffect2D::find_collider);
	ClassDB::bind_method(D_METHOD("get_particle_emitter"), &PixelpartEffect2D::get_particle_emitter);
	ClassDB::bind_method(D_METHOD("get_particle_type"), &PixelpartEffect2D::get_particle_type);
	ClassDB::bind_method(D_METHOD("get_force_field"), &PixelpartEffect2D::get_force_field);
	ClassDB::bind_method(D_METHOD("get_collider"), &PixelpartEffect2D::get_collider);
	ClassDB::bind_method(D_METHOD("get_particle_emitter_at_index"), &PixelpartEffect2D::get_particle_emitter_at_index);
	ClassDB::bind_method(D_METHOD("get_particle_type_at_index"), &PixelpartEffect2D::get_particle_type_at_index);
	ClassDB::bind_method(D_METHOD("get_force_field_at_index"), &PixelpartEffect2D::get_force_field_at_index);
	ClassDB::bind_method(D_METHOD("get_collider_at_index"), &PixelpartEffect2D::get_collider_at_index);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "effect", PROPERTY_HINT_RESOURCE_TYPE, "PixelpartEffectResource"), "set_effect", "get_effect");

	ADD_GROUP("Playback", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "play", "is_playing");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "loop"), "set_loop", "get_loop");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "loop_time", PROPERTY_HINT_RANGE, "0.0,1000.0,0.01"), "set_loop_time", "get_loop_time");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0.0,100.0,0.01"), "set_speed", "get_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "frame_rate", PROPERTY_HINT_RANGE, "1.0,100.0,1.0"), "set_frame_rate", "get_frame_rate");

	ADD_GROUP("Inputs", "");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "inputs"), "set_inputs", "get_inputs");

	ADD_GROUP("Rendering", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_h"), "set_flip_h", "get_flip_h");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_v"), "set_flip_v", "get_flip_v");
}

PixelpartEffect2D::PixelpartEffect2D() {
	ProjectSettings* settings = ProjectSettings::get_singleton();

	if(settings->has_setting("pixelpart/particle_capacity")) {
		particleCapacity = static_cast<uint32_t>(std::max((int)settings->get_setting("pixelpart/particle_capacity"), 1));
	}
}
PixelpartEffect2D::~PixelpartEffect2D() {

}

void PixelpartEffect2D::_enter_tree() {
	if(!Engine::get_singleton()->is_editor_hint()) {
		apply_input_values();
	}
}

void PixelpartEffect2D::_process(double dt) {
	if(Engine::get_singleton()->is_editor_hint() || !particleEngine) {
		return;
	}

	if(playing) {
		simulationTime += glm::clamp(static_cast<float>(dt), 0.0f, 1.0f) * speed;

		while(simulationTime > timeStep) {
			simulationTime -= timeStep;
			particleEngine->step(timeStep);

			if(loop) {
				if(particleEngine->getTime() > loopTime) {
					particleEngine->restart(false);
				}
			}
		}
	}

	queue_redraw();
}

void PixelpartEffect2D::_draw() {
	if(Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	Viewport* viewport = get_viewport();
	if(!particleEngine || !viewport) {
		return;
	}

	const pixelpart::Effect& effect = particleEngine->getEffect();

	std::vector<uint32_t> particleTypeIndicesSortedByLayer = effect.particleTypes.getSortedIndices(
		[](const pixelpart::ParticleType& t1, const pixelpart::ParticleType& t2) {
			return t1.layer < t2.layer;
		});

	pixelpart::float_t scaleX = static_cast<pixelpart::float_t>(get_import_scale()) * (flipH ? -1.0 : +1.0);
	pixelpart::float_t scaleY = static_cast<pixelpart::float_t>(get_import_scale()) * (flipV ? -1.0 : +1.0);

	for(uint32_t particleTypeIndex : particleTypeIndicesSortedByLayer) {
		if(particleTypeIndex >= particleMeshes.size() || !particleMeshes[particleTypeIndex]) {
			continue;
		}

		particleMeshes[particleTypeIndex]->draw(this,
			effect,
			effect.particleTypes.getByIndex(particleTypeIndex),
			particleEngine->getParticles(particleTypeIndex),
			particleEngine->getNumParticles(particleTypeIndex),
			scaleX, scaleY,
			particleEngine->getTime());
	}
}

void PixelpartEffect2D::set_effect(Ref<PixelpartEffectResource> effectRes) {
	RenderingServer* rs = RenderingServer::get_singleton();

	inputValues.clear();

	particleEmitters.clear();
	particleTypes.clear();
	forceFields.clear();
	colliders.clear();

	particleMeshes.clear();
	graphicsResources.textures.clear();
	graphicsResources.meshes.clear();

	effectResource = effectRes;
	if(effectResource.is_null()) {
		particleEngine = nullptr;
		notify_property_list_changed();

		return;
	}

	effectResource->load();

	effect = effectResource->get_project().effect;
	pixelpart::refreshEffectProperties(effect);

#ifndef __EMSCRIPTEN__
	particleEngine = std::unique_ptr<pixelpart::CPUParticleEngine>(
		new pixelpart::CPUParticleEngine(effect, particleCapacity, PixelpartSystem::get_instance()->get_thread_pool()));
#else
	particleEngine = std::unique_ptr<pixelpart::CPUParticleEngine>(
		new pixelpart::CPUParticleEngine(effect, particleCapacity));
#endif

	try {
		for(const auto& inputEntry : effect.inputs) {
			StringName inputName = StringName(inputEntry.second.name.c_str());
			inputValues[inputName] = toGd(inputEntry.second.value);
		}

		for(const auto& resource : effect.resources.images) {
			PackedByteArray imageData;
			imageData.resize(static_cast<int64_t>(resource.second.data.size()));

			std::memcpy(imageData.ptrw(), resource.second.data.data(), resource.second.data.size());

			Ref<Image> image = Image::create_from_data(
				static_cast<int32_t>(resource.second.width),
				static_cast<int32_t>(resource.second.height),
				false, Image::FORMAT_RGBA8, imageData);

			graphicsResources.textures[resource.first] = ImageTexture::create_from_image(image);
		}

		for(pixelpart::ParticleEmitter& particleEmitter : effect.particleEmitters) {
			Ref<PixelpartParticleEmitter> emitterRef;
			emitterRef.instantiate();
			emitterRef->init(effectResource, &particleEmitter, particleEngine.get());

			particleEmitters[particleEmitter.name] = emitterRef;
		}

		for(pixelpart::ParticleType& particleType : effect.particleTypes) {
			Ref<PixelpartParticleType> particleTypeRef;
			particleTypeRef.instantiate();
			particleTypeRef->init(effectResource, &particleType, particleEngine.get());

			particleTypes[particleType.name] = particleTypeRef;
		}

		for(pixelpart::ForceField& forceField : effect.forceFields) {
			Ref<PixelpartForceField> forceFieldRef;
			forceFieldRef.instantiate();
			forceFieldRef->init(effectResource, &forceField, particleEngine.get());

			forceFields[forceField.name] = forceFieldRef;
		}

		for(pixelpart::Collider& collider : effect.colliders) {
			Ref<PixelpartCollider> colliderRef;
			colliderRef.instantiate();
			colliderRef->init(effectResource, &collider, particleEngine.get());

			colliders[collider.name] = colliderRef;
		}

		for(uint32_t particleTypeIndex = 0u; particleTypeIndex < effect.particleTypes.getCount(); particleTypeIndex++) {
			particleMeshes.emplace_back(std::make_unique<PixelpartParticleMesh2D>(
				graphicsResources, PixelpartSystem::get_instance()->get_shader_generator(),
				effect, effect.particleTypes.getByIndex(particleTypeIndex)));
		}
	}
	catch(const std::exception& e) {
		particleEngine = nullptr;

		UtilityFunctions::printerr(String(e.what()));
	}

	notify_property_list_changed();
}
Ref<PixelpartEffectResource> PixelpartEffect2D::get_effect() const {
	return effectResource;
}

void PixelpartEffect2D::play(bool p) {
	playing = p;
}
void PixelpartEffect2D::pause() {
	playing = false;
}
void PixelpartEffect2D::restart() {
	particleEngine->restart(true);
}
void PixelpartEffect2D::reset() {
	particleEngine->restart(false);
}
bool PixelpartEffect2D::is_playing() const {
	return playing;
}
float PixelpartEffect2D::get_time() const {
	return static_cast<float>(particleEngine->getTime());
}

void PixelpartEffect2D::set_loop(bool l) {
	loop = l;
}
bool PixelpartEffect2D::get_loop() const {
	return loop;
}

void PixelpartEffect2D::set_loop_time(float l) {
	loopTime = std::max(l, 0.0f);
}
float PixelpartEffect2D::get_loop_time() const {
	return loopTime;
}

void PixelpartEffect2D::set_speed(float s) {
	speed = std::max(s, 0.0f);
}
float PixelpartEffect2D::get_speed() const {
	return speed;
}

void PixelpartEffect2D::set_frame_rate(float r) {
	timeStep = 1.0f / std::min(std::max(r, 1.0f), 100.0f);
}
float PixelpartEffect2D::get_frame_rate() const {
	return 1.0f / timeStep;
}

void PixelpartEffect2D::set_inputs(Dictionary in) {
	inputValues = in;
	apply_input_values();
}
Dictionary PixelpartEffect2D::get_inputs() const {
	return inputValues;
}

void PixelpartEffect2D::set_flip_h(bool flip) {
	flipH = flip;
}
void PixelpartEffect2D::set_flip_v(bool flip) {
	flipV = flip;
}
bool PixelpartEffect2D::get_flip_h() const {
	return flipH;
}
bool PixelpartEffect2D::get_flip_v() const {
	return flipV;
}

void PixelpartEffect2D::set_input_bool(String name, bool value) {
	set_input(name, pixelpart::VariantValue::Bool(value));
}
void PixelpartEffect2D::set_input_int(String name, int value) {
	set_input(name, pixelpart::VariantValue::Int(fromGd(value)));
}
void PixelpartEffect2D::set_input_float(String name, float value) {
	set_input(name, pixelpart::VariantValue::Float(fromGd(value)));
}
void PixelpartEffect2D::set_input_float2(String name, Vector2 value) {
	set_input(name, pixelpart::VariantValue::Float2(fromGd(value)));
}
void PixelpartEffect2D::set_input_float3(String name, Vector3 value) {
	set_input(name, pixelpart::VariantValue::Float3(fromGd(value)));
}
void PixelpartEffect2D::set_input_float4(String name, Vector4 value) {
	set_input(name, pixelpart::VariantValue::Float4(fromGd(value)));
}
bool PixelpartEffect2D::get_input_bool(String name) const {
	pixelpart::VariantValue value = get_input(name);
	if(value.type == pixelpart::VariantValue::type_null) {
		return false;
	}

	return value.toBool();
}
int PixelpartEffect2D::get_input_int(String name) const {
	pixelpart::VariantValue value = get_input(name);
	if(value.type == pixelpart::VariantValue::type_null) {
		return 0;
	}

	return toGd(value.toInt());
}
float PixelpartEffect2D::get_input_float(String name) const {
	pixelpart::VariantValue value = get_input(name);
	if(value.type == pixelpart::VariantValue::type_null) {
		return 0.0f;
	}

	return toGd(value.toFloat());
}
Vector2 PixelpartEffect2D::get_input_float2(String name) const {
	pixelpart::VariantValue value = get_input(name);
	if(value.type == pixelpart::VariantValue::type_null) {
		return Vector2(0.0f, 0.0f);
	}

	return toGd(value.toFloat2());
}
Vector3 PixelpartEffect2D::get_input_float3(String name) const {
	pixelpart::VariantValue value = get_input(name);
	if(value.type == pixelpart::VariantValue::type_null) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	return toGd(value.toFloat3());
}
Vector4 PixelpartEffect2D::get_input_float4(String name) const {
	pixelpart::VariantValue value = get_input(name);
	if(value.type == pixelpart::VariantValue::type_null) {
		return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return toGd(value.toFloat4());
}
int PixelpartEffect2D::get_input_type(String name) const {
	pixelpart::VariantValue value = get_input(name);
	if(value.type == pixelpart::VariantValue::type_null) {
		return -1;
	}

	return static_cast<int>(value.type);
}
TypedArray<String> PixelpartEffect2D::get_input_names() const {
	TypedArray<String> names;
	for(const std::pair<pixelpart::id_t, pixelpart::EffectInput>& entry : effect.inputs) {
		names.append(String(entry.second.name.c_str()));
	}

	return names;
}

void PixelpartEffect2D::spawn_particles(String particleTypeName, int count) {
	if(particleEngine && count > 0) {
		Ref<PixelpartParticleType> particleType = find_particle_type(particleTypeName);

		if(particleType.is_valid()) {
			particleEngine->spawnParticles(static_cast<pixelpart::id_t>(particleType->get_id()), static_cast<uint32_t>(count));
		}
		else {
			UtilityFunctions::push_warning("Could not find particle type \"", particleTypeName, "\"");
		}
	}
}

float PixelpartEffect2D::get_import_scale() const {
	if(!effectResource.is_valid()) {
		return 1.0f;
	}

	return effectResource->get_scale();
}

Ref<PixelpartParticleEmitter> PixelpartEffect2D::find_particle_emitter(String name) const {
	CharString nameCharString = name.utf8();
	std::string nameStdString = std::string(nameCharString.get_data(), nameCharString.length());

	if(particleEmitters.count(nameStdString) != 0u) {
		return particleEmitters.at(nameStdString);
	}

	return Ref<PixelpartParticleEmitter>();
}
Ref<PixelpartParticleType> PixelpartEffect2D::find_particle_type(String name) const {
	CharString nameCharString = name.utf8();
	std::string nameStdString = std::string(nameCharString.get_data(), nameCharString.length());

	if(particleTypes.count(nameStdString) != 0u) {
		return particleTypes.at(nameStdString);
	}

	return Ref<PixelpartParticleType>();
}
Ref<PixelpartForceField> PixelpartEffect2D::find_force_field(String name) const {
	CharString nameCharString = name.utf8();
	std::string nameStdString = std::string(nameCharString.get_data(), nameCharString.length());

	if(forceFields.count(nameStdString) != 0u) {
		return forceFields.at(nameStdString);
	}

	return Ref<PixelpartForceField>();
}
Ref<PixelpartCollider> PixelpartEffect2D::find_collider(String name) const {
	CharString nameCharString = name.utf8();
	std::string nameStdString = std::string(nameCharString.get_data(), nameCharString.length());

	if(colliders.count(nameStdString) != 0u) {
		return colliders.at(nameStdString);
	}

	return Ref<PixelpartCollider>();
}
Ref<PixelpartParticleEmitter> PixelpartEffect2D::get_particle_emitter(int id) const {
	if(id >= 0 && effect.particleEmitters.contains(static_cast<uint32_t>(id))) {
		std::string name = effect.particleEmitters.get(static_cast<uint32_t>(id)).name;

		if(particleEmitters.count(name) != 0u) {
			return particleEmitters.at(name);
		}
	}

	return Ref<PixelpartParticleEmitter>();
}
Ref<PixelpartParticleType> PixelpartEffect2D::get_particle_type(int id) const {
	if(id >= 0 && effect.particleTypes.contains(static_cast<uint32_t>(id))) {
		std::string name = effect.particleTypes.get(static_cast<uint32_t>(id)).name;

		if(particleTypes.count(name) != 0u) {
			return particleTypes.at(name);
		}
	}

	return Ref<PixelpartParticleType>();
}
Ref<PixelpartForceField> PixelpartEffect2D::get_force_field(int id) const {
	if(id >= 0 && effect.forceFields.contains(static_cast<uint32_t>(id))) {
		std::string name = effect.forceFields.get(static_cast<uint32_t>(id)).name;

		if(forceFields.count(name) != 0u) {
			return forceFields.at(name);
		}
	}

	return Ref<PixelpartForceField>();
}
Ref<PixelpartCollider> PixelpartEffect2D::get_collider(int id) const {
	if(id >= 0 && effect.colliders.contains(static_cast<uint32_t>(id))) {
		std::string name = effect.colliders.get(static_cast<uint32_t>(id)).name;

		if(colliders.count(name) != 0u) {
			return colliders.at(name);
		}
	}

	return Ref<PixelpartCollider>();
}
Ref<PixelpartParticleEmitter> PixelpartEffect2D::get_particle_emitter_at_index(int index) const {
	if(index >= 0 && effect.particleEmitters.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = effect.particleEmitters.getByIndex(static_cast<uint32_t>(index)).name;

		if(particleEmitters.count(name) != 0u) {
			return particleEmitters.at(name);
		}
	}

	return Ref<PixelpartParticleEmitter>();
}
Ref<PixelpartParticleType> PixelpartEffect2D::get_particle_type_at_index(int index) const {
	if(index >= 0 && effect.particleTypes.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = effect.particleTypes.getByIndex(static_cast<uint32_t>(index)).name;

		if(particleTypes.count(name) != 0u) {
			return particleTypes.at(name);
		}
	}

	return Ref<PixelpartParticleType>();
}
Ref<PixelpartForceField> PixelpartEffect2D::get_force_field_at_index(int index) const {
	if(index >= 0 && effect.forceFields.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = effect.forceFields.getByIndex(static_cast<uint32_t>(index)).name;

		if(forceFields.count(name) != 0u) {
			return forceFields.at(name);
		}
	}

	return Ref<PixelpartForceField>();
}
Ref<PixelpartCollider> PixelpartEffect2D::get_collider_at_index(int index) const {
	if(index >= 0 && effect.colliders.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = effect.colliders.getByIndex(static_cast<uint32_t>(index)).name;

		if(colliders.count(name) != 0u) {
			return colliders.at(name);
		}
	}

	return Ref<PixelpartCollider>();
}

void PixelpartEffect2D::apply_input_values() {
	for(auto& inputEntry : effect.inputs) {
		StringName inputName = StringName(inputEntry.second.name.c_str());
		if(!inputValues.has(inputName)) {
			continue;
		}

		inputEntry.second.value = fromGd(inputValues.get(inputName, Variant()));
	}

	pixelpart::refreshEffectProperties(effect);
}

void PixelpartEffect2D::set_input(String name, const pixelpart::VariantValue& value) {
	std::string inputName = std::string(name.utf8().get_data());
	pixelpart::EffectInputCollection::iterator inputIt = std::find_if(
		effect.inputs.begin(), effect.inputs.end(),
		[&inputName](const std::pair<pixelpart::id_t, pixelpart::EffectInput>& entry) {
			return entry.second.name == inputName;
		});

	if(inputIt == effect.inputs.end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return;
	}

	inputIt->second.value = value;
	inputValues[name] = toGd(value);

	pixelpart::refreshEffectProperties(effect);
}
pixelpart::VariantValue PixelpartEffect2D::get_input(String name) const {
	std::string inputName = std::string(name.utf8().get_data());
	pixelpart::EffectInputCollection::const_iterator inputIt = std::find_if(
		effect.inputs.begin(), effect.inputs.end(),
		[&inputName](const std::pair<pixelpart::id_t, pixelpart::EffectInput>& entry) {
			return entry.second.name == inputName;
		});

	if(inputIt == effect.inputs.end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return pixelpart::VariantValue();
	}

	return inputIt->second.value;
}
}