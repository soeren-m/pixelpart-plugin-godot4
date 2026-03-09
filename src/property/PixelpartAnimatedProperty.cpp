#include "PixelpartAnimatedProperty.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartAnimatedProperty::_bind_methods() {
	BIND_ENUM_CONSTANT(INTERPOLATION_STEP);
	BIND_ENUM_CONSTANT(INTERPOLATION_LINEAR);
	BIND_ENUM_CONSTANT(INTERPOLATION_SPLINE);
	BIND_ENUM_CONSTANT(INTERPOLATION_BEZIER);
}
}
