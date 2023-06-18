#include "PixelpartCurve.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartCurve::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get", "t"), &PixelpartCurve::get);
	ClassDB::bind_method(D_METHOD("get_point", "index"), &PixelpartCurve::get_point);
	ClassDB::bind_method(D_METHOD("set", "value"), &PixelpartCurve::set);
	ClassDB::bind_method(D_METHOD("add_point", "t", "value"), &PixelpartCurve::add_point);
	ClassDB::bind_method(D_METHOD("set_point", "index", "value"), &PixelpartCurve::set_point);
	ClassDB::bind_method(D_METHOD("move_point", "index", "delta"), &PixelpartCurve::move_point);
	ClassDB::bind_method(D_METHOD("shift_point", "index", "delta"), &PixelpartCurve::shift_point);
	ClassDB::bind_method(D_METHOD("remove_point", "index"), &PixelpartCurve::remove_point);
	ClassDB::bind_method(D_METHOD("clear"), &PixelpartCurve::clear);
	ClassDB::bind_method(D_METHOD("get_num_points"), &PixelpartCurve::get_num_points);
	ClassDB::bind_method(D_METHOD("move", "delta"), &PixelpartCurve::move);
	ClassDB::bind_method(D_METHOD("shift", "delta"), &PixelpartCurve::shift);
	ClassDB::bind_method(D_METHOD("set_interpolation", "method"), &PixelpartCurve::set_interpolation);
	ClassDB::bind_method(D_METHOD("get_interpolation"), &PixelpartCurve::get_interpolation);
	ClassDB::bind_method(D_METHOD("enable_adaptive_cache"), &PixelpartCurve::enable_adaptive_cache);
	ClassDB::bind_method(D_METHOD("enable_fixed_cache", "size"), &PixelpartCurve::enable_fixed_cache);
	ClassDB::bind_method(D_METHOD("get_cache_size"), &PixelpartCurve::get_cache_size);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "interpolation"), "set_interpolation", "get_interpolation");
}

PixelpartCurve::PixelpartCurve() {

}

void PixelpartCurve::init(pixelpart::Curve<pixelpart::floatd>* curve, pixelpart::ParticleEngine* engine, ObjectType type) {
	nativeCurve = curve;
	nativeParticleEngine = engine;
	objectType = type;
}

float PixelpartCurve::get(float t) const {
	if(nativeCurve) {
		return static_cast<float>(nativeCurve->get(t));
	}

	return 0.0f;
}
float PixelpartCurve::get_point(int index) const {
	if(nativeCurve && index >= 0 && static_cast<std::size_t>(index) < nativeCurve->getNumPoints()) {
		return static_cast<float>(nativeCurve->getPoints()[static_cast<std::size_t>(index)].value);
	}

	return 0.0f;
}

void PixelpartCurve::set(float value) {
	if(nativeCurve) {
		nativeCurve->clear();
		nativeCurve->setPoints({ pixelpart::Curve<pixelpart::floatd>::Point{ 0.5, value } });
		update_simulation();
	}
}
void PixelpartCurve::add_point(float t, float value) {
	if(nativeCurve) {
		nativeCurve->addPoint(t, value);
		update_simulation();
	}
}
void PixelpartCurve::set_point(int index, float value) {
	if(nativeCurve) {
		nativeCurve->setPoint(static_cast<std::size_t>(index), value);
		update_simulation();
	}
}
void PixelpartCurve::move_point(int index, float delta) {
	if(nativeCurve) {
		nativeCurve->movePoint(static_cast<std::size_t>(index), delta);
		update_simulation();
	}
}
void PixelpartCurve::shift_point(int index, float delta) {
	if(nativeCurve) {
		nativeCurve->shiftPoint(static_cast<std::size_t>(index), delta);
		update_simulation();
	}
}
void PixelpartCurve::remove_point(int index) {
	if(nativeCurve) {
		nativeCurve->removePoint(static_cast<std::size_t>(index));
		update_simulation();
	}
}
void PixelpartCurve::clear() {
	if(nativeCurve) {
		nativeCurve->clear();
		update_simulation();
	}
}
int PixelpartCurve::get_num_points() const {
	if(nativeCurve) {
		return static_cast<int>(nativeCurve->getNumPoints());
	}

	return 0;
}

void PixelpartCurve::move(float delta) {
	if(nativeCurve) {
		nativeCurve->move(delta);
		update_simulation();
	}
}
void PixelpartCurve::shift(float delta) {
	if(nativeCurve) {
		nativeCurve->shift(delta);
		update_simulation();
	}
}

void PixelpartCurve::set_interpolation(int method) {
	if(nativeCurve) {
		nativeCurve->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
		update_simulation();
	}
}
int PixelpartCurve::get_interpolation() const {
	if(nativeCurve) {
		return static_cast<int>(nativeCurve->getInterpolation());
	}

	return static_cast<int>(pixelpart::CurveInterpolation::none);
}

void PixelpartCurve::enable_adaptive_cache() {
	if(nativeCurve) {
		nativeCurve->enableAdaptiveCache();
		update_simulation();
	}
}
void PixelpartCurve::enable_fixed_cache(int size) {
	if(nativeCurve && size > 0) {
		nativeCurve->enableFixedCache(static_cast<std::size_t>(size));
		update_simulation();
	}
}
int PixelpartCurve::get_cache_size() const {
	if(nativeCurve) {
		return nativeCurve->getCacheSize();
	}

	return 0;
}

void PixelpartCurve::update_simulation() {
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