#ifndef PIXELPART_CURVE3_H
#define PIXELPART_CURVE3_H

#include "ParticleEngine.h"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace godot {
class PixelpartCurve3 : public RefCounted {
	GDCLASS(PixelpartCurve3, RefCounted)

public:
	enum class ObjectType {
		none,
		particle_emitter,
		particle_type,
		force_field,
		collider
	};

	static void _bind_methods();

	PixelpartCurve3();

	void init(pixelpart::Curve<pixelpart::vec3d>* curve, pixelpart::ParticleEngine* engine, ObjectType type);

	Vector3 get(float t) const;
	Vector3 get_point(int index) const;

	void set(Vector3 value);
	void add_point(float t, Vector3 value);
	void set_point(int index, Vector3 value);
	void move_point(int index, Vector3 delta);
	void shift_point(int index, float delta);
	void remove_point(int index);
	void clear();
	int get_num_points() const;

	void move(Vector3 delta);
	void shift(float delta);

	void set_interpolation(int method);
	int get_interpolation() const;

	void enable_adaptive_cache();
	void enable_fixed_cache(int size);
	int get_cache_size() const;

private:
	void update_simulation();

	pixelpart::Curve<pixelpart::vec3d>* nativeCurve = nullptr;
	pixelpart::ParticleEngine* nativeParticleEngine = nullptr;
	ObjectType objectType = ObjectType::none;
};
}

#endif