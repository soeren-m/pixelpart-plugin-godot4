#ifndef PIXELPART_EFFECT_2D_H
#define PIXELPART_EFFECT_2D_H

#include "PixelpartEffectResource.h"
#include "PixelpartEffectRuntime.h"
#include "rendering/PixelpartGraphicsResourceProvider.h"
#include "rendering/PixelpartParticleRenderer2D.h"
#include "particletype/PixelpartParticleType.h"
#include "node/PixelpartParticleEmitter.h"
#include "node/PixelpartForceField.h"
#include "node/PixelpartCollider.h"
#include <pixelpart-runtime/effect/ParticleRuntimeId.h>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <memory>
#include <unordered_map>

namespace godot {
class PixelpartEffect2D : public Node2D {
	GDCLASS(PixelpartEffect2D, Node2D)

public:
	PixelpartEffect2D();
	virtual ~PixelpartEffect2D();

	virtual void _enter_tree() override;

	virtual void _process(double dt) override;

	virtual void _draw() override;

	void set_effect(Ref<PixelpartEffectResource> resource);
	Ref<PixelpartEffectResource> get_effect() const;

	void play(bool state);
	void pause();
	void restart();
	void reset();
	bool is_playing() const;
	float get_time() const;

	void set_loop(bool mode);
	bool get_loop() const;

	void set_loop_time(float time);
	float get_loop_time() const;

	void set_warmup_time(float time);
	float get_warmup_time() const;

	void set_speed(float sp);
	float get_speed() const;

	void set_frame_rate(float rate);
	float get_frame_rate() const;

	void set_effect_scale(float scale);
	float get_effect_scale() const;

	void set_flip_h(bool flip);
	void set_flip_v(bool flip);
	bool get_flip_h() const;
	bool get_flip_v() const;

	void set_inputs(Dictionary inputs);
	Dictionary get_inputs() const;

	void set_input_bool(String name, bool value);
	void set_input_int(String name, int value);
	void set_input_float(String name, float value);
	void set_input_float2(String name, Vector2 value);
	void set_input_float3(String name, Vector3 value);
	void set_input_float4(String name, Vector4 value);
	bool get_input_bool(String name) const;
	int get_input_int(String name) const;
	float get_input_float(String name) const;
	Vector2 get_input_float2(String name) const;
	Vector3 get_input_float3(String name) const;
	Vector4 get_input_float4(String name) const;
	int get_input_type(String name) const;
	TypedArray<String> get_input_names() const;

	void activate_trigger(String name);
	bool is_trigger_activated(String name) const;

	void spawn_particles(String particleEmitterName, String particleTypeName, int count);

	Ref<PixelpartNode> find_node(String name) const;
	Ref<PixelpartNode> get_node(int id) const;
	Ref<PixelpartNode> get_node_at_index(int index) const;

	Ref<PixelpartParticleType> find_particle_type(String name) const;
	Ref<PixelpartParticleType> get_particle_type(int id) const;
	Ref<PixelpartParticleType> get_particle_type_at_index(int index) const;

	Ref<PixelpartParticleEmitter> find_particle_emitter(String name) const;
	Ref<PixelpartForceField> find_force_field(String name) const;
	Ref<PixelpartCollider> find_collider(String name) const;
	Ref<PixelpartParticleEmitter> get_particle_emitter(int id) const;
	Ref<PixelpartForceField> get_force_field(int id) const;
	Ref<PixelpartCollider> get_collider(int id) const;
	Ref<PixelpartParticleEmitter> get_particle_emitter_at_index(int index) const;
	Ref<PixelpartForceField> get_force_field_at_index(int index) const;
	Ref<PixelpartCollider> get_collider_at_index(int index) const;

protected:
	static void _bind_methods();

private:
	void update_transform();

	Ref<PixelpartEffectResource> effectResource;
	PixelpartEffectRuntime effectRuntime;

	float effectScale = 100.0f;
	bool flipH = false;
	bool flipV = true;

	PixelpartGraphicsResourceProvider graphicsResourceProvider;
	std::unordered_map<pixelpart::ParticleRuntimeId, std::unique_ptr<PixelpartParticleRenderer2D>> particleRenderers;
};
}

#endif