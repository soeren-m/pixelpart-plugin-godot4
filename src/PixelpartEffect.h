#ifndef PIXELPART_EFFECT_H
#define PIXELPART_EFFECT_H

#include "PixelpartEffectResource.h"
#include "PixelpartEffectRuntime.h"
#include "rendering/PixelpartGraphicsResourceProvider.h"
#include "rendering/PixelpartParticleRenderer3D.h"
#include "particletype/PixelpartParticleType.h"
#include "node/PixelpartParticleEmitter.h"
#include "node/PixelpartForceField.h"
#include "node/PixelpartCollider.h"
#include <pixelpart-runtime/effect/ParticleRuntimeId.h>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/visual_instance3d.hpp>
#include <memory>
#include <unordered_map>

namespace godot {
/**
 * @brief A node that plays a Pixelpart effect.
 *
 * This class offers methods and properties to change how the effect is simulated and rendered.
 */
class PixelpartEffect : public VisualInstance3D {
	GDCLASS(PixelpartEffect, VisualInstance3D)

public:
	PixelpartEffect();
	virtual ~PixelpartEffect();

#ifdef DOXYGEN
	/**
	 * @brief Effect resource that is shown.
	 */
	PixelpartEffectResource effect;

	/**
	 * @brief Whether the effect is currently playing or not.
	 */
	bool playing;

	/**
	 * @brief Whether the effect restarts automatically after time `loop_time`.
	 */
	bool loop;

	/**
	 * @brief Time in seconds after which the effect loops.
	 *
	 * Only effective if `loop = true`.
	 */
	float loop_time;

	/**
	 * @brief Time in seconds the effect is pre-simulated before being rendered.
	 *
	 * This value impacts performance and should be kept as low as possible.
	 */
	float warmup_time;

	/**
	 * @brief How fast the effect is being played.
	 */
	float speed;

	/**
	 * @brief At which rate the effect is simulated, in frames per second.
	 */
	float frame_rate;

	/**
	 * @brief Dictionary of effect input values.
	 *
	 * Do not use to change effect inputs at runtime, use the `set_input` methods instead.
	 */
	Dictionary inputs;

	/**
	 * @brief Multiplier for the size of the effect.
	 *
	 * Adjust this value if the effect appears too small or too large in the scene.
	 */
	float effect_scale;
#endif

	virtual void _enter_tree() override;
	virtual void _exit_tree() override;

	virtual void _process(double dt) override;

	void draw();

	/**
	 * @brief Change the effect resource that is shown.
	 *
	 * @param resource Effect resource to show
	 */
	void set_effect(Ref<PixelpartEffectResource> resource);

	/**
	 * @brief Return the effect resource that is being shown.
	 *
	 * @return Effect resource
	 */
	Ref<PixelpartEffectResource> get_effect() const;

	/**
	 * @brief Start playing or pause simulation of the effect.
	 *
	 * If paused, the effect is still rendered but freezed in time.
	 *
	 * @param state Whether to play or pause the effect.
	 */
	void play(bool state);

	/**
	 * @brief Pause simulation of the effect.
	 *
	 * If paused, the effect is still rendered but freezed in time.
	 */
	void pause();

	/**
	 * @brief Restart the effect and remove all existing particles.
	 *
	 */
	void restart();

	/**
	 * @brief Restart the effect, but do not remove existing particles.
	 *
	 */
	void reset();

	/**
	 * @brief Return whether the effect is currently playing or is paused.
	 *
	 * @return Whether the effect is currently playing or is paused.
	 */
	bool is_playing() const;

	/**
	 * @brief Return the time in seconds since the effect has started playing.
	 *
	 * @return Time in seconds since the effect has started playing.
	 */
	float get_time() const;

	/**
	 * @brief Set whether the effect restarts automatically after time `loop_time`.
	 *
	 * @param mode Loop mode
	 */
	void set_loop(bool mode);

	/**
	 * @brief Return whether the effect restarts automatically after time `loop_time`.
	 *
	 * @return Loop mode
	 */
	bool get_loop() const;

	/**
	 * @brief Set time in seconds after which the effect loops.
	 *
	 * Only effective if `loop = true`.
	 *
	 * @param time Loop time in seconds
	 */
	void set_loop_time(float time);

	/**
	 * @brief Return time in seconds after which the effect loops.
	 *
	 * @return Loop time in seconds
	 */
	float get_loop_time() const;

	/**
	 * @brief Set time in seconds the effect is pre-simulated before being rendered.
	 *
	 * This value impacts performance and should be kept as low as possible.
	 *
	 * @param time Warmup time in seconds
	 */
	void set_warmup_time(float time);

	/**
	 * @brief Return time in seconds the effect is pre-simulated before being rendered.
	 *
	 * @return Warmup time in seconds
	 */
	float get_warmup_time() const;

