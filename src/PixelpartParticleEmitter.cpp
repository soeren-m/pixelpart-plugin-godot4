#include "PixelpartParticleEmitter.h"
#include "PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
void PixelpartParticleEmitter::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_id"), &PixelpartParticleEmitter::get_id);
	ClassDB::bind_method(D_METHOD("get_parent_id"), &PixelpartParticleEmitter::get_parent_id);
	ClassDB::bind_method(D_METHOD("get_name"), &PixelpartParticleEmitter::get_name);
	ClassDB::bind_method(D_METHOD("set_lifetime_start", "time"), &PixelpartParticleEmitter::set_lifetime_start);
	ClassDB::bind_method(D_METHOD("set_lifetime_duration", "time"), &PixelpartParticleEmitter::set_lifetime_duration);
	ClassDB::bind_method(D_METHOD("set_repeat", "value"), &PixelpartParticleEmitter::set_repeat);
	ClassDB::bind_method(D_METHOD("get_lifetime_start"), &PixelpartParticleEmitter::get_lifetime_start);
	ClassDB::bind_method(D_METHOD("get_lifetime_duration"), &PixelpartParticleEmitter::get_lifetime_duration);
	ClassDB::bind_method(D_METHOD("get_repeat"), &PixelpartParticleEmitter::get_repeat);
	ClassDB::bind_method(D_METHOD("is_active"), &PixelpartParticleEmitter::is_active);
	ClassDB::bind_method(D_METHOD("get_local_time"), &PixelpartParticleEmitter::get_local_time);
	ClassDB::bind_method(D_METHOD("get_position"), &PixelpartParticleEmitter::get_position);
	ClassDB::bind_method(D_METHOD("set_shape", "type"), &PixelpartParticleEmitter::set_shape);
	ClassDB::bind_method(D_METHOD("get_shape"), &PixelpartParticleEmitter::get_shape);
	ClassDB::bind_method(D_METHOD("get_path"), &PixelpartParticleEmitter::get_path);
	ClassDB::bind_method(D_METHOD("get_size"), &PixelpartParticleEmitter::get_size);
	ClassDB::bind_method(D_METHOD("get_orientation"), &PixelpartParticleEmitter::get_orientation);
	ClassDB::bind_method(D_METHOD("set_distribution", "mode"), &PixelpartParticleEmitter::set_distribution);
	ClassDB::bind_method(D_METHOD("set_emission_mode", "mode"), &PixelpartParticleEmitter::set_emission_mode);
	ClassDB::bind_method(D_METHOD("set_direction_mode", "mode"), &PixelpartParticleEmitter::set_direction_mode);
	ClassDB::bind_method(D_METHOD("get_distribution"), &PixelpartParticleEmitter::get_distribution);
	ClassDB::bind_method(D_METHOD("get_emission_mode"), &PixelpartParticleEmitter::get_emission_mode);
	ClassDB::bind_method(D_METHOD("get_direction_mode"), &PixelpartParticleEmitter::get_direction_mode);
	ClassDB::bind_method(D_METHOD("get_direction"), &PixelpartParticleEmitter::get_direction);
	ClassDB::bind_method(D_METHOD("get_spread"), &PixelpartParticleEmitter::get_spread);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_start"), "set_lifetime_start", "get_lifetime_start");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_duration"), "set_lifetime_duration", "get_lifetime_duration");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "repeat"), "set_repeat", "get_repeat");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "shape"), "set_shape", "get_shape");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "distribution"), "set_distribution", "get_distribution");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "emission_mode"), "set_emission_mode", "get_emission_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "direction_mode"), "set_direction_mode", "get_direction_mode");
}

PixelpartParticleEmitter::PixelpartParticleEmitter() {

}

void PixelpartParticleEmitter::init(Ref<PixelpartEffectResource> resource, pixelpart::ParticleEmitter* particleEmitter, pixelpart::ParticleEngine* engine) {
	effectResource = resource;
	nativeParticleEmitter = particleEmitter;
	nativeParticleEngine = engine;
}

int PixelpartParticleEmitter::get_id() const {
	if(nativeParticleEmitter) {
		return static_cast<int>(nativeParticleEmitter->id);
	}

	return -1;
}
int PixelpartParticleEmitter::get_parent_id() const {
	if(nativeParticleEmitter) {
		return static_cast<int>(nativeParticleEmitter->parentId);
	}

	return -1;
}
String PixelpartParticleEmitter::get_name() const {
	if(nativeParticleEmitter) {
		return String(nativeParticleEmitter->name.c_str());
	}

	return String();
}

void PixelpartParticleEmitter::set_lifetime_start(float time) {
	if(nativeParticleEmitter) {
		nativeParticleEmitter->lifetimeStart = time;
	}
}
void PixelpartParticleEmitter::set_lifetime_duration(float time) {
	if(nativeParticleEmitter) {
		nativeParticleEmitter->lifetimeDuration = time;
	}
}
void PixelpartParticleEmitter::set_repeat(bool value) {
	if(nativeParticleEmitter) {
		nativeParticleEmitter->repeat = value;
	}
}
float PixelpartParticleEmitter::get_lifetime_start() const {
	if(nativeParticleEmitter) {
		return static_cast<float>(nativeParticleEmitter->lifetimeStart);
	}

	return 0.0f;
}
float PixelpartParticleEmitter::get_lifetime_duration() const {
	if(nativeParticleEmitter) {
		return static_cast<float>(nativeParticleEmitter->lifetimeDuration);
	}

	return 0.0f;
}
bool PixelpartParticleEmitter::get_repeat() const {
	if(nativeParticleEmitter) {
		return nativeParticleEmitter->repeat;
	}

	return false;
}
bool PixelpartParticleEmitter::is_active() const {
	if(nativeParticleEmitter && nativeParticleEngine) {
		return
			nativeParticleEngine->getTime() >= nativeParticleEmitter->lifetimeStart &&
			(nativeParticleEngine->getTime() <= nativeParticleEmitter->lifetimeStart + nativeParticleEmitter->lifetimeDuration || nativeParticleEmitter->repeat);
	}

	return false;
}
float PixelpartParticleEmitter::get_local_time() const {
	if(nativeParticleEngine) {
		return static_cast<float>(std::fmod(
			nativeParticleEngine->getTime() - nativeParticleEmitter->lifetimeStart, nativeParticleEmitter->lifetimeDuration) / nativeParticleEmitter->lifetimeDuration);
	}

	return 0.0f;
}

