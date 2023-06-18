#include "PixelpartCollider.h"
#include "PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartCollider::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_id"), &PixelpartCollider::get_id);
	ClassDB::bind_method(D_METHOD("get_name"), &PixelpartCollider::get_name);
	ClassDB::bind_method(D_METHOD("set_lifetime_start", "time"), &PixelpartCollider::set_lifetime_start);
	ClassDB::bind_method(D_METHOD("set_lifetime_duration", "time"), &PixelpartCollider::set_lifetime_duration);
	ClassDB::bind_method(D_METHOD("set_repeat", "value"), &PixelpartCollider::set_repeat);
	ClassDB::bind_method(D_METHOD("get_lifetime_start"), &PixelpartCollider::get_lifetime_start);
	ClassDB::bind_method(D_METHOD("get_lifetime_duration"), &PixelpartCollider::get_lifetime_duration);
	ClassDB::bind_method(D_METHOD("get_repeat"), &PixelpartCollider::get_repeat);
	ClassDB::bind_method(D_METHOD("is_active"), &PixelpartCollider::is_active);
	ClassDB::bind_method(D_METHOD("get_local_time"), &PixelpartCollider::get_local_time);
	ClassDB::bind_method(D_METHOD("add_point", "point"), &PixelpartCollider::add_point);
	ClassDB::bind_method(D_METHOD("set_point", "index", "point"), &PixelpartCollider::set_point);
	ClassDB::bind_method(D_METHOD("remove_point", "index"), &PixelpartCollider::remove_point);
	ClassDB::bind_method(D_METHOD("get_point", "index"), &PixelpartCollider::get_point);
	ClassDB::bind_method(D_METHOD("get_num_points"), &PixelpartCollider::get_num_points);
	ClassDB::bind_method(D_METHOD("get_bounce"), &PixelpartCollider::get_bounce);
	ClassDB::bind_method(D_METHOD("get_friction"), &PixelpartCollider::get_friction);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_start"), "set_lifetime_start", "get_lifetime_start");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_duration"), "set_lifetime_duration", "get_lifetime_duration");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "repeat"), "set_repeat", "get_repeat");
}

PixelpartCollider::PixelpartCollider() {

}

void PixelpartCollider::init(Ref<PixelpartEffectResource> resource, pixelpart::Collider* collider, pixelpart::ParticleEngine* engine) {
	effectResource = resource;
	nativeCollider = collider;
	nativeParticleEngine = engine;
}

int PixelpartCollider::get_id() const {
	if(nativeCollider) {
		return static_cast<int>(nativeCollider->id);
	}

	return -1;
}
String PixelpartCollider::get_name() const {
	if(nativeCollider) {
		return String(nativeCollider->name.c_str());
	}

	return String();
}

void PixelpartCollider::set_lifetime_start(float time) {
	if(nativeCollider && nativeParticleEngine) {
		nativeCollider->lifetimeStart = time;
		nativeParticleEngine->updateCollisionSolver();
	}
}
void PixelpartCollider::set_lifetime_duration(float time) {
	if(nativeCollider && nativeParticleEngine) {
		nativeCollider->lifetimeDuration = time;
		nativeParticleEngine->updateCollisionSolver();
	}
}
void PixelpartCollider::set_repeat(bool value) {
	if(nativeCollider && nativeParticleEngine) {
		nativeCollider->repeat = value;
		nativeParticleEngine->updateCollisionSolver();
	}
}
float PixelpartCollider::get_lifetime_start() const {
	if(nativeCollider && nativeParticleEngine) {
		return static_cast<float>(nativeCollider->lifetimeStart);
	}

	return 0.0f;
}
float PixelpartCollider::get_lifetime_duration() const {
	if(nativeCollider) {
		return static_cast<float>(nativeCollider->lifetimeDuration);
	}

	return 0.0f;
}
bool PixelpartCollider::get_repeat() const {
	if(nativeCollider) {
		return nativeCollider->repeat;
	}

	return false;
}
bool PixelpartCollider::is_active() const {
	if(nativeCollider && nativeParticleEngine) {
		return
			nativeParticleEngine->getTime() >= nativeCollider->lifetimeStart &&
			(nativeParticleEngine->getTime() <= nativeCollider->lifetimeStart + nativeCollider->lifetimeDuration || nativeCollider->repeat);
	}

	return false;
}
float PixelpartCollider::get_local_time() const {
	if(nativeCollider && nativeParticleEngine) {
		return static_cast<float>(std::fmod(
			nativeParticleEngine->getTime() - nativeCollider->lifetimeStart, nativeCollider->lifetimeDuration) / nativeCollider->lifetimeDuration);
	}

	return 0.0f;
}

void PixelpartCollider::add_point(Vector3 point) {
	if(nativeCollider && nativeParticleEngine) {
		nativeCollider->points.push_back(fromGd(point / effectResource->get_scale()));
		nativeParticleEngine->updateCollisionSolver();
	}
}
void PixelpartCollider::set_point(int index, Vector3 point) {
	if(nativeCollider && nativeParticleEngine) {
		if(index >= 0 && index < static_cast<int>(nativeCollider->points.size())) {
			nativeCollider->points[index] = fromGd(point / effectResource->get_scale());
			nativeParticleEngine->updateCollisionSolver();
		}
	}
}
void PixelpartCollider::remove_point(int index) {
	if(nativeCollider && nativeParticleEngine) {
		if(index >= 0 && index < static_cast<int>(nativeCollider->points.size())) {
			nativeCollider->points.erase(nativeCollider->points.begin() + index);
			nativeParticleEngine->updateCollisionSolver();
		}
	}
}
Vector3 PixelpartCollider::get_point(int index) const {
	if(nativeCollider && nativeParticleEngine) {
		if(index >= 0 && index < static_cast<int>(nativeCollider->points.size())) {
			return toGd(nativeCollider->points[index]) * effectResource->get_scale();
		}
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}
int PixelpartCollider::get_num_points() const {
	if(nativeCollider) {
		return static_cast<int>(nativeCollider->points.size());
	}

	return 0;
}

Ref<PixelpartCurve> PixelpartCollider::get_bounce() const {
	if(nativeCollider && nativeParticleEngine) {
		Ref<PixelpartCurve> curve;
		curve.instantiate();
		curve->init(&nativeCollider->bounce, nativeParticleEngine, PixelpartCurve::ObjectType::collider);

		return curve;
	}

	return Ref<PixelpartCurve>();
}
Ref<PixelpartCurve> PixelpartCollider::get_friction() const {
	if(nativeCollider && nativeParticleEngine) {
		Ref<PixelpartCurve> curve;
		curve.instantiate();
		curve->init(&nativeCollider->friction, nativeParticleEngine, PixelpartCurve::ObjectType::collider);

		return curve;
	}

	return Ref<PixelpartCurve>();
}
}