	/**
	 * @brief Set how fast the effect is being played.
	 *
	 * @param sp Speed multiplier
	 */
	void set_speed(float sp);

	/**
	 * @brief Return how fast the effect is being played.
	 *
	 * @return Speed multiplier
	 */
	float get_speed() const;

	/**
	 * @brief Set at which rate the effect is simulated, in frames per second.
	 *
	 * @param rate Frame rate, in frames per second
	 */
	void set_frame_rate(float rate);

	/**
	 * @brief Return at which rate the effect is simulated, in frames per second.
	 *
	 * @return Frame rate, in frames per second
	 */
	float get_frame_rate() const;

	/**
	 * @brief Set multiplier for the size of the effect.
	 *
	 * Adjust this value if the effect appears too small or too large in the scene.
	 *
	 * @param scale Effect scale
	 */
	void set_effect_scale(float scale);

	/**
	 * @brief Return multiplier for the size of the effect.
	 *
	 * @return Effect scale
	 */
	float get_effect_scale() const;

	/**
	 * @brief @brief Set dictionary of effect input values.
	 *
	 * Do not use to change effect inputs at runtime, use the `set_input` methods instead.
	 *
	 * @param inputs Effect input values
	 */
	void set_inputs(Dictionary inputs);

	/**
	 * @brief Return dictionary of effect input values.
	 *
	 * Do not use to change effect inputs at runtime, use the `set_input` methods instead.
	 *
	 * @return Effect input values
	 */
	Dictionary get_inputs() const;

	/**
	 * Set the effect input @p name to the given value. The effect input must be of type `bool`.
	 *
	 * @param name Name of the effect input
	 * @param value New value
	 */
	void set_input_bool(String name, bool value);

	/**
	 * Set the effect input @p name to the given value. The effect input must be of type `int`.
	 *
	 * @param name Name of the effect input
	 * @param value New value
	 */
	void set_input_int(String name, int value);

	/**
	 * Set the effect input @p name to the given value. The effect input must be of type `float`.
	 *
	 * @param name Name of the effect input
	 * @param value New value
	 */
	void set_input_float(String name, float value);

	/**
	 * Set the effect input @p name to the given value. The effect input must be of type `Vector2`.
	 *
	 * @param name Name of the effect input
	 * @param value New value
	 */
	void set_input_float2(String name, Vector2 value);

	/**
	 * Set the effect input @p name to the given value. The effect input must be of type `Vector3`.
	 *
	 * @param name Name of the effect input
	 * @param value New value
	 */
	void set_input_float3(String name, Vector3 value);

	/**
	 * Set the effect input @p name to the given value. The effect input must be of type `Vector4`.
	 *
	 * @param name Name of the effect input
	 * @param value New value
	 */
	void set_input_float4(String name, Vector4 value);

	/**
	 * @brief Return value of an effect input. The effect input must be of type `bool`.
	 *
	 * @param name Name of the effect input
	 * @return Value of the effect input
	 */
	bool get_input_bool(String name) const;

	/**
	 * @brief Return value of an effect input. The effect input must be of type `int`.
	 *
	 * @param name Name of the effect input
	 * @return Value of the effect input
	 */
	int get_input_int(String name) const;

	/**
	 * @brief Return value of an effect input. The effect input must be of type `float`.
	 *
	 * @param name Name of the effect input
	 * @return Value of the effect input
	 */
	float get_input_float(String name) const;

	/**
	 * @brief Return value of an effect input. The effect input must be of type `Vector2`.
	 *
	 * @param name Name of the effect input
	 * @return Value of the effect input
	 */
	Vector2 get_input_float2(String name) const;

	/**
	 * @brief Return value of an effect input. The effect input must be of type `Vector3`.
	 *
	 * @param name Name of the effect input
	 * @return Value of the effect input
	 */
	Vector3 get_input_float3(String name) const;

	/**
	 * @brief Return value of an effect input. The effect input must be of type `Vector4`.
	 *
	 * @param name Name of the effect input
	 * @return Value of the effect input
	 */
	Vector4 get_input_float4(String name) const;

	/**
	 * @brief Return the type of an effect input or `-1` if the effect input does not exist.
	 *
	 * @param name Name of the effect input
	 * @return Type index
	 */
	int get_input_type(String name) const;

	/**
	 * @brief Return names of available effect inputs.
	 *
	 * @return Array with effect input names
	 */
	TypedArray<String> get_input_names() const;

	/**
	 * @brief Activate trigger @p name.
	 *
	 * @param name Name of the trigger
	 */
	void activate_trigger(String name);

	/**
	 * @brief Return whether trigger @p name was activated.
	 *
	 * @param name Name of the trigger
	 * @return `true` if the trigger was activated
	 */
	bool is_trigger_activated(String name) const;

