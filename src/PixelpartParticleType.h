#ifndef PIXELPART_PARTICLE_TYPE_H
#define PIXELPART_PARTICLE_TYPE_H

#include "PixelpartEffectResource.h"
#include "property/PixelpartStaticPropertyFloat.h"
#include "property/PixelpartStaticPropertyFloat3.h"
#include "property/PixelpartStaticPropertyFloat4.h"
#include "property/PixelpartAnimatedPropertyFloat.h"
#include "property/PixelpartAnimatedPropertyFloat3.h"
#include "property/PixelpartAnimatedPropertyFloat4.h"

namespace godot {
class PixelpartParticleType : public RefCounted {
	GDCLASS(PixelpartParticleType, RefCounted)

public:
	static void _bind_methods();

	PixelpartParticleType();

	void init(Ref<PixelpartEffectResource> resource, pixelpart::ParticleType* particleTypePtr, pixelpart::ParticleEngine* particleEnginePtr);

	int get_id() const;
	int get_parent_id() const;
	String get_name() const;

	Ref<PixelpartAnimatedPropertyFloat3> get_position() const;

	Ref<PixelpartAnimatedPropertyFloat> get_num_particles() const;
	Ref<PixelpartAnimatedPropertyFloat> get_lifespan() const;
	Ref<PixelpartStaticPropertyFloat> get_lifespan_variance() const;

	void set_position_relative(bool mode);
	bool is_position_relative() const;

	Ref<PixelpartStaticPropertyFloat> get_motion_path_force() const;
	Ref<PixelpartAnimatedPropertyFloat> get_initial_velocity() const;
	Ref<PixelpartAnimatedPropertyFloat> get_inherited_velocity() const;
	Ref<PixelpartStaticPropertyFloat> get_velocity_variance() const;

	Ref<PixelpartAnimatedPropertyFloat> get_acceleration() const;
	Ref<PixelpartAnimatedPropertyFloat> get_radial_acceleration() const;

	void set_rotation_mode(int mode);
	int get_rotation_mode() const;

	void set_alignment_mode(int mode);
	int get_alignment_mode() const;

	Ref<PixelpartAnimatedPropertyFloat3> get_initial_rotation() const;
	Ref<PixelpartAnimatedPropertyFloat3> get_rotation() const;
	Ref<PixelpartAnimatedPropertyFloat3> get_rotation_by_speed() const;
	Ref<PixelpartStaticPropertyFloat3> get_rotation_variance() const;
	Ref<PixelpartStaticPropertyFloat3> get_angular_velocity_variance() const;

	Ref<PixelpartStaticPropertyFloat3> get_pivot() const;

	Ref<PixelpartAnimatedPropertyFloat> get_physical_size() const;
	Ref<PixelpartAnimatedPropertyFloat> get_weight() const;
	Ref<PixelpartAnimatedPropertyFloat> get_bounce() const;
	Ref<PixelpartAnimatedPropertyFloat> get_friction() const;

	void set_visible(bool mode);
	bool is_visible() const;

	void set_layer(int layer);
	int get_layer() const;

	Ref<PixelpartAnimatedPropertyFloat> get_initial_size() const;
	Ref<PixelpartAnimatedPropertyFloat3> get_size() const;
	Ref<PixelpartStaticPropertyFloat> get_size_variance() const;
	Ref<PixelpartAnimatedPropertyFloat3> get_stretch() const;

	Ref<PixelpartAnimatedPropertyFloat4> get_color() const;
	Ref<PixelpartStaticPropertyFloat4> get_color_variance() const;
	Ref<PixelpartAnimatedPropertyFloat> get_initial_opacity() const;
	Ref<PixelpartAnimatedPropertyFloat> get_opacity() const;
	Ref<PixelpartStaticPropertyFloat> get_opacity_variance() const;

private:
	Ref<PixelpartEffectResource> effectResource;
	pixelpart::ParticleType* particleType = nullptr;
	pixelpart::ParticleEngine* particleEngine = nullptr;
};
}

#endif