#ifndef PIXELPART_ANIMATED_PROPERTY_FLOAT_H
#define PIXELPART_ANIMATED_PROPERTY_FLOAT_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <pixelpart-runtime/common/Types.h>
#include <pixelpart-runtime/effect/AnimatedProperty.h>

namespace godot {
class PixelpartAnimatedPropertyFloat : public RefCounted {
	GDCLASS(PixelpartAnimatedPropertyFloat, RefCounted)

public:
	PixelpartAnimatedPropertyFloat();

	void init(pixelpart::AnimatedProperty<pixelpart::float_t>* prop);

	float at(float position) const;

	void add_keyframe(float position, float value);
	void remove_keyframe(int index);
	void set_keyframe_value(int index, float value);
	void set_keyframe_position(int index, float position);
	void clear_keyframes();

	bool contains_keyframes() const;
	int get_keyframe_count() const;
	float get_keyframe_value(int index) const;

	int get_keyframe_index(float position, float epsilon);

	void set_keyframe_interpolation(int method);
	int get_keyframe_interpolation() const;

	void enable_adaptive_cache();
	void enable_fixed_cache(int size);

protected:
	static void _bind_methods();

private:
	pixelpart::AnimatedProperty<pixelpart::float_t>* property = nullptr;
};
}

#endif