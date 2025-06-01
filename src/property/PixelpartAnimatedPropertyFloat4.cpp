#include "PixelpartAnimatedPropertyFloat4.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>
#include <algorithm>

namespace godot {
PixelpartAnimatedPropertyFloat4::PixelpartAnimatedPropertyFloat4() {

}

void PixelpartAnimatedPropertyFloat4::init(pixelpart::AnimatedProperty<pixelpart::float4_t>* prop) {
	property = prop;
}

Vector4 PixelpartAnimatedPropertyFloat4::at(float position) const {
	if(property) {
		return pxpt_to_gd(property->at(static_cast<pixelpart::float_t>(position)));
	}

	return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void PixelpartAnimatedPropertyFloat4::add_keyframe(float position, Vector4 value) {
	if(property) {
		property->addKeyframe(static_cast<pixelpart::float_t>(position), gd_to_pxpt(value));
	}
}
void PixelpartAnimatedPropertyFloat4::remove_keyframe(int index) {
	if(property) {
		property->removeKeyframe(static_cast<std::size_t>(std::max(index, 0)));
	}
}
void PixelpartAnimatedPropertyFloat4::set_keyframe_value(int index, Vector4 value) {
	if(property) {
		property->setKeyframeValue(static_cast<std::size_t>(std::max(index, 0)), gd_to_pxpt(value));
	}
}
void PixelpartAnimatedPropertyFloat4::set_keyframe_position(int index, float position) {
	if(property) {
		property->setKeyframePosition(static_cast<std::size_t>(std::max(index, 0)), static_cast<pixelpart::float_t>(position));
	}
}
void PixelpartAnimatedPropertyFloat4::clear_keyframes() {
	if(property) {
		property->clearKeyframes();
	}
}

bool PixelpartAnimatedPropertyFloat4::contains_keyframes() const {
	if(property) {
		return property->keyframeCount() > 0;
	}

	return false;
}
int PixelpartAnimatedPropertyFloat4::get_keyframe_count() const {
	if(property) {
		return static_cast<int>(property->keyframeCount());
	}

	return 0;
}
Vector4 PixelpartAnimatedPropertyFloat4::get_keyframe_value(int index) const {
	if(property && index >= 0 && static_cast<std::size_t>(index) < property->keyframeCount()) {
		return pxpt_to_gd(property->keyframe(static_cast<std::size_t>(index)).value);
	}

	return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

int PixelpartAnimatedPropertyFloat4::get_keyframe_index(float position, float epsilon) {
	if(property) {
		return static_cast<int>(property->keyframeIndex(static_cast<pixelpart::float_t>(position), static_cast<pixelpart::float_t>(epsilon)));
	}

	return -1;
}

void PixelpartAnimatedPropertyFloat4::set_keyframe_interpolation(int method) {
	if(property) {
		property->keyframeInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
	}
}
int PixelpartAnimatedPropertyFloat4::get_keyframe_interpolation() const {
	if(property) {
		return static_cast<int>(property->keyframeInterpolation());
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

void PixelpartAnimatedPropertyFloat4::_bind_methods() {
	ClassDB::bind_method(D_METHOD("at", "position"), &PixelpartAnimatedPropertyFloat4::at);
	ClassDB::bind_method(D_METHOD("add_keyframe", "position", "value"), &PixelpartAnimatedPropertyFloat4::add_keyframe);
	ClassDB::bind_method(D_METHOD("remove_keyframe", "index"), &PixelpartAnimatedPropertyFloat4::remove_keyframe);
	ClassDB::bind_method(D_METHOD("set_keyframe_value", "index", "value"), &PixelpartAnimatedPropertyFloat4::set_keyframe_value);
	ClassDB::bind_method(D_METHOD("set_keyframe_position", "index", "position"), &PixelpartAnimatedPropertyFloat4::set_keyframe_position);
	ClassDB::bind_method(D_METHOD("clear_keyframes"), &PixelpartAnimatedPropertyFloat4::clear_keyframes);
	ClassDB::bind_method(D_METHOD("contains_keyframes"), &PixelpartAnimatedPropertyFloat4::contains_keyframes);
	ClassDB::bind_method(D_METHOD("get_keyframe_count"), &PixelpartAnimatedPropertyFloat4::get_keyframe_count);
	ClassDB::bind_method(D_METHOD("get_keyframe_value", "index"), &PixelpartAnimatedPropertyFloat4::get_keyframe_value);
	ClassDB::bind_method(D_METHOD("get_keyframe_index", "position", "epsilon"), &PixelpartAnimatedPropertyFloat4::get_keyframe_index);
	ClassDB::bind_method(D_METHOD("set_keyframe_interpolation", "method"), &PixelpartAnimatedPropertyFloat4::set_keyframe_interpolation);
	ClassDB::bind_method(D_METHOD("get_keyframe_interpolation"), &PixelpartAnimatedPropertyFloat4::get_keyframe_interpolation);
	ClassDB::bind_method(D_METHOD("enable_adaptive_cache"), &PixelpartAnimatedPropertyFloat4::enable_adaptive_cache);
	ClassDB::bind_method(D_METHOD("enable_fixed_cache", "size"), &PixelpartAnimatedPropertyFloat4::enable_fixed_cache);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "keyframe_interpolation"), "set_keyframe_interpolation", "get_keyframe_interpolation");

	// Deprecated
	ClassDB::bind_method(D_METHOD("get", "position"), &PixelpartAnimatedPropertyFloat4::at);
	ClassDB::bind_method(D_METHOD("add_point", "position", "value"), &PixelpartAnimatedPropertyFloat4::add_keyframe);
	ClassDB::bind_method(D_METHOD("remove_point", "index"), &PixelpartAnimatedPropertyFloat4::remove_keyframe);
	ClassDB::bind_method(D_METHOD("set_point", "index", "value"), &PixelpartAnimatedPropertyFloat4::set_keyframe_value);
	ClassDB::bind_method(D_METHOD("set_point_position", "index", "position"), &PixelpartAnimatedPropertyFloat4::set_keyframe_position);
	ClassDB::bind_method(D_METHOD("clear"), &PixelpartAnimatedPropertyFloat4::clear_keyframes);
	ClassDB::bind_method(D_METHOD("contains_points"), &PixelpartAnimatedPropertyFloat4::contains_keyframes);
	ClassDB::bind_method(D_METHOD("get_num_points"), &PixelpartAnimatedPropertyFloat4::get_keyframe_count);
	ClassDB::bind_method(D_METHOD("get_point", "index"), &PixelpartAnimatedPropertyFloat4::get_keyframe_value);
	ClassDB::bind_method(D_METHOD("get_point_index", "position", "epsilon"), &PixelpartAnimatedPropertyFloat4::get_keyframe_index);
	ClassDB::bind_method(D_METHOD("set_interpolation", "method"), &PixelpartAnimatedPropertyFloat4::set_keyframe_interpolation);
	ClassDB::bind_method(D_METHOD("get_interpolation"), &PixelpartAnimatedPropertyFloat4::get_keyframe_interpolation);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "interpolation"), "set_keyframe_interpolation", "get_keyframe_interpolation");
}
}