Ref<PixelpartCurve3> PixelpartParticleEmitter::get_position() const {
	if(nativeParticleEmitter && nativeParticleEngine) {
		Ref<PixelpartCurve3> path;
		path.instantiate();
		path->init(&nativeParticleEmitter->position, nativeParticleEngine, PixelpartCurve3::ObjectType::particle_emitter);

		return path;
	}

	return Ref<PixelpartCurve3>();
}

void PixelpartParticleEmitter::set_shape(int type) {
	if(nativeParticleEmitter) {
		nativeParticleEmitter->shape = static_cast<pixelpart::ParticleEmitter::Shape>(type);
	}
}
int PixelpartParticleEmitter::get_shape() const {
	if(nativeParticleEmitter) {
		return static_cast<int>(nativeParticleEmitter->shape);
	}

	return static_cast<int>(pixelpart::ParticleEmitter::Shape::point);
}

Ref<PixelpartCurve3> PixelpartParticleEmitter::get_path() const {
	if(nativeParticleEmitter && nativeParticleEngine) {
		Ref<PixelpartCurve3> curve;
		curve.instantiate();
		curve->init(&nativeParticleEmitter->path, nativeParticleEngine, PixelpartCurve3::ObjectType::particle_emitter);

		return curve;
	}

	return Ref<PixelpartCurve3>();
}
Ref<PixelpartCurve3> PixelpartParticleEmitter::get_size() const {
	if(nativeParticleEmitter && nativeParticleEngine) {
		Ref<PixelpartCurve3> curve;
		curve.instantiate();
		curve->init(&nativeParticleEmitter->size, nativeParticleEngine, PixelpartCurve3::ObjectType::particle_emitter);

		return curve;
	}

	return Ref<PixelpartCurve3>();
}
Ref<PixelpartCurve3> PixelpartParticleEmitter::get_orientation() const {
	if(nativeParticleEmitter && nativeParticleEngine) {
		Ref<PixelpartCurve3> curve;
		curve.instantiate();
		curve->init(&nativeParticleEmitter->orientation, nativeParticleEngine, PixelpartCurve3::ObjectType::particle_emitter);

		return curve;
	}

	return Ref<PixelpartCurve3>();
}

void PixelpartParticleEmitter::set_distribution(int mode) {
	if(nativeParticleEmitter) {
		nativeParticleEmitter->distribution = static_cast<pixelpart::ParticleEmitter::Distribution>(mode);
	}
}
void PixelpartParticleEmitter::set_emission_mode(int mode) {
	if(nativeParticleEmitter) {
		nativeParticleEmitter->emissionMode = static_cast<pixelpart::ParticleEmitter::EmissionMode>(mode);
	}
}
void PixelpartParticleEmitter::set_direction_mode(int mode) {
	if(nativeParticleEmitter) {
		nativeParticleEmitter->directionMode = static_cast<pixelpart::ParticleEmitter::DirectionMode>(mode);
	}
}
int PixelpartParticleEmitter::get_distribution() const {
	if(nativeParticleEmitter) {
		return static_cast<int>(nativeParticleEmitter->distribution);
	}

	return static_cast<int>(pixelpart::ParticleEmitter::Distribution::uniform);
}
int PixelpartParticleEmitter::get_emission_mode() const {
	if(nativeParticleEmitter) {
		return static_cast<int>(nativeParticleEmitter->emissionMode);
	}

	return static_cast<int>(pixelpart::ParticleEmitter::EmissionMode::continuous);
}
int PixelpartParticleEmitter::get_direction_mode() const {
	if(nativeParticleEmitter) {
		return static_cast<int>(nativeParticleEmitter->directionMode);
	}

	return static_cast<int>(pixelpart::ParticleEmitter::DirectionMode::fixed);
}

Ref<PixelpartCurve3> PixelpartParticleEmitter::get_direction() const {
	if(nativeParticleEmitter && nativeParticleEngine) {
		Ref<PixelpartCurve3> curve;
		curve.instantiate();
		curve->init(&nativeParticleEmitter->direction, nativeParticleEngine, PixelpartCurve3::ObjectType::particle_emitter);

		return curve;
	}

	return Ref<PixelpartCurve3>();
}
Ref<PixelpartCurve> PixelpartParticleEmitter::get_spread() const {
	if(nativeParticleEmitter && nativeParticleEngine) {
		Ref<PixelpartCurve> curve;
		curve.instantiate();
		curve->init(&nativeParticleEmitter->spread, nativeParticleEngine, PixelpartCurve::ObjectType::particle_emitter);

		return curve;
	}

	return Ref<PixelpartCurve>();
}
}