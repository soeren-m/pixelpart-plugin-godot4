#include "PixelpartGradient.h"
#include "PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartGradient::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get", "t"), &PixelpartGradient::get);
	ClassDB::bind_method(D_METHOD("get_point", "index"), &PixelpartGradient::get_point);
	ClassDB::bind_method(D_METHOD("set", "value"), &PixelpartGradient::set);
	ClassDB::bind_method(D_METHOD("add_point", "t", "value"), &PixelpartGradient::add_point);
	ClassDB::bind_method(D_METHOD("set_point", "index", "value"), &PixelpartGradient::set_point);
	ClassDB::bind_method(D_METHOD("move_point", "index", "delta"), &PixelpartGradient::move_point);
	ClassDB::bind_method(D_METHOD("shift_point", "index", "delta"), &PixelpartGradient::shift_point);
	ClassDB::bind_method(D_METHOD("remove_point", "index"), &PixelpartGradient::remove_point);
	ClassDB::bind_method(D_METHOD("clear"), &PixelpartGradient::clear);
	ClassDB::bind_method(D_METHOD("get_num_points"), &PixelpartGradient::get_num_points);
	ClassDB::bind_method(D_METHOD("move", "delta"), &PixelpartGradient::move);
	ClassDB::bind_method(D_METHOD("shift", "delta"), &PixelpartGradient::shift);
	ClassDB::bind_method(D_METHOD("set_interpolation", "method"), &PixelpartGradient::set_interpolation);
	ClassDB::bind_method(D_METHOD("get_interpolation"), &PixelpartGradient::get_interpolation);
	ClassDB::bind_method(D_METHOD("enable_adaptive_cache"), &PixelpartGradient::enable_adaptive_cache);
	ClassDB::bind_method(D_METHOD("enable_fixed_cache", "size"), &PixelpartGradient::enable_fixed_cache);
	ClassDB::bind_method(D_METHOD("get_cache_size"), &PixelpartGradient::get_cache_size);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "interpolation"), "set_interpolation", "get_interpolation");
}

PixelpartGradient::PixelpartGradient() {

}

void PixelpartGradient::init(pixelpart::Curve<pixelpart::vec4d>* gradient, pixelpart::ParticleEngine* engine, ObjectType type) {
	nativeGradient = gradient;
	nativeParticleEngine = engine;
	objectType = type;
}

Color PixelpartGradient::get(float t) const {
	if(nativeGradient) {
		return toGd(nativeGradient->get(t));
	}

	return Color(0.0f, 0.0f, 0.0f, 0.0f);
}
Color PixelpartGradient::get_point(int index) const {
	if(nativeGradient && index >= 0 && static_cast<std::size_t>(index) < nativeGradient->getNumPoints()) {
		return toGd(nativeGradient->getPoints()[static_cast<std::size_t>(index)].value);
	}

	return Color(0.0f, 0.0f, 0.0f, 0.0f);
}

void PixelpartGradient::set(Color value) {
	if(nativeGradient) {
		nativeGradient->clear();
		nativeGradient->setPoints({ pixelpart::Curve<pixelpart::vec4d>::Point{ 0.5, fromGd(value) } });
		update_simulation();
	}
}
void PixelpartGradient::add_point(float t, Color value) {
	if(nativeGradient) {
		nativeGradient->addPoint(t, fromGd(value));
		update_simulation();
	}
}
void PixelpartGradient::set_point(int index, Color value) {
	if(nativeGradient) {
		nativeGradient->setPoint(static_cast<std::size_t>(index), fromGd(value));
		update_simulation();
	}
}
void PixelpartGradient::move_point(int index, Color delta) {
	if(nativeGradient) {
		nativeGradient->movePoint(static_cast<std::size_t>(index), fromGd(delta));
		update_simulation();
	}
}
void PixelpartGradient::shift_point(int index, float delta) {
	if(nativeGradient) {
		nativeGradient->shiftPoint(static_cast<std::size_t>(index), delta);
		update_simulation();
	}
}
void PixelpartGradient::remove_point(int index) {
	if(nativeGradient) {
		nativeGradient->removePoint(static_cast<std::size_t>(index));
		update_simulation();
	}
}
void PixelpartGradient::clear() {
	if(nativeGradient) {
		nativeGradient->clear();
		update_simulation();
	}
}
int PixelpartGradient::get_num_points() const {
	if(nativeGradient) {
		return static_cast<int>(nativeGradient->getNumPoints());
	}

	return 0;
}

void PixelpartGradient::move(Color delta) {
	if(nativeGradient) {
		nativeGradient->move(fromGd(delta));
		update_simulation();
	}
}
void PixelpartGradient::shift(float delta) {
	if(nativeGradient) {
		nativeGradient->shift(delta);
		update_simulation();
	}
}

void PixelpartGradient::set_interpolation(int method) {
	if(nativeGradient) {
		nativeGradient->setInterpolation(static_cast<pixelpart::CurveInterpolation>(method));
		update_simulation();
	}
}
int PixelpartGradient::get_interpolation() const {
	if(nativeGradient) {
		return static_cast<int>(nativeGradient->getInterpolation());
	}

	return static_cast<int>(pixelpart::CurveInterpolation::none);
}

void PixelpartGradient::enable_adaptive_cache() {
	if(nativeGradient) {
		nativeGradient->enableAdaptiveCache();
		update_simulation();
	}
}
void PixelpartGradient::enable_fixed_cache(int size) {
	if(nativeGradient && size > 0) {
		nativeGradient->enableFixedCache(static_cast<std::size_t>(size));
		update_simulation();
	}
}
int PixelpartGradient::get_cache_size() const {
	if(nativeGradient) {
		return nativeGradient->getCacheSize();
	}

	return 0;
}

void PixelpartGradient::update_simulation() {
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