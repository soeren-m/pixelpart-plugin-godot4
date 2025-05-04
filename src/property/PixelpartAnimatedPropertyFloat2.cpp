#include "PixelpartAnimatedPropertyFloat2.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>
#include <algorithm>

namespace godot {
PixelpartAnimatedPropertyFloat2::PixelpartAnimatedPropertyFloat2() {

}

void PixelpartAnimatedPropertyFloat2::init(pixelpart::AnimatedProperty<pixelpart::float2_t>* prop) {
	property = prop;
}

Vector2 PixelpartAnimatedPropertyFloat2::at(float position) const {
	if(property) {
		return pxpt_to_gd(property->at(static_cast<pixelpart::float_t>(position)));
	}

	return Vector2(0.0f, 0.0f);
}

void PixelpartAnimatedPropertyFloat2::add_keyframe(float position, Vector2 value) {
	if(property) {
		property->addKeyframe(static_cast<pixelpart::float_t>(position), gd_to_pxpt(value));
	}
}
void PixelpartAnimatedPropertyFloat2::remove_keyframe(int index) {
	if(property) {
		property->removeKeyframe(static_cast<std::size_t>(std::max(index, 0)));
	}
}
void PixelpartAnimatedPropertyFloat2::set_keyframe_value(int index, Vector2 value) {
	if(property) {
		property->setKeyframeValue(static_cast<std::size_t>(std::max(index, 0)), gd_to_pxpt(value));
	}
}
void PixelpartAnimatedPropertyFloat2::set_keyframe_position(int index, float position) {
	if(property) {
		property->setKeyframePosition(static_cast<std::size_t>(std::max(index, 0)), static_cast<pixelpart::float_t>(position));
	}
}
void PixelpartAnimatedPropertyFloat2::clear_keyframes() {
	if(property) {
		property->clearKeyframes();
	}
}

bool PixelpartAnimatedPropertyFloat2::contains_keyframes() const {
	if(property) {
		return property->keyframeCount() > 0;
	}

	return false;
}
int PixelpartAnimatedPropertyFloat2::get_keyframe_count() const {
	if(property) {
		return static_cast<int>(property->keyframeCount());
	}

	return 0;
}
Vector2 PixelpartAnimatedPropertyFloat2::get_keyframe_value(int index) const {
	if(property && index >= 0 && static_cast<std::size_t>(index) < property->keyframeCount()) {
		return pxpt_to_gd(property->keyframe(static_cast<std::size_t>(index)).value);
	}

	return Vector2(0.0f, 0.0f);
}

int PixelpartAnimatedPropertyFloat2::get_keyframe_index(float position, float epsilon) {
	if(property) {
		return static_cast<int>(property->keyframeIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon)));
	}

	return -1;
}

void PixelpartAnimatedPropertyFloat2::set_keyframe_interpolation(int method) {
	if(property) {
		property->keyframeInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
	}
}
int PixelpartAnimatedPropertyFloat2::get_keyframe_interpolation() const {
	if(property) {
		return static_cast<int>(property->keyframeInterpolation());
	}

	return static_cast<int>(pixelpart::CurveInterpolation::none);
}

void PixelpartAnimatedPropertyFloat2::enable_adaptive_cache() {
	if(property) {
		property->enableAdaptiveCache();
	}
}
void PixelpartAnimatedPropertyFloat2::enable_fixed_cache(int size) {
	if(property) {
		property->enableFixedCache(static_cast<std::size_t>(std::max(size, 0)));
	}
}

void PixelpartAnimatedPropertyFloat2::_bind_methods() {
	ClassDB::bind_method(D_METHOD("at", "position"), &PixelpartAnimatedPropertyFloat2::at);
	ClassDB::bind_method(D_METHOD("add_keyframe", "position", "value"), &PixelpartAnimatedPropertyFloat2::add_keyframe);
	ClassDB::bind_method(D_METHOD("remove_keyframe", "index"), &PixelpartAnimatedPropertyFloat2::remove_keyframe);
	ClassDB::bind_method(D_METHOD("set_keyframe_value", "index", "value"), &PixelpartAnimatedPropertyFloat2::set_keyframe_value);
	ClassDB::bind_method(D_METHOD("set_keyframe_position", "index", "position"), &PixelpartAnimatedPropertyFloat2::set_keyframe_position);
	ClassDB::bind_method(D_METHOD("clear_keyframes"), &PixelpartAnimatedPropertyFloat2::clear_keyframes);
	ClassDB::bind_method(D_METHOD("contains_keyframes"), &PixelpartAnimatedPropertyFloat2::contains_keyframes);
	ClassDB::bind_method(D_METHOD("get_keyframe_count"), &PixelpartAnimatedPropertyFloat2::get_keyframe_count);
	ClassDB::bind_method(D_METHOD("get_keyframe_value", "index"), &PixelpartAnimatedPropertyFloat2::get_keyframe_value);
	ClassDB::bind_method(D_METHOD("get_keyframe_index", "position", "epsilon"), &PixelpartAnimatedPropertyFloat2::get_keyframe_index);
	ClassDB::bind_method(D_METHOD("set_keyframe_interpolation", "method"), &PixelpartAnimatedPropertyFloat2::set_keyframe_interpolation);
	ClassDB::bind_method(D_METHOD("get_keyframe_interpolation"), &PixelpartAnimatedPropertyFloat2::get_keyframe_interpolation);
	ClassDB::bind_method(D_METHOD("enable_adaptive_cache"), &PixelpartAnimatedPropertyFloat2::enable_adaptive_cache);
	ClassDB::bind_method(D_METHOD("enable_fixed_cache", "size"), &PixelpartAnimatedPropertyFloat2::enable_fixed_cache);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "keyframe_interpolation"), "set_keyframe_interpolation", "get_keyframe_interpolation");

	// Deprecated
	ClassDB::bind_method(D_METHOD("get", "position"), &PixelpartAnimatedPropertyFloat2::at);
	ClassDB::bind_method(D_METHOD("add_point", "position", "value"), &PixelpartAnimatedPropertyFloat2::add_keyframe);
	ClassDB::bind_method(D_METHOD("remove_point", "index"), &PixelpartAnimatedPropertyFloat2::remove_keyframe);
	ClassDB::bind_method(D_METHOD("set_point", "index", "value"), &PixelpartAnimatedPropertyFloat2::set_keyframe_value);
	ClassDB::bind_method(D_METHOD("set_point_position", "index", "position"), &PixelpartAnimatedPropertyFloat2::set_keyframe_position);
	ClassDB::bind_method(D_METHOD("clear"), &PixelpartAnimatedPropertyFloat2::clear_keyframes);
	ClassDB::bind_method(D_METHOD("contains_points"), &PixelpartAnimatedPropertyFloat2::contains_keyframes);
	ClassDB::bind_method(D_METHOD("get_num_points"), &PixelpartAnimatedPropertyFloat2::get_keyframe_count);
	ClassDB::bind_method(D_METHOD("get_point", "index"), &PixelpartAnimatedPropertyFloat2::get_keyframe_value);
	ClassDB::bind_method(D_METHOD("get_point_index", "position", "epsilon"), &PixelpartAnimatedPropertyFloat2::get_keyframe_index);
	ClassDB::bind_method(D_METHOD("set_interpolation", "method"), &PixelpartAnimatedPropertyFloat2::set_keyframe_interpolation);
	ClassDB::bind_method(D_METHOD("get_interpolation"), &PixelpartAnimatedPropertyFloat2::get_keyframe_interpolation);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "interpolation"), "set_keyframe_interpolation", "get_keyframe_interpolation");
}
}