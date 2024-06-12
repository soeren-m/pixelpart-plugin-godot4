#ifndef PIXELPART_ANIMATED_PROPERTY_FLOAT_H
#define PIXELPART_ANIMATED_PROPERTY_FLOAT_H

#include "engine/ParticleEngine.h"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace godot {
class PixelpartAnimatedPropertyFloat : public RefCounted {
	GDCLASS(PixelpartAnimatedPropertyFloat, RefCounted)

public:
	static void _bind_methods();

	PixelpartAnimatedPropertyFloat();

	void init(pixelpart::AnimatedProperty<pixelpart::float_t>* prop);

	float get(float position) const;

	void add_point(float position, float value);
	void remove_point(int index);
	void set_point(int index, float value);
	void set_point_position(int index, float position);
	void clear();

	bool contains_points() const;
	int get_num_points() const;
	float get_point(int index) const;

	int get_point_index(float position, float epsilon);

	void set_interpolation(int method);
	int get_interpolation() const;

	void enable_adaptive_cache();
	void enable_fixed_cache(int size);

private:
	pixelpart::AnimatedProperty<pixelpart::float_t>* property = nullptr;
};
}

#endif