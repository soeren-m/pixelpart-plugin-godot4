#include "PixelpartStaticPropertyFloat2.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartStaticPropertyFloat2::PixelpartStaticPropertyFloat2() {

}

void PixelpartStaticPropertyFloat2::init(pixelpart::StaticProperty<pixelpart::float2_t>* prop) {
	property = prop;
}

Vector2 PixelpartStaticPropertyFloat2::value() const {
	if(property) {
		return pxpt_to_gd(property->value());
	}

	return Vector2(0.0f, 0.0f);
}

void PixelpartStaticPropertyFloat2::set_base_value(Vector2 value) {
	if(property) {
		property->baseValue(gd_to_pxpt(value));
	}
}
Vector2 PixelpartStaticPropertyFloat2::get_base_value() const {
	if(property) {
		return pxpt_to_gd(property->baseValue());
	}

	return Vector2(0.0f, 0.0f);
}

void PixelpartStaticPropertyFloat2::_bind_methods() {
	ClassDB::bind_method(D_METHOD("value"), &PixelpartStaticPropertyFloat2::value);
	ClassDB::bind_method(D_METHOD("set_base_value", "value"), &PixelpartStaticPropertyFloat2::set_base_value);
	ClassDB::bind_method(D_METHOD("get_base_value"), &PixelpartStaticPropertyFloat2::get_base_value);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "base_value"), "set_base_value", "get_base_value");

	// Deprecated
	ClassDB::bind_method(D_METHOD("get"), &PixelpartStaticPropertyFloat2::value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &PixelpartStaticPropertyFloat2::set_base_value);
	ClassDB::bind_method(D_METHOD("get_value"), &PixelpartStaticPropertyFloat2::get_base_value);
}
}
