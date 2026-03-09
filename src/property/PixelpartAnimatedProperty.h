#ifndef PIXELPART_ANIMATED_PROPERTY_H
#define PIXELPART_ANIMATED_PROPERTY_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace godot {
/**
 * @brief Base class for properties animated with keyframes and affected by effect inputs.
 *
 * Scene objects of an effect like particle emitters and force fields have properties that change over time
 * and are affected by effect inputs. Such properties are represented by one of the `PixelpartAnimatedProperty` classes,
 * depending on the type of the property.
 */
class PixelpartAnimatedProperty : public RefCounted {
	GDCLASS(PixelpartAnimatedProperty, RefCounted)

public:
	/**
	 * @brief Keyframe interpolation types.
	 */
	enum InterpolationType {
		/**
		 * Values change abruptly between keyframes.
		 */
		INTERPOLATION_STEP = 0,

		/**
		 * Values are interpolated linearly.
		 */
		INTERPOLATION_LINEAR = 1,

		/**
		 * Values are interpolated with Spline interpolation producing smooth transitions.
		 */
		INTERPOLATION_SPLINE = 2,

		/**
		 * Values are interpolated with Bezier interpolation producing smooth transitions.
		 */
		INTERPOLATION_BEZIER = 3
	};

protected:
	static void _bind_methods();
};
}

VARIANT_ENUM_CAST(PixelpartAnimatedProperty::InterpolationType);

#endif
