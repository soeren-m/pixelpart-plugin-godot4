#ifndef PIXELPART_EFFECT_2D_H
#define PIXELPART_EFFECT_2D_H

#include "PixelpartEffectResource.h"
#include "PixelpartParticleEmitter.h"
#include "PixelpartParticleType.h"
#include "PixelpartForceField.h"
#include "PixelpartCollider.h"
#include "rendering/PixelpartParticleMesh2D.h"
#include <engine/ParticleEngine.h>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/node2d.hpp>

namespace godot {
class PixelpartEffect2D : public Node2D {
	GDCLASS(PixelpartEffect2D, Node2D)

public:
	static void _bind_methods();

	PixelpartEffect2D();
	~PixelpartEffect2D();

	virtual void _enter_tree() override;

	virtual void _process(double dt) override;

	virtual void _draw() override;

	void set_effect(Ref<PixelpartEffectResource> effectRes);
	Ref<PixelpartEffectResource> get_effect() const;

	void play(bool p);
	void pause();
	void restart();
	void reset();
	bool is_playing() const;
	float get_time() const;

	void set_loop(bool l);
	bool get_loop() const;

	void set_loop_time(float l);
	float get_loop_time() const;

	void set_speed(float s);
	float get_speed() const;

	void set_frame_rate(float r);
	float get_frame_rate() const;

	void set_inputs(Dictionary in);
	Dictionary get_inputs() const;

	void set_flip_h(bool flip);
	void set_flip_v(bool flip);
	bool get_flip_h() const;
	bool get_flip_v() const;

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

	void spawn_particles(String particleTypeName, int count);

	float get_import_scale() const;

	Ref<PixelpartParticleEmitter> find_particle_emitter(String name) const;
	Ref<PixelpartParticleType> find_particle_type(String name) const;
	Ref<PixelpartForceField> find_force_field(String name) const;
	Ref<PixelpartCollider> find_collider(String name) const;
	Ref<PixelpartParticleEmitter> get_particle_emitter(int id) const;
	Ref<PixelpartParticleType> get_particle_type(int id) const;
	Ref<PixelpartForceField> get_force_field(int id) const;
	Ref<PixelpartCollider> get_collider(int id) const;
	Ref<PixelpartParticleEmitter> get_particle_emitter_at_index(int index) const;
	Ref<PixelpartParticleType> get_particle_type_at_index(int index) const;
	Ref<PixelpartForceField> get_force_field_at_index(int index) const;
	Ref<PixelpartCollider> get_collider_at_index(int index) const;

private:
	void apply_input_values();

	void set_input(String name, const pixelpart::VariantValue& value);
	pixelpart::VariantValue get_input(String name) const;

	Ref<PixelpartEffectResource> effectResource;
	pixelpart::Effect effect;

	std::unordered_map<std::string, Ref<PixelpartParticleEmitter>> particleEmitters;
	std::unordered_map<std::string, Ref<PixelpartParticleType>> particleTypes;
	std::unordered_map<std::string, Ref<PixelpartForceField>> forceFields;
	std::unordered_map<std::string, Ref<PixelpartCollider>> colliders;

	std::unique_ptr<pixelpart::ParticleEngine> particleEngine;
	uint32_t particleCapacity = 10000u;
	float simulationTime = 0.0f;

	bool playing = true;
	bool loop = false;
	float loopTime = 1.0f;
	float speed = 1.0f;
	float timeStep = 1.0f / 60.0f;

	Dictionary inputValues;

	bool flipH = false;
	bool flipV = true;

	std::vector<std::unique_ptr<PixelpartParticleMesh2D>> particleMeshes;

	PixelpartGraphicsResourceStore graphicsResources;
};
}

#endif