#include "PixelpartStaticPropertyBool.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartStaticPropertyBool::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get"), &PixelpartStaticPropertyBool::get);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &PixelpartStaticPropertyBool::set_value);
	ClassDB::bind_method(D_METHOD("get_value"), &PixelpartStaticPropertyBool::get_value);
}

PixelpartStaticPropertyBool::PixelpartStaticPropertyBool() {

}

void PixelpartStaticPropertyBool::init(pixelpart::StaticProperty<bool>* prop) {
	property = prop;
}

bool PixelpartStaticPropertyBool::get() const {
	if(property) {
		return property->get();
	}

	return false;
}

void PixelpartStaticPropertyBool::set_value(bool value) {
	if(property) {
		property->setValue(value);
	}
}
bool PixelpartStaticPropertyBool::get_value() const {
	if(property) {
		return property->getValue();
	}

	return false;
}
}