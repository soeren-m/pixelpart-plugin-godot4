#ifndef PIXELPART_ANIMATED_PROPERTY_FLOAT2_H
#define PIXELPART_ANIMATED_PROPERTY_FLOAT2_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <pixelpart-runtime/common/Math.h>
#include <pixelpart-runtime/effect/AnimatedProperty.h>

namespace godot {
/**
 * @brief `Vector2` property animated with keyframes and affected by effect inputs.
 *
 * Scene objects of an effect like particle emitters and force fields have properties that change over time
 * and are affected by effect inputs. Such properties are represented by one of the `PixelpartAnimatedProperty` classes,
 * depending on the type of the property.
 */
class PixelpartAnimatedPropertyFloat2 : public RefCounted {
	GDCLASS(PixelpartAnimatedPropertyFloat2, RefCounted)

public:
	PixelpartAnimatedPropertyFloat2();

#ifdef DOXYGEN
	/**
	 * @brief Interpolation applied to the animation curve.
	 */
	int keyframe_interpolation;
#endif

	void init(pixelpart::AnimatedProperty<pixelpart::float2_t>* prop);

	/**
	 * @brief Return the (interpolated) value of the animation property at time @p position
	 *
	 * @param position Time between 0 and 1
	 * @return Value of the property
	 */
	Vector2 at(float position) const;

	/**
	 * @brief Add a keyframe at time @p position with value @p value.
	 *
	 * @param position Time between 0 and 1
	 * @param value Value of the property at the given time
	 */
	void add_keyframe(float position, Vector2 value);

	/**
	 * @brief Remove the keyframe with the given index from the animation.
	 *
	 * @param index Index to remove
	 */
	void remove_keyframe(int index);

	/**
	 * @brief Change the value of the keyframe with the given index.
	 *
	 * @param index Keyframe index
	 * @param value New value
	 */
	void set_keyframe_value(int index, Vector2 value);

	/**
	 * @brief Move the time of the keyframe with the given index to @p position.
	 *
	 * @param index Keyframe index
	 * @param position New time between 0 and 1
	 */
	void set_keyframe_position(int index, float position);

	/**
	 * @brief Remove all keyframes from the animation.
	 *
	 */
	void clear_keyframes();

	/**
	 * @brief Return whether the animation property contains any keyframes.
	 *
	 * @return Whether the animation property contains any keyframes
	 */
	bool contains_keyframes() const;

	/**
	 * @brief Return the number of keyframes.
	 *
	 * @return Number of keyframes
	 */
	int get_keyframe_count() const;

	/**
	 * @brief Return the value of the keyframe with the given index.
	 *
	 * @param index Keyframe index
	 * @return Keyframe value
	 */
	Vector2 get_keyframe_value(int index) const;

	/**
	 * @brief Return the index of the keyframe closest to time @p position.
	 *
	 * @param position Time between 0 and 1
	 * @param epsilon Maximum differerence in time to consider to keyframes at the same time
	 * @return Keyframe index
	 */
	int get_keyframe_index(float position, float epsilon);

	/**
	 * @brief Set interpolation applied to the animation curve.
	 *
	 * @param method Interpolation method
	 */
	void set_keyframe_interpolation(int method);

	/**
	 * @brief Return interpolation applied to the animation curve.
	 *
	 * @return Interpolation method
	 */
	int get_keyframe_interpolation() const;

	/**
	 * @brief Enable an adaptive cache, which resizes itself automatically if the number of keyframes increases or decreases a lot.
	 *
	 */
	void enable_adaptive_cache();

	/**
	 * @brief Enable a fixed cache with the given size, which only stores up to @p size different value.
	 *
	 * @param size Cache size
	 */
	void enable_fixed_cache(int size);

protected:
	static void _bind_methods();

private:
	pixelpart::AnimatedProperty<pixelpart::float2_t>* property = nullptr;
};
}

#endif