#include "PixelpartStaticPropertyFloat3.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartStaticPropertyFloat3::PixelpartStaticPropertyFloat3() {

}

void PixelpartStaticPropertyFloat3::init(pixelpart::StaticProperty<pixelpart::float3_t>* prop) {
	property = prop;
}

Vector3 PixelpartStaticPropertyFloat3::value() const {
	if(property) {
		return pxpt_to_gd(property->value());
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}

void PixelpartStaticPropertyFloat3::set_base_value(Vector3 value) {
	if(property) {
		property->baseValue(gd_to_pxpt(value));
	}
}
Vector3 PixelpartStaticPropertyFloat3::get_base_value() const {
	if(property) {
		return pxpt_to_gd(property->baseValue());
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}

void PixelpartStaticPropertyFloat3::_bind_methods() {
	ClassDB::bind_method(D_METHOD("value"), &PixelpartStaticPropertyFloat3::value);
	ClassDB::bind_method(D_METHOD("set_base_value", "value"), &PixelpartStaticPropertyFloat3::set_base_value);
	ClassDB::bind_method(D_METHOD("get_base_value"), &PixelpartStaticPropertyFloat3::get_base_value);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "base_value"), "set_base_value", "get_base_value");

	// Deprecated
	ClassDB::bind_method(D_METHOD("get"), &PixelpartStaticPropertyFloat3::value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &PixelpartStaticPropertyFloat3::set_base_value);
	ClassDB::bind_method(D_METHOD("get_value"), &PixelpartStaticPropertyFloat3::get_base_value);
}
}
