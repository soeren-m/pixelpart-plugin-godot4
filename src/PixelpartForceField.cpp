#include "PixelpartForceField.h"
#include "util/PixelpartUtil.h"

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
	ClassDB::bind_method(D_METHOD("get_strength"), &PixelpartForceField::get_strength);
	ClassDB::bind_method(D_METHOD("get_acceleration_direction"), &PixelpartForceField::get_acceleration_direction);
	ClassDB::bind_method(D_METHOD("get_acceleration_direction_variance"), &PixelpartForceField::get_acceleration_direction_variance);
	ClassDB::bind_method(D_METHOD("get_acceleration_strength_variance"), &PixelpartForceField::get_acceleration_strength_variance);
	ClassDB::bind_method(D_METHOD("set_acceleration_grid_size", "width", "height", "depth"), &PixelpartForceField::set_acceleration_grid_size);
	ClassDB::bind_method(D_METHOD("get_acceleration_grid_width"), &PixelpartForceField::get_acceleration_grid_width);
	ClassDB::bind_method(D_METHOD("get_acceleration_grid_height"), &PixelpartForceField::get_acceleration_grid_height);
	ClassDB::bind_method(D_METHOD("get_acceleration_grid_depth"), &PixelpartForceField::get_acceleration_grid_depth);
	ClassDB::bind_method(D_METHOD("set_vector_filter", "filter"), &PixelpartForceField::set_vector_filter);
	ClassDB::bind_method(D_METHOD("get_vector_filter"), &PixelpartForceField::get_vector_filter);
	ClassDB::bind_method(D_METHOD("get_vector_tightness"), &PixelpartForceField::get_vector_tightness);
	ClassDB::bind_method(D_METHOD("get_noise_octaves"), &PixelpartForceField::get_noise_octaves);
	ClassDB::bind_method(D_METHOD("get_noise_frequency"), &PixelpartForceField::get_noise_frequency);
	ClassDB::bind_method(D_METHOD("get_noise_persistence"), &PixelpartForceField::get_noise_persistence);
	ClassDB::bind_method(D_METHOD("get_noise_lacunarity"), &PixelpartForceField::get_noise_lacunarity);
	ClassDB::bind_method(D_METHOD("set_noise_animated", "animated"), &PixelpartForceField::set_noise_animated);
	ClassDB::bind_method(D_METHOD("get_noise_animated"), &PixelpartForceField::get_noise_animated);
	ClassDB::bind_method(D_METHOD("get_noise_animation_time_scale"), &PixelpartForceField::get_noise_animation_time_scale);
	ClassDB::bind_method(D_METHOD("get_noise_animation_time_base"), &PixelpartForceField::get_noise_animation_time_base);
	ClassDB::bind_method(D_METHOD("get_drag_velocity_influence"), &PixelpartForceField::get_drag_velocity_influence);
	ClassDB::bind_method(D_METHOD("get_drag_size_influence"), &PixelpartForceField::get_drag_size_influence);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_start"), "set_lifetime_start", "get_lifetime_start");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_duration"), "set_lifetime_duration", "get_lifetime_duration");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "repeat"), "set_repeat", "get_repeat");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "vector_filter"), "set_vector_filter", "get_vector_filter");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "noise_animated"), "set_noise_animated", "get_noise_animated");
}

PixelpartForceField::PixelpartForceField() {

}

void PixelpartForceField::init(Ref<PixelpartEffectResource> resource, pixelpart::ForceField* forceFieldPtr, pixelpart::ParticleEngine* particleEnginePtr) {
	effectResource = resource;
	forceField = forceFieldPtr;
	particleEngine = particleEnginePtr;
}

int PixelpartForceField::get_id() const {
	if(forceField) {
		return static_cast<int>(forceField->id);
	}

	return -1;
}
String PixelpartForceField::get_name() const {
	if(forceField) {
		return String(forceField->name.c_str());
	}

	return String();
}

