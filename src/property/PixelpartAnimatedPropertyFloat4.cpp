#include "PixelpartAnimatedPropertyFloat4.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartAnimatedPropertyFloat4::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get", "position"), &PixelpartAnimatedPropertyFloat4::get);
	ClassDB::bind_method(D_METHOD("add_point", "position", "value"), &PixelpartAnimatedPropertyFloat4::add_point);
	ClassDB::bind_method(D_METHOD("remove_point", "index"), &PixelpartAnimatedPropertyFloat4::remove_point);
	ClassDB::bind_method(D_METHOD("set_point", "index", "value"), &PixelpartAnimatedPropertyFloat4::set_point);
	ClassDB::bind_method(D_METHOD("set_point_position", "index", "position"), &PixelpartAnimatedPropertyFloat4::set_point_position);
	ClassDB::bind_method(D_METHOD("clear"), &PixelpartAnimatedPropertyFloat4::clear);
	ClassDB::bind_method(D_METHOD("contains_points"), &PixelpartAnimatedPropertyFloat4::contains_points);
	ClassDB::bind_method(D_METHOD("get_num_points"), &PixelpartAnimatedPropertyFloat4::get_num_points);
	ClassDB::bind_method(D_METHOD("get_point", "index"), &PixelpartAnimatedPropertyFloat4::get_point);
	ClassDB::bind_method(D_METHOD("get_point_index", "position", "epsilon"), &PixelpartAnimatedPropertyFloat4::get_point_index);
	ClassDB::bind_method(D_METHOD("set_interpolation", "method"), &PixelpartAnimatedPropertyFloat4::set_interpolation);
	ClassDB::bind_method(D_METHOD("get_interpolation"), &PixelpartAnimatedPropertyFloat4::get_interpolation);
	ClassDB::bind_method(D_METHOD("enable_adaptive_cache"), &PixelpartAnimatedPropertyFloat4::enable_adaptive_cache);
	ClassDB::bind_method(D_METHOD("enable_fixed_cache", "size"), &PixelpartAnimatedPropertyFloat4::enable_fixed_cache);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "interpolation"), "set_interpolation", "get_interpolation");
}

PixelpartAnimatedPropertyFloat4::PixelpartAnimatedPropertyFloat4() {

}

void PixelpartAnimatedPropertyFloat4::init(pixelpart::AnimatedProperty<pixelpart::vec4_t>* prop) {
	property = prop;
}

Vector4 PixelpartAnimatedPropertyFloat4::get(float position) const {
	if(property) {
		return toGd(property->get(static_cast<pixelpart::float_t>(position)));
	}

	return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void PixelpartAnimatedPropertyFloat4::add_point(float position, Vector4 value) {
	if(property) {
		property->addPoint(static_cast<pixelpart::float_t>(position), fromGd(value));
	}
}
void PixelpartAnimatedPropertyFloat4::remove_point(int index) {
	if(property) {
		property->removePoint(static_cast<std::size_t>(std::max(index, 0)));
	}
}
void PixelpartAnimatedPropertyFloat4::set_point(int index, Vector4 value) {
	if(property) {
		property->setPoint(static_cast<std::size_t>(std::max(index, 0)), fromGd(value));
	}
}
void PixelpartAnimatedPropertyFloat4::set_point_position(int index, float position) {
	if(property) {
		property->setPointPosition(static_cast<std::size_t>(std::max(index, 0)), static_cast<pixelpart::float_t>(position));
	}
}
void PixelpartAnimatedPropertyFloat4::clear() {
	if(property) {
		property->clear();
	}
}

bool PixelpartAnimatedPropertyFloat4::contains_points() const {
	if(property) {
		return property->containsPoints();
	}

	return false;
}
int PixelpartAnimatedPropertyFloat4::get_num_points() const {
	if(property) {
		return static_cast<int>(property->getNumPoints());
	}

	return 0;
}
Vector4 PixelpartAnimatedPropertyFloat4::get_point(int index) const {
	if(property && index >= 0 && static_cast<std::size_t>(index) < property->getNumPoints()) {
		return toGd(property->getPoint(static_cast<std::size_t>(index)).value);
	}

	return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

int PixelpartAnimatedPropertyFloat4::get_point_index(float position, float epsilon) {
	if(property) {
		return static_cast<int>(property->getPointIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon)));
	}

	return -1;
}

void PixelpartAnimatedPropertyFloat4::set_interpolation(int method) {
	if(property) {
		property->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
	}
}
int PixelpartAnimatedPropertyFloat4::get_interpolation() const {
	if(property) {
		return static_cast<int>(property->getInterpolation());
	}

	return static_cast<int>(pixelpart::CurveInterpolation::none);
}

void PixelpartAnimatedPropertyFloat4::enable_adaptive_cache() {
	if(property) {
		property->enableAdaptiveCache();
	}
}
void PixelpartAnimatedPropertyFloat4::enable_fixed_cache(int size) {
	if(property) {
		property->enableFixedCache(static_cast<std::size_t>(std::max(size, 0)));
	}
}
}