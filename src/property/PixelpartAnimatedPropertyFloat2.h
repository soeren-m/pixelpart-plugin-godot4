#ifndef PIXELPART_ANIMATED_PROPERTY_FLOAT2_H
#define PIXELPART_ANIMATED_PROPERTY_FLOAT2_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <pixelpart-runtime/common/Math.h>
#include <pixelpart-runtime/effect/AnimatedProperty.h>

namespace godot {
class PixelpartAnimatedPropertyFloat2 : public RefCounted {
	GDCLASS(PixelpartAnimatedPropertyFloat2, RefCounted)

public:
	PixelpartAnimatedPropertyFloat2();

	void init(pixelpart::AnimatedProperty<pixelpart::float2_t>* prop);

	Vector2 at(float position) const;

	void add_keyframe(float position, Vector2 value);
	void remove_keyframe(int index);
	void set_keyframe_value(int index, Vector2 value);
	void set_keyframe_position(int index, float position);
	void clear_keyframes();

	bool contains_keyframes() const;
	int get_keyframe_count() const;
	Vector2 get_keyframe_value(int index) const;

	int get_keyframe_index(float position, float epsilon);

	void set_keyframe_interpolation(int method);
	int get_keyframe_interpolation() const;

	void enable_adaptive_cache();
	void enable_fixed_cache(int size);

protected:
	static void _bind_methods();

private:
	pixelpart::AnimatedProperty<pixelpart::float2_t>* property = nullptr;
};
}

#endif