#include "PixelpartForceField.h"
#include "PixelpartUtil.h"

namespace godot {
void PixelpartForceField::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_id"), &PixelpartForceField::get_id);
	ClassDB::bind_method(D_METHOD("get_name"), &PixelpartForceField::get_name);
	ClassDB::bind_method(D_METHOD("set_lifetime_start", "time"), &PixelpartForceField::set_lifetime_start);
	ClassDB::bind_method(D_METHOD("set_lifetime_duration", "time"), &PixelpartForceField::set_lifetime_duration);
	ClassDB::bind_method(D_METHOD("set_repeat", "value"), &PixelpartForceField::set_repeat);
	ClassDB::bind_method(D_METHOD("get_lifetime_start"), &PixelpartForceField::get_lifetime_start);
	ClassDB::bind_method(D_METHOD("get_lifetime_duration"), &PixelpartForceField::get_lifetime_duration);
	ClassDB::bind_method(D_METHOD("get_repeat"), &PixelpartForceField::get_repeat);
	ClassDB::bind_method(D_METHOD("is_active"), &PixelpartForceField::is_active);
	ClassDB::bind_method(D_METHOD("get_local_time"), &PixelpartForceField::get_local_time);
	ClassDB::bind_method(D_METHOD("get_position"), &PixelpartForceField::get_position);
	ClassDB::bind_method(D_METHOD("set_type", "type"), &PixelpartForceField::set_type);
	ClassDB::bind_method(D_METHOD("get_type"), &PixelpartForceField::get_type);
	ClassDB::bind_method(D_METHOD("get_size"), &PixelpartForceField::get_size);
	ClassDB::bind_method(D_METHOD("get_orientation"), &PixelpartForceField::get_orientation);
	ClassDB::bind_method(D_METHOD("get_direction"), &PixelpartForceField::get_direction);
	ClassDB::bind_method(D_METHOD("get_strength"), &PixelpartForceField::get_strength);
	ClassDB::bind_method(D_METHOD("set_direction_variance", "variance"), &PixelpartForceField::set_direction_variance);
	ClassDB::bind_method(D_METHOD("set_strength_variance", "variance"), &PixelpartForceField::set_strength_variance);
	ClassDB::bind_method(D_METHOD("get_direction_variance"), &PixelpartForceField::get_direction_variance);
	ClassDB::bind_method(D_METHOD("get_strength_variance"), &PixelpartForceField::get_strength_variance);
	ClassDB::bind_method(D_METHOD("set_grid_size", "width", "height", "depth"), &PixelpartForceField::set_grid_size);
	ClassDB::bind_method(D_METHOD("get_grid_width"), &PixelpartForceField::get_grid_width);
	ClassDB::bind_method(D_METHOD("get_grid_height"), &PixelpartForceField::get_grid_height);
	ClassDB::bind_method(D_METHOD("get_grid_depth"), &PixelpartForceField::get_grid_depth);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_start"), "set_lifetime_start", "get_lifetime_start");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_duration"), "set_lifetime_duration", "get_lifetime_duration");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "repeat"), "set_repeat", "get_repeat");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "direction_variance"), "set_strength_variance", "get_direction_variance");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "strength_variance"), "set_strength_variance", "get_strength_variance");
}

PixelpartForceField::PixelpartForceField() {

}

void PixelpartForceField::init(Ref<PixelpartEffectResource> resource, pixelpart::ForceField* forceField, pixelpart::ParticleEngine* engine) {
	effectResource = resource;
	nativeForceField = forceField;
	nativeParticleEngine = engine;
}

int PixelpartForceField::get_id() const {
	if(nativeForceField) {
		return static_cast<int>(nativeForceField->id);
	}

	return -1;
}
String PixelpartForceField::get_name() const {
	if(nativeForceField) {
		return String(nativeForceField->name.c_str());
	}

	return String();
}

void PixelpartForceField::set_lifetime_start(float time) {
	if(nativeForceField && nativeParticleEngine) {
		nativeForceField->lifetimeStart = time;
		nativeParticleEngine->updateForceSolver();
	}
}
void PixelpartForceField::set_lifetime_duration(float time) {
	if(nativeForceField && nativeParticleEngine) {
		nativeForceField->lifetimeDuration = time;
		nativeParticleEngine->updateForceSolver();
	}
}
void PixelpartForceField::set_repeat(bool value) {
	if(nativeForceField && nativeParticleEngine) {
		nativeForceField->repeat = value;
		nativeParticleEngine->updateForceSolver();
	}
}
float PixelpartForceField::get_lifetime_start() const {
	if(nativeForceField) {
		return static_cast<float>(nativeForceField->lifetimeStart);
	}

	return 0.0f;
}
float PixelpartForceField::get_lifetime_duration() const {
	if(nativeForceField) {
		return static_cast<float>(nativeForceField->lifetimeDuration);
	}

	return 0.0f;
}
bool PixelpartForceField::get_repeat() const {
	if(nativeForceField) {
		return nativeForceField->repeat;
	}

	return false;
}
bool PixelpartForceField::is_active() const {
	if(nativeForceField && nativeParticleEngine) {
		return
			nativeParticleEngine->getTime() >= nativeForceField->lifetimeStart &&
			(nativeParticleEngine->getTime() <= nativeForceField->lifetimeStart + nativeForceField->lifetimeDuration || nativeForceField->repeat);
	}

	return false;
}
float PixelpartForceField::get_local_time() const {
	if(nativeParticleEngine) {
		return static_cast<float>(std::fmod(
			nativeParticleEngine->getTime() - nativeForceField->lifetimeStart, nativeForceField->lifetimeDuration) / nativeForceField->lifetimeDuration);
	}

	return 0.0f;
}

