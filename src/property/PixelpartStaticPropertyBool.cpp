#include "PixelpartStaticPropertyBool.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartStaticPropertyBool::PixelpartStaticPropertyBool() {

}

void PixelpartStaticPropertyBool::init(pixelpart::StaticProperty<bool>* prop) {
	property = prop;
}

bool PixelpartStaticPropertyBool::value() const {
	if(property) {
		return property->value();
	}

	return false;
}

void PixelpartStaticPropertyBool::set_base_value(bool value) {
	if(property) {
		property->baseValue(value);
	}
}
bool PixelpartStaticPropertyBool::get_base_value() const {
	if(property) {
		return property->baseValue();
	}

	return false;
}

void PixelpartStaticPropertyBool::_bind_methods() {
	ClassDB::bind_method(D_METHOD("value"), &PixelpartStaticPropertyBool::value);
	ClassDB::bind_method(D_METHOD("set_base_value", "value"), &PixelpartStaticPropertyBool::set_base_value);
	ClassDB::bind_method(D_METHOD("get_base_value"), &PixelpartStaticPropertyBool::get_base_value);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "base_value"), "set_base_value", "get_base_value");

	// Deprecated
	ClassDB::bind_method(D_METHOD("get"), &PixelpartStaticPropertyBool::value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &PixelpartStaticPropertyBool::set_base_value);
	ClassDB::bind_method(D_METHOD("get_value"), &PixelpartStaticPropertyBool::get_base_value);
}
}