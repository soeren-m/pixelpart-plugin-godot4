#ifndef PIXELPART_PARTICLE_TYPE_H
#define PIXELPART_PARTICLE_TYPE_H

#include "../property/PixelpartStaticPropertyFloat.h"
#include "../property/PixelpartStaticPropertyFloat3.h"
#include "../property/PixelpartStaticPropertyFloat4.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include "../property/PixelpartAnimatedPropertyFloat3.h"
#include "../property/PixelpartAnimatedPropertyFloat4.h"
#include <pixelpart-runtime/effect/ParticleType.h>
#include <pixelpart-runtime/engine/EffectEngine.h>
#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
/**
 * @brief Defines the appearance and behavior of particles.
 *
 * Each particle emitter has a list of assigned particle types, which it will use for generating particles.
 * The particle type defines how particles move through space (e.g. velocity, acceleration) and
 * how they look (e.g. size, color).
 *
 * @paragraph nesting Nested particle types
 * Particle types can be assigned a parent particle type. This causes a particle of the parent type
 * to emit particles of the child particle type.
 *
 * @paragraph renderers Particle renderers
 * There are three types of particles, which determines how they are rendered.
 * - Sprite particle: The sprite renderer renders each particle as a separate sprite.
 * - Trail particle: The trail renderer generates continuous trails along the path of particles
 * with the same parent particle.
 * - Mesh particle: The mesh renderer renders each particles as a three-dimensional mesh.
 */
class PixelpartParticleType : public RefCounted {
	GDCLASS(PixelpartParticleType, RefCounted)

public:
	PixelpartParticleType();
	virtual ~PixelpartParticleType();

#ifdef DOXYGEN
	/**
	 * @brief Whether the position of particles is tied to the position of the emitter.
	 */
	bool position_relative;

	/**
	 * @brief Whether the rotation property represents the exact rotation of particles
	 * or their angular velocity.
	 *
	 * Possible values:
	 * - ANGLE
	 * - VELOCITY
	 */
	int rotation_mode;

	/**
	 * @brief Direction to which particles are aligned.
	 *
	 * Possible values:
	 * - NONE
	 * - CAMERA
	 * - MOTION
	 * - EMISSION
	 * - EMITTER
	 */
	int alignment_mode;

	/**
	 * @brief Whether particles of this type are visible.
	 */
	bool visible;

	/**
	 * @brief Layer particles are drawn on.
	 *
	 * Particles on upper layers occlude those on lower layers.
	 */
	int layer;
#endif

	void init(pixelpart::ParticleType* particleTypePtr, pixelpart::EffectEngine* effectEnginePtr);

	/**
	 * @brief ID of the particle type.
	 *
	 * @return ID
	 */
	int get_id() const;

	/**
	 * @brief ID of the parent particle type.
	 *
	 * @return Parent ID or `-1` if the particle type has no parent
	 */
	int get_parent_id() const;

	/**
	 * @brief Name of the particle type.
	 *
	 * @return Name
	 */
	String get_name() const;

	/**
	 * @brief Set whether the position of particles is tied to the position of the emitter.
	 *
	 * @param mode Relative particle position
	 */
	void set_position_relative(bool mode);

	/**
	 * @brief Whether the position of particles is tied to the position of the emitter.
	 *
	 * @return Relative particle position
	 */
	bool is_position_relative() const;

	/**
	 * @brief Set whether the rotation property represents the exact rotation of particles
	 * or their angular velocity.
	 *
	 * @param mode Rotation mode
	 */
	void set_rotation_mode(int mode);

	/**
	 * @brief Whether the rotation property represents the exact rotation of particles
	 * or their angular velocity.
	 *
	 * @return Rotation mode
	 */
	int get_rotation_mode() const;

	/**
	 * @brief Set direction to which particles are aligned.
	 *
	 * @param mode Alignment mode
	 */
	void set_alignment_mode(int mode);

	/**
	 * @brief Direction to which particles are aligned.
	 *
	 * @return Alignment mode
	 */
	int get_alignment_mode() const;

	/**
	 * @brief Center of rotation relative to the particle’s center.
	 *
	 * @return Pivot property
	 */
	Ref<PixelpartStaticPropertyFloat3> get_pivot() const;

	/**
	 * @brief How much particles are influenced by the motion path.
	 *
	 * Set to `0` to disable motion paths.
	 *
	 * @return Motion path force property
	 */
	Ref<PixelpartStaticPropertyFloat> get_motion_path_force() const;

	/**
	 * @brief Set whether particles of this type are visible.
	 *
	 * @param mode Whether particles of this type are visible
	 */
	void set_visible(bool mode);

	/**
	 * @brief Whether particles of this type are visible.
	 *
	 * @return Whether particles of this type are visible
	 */
	bool is_visible() const;

	/**
	 * @brief Set layer particles are drawn on.
	 *
	 * Particles on upper layers occlude those on lower layers.
	 *
	 * @param layer Layer
	 */
	void set_layer(int layer);

