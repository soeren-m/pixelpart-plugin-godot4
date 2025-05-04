#include "PixelpartLineCollider.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartLineCollider::PixelpartLineCollider() : PixelpartCollider() {

}
PixelpartLineCollider::~PixelpartLineCollider() {

}

void PixelpartLineCollider::init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) {
	PixelpartCollider::init(internalNode, effectEnginePtr);

	lineCollider = dynamic_cast<pixelpart::LineCollider*>(internalNode);
}

void PixelpartLineCollider::add_point(Vector3 point) {
	if(!lineCollider) {
		return;
	}

	lineCollider->points().push_back(gd_to_pxpt(point));
}
void PixelpartLineCollider::set_point(int index, Vector3 point) {
	if(!lineCollider || index < 0 || index >= static_cast<int>(lineCollider->points().size())) {
		return;
	}

	lineCollider->point(index) = gd_to_pxpt(point);
}
void PixelpartLineCollider::remove_point(int index) {
	if(!lineCollider || index < 0 || index >= static_cast<int>(lineCollider->points().size())) {
		return;
	}

	lineCollider->points().erase(lineCollider->points().begin() + index);
}
Vector3 PixelpartLineCollider::get_point(int index) const {
	if(!lineCollider || index < 0 || index >= static_cast<int>(lineCollider->points().size())) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	return pxpt_to_gd(lineCollider->point(index));
}
int PixelpartLineCollider::get_point_count() const {
	if(!lineCollider) {
		return 0;
	}

	return static_cast<int>(lineCollider->points().size());
}

void PixelpartLineCollider::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_point", "point"), &PixelpartLineCollider::add_point);
	ClassDB::bind_method(D_METHOD("set_point", "index", "point"), &PixelpartLineCollider::set_point);
	ClassDB::bind_method(D_METHOD("remove_point", "index"), &PixelpartLineCollider::remove_point);
	ClassDB::bind_method(D_METHOD("get_point", "index"), &PixelpartLineCollider::get_point);
	ClassDB::bind_method(D_METHOD("get_point_count"), &PixelpartLineCollider::get_point_count);

	// Deprecated
	ClassDB::bind_method(D_METHOD("get_num_points"), &PixelpartLineCollider::get_point_count);
}
}