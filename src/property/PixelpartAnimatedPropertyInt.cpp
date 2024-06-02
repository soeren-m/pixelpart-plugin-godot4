#include "PixelpartAnimatedPropertyInt.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartAnimatedPropertyInt::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get", "position"), &PixelpartAnimatedPropertyInt::get);
	ClassDB::bind_method(D_METHOD("add_point", "position", "value"), &PixelpartAnimatedPropertyInt::add_point);
	ClassDB::bind_method(D_METHOD("remove_point", "index"), &PixelpartAnimatedPropertyInt::remove_point);
	ClassDB::bind_method(D_METHOD("set_point", "index", "value"), &PixelpartAnimatedPropertyInt::set_point);
	ClassDB::bind_method(D_METHOD("set_point_position", "index", "position"), &PixelpartAnimatedPropertyInt::set_point_position);
	ClassDB::bind_method(D_METHOD("clear"), &PixelpartAnimatedPropertyInt::clear);
	ClassDB::bind_method(D_METHOD("contains_points"), &PixelpartAnimatedPropertyInt::contains_points);
	ClassDB::bind_method(D_METHOD("get_num_points"), &PixelpartAnimatedPropertyInt::get_num_points);
	ClassDB::bind_method(D_METHOD("get_point", "index"), &PixelpartAnimatedPropertyInt::get_point);
	ClassDB::bind_method(D_METHOD("get_point_index", "position", "epsilon"), &PixelpartAnimatedPropertyInt::get_point_index);
	ClassDB::bind_method(D_METHOD("set_interpolation", "method"), &PixelpartAnimatedPropertyInt::set_interpolation);
	ClassDB::bind_method(D_METHOD("get_interpolation"), &PixelpartAnimatedPropertyInt::get_interpolation);
	ClassDB::bind_method(D_METHOD("enable_adaptive_cache"), &PixelpartAnimatedPropertyInt::enable_adaptive_cache);
	ClassDB::bind_method(D_METHOD("enable_fixed_cache", "size"), &PixelpartAnimatedPropertyInt::enable_fixed_cache);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "interpolation"), "set_interpolation", "get_interpolation");
}

PixelpartAnimatedPropertyInt::PixelpartAnimatedPropertyInt() {

}

void PixelpartAnimatedPropertyInt::init(pixelpart::AnimatedProperty<pixelpart::int_t>* prop) {
	property = prop;
}

int PixelpartAnimatedPropertyInt::get(float position) const {
	if(property) {
		return toGd(property->get(static_cast<pixelpart::float_t>(position)));
	}

	return 0;
}

void PixelpartAnimatedPropertyInt::add_point(float position, int value) {
	if(property) {
		property->addPoint(static_cast<pixelpart::float_t>(position), fromGd(value));
	}
}
void PixelpartAnimatedPropertyInt::remove_point(int index) {
	if(property) {
		property->removePoint(static_cast<std::size_t>(std::max(index, 0)));
	}
}
void PixelpartAnimatedPropertyInt::set_point(int index, int value) {
	if(property) {
		property->setPoint(static_cast<std::size_t>(std::max(index, 0)), fromGd(value));
	}
}
void PixelpartAnimatedPropertyInt::set_point_position(int index, float position) {
	if(property) {
		property->setPointPosition(static_cast<std::size_t>(std::max(index, 0)), static_cast<pixelpart::float_t>(position));
	}
}
void PixelpartAnimatedPropertyInt::clear() {
	if(property) {
		property->clear();
	}
}

bool PixelpartAnimatedPropertyInt::contains_points() const {
	if(property) {
		return property->containsPoints();
	}

	return false;
}
int PixelpartAnimatedPropertyInt::get_num_points() const {
	if(property) {
		return static_cast<int>(property->getNumPoints());
	}

	return 0;
}
int PixelpartAnimatedPropertyInt::get_point(int index) const {
	if(property && index >= 0 && static_cast<std::size_t>(index) < property->getNumPoints()) {
		return toGd(property->getPoint(static_cast<std::size_t>(index)).value);
	}

	return 0;
}

int PixelpartAnimatedPropertyInt::get_point_index(float position, float epsilon) {
	if(property) {
		return static_cast<int>(property->getPointIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon)));
	}

	return -1;
}

void PixelpartAnimatedPropertyInt::set_interpolation(int method) {
	if(property) {
		property->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
	}
}
int PixelpartAnimatedPropertyInt::get_interpolation() const {
	if(property) {
		return static_cast<int>(property->getInterpolation());
	}

	return static_cast<int>(pixelpart::CurveInterpolation::none);
}

void PixelpartAnimatedPropertyInt::enable_adaptive_cache() {
	if(property) {
		property->enableAdaptiveCache();
	}
}
void PixelpartAnimatedPropertyInt::enable_fixed_cache(int size) {
	if(property) {
		property->enableFixedCache(static_cast<std::size_t>(std::max(size, 0)));
	}
}
}