	/**
	 * @brief Generate @p count particles of the given type from the given emitter.
	 *
	 * @param particleEmitterName Name of the particle emitter
	 * @param particleTypeName Name of the particle type
	 * @param count Number of particles to generate
	 */
	void spawn_particles(String particleEmitterName, String particleTypeName, int count);

	/**
	 * @brief Return the node with the given name.
	 *
	 * @param name Node name
	 * @return Node or `null` if no node with this name exists
	 */
	Ref<PixelpartNode> find_node(String name) const;

	/**
	 * @brief Return the node with the given ID.
	 *
	 * @param id Node ID
	 * @return Node or `null` if no node with this ID exists
	 */
	Ref<PixelpartNode> get_node(int id) const;

	/**
	 * @brief Return the node at the given index.
	 *
	 * @param index Node index, starting from `0`
	 * @return Node or `null` if no node at this index exists
	 */
	Ref<PixelpartNode> get_node_at_index(int index) const;

	/**
	 * @brief Return the particle type with the given name.
	 *
	 * @param name Particle type name
	 * @return Particle type or `null` if no particle type with this name exists
	 */
	Ref<PixelpartParticleType> find_particle_type(String name) const;

	/**
	 * @brief Return the particle type with the given ID.
	 *
	 * @param id Particle type ID
	 * @return Particle type or `null` if no particle type with this ID exists
	 */
	Ref<PixelpartParticleType> get_particle_type(int id) const;

	/**
	 * @brief Return the particle type at the given index.
	 *
	 * @param index Particle type index
	 * @return Particle type or `null` if no particle type at this index exists
	 */
	Ref<PixelpartParticleType> get_particle_type_at_index(int index) const;

	/**
	 * @brief Return the particle emitter with the given name.
	 *
	 * @deprecated Use `find_node` instead.
	 *
	 * @param name Node name
	 * @return Node or `null` if no node with this name exists
	 */
	Ref<PixelpartParticleEmitter> find_particle_emitter(String name) const;

	/**
	 * @brief Return the force field with the given name.
	 *
	 * @deprecated Use `find_node` instead.
	 *
	 * @param name Node name
	 * @return Node or `null` if no node with this name exists
	 */
	Ref<PixelpartForceField> find_force_field(String name) const;

	/**
	 * @brief Return the collider with the given name.
	 *
	 * @deprecated Use `find_node` instead.
	 *
	 * @param name Node name
	 * @return Node or `null` if no node with this name exists
	 */
	Ref<PixelpartCollider> find_collider(String name) const;

	/**
	 * @brief Return the particle emitter with the given ID.
	 *
	 * @deprecated Use `get_node` instead.
	 *
	 * @param id Node ID
	 * @return Node or `null` if no node with this ID exists
	 */
	Ref<PixelpartParticleEmitter> get_particle_emitter(int id) const;

	/**
	 * @brief Return the force field with the given ID.
	 *
	 * @deprecated Use `get_node` instead.
	 *
	 * @param id Node ID
	 * @return Node or `null` if no node with this ID exists
	 */
	Ref<PixelpartForceField> get_force_field(int id) const;

	/**
	 * @brief Return the collider with the given ID.
	 *
	 * @deprecated Use `get_node` instead.
	 *
	 * @param id Node ID
	 * @return Node or `null` if no node with this ID exists
	 */
	Ref<PixelpartCollider> get_collider(int id) const;

	/**
	 * @brief Return the particle emitter at the given index.
	 *
	 * @deprecated Use `get_node_at_index` instead.
	 *
	 * @param index Node index
	 * @return Node or `null` if no node at this index exists
	 */
	Ref<PixelpartParticleEmitter> get_particle_emitter_at_index(int index) const;

	/**
	 * @brief Return the force field at the given index.
	 *
	 * @deprecated Use `get_node_at_index` instead.
	 *
	 * @param index Node index
	 * @return Node or `null` if no node at this index exists
	 */
	Ref<PixelpartForceField> get_force_field_at_index(int index) const;

	/**
	 * @brief Return the collider at the given index.
	 *
	 * @deprecated Use `get_node_at_index` instead.
	 *
	 * @param index Node index
	 * @return Node or `null` if no node at this index exists
	 */
	Ref<PixelpartCollider> get_collider_at_index(int index) const;

protected:
	static void _bind_methods();

private:
	void update_transform();

	Ref<PixelpartEffectResource> effectResource;
	PixelpartEffectRuntime effectRuntime;

	float effectScale = 1.0f;

	PixelpartGraphicsResourceProvider graphicsResourceProvider;
	std::unordered_map<pixelpart::ParticleRuntimeId, std::unique_ptr<PixelpartParticleRenderer3D>> particleRenderers;
};
}

#endif
