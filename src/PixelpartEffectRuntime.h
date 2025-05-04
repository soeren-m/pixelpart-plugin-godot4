#ifndef PIXELPART_EFFECT_RUNTIME_H
#define PIXELPART_EFFECT_RUNTIME_H

#include "node/PixelpartNode.h"
#include "particletype/PixelpartParticleType.h"
#include <godot_cpp/core/binder_common.hpp>
#include <pixelpart-runtime/common/VariantValue.h>
#include <pixelpart-runtime/effect/Effect.h>
#include <pixelpart-runtime/engine/EffectEngine.h>
#include <memory>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace godot {
class PixelpartEffectRuntime {
public:
	PixelpartEffectRuntime();

	void set_effect(const pixelpart::Effect& eff);
	void reset_effect();
	const pixelpart::Effect& get_effect() const;
	const pixelpart::EffectEngine* get_effect_engine() const;

	void start();
	void advance(double dt);

	void play(bool mode);
	void restart(bool reset);
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

	void set_inputs(Dictionary inputs);
	Dictionary get_inputs() const;

	void set_input(String name, const pixelpart::VariantValue& value);
	pixelpart::VariantValue get_input(String name) const;
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

private:
	void apply_inputs();

	pixelpart::Effect effect;

	bool playing = true;
	bool loop = false;
	float loopTime = 1.0f;
	float warmupTime = 0.0f;
	float speed = 1.0f;
	float timeStep = 1.0f / 60.0f;
	Dictionary inputValues;

	std::unique_ptr<pixelpart::EffectEngine> effectEngine;
	uint32_t particleCapacity = 10000;
	float simulationTime = 0.0f;

	std::unordered_map<std::string, Ref<PixelpartNode>> nodeRefs;
	std::unordered_map<std::string, Ref<PixelpartParticleType>> particleTypeRefs;
};
}

#endif