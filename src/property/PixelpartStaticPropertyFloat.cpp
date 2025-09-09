#include "PixelpartStaticPropertyFloat.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartStaticPropertyFloat::PixelpartStaticPropertyFloat() {

}

void PixelpartStaticPropertyFloat::init(pixelpart::StaticProperty<pixelpart::float_t>* prop) {
	property = prop;
}

float PixelpartStaticPropertyFloat::value() const {
	if(property) {
		return pxpt_to_gd(property->value());
	}

	return 0.0f;
}

void PixelpartStaticPropertyFloat::set_base_value(float value) {
	if(property) {
		property->baseValue(gd_to_pxpt(value));
	}
}
float PixelpartStaticPropertyFloat::get_base_value() const {
	if(property) {
		return pxpt_to_gd(property->baseValue());
	}

	return 0.0f;
}

void PixelpartStaticPropertyFloat::_bind_methods() {
	ClassDB::bind_method(D_METHOD("value"), &PixelpartStaticPropertyFloat::value);
	ClassDB::bind_method(D_METHOD("set_base_value", "value"), &PixelpartStaticPropertyFloat::set_base_value);
	ClassDB::bind_method(D_METHOD("get_base_value"), &PixelpartStaticPropertyFloat::get_base_value);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "base_value"), "set_base_value", "get_base_value");

	// Deprecated
	ClassDB::bind_method(D_METHOD("get"), &PixelpartStaticPropertyFloat::value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &PixelpartStaticPropertyFloat::set_base_value);
	ClassDB::bind_method(D_METHOD("get_value"), &PixelpartStaticPropertyFloat::get_base_value);
}
}
