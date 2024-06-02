#include "PixelpartStaticPropertyFloat3.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartStaticPropertyFloat3::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get"), &PixelpartStaticPropertyFloat3::get);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &PixelpartStaticPropertyFloat3::set_value);
	ClassDB::bind_method(D_METHOD("get_value"), &PixelpartStaticPropertyFloat3::get_value);
}

PixelpartStaticPropertyFloat3::PixelpartStaticPropertyFloat3() {

}

void PixelpartStaticPropertyFloat3::init(pixelpart::StaticProperty<pixelpart::vec3_t>* prop) {
	property = prop;
}

Vector3 PixelpartStaticPropertyFloat3::get() const {
	if(property) {
		return toGd(property->get());
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}

void PixelpartStaticPropertyFloat3::set_value(Vector3 value) {
	if(property) {
		property->setValue(fromGd(value));
	}
}
Vector3 PixelpartStaticPropertyFloat3::get_value() const {
	if(property) {
		return toGd(property->getValue());
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}
}