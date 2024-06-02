#include "PixelpartStaticPropertyFloat4.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartStaticPropertyFloat4::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get"), &PixelpartStaticPropertyFloat4::get);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &PixelpartStaticPropertyFloat4::set_value);
	ClassDB::bind_method(D_METHOD("get_value"), &PixelpartStaticPropertyFloat4::get_value);
}

PixelpartStaticPropertyFloat4::PixelpartStaticPropertyFloat4() {

}

void PixelpartStaticPropertyFloat4::init(pixelpart::StaticProperty<pixelpart::vec4_t>* prop) {
	property = prop;
}

Vector4 PixelpartStaticPropertyFloat4::get() const {
	if(property) {
		return toGd(property->get());
	}

	return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void PixelpartStaticPropertyFloat4::set_value(Vector4 value) {
	if(property) {
		property->setValue(fromGd(value));
	}
}
Vector4 PixelpartStaticPropertyFloat4::get_value() const {
	if(property) {
		return toGd(property->getValue());
	}

	return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}
}