void PixelpartForceField::set_lifetime_start(float time) {
	if(forceField) {
		forceField->lifetimeStart = time;
	}
}
void PixelpartForceField::set_lifetime_duration(float time) {
	if(forceField) {
		forceField->lifetimeDuration = time;
	}
}
void PixelpartForceField::set_repeat(bool value) {
	if(forceField) {
		forceField->repeat = value;
	}
}
float PixelpartForceField::get_lifetime_start() const {
	if(forceField) {
		return static_cast<float>(forceField->lifetimeStart);
	}

	return 0.0f;
}
float PixelpartForceField::get_lifetime_duration() const {
	if(forceField) {
		return static_cast<float>(forceField->lifetimeDuration);
	}

	return 0.0f;
}
bool PixelpartForceField::get_repeat() const {
	if(forceField) {
		return forceField->repeat;
	}

	return false;
}
bool PixelpartForceField::is_active() const {
	if(forceField && particleEngine) {
		return
			particleEngine->getTime() >= forceField->lifetimeStart &&
			(particleEngine->getTime() <= forceField->lifetimeStart + forceField->lifetimeDuration || forceField->repeat);
	}

	return false;
}
float PixelpartForceField::get_local_time() const {
	if(forceField && particleEngine) {
		return static_cast<float>(std::fmod(
			particleEngine->getTime() - forceField->lifetimeStart, forceField->lifetimeDuration) / forceField->lifetimeDuration);
	}

	return 0.0f;
}

