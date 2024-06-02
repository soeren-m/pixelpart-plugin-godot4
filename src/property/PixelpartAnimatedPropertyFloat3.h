#ifndef PIXELPART_ANIMATED_PROPERTY_FLOAT3_H
#define PIXELPART_ANIMATED_PROPERTY_FLOAT3_H

#include "engine/ParticleEngine.h"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace godot {
class PixelpartAnimatedPropertyFloat3 : public RefCounted {
	GDCLASS(PixelpartAnimatedPropertyFloat3, RefCounted)

public:
	static void _bind_methods();

	PixelpartAnimatedPropertyFloat3();

	void init(pixelpart::AnimatedProperty<pixelpart::vec3_t>* prop);

	Vector3 get(float position) const;

	void add_point(float position, Vector3 value);
	void remove_point(int index);
	void set_point(int index, Vector3 value);
	void set_point_position(int index, float position);
	void clear();

	bool contains_points() const;
	int get_num_points() const;
	Vector3 get_point(int index) const;

	int get_point_index(float position, float epsilon);

	void set_interpolation(int method);
	int get_interpolation() const;

	void enable_adaptive_cache();
	void enable_fixed_cache(int size);

private:
	pixelpart::AnimatedProperty<pixelpart::vec3_t>* property = nullptr;
};
}

#endif