#include "PixelpartStaticPropertyFloat2.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartStaticPropertyFloat2::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get"), &PixelpartStaticPropertyFloat2::get);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &PixelpartStaticPropertyFloat2::set_value);
	ClassDB::bind_method(D_METHOD("get_value"), &PixelpartStaticPropertyFloat2::get_value);
}

PixelpartStaticPropertyFloat2::PixelpartStaticPropertyFloat2() {

}

void PixelpartStaticPropertyFloat2::init(pixelpart::StaticProperty<pixelpart::vec2_t>* prop) {
	property = prop;
}

Vector2 PixelpartStaticPropertyFloat2::get() const {
	if(property) {
		return toGd(property->get());
	}

	return Vector2(0.0f, 0.0f);
}

void PixelpartStaticPropertyFloat2::set_value(Vector2 value) {
	if(property) {
		property->setValue(fromGd(value));
	}
}
Vector2 PixelpartStaticPropertyFloat2::get_value() const {
	if(property) {
		return toGd(property->getValue());
	}

	return Vector2(0.0f, 0.0f);
}
}