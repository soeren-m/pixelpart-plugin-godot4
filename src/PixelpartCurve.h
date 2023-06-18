#ifndef PIXELPART_CURVE_H
#define PIXELPART_CURVE_H

#include "ParticleEngine.h"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace godot {
class PixelpartCurve : public RefCounted {
	GDCLASS(PixelpartCurve, RefCounted)

public:
	enum class ObjectType {
		none,
		particle_emitter,
		particle_type,
		force_field,
		collider
	};

	static void _bind_methods();

	PixelpartCurve();

	void init(pixelpart::Curve<pixelpart::floatd>* curve, pixelpart::ParticleEngine* engine, ObjectType type);

	float get(float t) const;
	float get_point(int index) const;

	void set(float value);
	void add_point(float t, float value);
	void set_point(int index, float value);
	void move_point(int index, float delta);
	void shift_point(int index, float delta);
	void remove_point(int index);
	void clear();
	int get_num_points() const;

	void move(float delta);
	void shift(float delta);

	void set_interpolation(int method);
	int get_interpolation() const;

	void enable_adaptive_cache();
	void enable_fixed_cache(int size);
	int get_cache_size() const;

private:
	void update_simulation();

	pixelpart::Curve<pixelpart::floatd>* nativeCurve = nullptr;
	pixelpart::ParticleEngine* nativeParticleEngine = nullptr;
	ObjectType objectType = ObjectType::none;
};
}

#endif