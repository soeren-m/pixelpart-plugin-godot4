#include "PixelpartCollider.h"
#include "util/PixelpartUtil.h"
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
	ClassDB::bind_method(D_METHOD("get_width"), &PixelpartCollider::get_width);
	ClassDB::bind_method(D_METHOD("get_orientation"), &PixelpartCollider::get_orientation);
	ClassDB::bind_method(D_METHOD("get_kill_on_contact"), &PixelpartCollider::get_kill_on_contact);
	ClassDB::bind_method(D_METHOD("get_bounce"), &PixelpartCollider::get_bounce);
	ClassDB::bind_method(D_METHOD("get_friction"), &PixelpartCollider::get_friction);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_start"), "set_lifetime_start", "get_lifetime_start");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_duration"), "set_lifetime_duration", "get_lifetime_duration");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "repeat"), "set_repeat", "get_repeat");
}

PixelpartCollider::PixelpartCollider() {

}

void PixelpartCollider::init(Ref<PixelpartEffectResource> resource, pixelpart::Collider* colliderPtr, pixelpart::ParticleEngine* particleEnginePtr) {
	effectResource = resource;
	collider = colliderPtr;
	particleEngine = particleEnginePtr;
}

int PixelpartCollider::get_id() const {
	if(collider) {
		return static_cast<int>(collider->id);
	}

	return -1;
}
String PixelpartCollider::get_name() const {
	if(collider) {
		return String(collider->name.c_str());
	}

	return String();
}

void PixelpartCollider::set_lifetime_start(float time) {
	if(collider) {
		collider->lifetimeStart = time;
	}
}
void PixelpartCollider::set_lifetime_duration(float time) {
	if(collider) {
		collider->lifetimeDuration = time;
	}
}
void PixelpartCollider::set_repeat(bool value) {
	if(collider) {
		collider->repeat = value;
	}
}
float PixelpartCollider::get_lifetime_start() const {
	if(collider) {
		return static_cast<float>(collider->lifetimeStart);
	}

	return 0.0f;
}
float PixelpartCollider::get_lifetime_duration() const {
	if(collider) {
		return static_cast<float>(collider->lifetimeDuration);
	}

	return 0.0f;
}
bool PixelpartCollider::get_repeat() const {
	if(collider) {
		return collider->repeat;
	}

	return false;
}
bool PixelpartCollider::is_active() const {
	if(collider && particleEngine) {
		return
			particleEngine->getTime() >= collider->lifetimeStart &&
			(particleEngine->getTime() <= collider->lifetimeStart + collider->lifetimeDuration || collider->repeat);
	}

	return false;
}
float PixelpartCollider::get_local_time() const {
	if(collider && particleEngine) {
		return static_cast<float>(std::fmod(
			particleEngine->getTime() - collider->lifetimeStart, collider->lifetimeDuration) / collider->lifetimeDuration);
	}

	return 0.0f;
}

void PixelpartCollider::add_point(Vector3 point) {
	if(collider) {
		collider->points.push_back(fromGd(point / effectResource->get_scale()));
	}
}
void PixelpartCollider::set_point(int index, Vector3 point) {
	if(collider) {
		if(index >= 0 && index < static_cast<int>(collider->points.size())) {
			collider->points[index] = fromGd(point / effectResource->get_scale());
		}
	}
}
void PixelpartCollider::remove_point(int index) {
	if(collider) {
		if(index >= 0 && index < static_cast<int>(collider->points.size())) {
			collider->points.erase(collider->points.begin() + index);
		}
	}
}
Vector3 PixelpartCollider::get_point(int index) const {
	if(collider) {
		if(index >= 0 && index < static_cast<int>(collider->points.size())) {
			return toGd(collider->points[index]) * effectResource->get_scale();
		}
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}
int PixelpartCollider::get_num_points() const {
	if(collider) {
		return static_cast<int>(collider->points.size());
	}

	return 0;
}

Ref<PixelpartStaticPropertyFloat> PixelpartCollider::get_width() const {
	if(collider) {
		Ref<PixelpartStaticPropertyFloat> property;
		property.instantiate();
		property->init(&collider->width);

		return property;
	}

	return Ref<PixelpartStaticPropertyFloat>();
}
Ref<PixelpartStaticPropertyFloat> PixelpartCollider::get_orientation() const {
	if(collider) {
		Ref<PixelpartStaticPropertyFloat> property;
		property.instantiate();
		property->init(&collider->orientation);

		return property;
	}

	return Ref<PixelpartStaticPropertyFloat>();
}
Ref<PixelpartStaticPropertyBool> PixelpartCollider::get_kill_on_contact() const {
	if(collider) {
		Ref<PixelpartStaticPropertyBool> property;
		property.instantiate();
		property->init(&collider->killOnContact);

		return property;
	}

	return Ref<PixelpartStaticPropertyBool>();
}

Ref<PixelpartAnimatedPropertyFloat> PixelpartCollider::get_bounce() const {
	if(collider) {
		Ref<PixelpartAnimatedPropertyFloat> property;
		property.instantiate();
		property->init(&collider->bounce);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat>();
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartCollider::get_friction() const {
	if(collider) {
		Ref<PixelpartAnimatedPropertyFloat> property;
		property.instantiate();
		property->init(&collider->friction);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat>();
}
}