Ref<PixelpartCurve3> PixelpartForceField::get_position() const {
	if(nativeForceField && nativeParticleEngine) {
		Ref<PixelpartCurve3> curve;
		curve.instantiate();
		curve->init(&nativeForceField->position, nativeParticleEngine, PixelpartCurve3::ObjectType::force_field);

		return curve;
	}

	return Ref<PixelpartCurve3>();
}

void PixelpartForceField::set_type(int type) {
	if(nativeForceField && nativeParticleEngine) {
		nativeForceField->type = static_cast<pixelpart::ForceField::Type>(type);
		nativeParticleEngine->updateForceSolver();
	}
}
int PixelpartForceField::get_type() const {
	if(nativeForceField) {
		return static_cast<int>(nativeForceField->type);
	}

	return 0;
}

Ref<PixelpartCurve3> PixelpartForceField::get_size() const {
	if(nativeForceField && nativeParticleEngine) {
		Ref<PixelpartCurve3> curve;
		curve.instantiate();
		curve->init(&nativeForceField->size, nativeParticleEngine, PixelpartCurve3::ObjectType::force_field);

		return curve;
	}

	return Ref<PixelpartCurve3>();
}
Ref<PixelpartCurve3> PixelpartForceField::get_orientation() const {
	if(nativeForceField && nativeParticleEngine) {
		Ref<PixelpartCurve3> curve;
		curve.instantiate();
		curve->init(&nativeForceField->orientation, nativeParticleEngine, PixelpartCurve3::ObjectType::force_field);

		return curve;
	}

	return Ref<PixelpartCurve3>();
}
Ref<PixelpartCurve3> PixelpartForceField::get_direction() const {
	if(nativeForceField && nativeParticleEngine) {
		Ref<PixelpartCurve3> curve;
		curve.instantiate();
		curve->init(&nativeForceField->direction, nativeParticleEngine, PixelpartCurve3::ObjectType::force_field);

		return curve;
	}

	return Ref<PixelpartCurve3>();
}
Ref<PixelpartCurve> PixelpartForceField::get_strength() const {
	if(nativeForceField && nativeParticleEngine) {
		Ref<PixelpartCurve> curve;
		curve.instantiate();
		curve->init(&nativeForceField->strength, nativeParticleEngine, PixelpartCurve::ObjectType::force_field);

		return curve;
	}

	return Ref<PixelpartCurve>();
}

void PixelpartForceField::set_direction_variance(float variance) {
	if(nativeForceField && nativeParticleEngine) {
		nativeForceField->directionVariance = static_cast<pixelpart::floatd>(variance);
		nativeParticleEngine->updateForceSolver();
	}
}
void PixelpartForceField::set_strength_variance(float variance) {
	if(nativeForceField && nativeParticleEngine) {
		nativeForceField->strengthVariance = static_cast<pixelpart::floatd>(variance);
		nativeParticleEngine->updateForceSolver();
	}
}
float PixelpartForceField::get_direction_variance() const {
	if(nativeForceField) {
		return static_cast<float>(nativeForceField->directionVariance);
	}

	return 0.0f;
}
float PixelpartForceField::get_strength_variance() const {
	if(nativeForceField) {
		return static_cast<float>(nativeForceField->strengthVariance);
	}

	return 0.0f;
}

void PixelpartForceField::set_grid_size(int width, int height, int depth) {
	if(nativeForceField && nativeParticleEngine) {
		nativeForceField->gridSize[0] = static_cast<uint32_t>(std::max(width, 1));
		nativeForceField->gridSize[1] = static_cast<uint32_t>(std::max(height, 1));
		nativeForceField->gridSize[2] = static_cast<uint32_t>(std::max(depth, 1));
		nativeForceField->randomize(rng);
		nativeParticleEngine->updateForceSolver();
	}
}
int PixelpartForceField::get_grid_width() const {
	if(nativeForceField) {
		return static_cast<int>(nativeForceField->gridSize[0]);
	}

	return 1;
}
int PixelpartForceField::get_grid_height() const {
	if(nativeForceField) {
		return static_cast<int>(nativeForceField->gridSize[1]);
	}

	return 1;
}
int PixelpartForceField::get_grid_depth() const {
	if(nativeForceField) {
		return static_cast<int>(nativeForceField->gridSize[2]);
	}

	return 1;
}
}