#ifndef PIXELPART_ANIMATED_PROPERTY_FLOAT4_H
#define PIXELPART_ANIMATED_PROPERTY_FLOAT4_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <pixelpart-runtime/common/Math.h>
#include <pixelpart-runtime/effect/AnimatedProperty.h>

namespace godot {
class PixelpartAnimatedPropertyFloat4 : public RefCounted {
	GDCLASS(PixelpartAnimatedPropertyFloat4, RefCounted)

public:
	PixelpartAnimatedPropertyFloat4();

	void init(pixelpart::AnimatedProperty<pixelpart::float4_t>* prop);

	Vector4 at(float position) const;

	void add_keyframe(float position, Vector4 value);
	void remove_keyframe(int index);
	void set_keyframe_value(int index, Vector4 value);
	void set_keyframe_position(int index, float position);
	void clear_keyframes();

	bool contains_keyframes() const;
	int get_keyframe_count() const;
	Vector4 get_keyframe_value(int index) const;

	int get_keyframe_index(float position, float epsilon);

	void set_keyframe_interpolation(int method);
	int get_keyframe_interpolation() const;

	void enable_adaptive_cache();
	void enable_fixed_cache(int size);

protected:
	static void _bind_methods();

private:
	pixelpart::AnimatedProperty<pixelpart::float4_t>* property = nullptr;
};
}

#endif