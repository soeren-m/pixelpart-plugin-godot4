#include "PixelpartEffect.h"
#include "PixelpartSystem.h"
#include "util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <vector>
#include <algorithm>

namespace godot {
PixelpartEffect::PixelpartEffect() : Node3D() {

}
PixelpartEffect::~PixelpartEffect() {

}

void PixelpartEffect::_enter_tree() {
	if(Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	effectRuntime.apply_inputs();

	RenderingServer::get_singleton()->connect("frame_pre_draw", Callable(this, "draw"));
}
void PixelpartEffect::_exit_tree() {
	if(Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	RenderingServer::get_singleton()->disconnect("frame_pre_draw", Callable(this, "draw"));
}

void PixelpartEffect::_process(double dt) {
	if(Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	effectRuntime.advance(dt);
}

void PixelpartEffect::draw() {
	if(Engine::get_singleton()->is_editor_hint() || !effectRuntime.get_effect_engine()) {
		return;
	}

	if(!get_viewport()) {
		return;
	}

	const pixelpart::Effect& effect = effectRuntime.get_effect();
	const pixelpart::EffectEngine* effectEngine = effectRuntime.get_effect_engine();

	std::vector<pixelpart::ParticleRuntimeId> sortedParticleRuntimeInstances = effect.particleRuntimeIds();
	std::sort(sortedParticleRuntimeInstances.begin(), sortedParticleRuntimeInstances.end(),
		[this, &effect](const pixelpart::ParticleRuntimeId& p1, const pixelpart::ParticleRuntimeId& p2) {
			const pixelpart::ParticleType& pt1 = effect.particleTypes().at(p1.typeId);
			const pixelpart::ParticleType& pt2 = effect.particleTypes().at(p2.typeId);

			return pt1.layer() < pt2.layer();
		});

	pixelpart::float_t scale = static_cast<pixelpart::float_t>(get_import_scale());

	for(const pixelpart::ParticleRuntimeId& runtimeId : sortedParticleRuntimeInstances) {
		if(particleRenderers.count(runtimeId) == 0) {
			continue;
		}

		const pixelpart::ParticleCollection* particleCollection = effectEngine->particles(runtimeId.emitterId, runtimeId.typeId);
		if(!particleCollection) {
			continue;
		}

		particleRenderers.at(runtimeId)->draw(this,
			particleCollection->readPtr(),
			effectEngine->particleCount(runtimeId.emitterId, runtimeId.typeId),
			effectEngine->runtimeContext(),
			scale);
	}
}

void PixelpartEffect::set_effect(Ref<PixelpartEffectResource> resource) {
	effectRuntime.reset_effect();

	graphicsResourceProvider.clear();
	particleRenderers.clear();

	effectResource = resource;
	if(effectResource.is_null()) {
		notify_property_list_changed();
		return;
	}

	effectResource->load();

	effectRuntime.set_effect(effectResource->get_asset().effect());

	try {
		graphicsResourceProvider.load(effectRuntime.get_effect());

		for(const pixelpart::ParticleRuntimeId& runtimeId : effectRuntime.get_effect().particleRuntimeIds()) {
			particleRenderers[runtimeId] = std::unique_ptr<PixelpartParticleRenderer3D>(new PixelpartParticleRenderer3D(
				graphicsResourceProvider,
				PixelpartSystem::get_instance()->get_shader_provider(),
				effectRuntime.get_effect(),
				runtimeId.emitterId,
				runtimeId.typeId));
		}
	}
	catch(const std::exception& e) {
		effectRuntime.reset_effect();
		UtilityFunctions::push_error(String("Failed to prepare resources for rendering: ") + String(e.what()));
	}

	notify_property_list_changed();
}
Ref<PixelpartEffectResource> PixelpartEffect::get_effect() const {
	return effectResource;
}

float PixelpartEffect::get_import_scale() const {
	return effectResource.is_valid() ? effectResource->get_scale() : 1.0f;
}

void PixelpartEffect::play(bool state) {
	effectRuntime.play(state);
}
void PixelpartEffect::pause() {
	effectRuntime.play(false);
}
void PixelpartEffect::restart() {
	effectRuntime.restart(true);
}
void PixelpartEffect::reset() {
	effectRuntime.restart(false);
}
bool PixelpartEffect::is_playing() const {
	return effectRuntime.is_playing();
}
float PixelpartEffect::get_time() const {
	return effectRuntime.get_time();
}

void PixelpartEffect::set_loop(bool mode) {
	effectRuntime.set_loop(mode);
}
bool PixelpartEffect::get_loop() const {
	return effectRuntime.get_loop();
}

void PixelpartEffect::set_loop_time(float time) {
	effectRuntime.set_loop_time(time);
}
float PixelpartEffect::get_loop_time() const {
	return effectRuntime.get_loop_time();
}

void PixelpartEffect::set_speed(float sp) {
	effectRuntime.set_speed(sp);
}
float PixelpartEffect::get_speed() const {
	return effectRuntime.get_speed();
}

void PixelpartEffect::set_frame_rate(float rate) {
	effectRuntime.set_frame_rate(rate);
}
float PixelpartEffect::get_frame_rate() const {
	return effectRuntime.get_frame_rate();
}

void PixelpartEffect::set_inputs(Dictionary inputs) {
	effectRuntime.set_inputs(inputs);
}
Dictionary PixelpartEffect::get_inputs() const {
	return effectRuntime.get_inputs();
}

void PixelpartEffect::set_input_bool(String name, bool value) {
	effectRuntime.set_input(name, pixelpart::VariantValue::Bool(value));
}
void PixelpartEffect::set_input_int(String name, int value) {
	effectRuntime.set_input(name, pixelpart::VariantValue::Int(gd_to_pxpt(value)));
}
void PixelpartEffect::set_input_float(String name, float value) {
	effectRuntime.set_input(name, pixelpart::VariantValue::Float(gd_to_pxpt(value)));
}
void PixelpartEffect::set_input_float2(String name, Vector2 value) {
	effectRuntime.set_input(name, pixelpart::VariantValue::Float2(gd_to_pxpt(value)));
}
void PixelpartEffect::set_input_float3(String name, Vector3 value) {
	effectRuntime.set_input(name, pixelpart::VariantValue::Float3(gd_to_pxpt(value)));
}
void PixelpartEffect::set_input_float4(String name, Vector4 value) {
	effectRuntime.set_input(name, pixelpart::VariantValue::Float4(gd_to_pxpt(value)));
}
bool PixelpartEffect::get_input_bool(String name) const {
	return effectRuntime.get_input(name).toBool();
}
int PixelpartEffect::get_input_int(String name) const {
	return pxpt_to_gd(effectRuntime.get_input(name).toInt());
}
float PixelpartEffect::get_input_float(String name) const {
	return pxpt_to_gd(effectRuntime.get_input(name).toFloat());
}
Vector2 PixelpartEffect::get_input_float2(String name) const {
	return pxpt_to_gd(effectRuntime.get_input(name).toFloat2());
}
Vector3 PixelpartEffect::get_input_float3(String name) const {
	return pxpt_to_gd(effectRuntime.get_input(name).toFloat3());
}
Vector4 PixelpartEffect::get_input_float4(String name) const {
	return pxpt_to_gd(effectRuntime.get_input(name).toFloat4());
}
int PixelpartEffect::get_input_type(String name) const {
	return static_cast<int>(effectRuntime.get_input(name).type());
}
TypedArray<String> PixelpartEffect::get_input_names() const {
	return effectRuntime.get_input_names();
}

void PixelpartEffect::activate_trigger(String name) {
	effectRuntime.activate_trigger(name);
}
bool PixelpartEffect::is_trigger_activated(String name) const {
	return effectRuntime.is_trigger_activated(name);
}

void PixelpartEffect::spawn_particles(String particleEmitterName, String particleTypeName, int count) {
	effectRuntime.spawn_particles(particleEmitterName, particleTypeName, count);
}

Ref<PixelpartNode> PixelpartEffect::find_node(String name) const {
	return effectRuntime.find_node(name);
}
Ref<PixelpartNode> PixelpartEffect::get_node(int id) const {
	return effectRuntime.get_node(id);
}
Ref<PixelpartNode> PixelpartEffect::get_node_at_index(int index) const {
	return effectRuntime.get_node_at_index(index);
}

Ref<PixelpartParticleType> PixelpartEffect::find_particle_type(String name) const {
	return effectRuntime.find_particle_type(name);
}
Ref<PixelpartParticleType> PixelpartEffect::get_particle_type(int id) const {
	return effectRuntime.get_particle_type(id);
}
Ref<PixelpartParticleType> PixelpartEffect::get_particle_type_at_index(int index) const {
	return effectRuntime.get_particle_type_at_index(index);
}

Ref<PixelpartParticleEmitter> PixelpartEffect::find_particle_emitter(String name) const {
	return find_node(name);
}
Ref<PixelpartForceField> PixelpartEffect::find_force_field(String name) const {
	return find_node(name);
}
Ref<PixelpartCollider> PixelpartEffect::find_collider(String name) const {
	return find_node(name);
}
Ref<PixelpartParticleEmitter> PixelpartEffect::get_particle_emitter(int id) const {
	return get_node(id);
}
Ref<PixelpartForceField> PixelpartEffect::get_force_field(int id) const {
	return get_node(id);
}
Ref<PixelpartCollider> PixelpartEffect::get_collider(int id) const {
	return get_node(id);
}
Ref<PixelpartParticleEmitter> PixelpartEffect::get_particle_emitter_at_index(int index) const {
	return get_node_at_index(index);
}
Ref<PixelpartForceField> PixelpartEffect::get_force_field_at_index(int index) const {
	return get_node_at_index(index);
}
Ref<PixelpartCollider> PixelpartEffect::get_collider_at_index(int index) const {
	return get_node_at_index(index);
}

void PixelpartEffect::_bind_methods() {
	ClassDB::bind_method(D_METHOD("draw"), &PixelpartEffect::draw);
	ClassDB::bind_method(D_METHOD("set_effect", "resource"), &PixelpartEffect::set_effect);
	ClassDB::bind_method(D_METHOD("get_effect"), &PixelpartEffect::get_effect);
	ClassDB::bind_method(D_METHOD("play", "state"), &PixelpartEffect::play);
	ClassDB::bind_method(D_METHOD("pause"), &PixelpartEffect::pause);
	ClassDB::bind_method(D_METHOD("restart"), &PixelpartEffect::restart);
	ClassDB::bind_method(D_METHOD("reset"), &PixelpartEffect::reset);
	ClassDB::bind_method(D_METHOD("is_playing"), &PixelpartEffect::is_playing);
	ClassDB::bind_method(D_METHOD("get_time"), &PixelpartEffect::get_time);
	ClassDB::bind_method(D_METHOD("set_loop", "mode"), &PixelpartEffect::set_loop);
	ClassDB::bind_method(D_METHOD("get_loop"), &PixelpartEffect::get_loop);
	ClassDB::bind_method(D_METHOD("set_loop_time", "time"), &PixelpartEffect::set_loop_time);
	ClassDB::bind_method(D_METHOD("get_loop_time"), &PixelpartEffect::get_loop_time);
	ClassDB::bind_method(D_METHOD("set_speed", "sp"), &PixelpartEffect::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &PixelpartEffect::get_speed);
	ClassDB::bind_method(D_METHOD("set_frame_rate", "rate"), &PixelpartEffect::set_frame_rate);
	ClassDB::bind_method(D_METHOD("get_frame_rate"), &PixelpartEffect::get_frame_rate);
	ClassDB::bind_method(D_METHOD("set_inputs", "inputs"), &PixelpartEffect::set_inputs);
	ClassDB::bind_method(D_METHOD("get_inputs"), &PixelpartEffect::get_inputs);
	ClassDB::bind_method(D_METHOD("set_input_bool", "name", "value"), &PixelpartEffect::set_input_bool);
	ClassDB::bind_method(D_METHOD("set_input_int", "name", "value"), &PixelpartEffect::set_input_int);
	ClassDB::bind_method(D_METHOD("set_input_float", "name", "value"), &PixelpartEffect::set_input_float);
	ClassDB::bind_method(D_METHOD("set_input_float2", "name", "value"), &PixelpartEffect::set_input_float2);
	ClassDB::bind_method(D_METHOD("set_input_float3", "name", "value"), &PixelpartEffect::set_input_float3);
	ClassDB::bind_method(D_METHOD("set_input_float4", "name", "value"), &PixelpartEffect::set_input_float4);
	ClassDB::bind_method(D_METHOD("get_input_bool", "name"), &PixelpartEffect::get_input_bool);
	ClassDB::bind_method(D_METHOD("get_input_int", "name"), &PixelpartEffect::get_input_int);
	ClassDB::bind_method(D_METHOD("get_input_float", "name"), &PixelpartEffect::get_input_float);
	ClassDB::bind_method(D_METHOD("get_input_float2", "name"), &PixelpartEffect::get_input_float2);
	ClassDB::bind_method(D_METHOD("get_input_float3", "name"), &PixelpartEffect::get_input_float3);
	ClassDB::bind_method(D_METHOD("get_input_float4", "name"), &PixelpartEffect::get_input_float4);
	ClassDB::bind_method(D_METHOD("get_input_type", "name"), &PixelpartEffect::get_input_type);
	ClassDB::bind_method(D_METHOD("get_input_names"), &PixelpartEffect::get_input_names);
	ClassDB::bind_method(D_METHOD("activate_trigger", "name"), &PixelpartEffect::activate_trigger);
	ClassDB::bind_method(D_METHOD("is_trigger_activated", "name"), &PixelpartEffect::is_trigger_activated);
	ClassDB::bind_method(D_METHOD("spawn_particles", "particleEmitterName", "particleTypeName", "count"), &PixelpartEffect::spawn_particles);
	ClassDB::bind_method(D_METHOD("get_import_scale"), &PixelpartEffect::get_import_scale);
	ClassDB::bind_method(D_METHOD("find_node", "name"), &PixelpartEffect::find_node);
	ClassDB::bind_method(D_METHOD("get_node", "id"), &PixelpartEffect::get_node);
	ClassDB::bind_method(D_METHOD("get_node_at_index", "index"), &PixelpartEffect::get_node_at_index);
	ClassDB::bind_method(D_METHOD("find_particle_type", "name"), &PixelpartEffect::find_particle_type);
	ClassDB::bind_method(D_METHOD("get_particle_type", "id"), &PixelpartEffect::get_particle_type);
	ClassDB::bind_method(D_METHOD("get_particle_type_at_index", "index"), &PixelpartEffect::get_particle_type_at_index);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "effect", PROPERTY_HINT_RESOURCE_TYPE, "PixelpartEffectResource"), "set_effect", "get_effect");

	ADD_GROUP("Playback", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "play", "is_playing");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "loop"), "set_loop", "get_loop");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "loop_time", PROPERTY_HINT_RANGE, "0.0,1000.0,0.01"), "set_loop_time", "get_loop_time");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0.0,100.0,0.01"), "set_speed", "get_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "frame_rate", PROPERTY_HINT_RANGE, "1.0,100.0,1.0"), "set_frame_rate", "get_frame_rate");

	ADD_GROUP("Inputs", "");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "inputs"), "set_inputs", "get_inputs");

	// Deprecated
	ClassDB::bind_method(D_METHOD("find_particle_emitter", "name"), &PixelpartEffect::find_particle_emitter);
	ClassDB::bind_method(D_METHOD("find_force_field", "name"), &PixelpartEffect::find_force_field);
	ClassDB::bind_method(D_METHOD("find_collider", "name"), &PixelpartEffect::find_collider);
	ClassDB::bind_method(D_METHOD("get_particle_emitter", "id"), &PixelpartEffect::get_particle_emitter);
	ClassDB::bind_method(D_METHOD("get_force_field", "id"), &PixelpartEffect::get_force_field);
	ClassDB::bind_method(D_METHOD("get_collider", "id"), &PixelpartEffect::get_collider);
	ClassDB::bind_method(D_METHOD("get_particle_emitter_at_index", "index"), &PixelpartEffect::get_particle_emitter_at_index);
	ClassDB::bind_method(D_METHOD("get_force_field_at_index", "index"), &PixelpartEffect::get_force_field_at_index);
	ClassDB::bind_method(D_METHOD("get_collider_at_index", "index"), &PixelpartEffect::get_collider_at_index);
}
}