#include "PixelpartStaticPropertyFloat4.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartStaticPropertyFloat4::PixelpartStaticPropertyFloat4() {

}

void PixelpartStaticPropertyFloat4::init(pixelpart::StaticProperty<pixelpart::float4_t>* prop) {
	property = prop;
}

Vector4 PixelpartStaticPropertyFloat4::value() const {
	if(property) {
		return pxpt_to_gd(property->value());
	}

	return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void PixelpartStaticPropertyFloat4::set_base_value(Vector4 value) {
	if(property) {
		property->baseValue(gd_to_pxpt(value));
	}
}
Vector4 PixelpartStaticPropertyFloat4::get_base_value() const {
	if(property) {
		return pxpt_to_gd(property->baseValue());
	}

	return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void PixelpartStaticPropertyFloat4::_bind_methods() {
	ClassDB::bind_method(D_METHOD("value"), &PixelpartStaticPropertyFloat4::value);
	ClassDB::bind_method(D_METHOD("set_base_value", "value"), &PixelpartStaticPropertyFloat4::set_base_value);
	ClassDB::bind_method(D_METHOD("get_base_value"), &PixelpartStaticPropertyFloat4::get_base_value);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR4, "base_value"), "set_base_value", "get_base_value");

	// Deprecated
	ClassDB::bind_method(D_METHOD("get"), &PixelpartStaticPropertyFloat4::value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &PixelpartStaticPropertyFloat4::set_base_value);
	ClassDB::bind_method(D_METHOD("get_value"), &PixelpartStaticPropertyFloat4::get_base_value);
}
}