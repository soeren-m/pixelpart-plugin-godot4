#include "PixelpartCurve3.h"
#include "PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartCurve3::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get", "t"), &PixelpartCurve3::get);
	ClassDB::bind_method(D_METHOD("get_point", "index"), &PixelpartCurve3::get_point);
	ClassDB::bind_method(D_METHOD("set", "value"), &PixelpartCurve3::set);
	ClassDB::bind_method(D_METHOD("add_point", "t", "value"), &PixelpartCurve3::add_point);
	ClassDB::bind_method(D_METHOD("set_point", "index", "value"), &PixelpartCurve3::set_point);
	ClassDB::bind_method(D_METHOD("move_point", "index", "delta"), &PixelpartCurve3::move_point);
	ClassDB::bind_method(D_METHOD("shift_point", "index", "delta"), &PixelpartCurve3::shift_point);
	ClassDB::bind_method(D_METHOD("remove_point", "index"), &PixelpartCurve3::remove_point);
	ClassDB::bind_method(D_METHOD("clear"), &PixelpartCurve3::clear);
	ClassDB::bind_method(D_METHOD("get_num_points"), &PixelpartCurve3::get_num_points);
	ClassDB::bind_method(D_METHOD("move", "delta"), &PixelpartCurve3::move);
	ClassDB::bind_method(D_METHOD("shift", "delta"), &PixelpartCurve3::shift);
	ClassDB::bind_method(D_METHOD("set_interpolation", "method"), &PixelpartCurve3::set_interpolation);
	ClassDB::bind_method(D_METHOD("get_interpolation"), &PixelpartCurve3::get_interpolation);
	ClassDB::bind_method(D_METHOD("enable_adaptive_cache"), &PixelpartCurve3::enable_adaptive_cache);
	ClassDB::bind_method(D_METHOD("enable_fixed_cache", "size"), &PixelpartCurve3::enable_fixed_cache);
	ClassDB::bind_method(D_METHOD("get_cache_size"), &PixelpartCurve3::get_cache_size);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "interpolation"), "set_interpolation", "get_interpolation");
}

PixelpartCurve3::PixelpartCurve3() {

}

void PixelpartCurve3::init(pixelpart::Curve<pixelpart::vec3d>* curve, pixelpart::ParticleEngine* engine, ObjectType type) {
	nativeCurve = curve;
	nativeParticleEngine = engine;
	objectType = type;
}

Vector3 PixelpartCurve3::get(float t) const {
	if(nativeCurve) {
		return toGd(nativeCurve->get(t));
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}
Vector3 PixelpartCurve3::get_point(int index) const {
	if(nativeCurve && index >= 0 && static_cast<std::size_t>(index) < nativeCurve->getNumPoints()) {
		return toGd(nativeCurve->getPoints()[static_cast<std::size_t>(index)].value);
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}

void PixelpartCurve3::set(Vector3 value) {
	if(nativeCurve) {
		nativeCurve->clear();
		nativeCurve->setPoints({ pixelpart::Curve<pixelpart::vec3d>::Point{ 0.5, fromGd(value) } });
		update_simulation();
	}
}
void PixelpartCurve3::add_point(float t, Vector3 value) {
	if(nativeCurve) {
		nativeCurve->addPoint(t, fromGd(value));
		update_simulation();
	}
}
void PixelpartCurve3::set_point(int index, Vector3 value) {
	if(nativeCurve) {
		nativeCurve->setPoint(static_cast<std::size_t>(index), fromGd(value));
		update_simulation();
	}
}
void PixelpartCurve3::move_point(int index, Vector3 delta) {
	if(nativeCurve) {
		nativeCurve->movePoint(static_cast<std::size_t>(index), fromGd(delta));
		update_simulation();
	}
}
void PixelpartCurve3::shift_point(int index, float delta) {
	if(nativeCurve) {
		nativeCurve->shiftPoint(static_cast<std::size_t>(index), delta);
		update_simulation();
	}
}
void PixelpartCurve3::remove_point(int index) {
	if(nativeCurve) {
		nativeCurve->removePoint(static_cast<std::size_t>(index));
		update_simulation();
	}
}
void PixelpartCurve3::clear() {
	if(nativeCurve) {
		nativeCurve->clear();
		update_simulation();
	}
}
int PixelpartCurve3::get_num_points() const {
	if(nativeCurve) {
		return static_cast<int>(nativeCurve->getNumPoints());
	}

	return 0;
}

void PixelpartCurve3::move(Vector3 delta) {
	if(nativeCurve) {
		nativeCurve->move(fromGd(delta));
		update_simulation();
	}
}
void PixelpartCurve3::shift(float delta) {
	if(nativeCurve) {
		nativeCurve->shift(delta);
		update_simulation();
	}
}

void PixelpartCurve3::set_interpolation(int method) {
	if(nativeCurve) {
		nativeCurve->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
		update_simulation();
	}
}
int PixelpartCurve3::get_interpolation() const {
	if(nativeCurve) {
		return static_cast<int>(nativeCurve->getInterpolation());
	}

	return static_cast<int>(pixelpart::CurveInterpolation::none);
}

void PixelpartCurve3::enable_adaptive_cache() {
	if(nativeCurve) {
		nativeCurve->enableAdaptiveCache();
		update_simulation();
	}
}
void PixelpartCurve3::enable_fixed_cache(int size) {
	if(nativeCurve && size > 0) {
		nativeCurve->enableFixedCache(static_cast<std::size_t>(size));
		update_simulation();
	}
}
int PixelpartCurve3::get_cache_size() const {
	if(nativeCurve) {
		return nativeCurve->getCacheSize();
	}

	return 0;
}

void PixelpartCurve3::update_simulation() {
	if(!nativeParticleEngine) {
		return;
	}

	if(objectType == ObjectType::force_field) {
		nativeParticleEngine->updateForceSolver();
	}
	else if(objectType == ObjectType::collider) {
		nativeParticleEngine->updateCollisionSolver();
	}
}
}