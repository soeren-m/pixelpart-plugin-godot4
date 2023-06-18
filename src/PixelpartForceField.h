#ifndef PIXELPART_FORCEFIELD_H
#define PIXELPART_FORCEFIELD_H

#include "PixelpartEffectResource.h"
#include "PixelpartCurve.h"
#include "PixelpartCurve3.h"
#include "ForceField.h"
#include "ParticleEngine.h"

namespace godot {
class PixelpartForceField : public RefCounted {
	GDCLASS(PixelpartForceField, RefCounted)

public:
	static void _bind_methods();

	PixelpartForceField();

	void init(Ref<PixelpartEffectResource> resource, pixelpart::ForceField* forceField, pixelpart::ParticleEngine* engine);

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

	Ref<PixelpartCurve3> get_position() const;

	void set_type(int type);
	int get_type() const;

	Ref<PixelpartCurve3> get_size() const;
	Ref<PixelpartCurve3> get_orientation() const;
	Ref<PixelpartCurve3> get_direction() const;
	Ref<PixelpartCurve> get_strength() const;

	void set_direction_variance(float variance);
	void set_strength_variance(float variance);
	float get_direction_variance() const;
	float get_strength_variance() const;

	void set_grid_size(int width, int height, int depth);
	int get_grid_width() const;
	int get_grid_height() const;
	int get_grid_depth() const;

private:
	Ref<PixelpartEffectResource> effectResource;
	pixelpart::ForceField* nativeForceField = nullptr;
	pixelpart::ParticleEngine* nativeParticleEngine = nullptr;
	std::mt19937 rng;
};
}

#endif