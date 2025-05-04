#include "PixelpartEffect2D.h"
#include "PixelpartSystem.h"
#include "util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <vector>
#include <algorithm>

namespace godot {
PixelpartEffect2D::PixelpartEffect2D() : Node2D() {

}
PixelpartEffect2D::~PixelpartEffect2D() {

}

void PixelpartEffect2D::_enter_tree() {
	if(Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	effectRuntime.apply_inputs();
}

void PixelpartEffect2D::_process(double dt) {
	if(Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	effectRuntime.advance(dt);

	queue_redraw();
}

void PixelpartEffect2D::_draw() {
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
		[&effect](const pixelpart::ParticleRuntimeId& p1, const pixelpart::ParticleRuntimeId& p2) {
			const pixelpart::ParticleType& pt1 = effect.particleTypes().at(p1.typeId);
			const pixelpart::ParticleType& pt2 = effect.particleTypes().at(p2.typeId);

			return pt1.layer() < pt2.layer();
		});

	pixelpart::float2_t scale = pixelpart::float2_t(
		flipH ? -1.0 : +1.0,
		flipV ? -1.0 : +1.0) * static_cast<pixelpart::float_t>(get_import_scale());

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

void PixelpartEffect2D::set_effect(Ref<PixelpartEffectResource> resource) {
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

	if(!Engine::get_singleton()->is_editor_hint()) {
		try {
			graphicsResourceProvider.load(effectRuntime.get_effect());

			for(const pixelpart::ParticleRuntimeId& runtimeId : effectRuntime.get_effect().particleRuntimeIds()) {
				particleRenderers[runtimeId] = std::unique_ptr<PixelpartParticleRenderer2D>(new PixelpartParticleRenderer2D(
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
	}

	notify_property_list_changed();
}
Ref<PixelpartEffectResource> PixelpartEffect2D::get_effect() const {
	return effectResource;
}

float PixelpartEffect2D::get_import_scale() const {
	return effectResource.is_valid() ? effectResource->get_scale() : 1.0f;
}

void PixelpartEffect2D::play(bool state) {
	effectRuntime.play(state);
}
void PixelpartEffect2D::pause() {
	effectRuntime.play(false);
}
void PixelpartEffect2D::restart() {
	effectRuntime.restart(true);
}
void PixelpartEffect2D::reset() {
	effectRuntime.restart(false);
}
bool PixelpartEffect2D::is_playing() const {
	return effectRuntime.is_playing();
}
float PixelpartEffect2D::get_time() const {
	return effectRuntime.get_time();
}

void PixelpartEffect2D::set_loop(bool mode) {
	effectRuntime.set_loop(mode);
}
bool PixelpartEffect2D::get_loop() const {
	return effectRuntime.get_loop();
}

void PixelpartEffect2D::set_loop_time(float time) {
	effectRuntime.set_loop_time(time);
}
float PixelpartEffect2D::get_loop_time() const {
	return effectRuntime.get_loop_time();
}

void PixelpartEffect2D::set_speed(float sp) {
	effectRuntime.set_speed(sp);
}
float PixelpartEffect2D::get_speed() const {
	return effectRuntime.get_speed();
}

void PixelpartEffect2D::set_frame_rate(float rate) {
	effectRuntime.set_frame_rate(rate);
}
float PixelpartEffect2D::get_frame_rate() const {
	return effectRuntime.get_frame_rate();
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

void PixelpartEffect2D::set_inputs(Dictionary inputs) {
	effectRuntime.set_inputs(inputs);
}
Dictionary PixelpartEffect2D::get_inputs() const {
	return effectRuntime.get_inputs();
}

void PixelpartEffect2D::set_input_bool(String name, bool value) {
	effectRuntime.set_input(name, pixelpart::VariantValue::Bool(value));
}
void PixelpartEffect2D::set_input_int(String name, int value) {
	effectRuntime.set_input(name, pixelpart::VariantValue::Int(gd_to_pxpt(value)));
}
void PixelpartEffect2D::set_input_float(String name, float value) {
	effectRuntime.set_input(name, pixelpart::VariantValue::Float(gd_to_pxpt(value)));
}
void PixelpartEffect2D::set_input_float2(String name, Vector2 value) {
	effectRuntime.set_input(name, pixelpart::VariantValue::Float2(gd_to_pxpt(value)));
}
void PixelpartEffect2D::set_input_float3(String name, Vector3 value) {
	effectRuntime.set_input(name, pixelpart::VariantValue::Float3(gd_to_pxpt(value)));
}
void PixelpartEffect2D::set_input_float4(String name, Vector4 value) {
	effectRuntime.set_input(name, pixelpart::VariantValue::Float4(gd_to_pxpt(value)));
}
bool PixelpartEffect2D::get_input_bool(String name) const {
	return effectRuntime.get_input(name).toBool();
}
int PixelpartEffect2D::get_input_int(String name) const {
	return pxpt_to_gd(effectRuntime.get_input(name).toInt());
}
float PixelpartEffect2D::get_input_float(String name) const {
	return pxpt_to_gd(effectRuntime.get_input(name).toFloat());
}
Vector2 PixelpartEffect2D::get_input_float2(String name) const {
	return pxpt_to_gd(effectRuntime.get_input(name).toFloat2());
}
Vector3 PixelpartEffect2D::get_input_float3(String name) const {
	return pxpt_to_gd(effectRuntime.get_input(name).toFloat3());
}
Vector4 PixelpartEffect2D::get_input_float4(String name) const {
	return pxpt_to_gd(effectRuntime.get_input(name).toFloat4());
}
int PixelpartEffect2D::get_input_type(String name) const {
	return static_cast<int>(effectRuntime.get_input(name).type());
}
TypedArray<String> PixelpartEffect2D::get_input_names() const {
	return effectRuntime.get_input_names();
}

void PixelpartEffect2D::activate_trigger(String name) {
	effectRuntime.activate_trigger(name);
}
bool PixelpartEffect2D::is_trigger_activated(String name) const {
	return effectRuntime.is_trigger_activated(name);
}

void PixelpartEffect2D::spawn_particles(String particleEmitterName, String particleTypeName, int count) {
	effectRuntime.spawn_particles(particleEmitterName, particleTypeName, count);
}

Ref<PixelpartNode> PixelpartEffect2D::find_node(String name) const {
	return effectRuntime.find_node(name);
}
Ref<PixelpartNode> PixelpartEffect2D::get_node(int id) const {
	return effectRuntime.get_node(id);
}
Ref<PixelpartNode> PixelpartEffect2D::get_node_at_index(int index) const {
	return effectRuntime.get_node_at_index(index);
}

Ref<PixelpartParticleType> PixelpartEffect2D::find_particle_type(String name) const {
	return effectRuntime.find_particle_type(name);
}
Ref<PixelpartParticleType> PixelpartEffect2D::get_particle_type(int id) const {
	return effectRuntime.get_particle_type(id);
}
Ref<PixelpartParticleType> PixelpartEffect2D::get_particle_type_at_index(int index) const {
	return effectRuntime.get_particle_type_at_index(index);
}

Ref<PixelpartParticleEmitter> PixelpartEffect2D::find_particle_emitter(String name) const {
	return find_node(name);
}
Ref<PixelpartForceField> PixelpartEffect2D::find_force_field(String name) const {
	return find_node(name);
}
Ref<PixelpartCollider> PixelpartEffect2D::find_collider(String name) const {
	return find_node(name);
}
Ref<PixelpartParticleEmitter> PixelpartEffect2D::get_particle_emitter(int id) const {
	return get_node(id);
}
Ref<PixelpartForceField> PixelpartEffect2D::get_force_field(int id) const {
	return get_node(id);
}
Ref<PixelpartCollider> PixelpartEffect2D::get_collider(int id) const {
	return get_node(id);
}
Ref<PixelpartParticleEmitter> PixelpartEffect2D::get_particle_emitter_at_index(int index) const {
	return get_node_at_index(index);
}
Ref<PixelpartForceField> PixelpartEffect2D::get_force_field_at_index(int index) const {
	return get_node_at_index(index);
}
Ref<PixelpartCollider> PixelpartEffect2D::get_collider_at_index(int index) const {
	return get_node_at_index(index);
}

void PixelpartEffect2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_effect", "resource"), &PixelpartEffect2D::set_effect);
	ClassDB::bind_method(D_METHOD("get_effect"), &PixelpartEffect2D::get_effect);
	ClassDB::bind_method(D_METHOD("get_import_scale"), &PixelpartEffect2D::get_import_scale);
	ClassDB::bind_method(D_METHOD("play", "state"), &PixelpartEffect2D::play);
	ClassDB::bind_method(D_METHOD("pause"), &PixelpartEffect2D::pause);
	ClassDB::bind_method(D_METHOD("restart"), &PixelpartEffect2D::restart);
	ClassDB::bind_method(D_METHOD("reset"), &PixelpartEffect2D::reset);
	ClassDB::bind_method(D_METHOD("is_playing"), &PixelpartEffect2D::is_playing);
	ClassDB::bind_method(D_METHOD("get_time"), &PixelpartEffect2D::get_time);
	ClassDB::bind_method(D_METHOD("set_loop", "mode"), &PixelpartEffect2D::set_loop);
	ClassDB::bind_method(D_METHOD("get_loop"), &PixelpartEffect2D::get_loop);
	ClassDB::bind_method(D_METHOD("set_loop_time", "time"), &PixelpartEffect2D::set_loop_time);
	ClassDB::bind_method(D_METHOD("get_loop_time"), &PixelpartEffect2D::get_loop_time);
	ClassDB::bind_method(D_METHOD("set_speed", "sp"), &PixelpartEffect2D::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &PixelpartEffect2D::get_speed);
	ClassDB::bind_method(D_METHOD("set_frame_rate", "rate"), &PixelpartEffect2D::set_frame_rate);
	ClassDB::bind_method(D_METHOD("get_frame_rate"), &PixelpartEffect2D::get_frame_rate);
	ClassDB::bind_method(D_METHOD("set_flip_h", "flip"), &PixelpartEffect2D::set_flip_h);
	ClassDB::bind_method(D_METHOD("set_flip_v", "flip"), &PixelpartEffect2D::set_flip_v);
	ClassDB::bind_method(D_METHOD("get_flip_h"), &PixelpartEffect2D::get_flip_h);
	ClassDB::bind_method(D_METHOD("get_flip_v"), &PixelpartEffect2D::get_flip_v);
	ClassDB::bind_method(D_METHOD("set_inputs", "inputs"), &PixelpartEffect2D::set_inputs);
	ClassDB::bind_method(D_METHOD("get_inputs"), &PixelpartEffect2D::get_inputs);
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
	ClassDB::bind_method(D_METHOD("activate_trigger", "name"), &PixelpartEffect2D::activate_trigger);
	ClassDB::bind_method(D_METHOD("is_trigger_activated", "name"), &PixelpartEffect2D::is_trigger_activated);
	ClassDB::bind_method(D_METHOD("spawn_particles", "particleEmitterName", "particleTypeName", "count"), &PixelpartEffect2D::spawn_particles);
	ClassDB::bind_method(D_METHOD("find_node", "name"), &PixelpartEffect2D::find_node);
	ClassDB::bind_method(D_METHOD("get_node", "id"), &PixelpartEffect2D::get_node);
	ClassDB::bind_method(D_METHOD("get_node_at_index", "index"), &PixelpartEffect2D::get_node_at_index);
	ClassDB::bind_method(D_METHOD("find_particle_type", "name"), &PixelpartEffect2D::find_particle_type);
	ClassDB::bind_method(D_METHOD("get_particle_type", "id"), &PixelpartEffect2D::get_particle_type);
	ClassDB::bind_method(D_METHOD("get_particle_type_at_index", "index"), &PixelpartEffect2D::get_particle_type_at_index);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "effect", PROPERTY_HINT_RESOURCE_TYPE, "PixelpartEffectResource"), "set_effect", "get_effect");

	ADD_GROUP("Playback", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "play", "is_playing");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "loop"), "set_loop", "get_loop");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "loop_time", PROPERTY_HINT_RANGE, "0.0,100.0,0.01,or_greater"), "set_loop_time", "get_loop_time");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0.0,10.0,0.01,or_greater,exp"), "set_speed", "get_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "frame_rate", PROPERTY_HINT_RANGE, "1.0,100.0,1.0"), "set_frame_rate", "get_frame_rate");

	ADD_GROUP("Inputs", "");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "inputs"), "set_inputs", "get_inputs");

	ADD_GROUP("Rendering", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_h"), "set_flip_h", "get_flip_h");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_v"), "set_flip_v", "get_flip_v");

	// Deprecated
	ClassDB::bind_method(D_METHOD("find_particle_emitter", "name"), &PixelpartEffect2D::find_particle_emitter);
	ClassDB::bind_method(D_METHOD("find_force_field", "name"), &PixelpartEffect2D::find_force_field);
	ClassDB::bind_method(D_METHOD("find_collider", "name"), &PixelpartEffect2D::find_collider);
	ClassDB::bind_method(D_METHOD("get_particle_emitter", "id"), &PixelpartEffect2D::get_particle_emitter);
	ClassDB::bind_method(D_METHOD("get_force_field", "id"), &PixelpartEffect2D::get_force_field);
	ClassDB::bind_method(D_METHOD("get_collider", "id"), &PixelpartEffect2D::get_collider);
	ClassDB::bind_method(D_METHOD("get_particle_emitter_at_index", "index"), &PixelpartEffect2D::get_particle_emitter_at_index);
	ClassDB::bind_method(D_METHOD("get_force_field_at_index", "index"), &PixelpartEffect2D::get_force_field_at_index);
	ClassDB::bind_method(D_METHOD("get_collider_at_index", "index"), &PixelpartEffect2D::get_collider_at_index);
}
}