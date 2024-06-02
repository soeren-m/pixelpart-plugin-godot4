#ifndef PIXELPART_ANIMATED_PROPERTY_FLOAT2_H
#define PIXELPART_ANIMATED_PROPERTY_FLOAT2_H

#include "engine/ParticleEngine.h"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace godot {
class PixelpartAnimatedPropertyFloat2 : public RefCounted {
	GDCLASS(PixelpartAnimatedPropertyFloat2, RefCounted)

public:
	static void _bind_methods();

	PixelpartAnimatedPropertyFloat2();

	void init(pixelpart::AnimatedProperty<pixelpart::vec2_t>* prop);

	Vector2 get(float position) const;

	void add_point(float position, Vector2 value);
	void remove_point(int index);
	void set_point(int index, Vector2 value);
	void set_point_position(int index, float position);
	void clear();

	bool contains_points() const;
	int get_num_points() const;
	Vector2 get_point(int index) const;

	int get_point_index(float position, float epsilon);

	void set_interpolation(int method);
	int get_interpolation() const;

	void enable_adaptive_cache();
	void enable_fixed_cache(int size);

private:
	pixelpart::AnimatedProperty<pixelpart::vec2_t>* property = nullptr;
};
}

#endif