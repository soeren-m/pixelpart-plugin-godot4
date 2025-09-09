#include "PixelpartStaticPropertyInt.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartStaticPropertyInt::PixelpartStaticPropertyInt() {

}

void PixelpartStaticPropertyInt::init(pixelpart::StaticProperty<pixelpart::int_t>* prop) {
	property = prop;
}

int PixelpartStaticPropertyInt::value() const {
	if(property) {
		return pxpt_to_gd(property->value());
	}

	return 0;
}

void PixelpartStaticPropertyInt::set_base_value(int value) {
	if(property) {
		property->baseValue(gd_to_pxpt(value));
	}
}
int PixelpartStaticPropertyInt::get_base_value() const {
	if(property) {
		return pxpt_to_gd(property->baseValue());
	}

	return 0;
}

void PixelpartStaticPropertyInt::_bind_methods() {
	ClassDB::bind_method(D_METHOD("value"), &PixelpartStaticPropertyInt::value);
	ClassDB::bind_method(D_METHOD("set_base_value", "value"), &PixelpartStaticPropertyInt::set_base_value);
	ClassDB::bind_method(D_METHOD("get_base_value"), &PixelpartStaticPropertyInt::get_base_value);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "base_value"), "set_base_value", "get_base_value");

	// Deprecated
	ClassDB::bind_method(D_METHOD("get"), &PixelpartStaticPropertyInt::value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &PixelpartStaticPropertyInt::set_base_value);
	ClassDB::bind_method(D_METHOD("get_value"), &PixelpartStaticPropertyInt::get_base_value);
}
}
