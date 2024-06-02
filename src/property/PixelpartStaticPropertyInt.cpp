#include "PixelpartStaticPropertyInt.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartStaticPropertyInt::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get"), &PixelpartStaticPropertyInt::get);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &PixelpartStaticPropertyInt::set_value);
	ClassDB::bind_method(D_METHOD("get_value"), &PixelpartStaticPropertyInt::get_value);
}

PixelpartStaticPropertyInt::PixelpartStaticPropertyInt() {

}

void PixelpartStaticPropertyInt::init(pixelpart::StaticProperty<pixelpart::int_t>* prop) {
	property = prop;
}

int PixelpartStaticPropertyInt::get() const {
	if(property) {
		return toGd(property->get());
	}

	return 0;
}

void PixelpartStaticPropertyInt::set_value(int value) {
	if(property) {
		property->setValue(fromGd(value));
	}
}
int PixelpartStaticPropertyInt::get_value() const {
	if(property) {
		return toGd(property->getValue());
	}

	return 0;
}
}