	/**
	 * @brief Layer particles are drawn on.
	 *
	 * @return Layer
	 */
	int get_layer() const;

	/**
	 * @brief How many particles of this type are spawned by emitters.
	 *
	 * @return Count property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_count() const;

	/**
	 * @brief How long particles exist after being spawned by emitters.
	 *
	 * @return Lifespan property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_lifespan() const;

	/**
	 * @brief Size of each particle when created.
	 *
	 * @return Initial size property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_initial_size() const;

	/**
	 * @brief Rotation of each particle when created, in degrees.
	 *
	 * For 3D effects, rotation is composed of roll, yaw and pitch.
	 *
	 * @return Initial rotation property
	 */
	Ref<PixelpartAnimatedPropertyFloat3> get_initial_rotation() const;

	/**
	 * @brief How fast each particle is initially after being spawned by the emitter.
	 *
	 * @return Initial velocity property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_initial_velocity() const;

	/**
	 * @brief How much speed particles inherit from the parent particle or emitter.
	 *
	 * @return Inherited velocity property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_inherited_velocity() const;

	/**
	 * @brief Opacity of each particle when created.
	 *
	 * @return Initial opacity property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_initial_opacity() const;

	/**
	 * @brief Motion path for particles.
	 *
	 * @return Position property
	 */
	Ref<PixelpartAnimatedPropertyFloat3> get_position() const;

	/**
	 * @brief Size of each particle over its lifetime as a fraction of its initial size.
	 *
	 * @return Size property
	 */
	Ref<PixelpartAnimatedPropertyFloat3> get_size() const;

	/**
	 * @brief How much particles are stretched based on their speed.
	 *
	 * @return Stretch property
	 */
	Ref<PixelpartAnimatedPropertyFloat3> get_stretch() const;

	/**
	 * @brief Relative size of each particle for motion and collision calculations over its lifetime.
	 *
	 * @return Physical size property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_physical_size() const;

	/**
	 * @brief How particles rotate over time from their initial rotation, in degrees.
	 *
	 * For 3D effects, rotation is composed of roll, yaw and pitch.
	 *
	 * @return Rotation property
	 */
	Ref<PixelpartAnimatedPropertyFloat3> get_rotation() const;

	/**
	 * @brief How much particles rotate based on their velocity, in degrees.
	 *
	 * @return Rotation by speed property
	 */
	Ref<PixelpartAnimatedPropertyFloat3> get_rotation_by_speed() const;

	/**
	 * @brief Linear acceleration applied to each particle over its lifetime.
	 *
	 * @return Acceleration property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_acceleration() const;

	/**
	 * @brief Acceleration of each particle towards the emitter over its lifetime.
	 *
	 * @return Radial acceleration property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_radial_acceleration() const;

	/**
	 * @brief How strongly each particle is affected by force fields over its lifetime.
	 *
	 * @return Weight property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_weight() const;

	/**
	 * @brief How much each particle bounces back from colliders over its lifetime.
	 *
	 * @return Bounce property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_bounce() const;

	/**
	 * @brief How resistant each particle is to sliding down colliders over its lifetime.
	 *
	 * @return Friction property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_friction() const;

	/**
	 * @brief Color of each particle over its lifetime.
	 *
	 * @return Color property
	 */
	Ref<PixelpartAnimatedPropertyFloat4> get_color() const;

	/**
	 * @brief Opacity of each particle over its lifetime as a fraction of its initial opacity.
	 *
	 * @return Opacity property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_opacity() const;

	/**
	 * @brief How much the lifespan varies between particles.
	 *
	 * @return Lifespan variance property
	 */
	Ref<PixelpartStaticPropertyFloat> get_lifespan_variance() const;

	/**
	 * @brief How much the size varies between particles.
	 *
	 * @return Size variance property
	 */
	Ref<PixelpartStaticPropertyFloat> get_size_variance() const;

	/**
	 * @brief How much the rotation varies between particles, in degrees.
	 *
	 * @return Rotation variance property
	 */
	Ref<PixelpartStaticPropertyFloat3> get_rotation_variance() const;

	/**
	 * @brief How much the angular velocity varies between particles, in degrees/second.
	 *
	 * @return Angular velocity variance property
	 */
	Ref<PixelpartStaticPropertyFloat3> get_angular_velocity_variance() const;

	/**
	 * @brief How much the speed varies between particles.
	 *
	 * @return Velocity variance property
	 */
	Ref<PixelpartStaticPropertyFloat> get_velocity_variance() const;

	/**
	 * @brief How much the color varies between particles.
	 *
	 * @return Color variance property
	 */
	Ref<PixelpartStaticPropertyFloat4> get_color_variance() const;

	/**
	 * @brief How much the opacity varies between particles.
	 *
	 * @return Opacity variance property
	 */
	Ref<PixelpartStaticPropertyFloat> get_opacity_variance() const;

protected:
	static void _bind_methods();

private:
	pixelpart::ParticleType* particleType = nullptr;
	pixelpart::EffectEngine* effectEngine = nullptr;
};
}

#endif
