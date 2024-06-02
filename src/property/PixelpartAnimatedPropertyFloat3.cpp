#include "PixelpartAnimatedPropertyFloat3.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartAnimatedPropertyFloat3::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get", "position"), &PixelpartAnimatedPropertyFloat3::get);
	ClassDB::bind_method(D_METHOD("add_point", "position", "value"), &PixelpartAnimatedPropertyFloat3::add_point);
	ClassDB::bind_method(D_METHOD("remove_point", "index"), &PixelpartAnimatedPropertyFloat3::remove_point);
	ClassDB::bind_method(D_METHOD("set_point", "index", "value"), &PixelpartAnimatedPropertyFloat3::set_point);
	ClassDB::bind_method(D_METHOD("set_point_position", "index", "position"), &PixelpartAnimatedPropertyFloat3::set_point_position);
	ClassDB::bind_method(D_METHOD("clear"), &PixelpartAnimatedPropertyFloat3::clear);
	ClassDB::bind_method(D_METHOD("contains_points"), &PixelpartAnimatedPropertyFloat3::contains_points);
	ClassDB::bind_method(D_METHOD("get_num_points"), &PixelpartAnimatedPropertyFloat3::get_num_points);
	ClassDB::bind_method(D_METHOD("get_point", "index"), &PixelpartAnimatedPropertyFloat3::get_point);
	ClassDB::bind_method(D_METHOD("get_point_index", "position", "epsilon"), &PixelpartAnimatedPropertyFloat3::get_point_index);
	ClassDB::bind_method(D_METHOD("set_interpolation", "method"), &PixelpartAnimatedPropertyFloat3::set_interpolation);
	ClassDB::bind_method(D_METHOD("get_interpolation"), &PixelpartAnimatedPropertyFloat3::get_interpolation);
	ClassDB::bind_method(D_METHOD("enable_adaptive_cache"), &PixelpartAnimatedPropertyFloat3::enable_adaptive_cache);
	ClassDB::bind_method(D_METHOD("enable_fixed_cache", "size"), &PixelpartAnimatedPropertyFloat3::enable_fixed_cache);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "interpolation"), "set_interpolation", "get_interpolation");
}

PixelpartAnimatedPropertyFloat3::PixelpartAnimatedPropertyFloat3() {

}

void PixelpartAnimatedPropertyFloat3::init(pixelpart::AnimatedProperty<pixelpart::vec3_t>* prop) {
	property = prop;
}

Vector3 PixelpartAnimatedPropertyFloat3::get(float position) const {
	if(property) {
		return toGd(property->get(static_cast<pixelpart::float_t>(position)));
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}

void PixelpartAnimatedPropertyFloat3::add_point(float position, Vector3 value) {
	if(property) {
		property->addPoint(static_cast<pixelpart::float_t>(position), fromGd(value));

	}
}
void PixelpartAnimatedPropertyFloat3::remove_point(int index) {
	if(property) {
		property->removePoint(static_cast<std::size_t>(std::max(index, 0)));
	}
}
void PixelpartAnimatedPropertyFloat3::set_point(int index, Vector3 value) {
	if(property) {
		property->setPoint(static_cast<std::size_t>(std::max(index, 0)), fromGd(value));;
	}
}
void PixelpartAnimatedPropertyFloat3::set_point_position(int index, float position) {
	if(property) {
		property->setPointPosition(static_cast<std::size_t>(std::max(index, 0)), static_cast<pixelpart::float_t>(position));
	}
}
void PixelpartAnimatedPropertyFloat3::clear() {
	if(property) {
		property->clear();
	}
}

bool PixelpartAnimatedPropertyFloat3::contains_points() const {
	if(property) {
		return property->containsPoints();
	}

	return false;
}
int PixelpartAnimatedPropertyFloat3::get_num_points() const {
	if(property) {
		return static_cast<int>(property->getNumPoints());
	}

	return 0;
}
Vector3 PixelpartAnimatedPropertyFloat3::get_point(int index) const {
	if(property && index >= 0 && static_cast<std::size_t>(index) < property->getNumPoints()) {
		return toGd(property->getPoint(static_cast<std::size_t>(index)).value);
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}

int PixelpartAnimatedPropertyFloat3::get_point_index(float position, float epsilon) {
	if(property) {
		return static_cast<int>(property->getPointIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon)));
	}

	return -1;
}

void PixelpartAnimatedPropertyFloat3::set_interpolation(int method) {
	if(property) {
		property->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
	}
}
int PixelpartAnimatedPropertyFloat3::get_interpolation() const {
	if(property) {
		return static_cast<int>(property->getInterpolation());
	}

	return static_cast<int>(pixelpart::CurveInterpolation::none);
}

void PixelpartAnimatedPropertyFloat3::enable_adaptive_cache() {
	if(property) {
		property->enableAdaptiveCache();
	}
}
void PixelpartAnimatedPropertyFloat3::enable_fixed_cache(int size) {
	if(property) {
		property->enableFixedCache(static_cast<std::size_t>(std::max(size, 0)));
	}
}
}