Ref<PixelpartAnimatedPropertyFloat3> PixelpartForceField::get_position() const {
	if(forceField) {
		Ref<PixelpartAnimatedPropertyFloat3> property;
		property.instantiate();
		property->init(&forceField->position);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat3>();
}

void PixelpartForceField::set_type(int type) {
	if(forceField) {
		forceField->type = static_cast<pixelpart::ForceField::Type>(type);
	}
}
int PixelpartForceField::get_type() const {
	if(forceField) {
		return static_cast<int>(forceField->type);
	}

	return 0;
}

Ref<PixelpartAnimatedPropertyFloat3> PixelpartForceField::get_size() const {
	if(forceField) {
		Ref<PixelpartAnimatedPropertyFloat3> property;
		property.instantiate();
		property->init(&forceField->size);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat3>();
}
Ref<PixelpartAnimatedPropertyFloat3> PixelpartForceField::get_orientation() const {
	if(forceField) {
		Ref<PixelpartAnimatedPropertyFloat3> property;
		property.instantiate();
		property->init(&forceField->orientation);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat3>();
}

Ref<PixelpartAnimatedPropertyFloat> PixelpartForceField::get_strength() const {
	if(forceField) {
		Ref<PixelpartAnimatedPropertyFloat> property;
		property.instantiate();
		property->init(&forceField->strength);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat>();
}

Ref<PixelpartAnimatedPropertyFloat3> PixelpartForceField::get_acceleration_direction() const {
	if(forceField) {
		Ref<PixelpartAnimatedPropertyFloat3> property;
		property.instantiate();
		property->init(&forceField->accelerationField.direction);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat3>();
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartForceField::get_acceleration_direction_variance() const {
	if(forceField) {
		Ref<PixelpartAnimatedPropertyFloat> property;
		property.instantiate();
		property->init(&forceField->accelerationField.directionVariance);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat>();
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartForceField::get_acceleration_strength_variance() const {
	if(forceField) {
		Ref<PixelpartAnimatedPropertyFloat> property;
		property.instantiate();
		property->init(&forceField->accelerationField.strengthVariance);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat>();
}
void PixelpartForceField::set_acceleration_grid_size(int width, int height, int depth) {
	if(forceField) {
		forceField->accelerationField.gridSize[0] = static_cast<uint32_t>(std::max(width, 1));
		forceField->accelerationField.gridSize[1] = static_cast<uint32_t>(std::max(height, 1));
		forceField->accelerationField.gridSize[2] = static_cast<uint32_t>(std::max(depth, 1));
		forceField->accelerationField.randomizeGrid(rng);
	}
}
int PixelpartForceField::get_acceleration_grid_width() const {
	if(forceField) {
		return static_cast<int>(forceField->accelerationField.gridSize[0]);
	}

	return 1;
}
int PixelpartForceField::get_acceleration_grid_height() const {
	if(forceField) {
		return static_cast<int>(forceField->accelerationField.gridSize[1]);
	}

	return 1;
}
int PixelpartForceField::get_acceleration_grid_depth() const {
	if(forceField) {
		return static_cast<int>(forceField->accelerationField.gridSize[2]);
	}

	return 1;
}

void PixelpartForceField::set_vector_filter(int filter) {
	if(forceField) {
		forceField->vectorField.filter = static_cast<pixelpart::ForceField::VectorField::Filter>(filter);
	}
}
int PixelpartForceField::get_vector_filter() const {
	if(forceField) {
		return static_cast<int>(forceField->vectorField.filter);
	}

	return static_cast<int>(pixelpart::ForceField::VectorField::Filter::none);
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartForceField::get_vector_tightness() const {
	if(forceField) {
		Ref<PixelpartAnimatedPropertyFloat> property;
		property.instantiate();
		property->init(&forceField->vectorField.tightness);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat>();
}

Ref<PixelpartStaticPropertyInt> PixelpartForceField::get_noise_octaves() const {
	if(forceField) {
		Ref<PixelpartStaticPropertyInt> property;
		property.instantiate();
		property->init(&forceField->noiseField.octaves);

		return property;
	}

	return Ref<PixelpartStaticPropertyInt>();
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartForceField::get_noise_frequency() const {
	if(forceField) {
		Ref<PixelpartAnimatedPropertyFloat> property;
		property.instantiate();
		property->init(&forceField->noiseField.frequency);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat>();
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartForceField::get_noise_persistence() const {
	if(forceField) {
		Ref<PixelpartAnimatedPropertyFloat> property;
		property.instantiate();
		property->init(&forceField->noiseField.persistence);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat>();
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartForceField::get_noise_lacunarity() const {
	if(forceField) {
		Ref<PixelpartAnimatedPropertyFloat> property;
		property.instantiate();
		property->init(&forceField->noiseField.lacunarity);

		return property;
	}

	return Ref<PixelpartAnimatedPropertyFloat>();
}
void PixelpartForceField::set_noise_animated(bool animated) {
	if(forceField) {
		forceField->noiseField.animated = animated;
	}
}
bool PixelpartForceField::get_noise_animated() const {
	if(forceField) {
		return forceField->noiseField.animated;
	}

	return false;
}
Ref<PixelpartStaticPropertyFloat> PixelpartForceField::get_noise_animation_time_scale() const {
	if(forceField) {
		Ref<PixelpartStaticPropertyFloat> property;
		property.instantiate();
		property->init(&forceField->noiseField.animationTimeScale);

		return property;
	}

	return Ref<PixelpartStaticPropertyFloat>();
}
Ref<PixelpartStaticPropertyFloat> PixelpartForceField::get_noise_animation_time_base() const {
	if(forceField) {
		Ref<PixelpartStaticPropertyFloat> property;
		property.instantiate();
		property->init(&forceField->noiseField.animationTimeBase);

		return property;
	}

	return Ref<PixelpartStaticPropertyFloat>();
}

Ref<PixelpartStaticPropertyFloat> PixelpartForceField::get_drag_velocity_influence() const {
	if(forceField) {
		Ref<PixelpartStaticPropertyFloat> property;
		property.instantiate();
		property->init(&forceField->dragField.velocityInfluence);

		return property;
	}

	return Ref<PixelpartStaticPropertyFloat>();
}
Ref<PixelpartStaticPropertyFloat> PixelpartForceField::get_drag_size_influence() const {
	if(forceField) {
		Ref<PixelpartStaticPropertyFloat> property;
		property.instantiate();
		property->init(&forceField->dragField.sizeInfluence);

		return property;
	}

	return Ref<PixelpartStaticPropertyFloat>();
}
}