#include "PixelpartStaticPropertyFloat.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartStaticPropertyFloat::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get"), &PixelpartStaticPropertyFloat::get);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &PixelpartStaticPropertyFloat::set_value);
	ClassDB::bind_method(D_METHOD("get_value"), &PixelpartStaticPropertyFloat::get_value);
}

PixelpartStaticPropertyFloat::PixelpartStaticPropertyFloat() {

}

void PixelpartStaticPropertyFloat::init(pixelpart::StaticProperty<pixelpart::float_t>* prop) {
	property = prop;
}

float PixelpartStaticPropertyFloat::get() const {
	if(property) {
		return toGd(property->get());
	}

	return 0.0f;
}

void PixelpartStaticPropertyFloat::set_value(float value) {
	if(property) {
		property->setValue(fromGd(value));
	}
}
float PixelpartStaticPropertyFloat::get_value() const {
	if(property) {
		return toGd(property->getValue());
	}

	return 0.0f;
}
}