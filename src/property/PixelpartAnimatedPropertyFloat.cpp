#include "PixelpartAnimatedPropertyFloat.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartAnimatedPropertyFloat::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get", "position"), &PixelpartAnimatedPropertyFloat::get);
	ClassDB::bind_method(D_METHOD("add_point", "position", "value"), &PixelpartAnimatedPropertyFloat::add_point);
	ClassDB::bind_method(D_METHOD("remove_point", "index"), &PixelpartAnimatedPropertyFloat::remove_point);
	ClassDB::bind_method(D_METHOD("set_point", "index", "value"), &PixelpartAnimatedPropertyFloat::set_point);
	ClassDB::bind_method(D_METHOD("set_point_position", "index", "position"), &PixelpartAnimatedPropertyFloat::set_point_position);
	ClassDB::bind_method(D_METHOD("clear"), &PixelpartAnimatedPropertyFloat::clear);
	ClassDB::bind_method(D_METHOD("contains_points"), &PixelpartAnimatedPropertyFloat::contains_points);
	ClassDB::bind_method(D_METHOD("get_num_points"), &PixelpartAnimatedPropertyFloat::get_num_points);
	ClassDB::bind_method(D_METHOD("get_point", "index"), &PixelpartAnimatedPropertyFloat::get_point);
	ClassDB::bind_method(D_METHOD("get_point_index", "position", "epsilon"), &PixelpartAnimatedPropertyFloat::get_point_index);
	ClassDB::bind_method(D_METHOD("set_interpolation", "method"), &PixelpartAnimatedPropertyFloat::set_interpolation);
	ClassDB::bind_method(D_METHOD("get_interpolation"), &PixelpartAnimatedPropertyFloat::get_interpolation);
	ClassDB::bind_method(D_METHOD("enable_adaptive_cache"), &PixelpartAnimatedPropertyFloat::enable_adaptive_cache);
	ClassDB::bind_method(D_METHOD("enable_fixed_cache", "size"), &PixelpartAnimatedPropertyFloat::enable_fixed_cache);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "interpolation"), "set_interpolation", "get_interpolation");
}

PixelpartAnimatedPropertyFloat::PixelpartAnimatedPropertyFloat() {

}

void PixelpartAnimatedPropertyFloat::init(pixelpart::AnimatedProperty<pixelpart::float_t>* prop) {
	property = prop;
}

float PixelpartAnimatedPropertyFloat::get(float position) const {
	if(property) {
		return toGd(property->get(static_cast<pixelpart::float_t>(position)));
	}

	return 0.0f;
}

void PixelpartAnimatedPropertyFloat::add_point(float position, float value) {
	if(property) {
		property->addPoint(static_cast<pixelpart::float_t>(position), fromGd(value));
	}
}
void PixelpartAnimatedPropertyFloat::remove_point(int index) {
	if(property) {
		property->removePoint(static_cast<std::size_t>(std::max(index, 0)));
	}
}
void PixelpartAnimatedPropertyFloat::set_point(int index, float value) {
	if(property) {
		property->setPoint(static_cast<std::size_t>(std::max(index, 0)), fromGd(value));
	}
}
void PixelpartAnimatedPropertyFloat::set_point_position(int index, float position) {
	if(property) {
		property->setPointPosition(static_cast<std::size_t>(std::max(index, 0)), static_cast<pixelpart::float_t>(position));
	}
}
void PixelpartAnimatedPropertyFloat::clear() {
	if(property) {
		property->clear();
	}
}

bool PixelpartAnimatedPropertyFloat::contains_points() const {
	if(property) {
		return property->containsPoints();
	}

	return false;
}
int PixelpartAnimatedPropertyFloat::get_num_points() const {
	if(property) {
		return static_cast<int>(property->getNumPoints());
	}

	return 0;
}
float PixelpartAnimatedPropertyFloat::get_point(int index) const {
	if(property && index >= 0 && static_cast<std::size_t>(index) < property->getNumPoints()) {
		return toGd(property->getPoint(static_cast<std::size_t>(index)).value);
	}

	return 0.0f;
}

int PixelpartAnimatedPropertyFloat::get_point_index(float position, float epsilon) {
	if(property) {
		return static_cast<int>(property->getPointIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon)));
	}

	return -1;
}

void PixelpartAnimatedPropertyFloat::set_interpolation(int method) {
	if(property) {
		property->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
	}
}
int PixelpartAnimatedPropertyFloat::get_interpolation() const {
	if(property) {
		return static_cast<int>(property->getInterpolation());
	}

	return static_cast<int>(pixelpart::CurveInterpolation::none);
}

void PixelpartAnimatedPropertyFloat::enable_adaptive_cache() {
	if(property) {
		property->enableAdaptiveCache();
	}
}
void PixelpartAnimatedPropertyFloat::enable_fixed_cache(int size) {
	if(property) {
		property->enableFixedCache(static_cast<std::size_t>(std::max(size, 0)));
	}
}
}