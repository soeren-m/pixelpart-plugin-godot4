#ifndef PIXELPART_FORCE_FIELD_H
#define PIXELPART_FORCE_FIELD_H

#include "PixelpartEffectResource.h"
#include "property/PixelpartStaticPropertyInt.h"
#include "property/PixelpartStaticPropertyFloat.h"
#include "property/PixelpartAnimatedPropertyFloat.h"
#include "property/PixelpartAnimatedPropertyFloat3.h"

namespace godot {
class PixelpartForceField : public RefCounted {
	GDCLASS(PixelpartForceField, RefCounted)

public:
	static void _bind_methods();

	PixelpartForceField();

	void init(Ref<PixelpartEffectResource> resource, pixelpart::ForceField* forceFieldPtr, pixelpart::ParticleEngine* particleEnginePtr);

	int get_id() const;
	String get_name() const;

	void set_lifetime_start(float time);
	void set_lifetime_duration(float time);
	void set_repeat(bool value);
	float get_lifetime_start() const;
	float get_lifetime_duration() const;
	bool get_repeat() const;
	bool is_active() const;
	float get_local_time() const;

	Ref<PixelpartAnimatedPropertyFloat3> get_position() const;

	void set_type(int type);
	int get_type() const;

	Ref<PixelpartAnimatedPropertyFloat3> get_size() const;
	Ref<PixelpartAnimatedPropertyFloat3> get_orientation() const;

	Ref<PixelpartAnimatedPropertyFloat> get_strength() const;

	Ref<PixelpartAnimatedPropertyFloat3> get_acceleration_direction() const;
	Ref<PixelpartAnimatedPropertyFloat> get_acceleration_direction_variance() const;
	Ref<PixelpartAnimatedPropertyFloat> get_acceleration_strength_variance() const;
	void set_acceleration_grid_size(int width, int height, int depth);
	int get_acceleration_grid_width() const;
	int get_acceleration_grid_height() const;
	int get_acceleration_grid_depth() const;

	void set_vector_filter(int filter);
	int get_vector_filter() const;
	Ref<PixelpartAnimatedPropertyFloat> get_vector_tightness() const;

	Ref<PixelpartStaticPropertyInt> get_noise_octaves() const;
	Ref<PixelpartAnimatedPropertyFloat> get_noise_frequency() const;
	Ref<PixelpartAnimatedPropertyFloat> get_noise_persistence() const;
	Ref<PixelpartAnimatedPropertyFloat> get_noise_lacunarity() const;
	void set_noise_animated(bool animated);
	bool get_noise_animated() const;
	Ref<PixelpartStaticPropertyFloat> get_noise_animation_time_scale() const;
	Ref<PixelpartStaticPropertyFloat> get_noise_animation_time_base() const;

	Ref<PixelpartStaticPropertyFloat> get_drag_velocity_influence() const;
	Ref<PixelpartStaticPropertyFloat> get_drag_size_influence() const;

private:
	Ref<PixelpartEffectResource> effectResource;
	pixelpart::ForceField* forceField = nullptr;
	pixelpart::ParticleEngine* particleEngine = nullptr;
	std::mt19937 rng;
};
}

#endif