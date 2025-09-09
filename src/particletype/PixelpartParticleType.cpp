#include "PixelpartParticleType.h"
#include "../util/PixelpartUtil.h"
#include <algorithm>

namespace godot {
PixelpartParticleType::PixelpartParticleType() : RefCounted() {

}
PixelpartParticleType::~PixelpartParticleType() {

}

void PixelpartParticleType::init(pixelpart::ParticleType* particleTypePtr, pixelpart::EffectEngine* effectEnginePtr) {
	particleType = particleTypePtr;
	effectEngine = effectEnginePtr;
}

int PixelpartParticleType::get_id() const {
	return particleType ? static_cast<int>(particleType->id().value()) : -1;
}
int PixelpartParticleType::get_parent_id() const {
	return particleType && particleType->parentId().valid() ? static_cast<int>(particleType->parentId().value()) : -1;
}
String PixelpartParticleType::get_name() const {
	return particleType ? String(particleType->name().c_str()) : String();
}

void PixelpartParticleType::set_position_relative(bool mode) {
	if(!particleType) {
		return;
	}

	particleType->positionRelative(mode);
}
bool PixelpartParticleType::is_position_relative() const {
	return particleType ? particleType->positionRelative() : false;
}

void PixelpartParticleType::set_rotation_mode(int mode) {
	if(!particleType) {
		return;
	}

	particleType->rotationMode(static_cast<pixelpart::RotationMode>(mode));
}
int PixelpartParticleType::get_rotation_mode() const {
	return particleType
		? static_cast<int>(particleType->rotationMode())
		: static_cast<int>(pixelpart::RotationMode::angle);
}

void PixelpartParticleType::set_alignment_mode(int mode) {
	if(!particleType) {
		return;
	}

	particleType->alignmentMode(static_cast<pixelpart::AlignmentMode>(mode));
}
int PixelpartParticleType::get_alignment_mode() const {
	return particleType
		? static_cast<int>(particleType->alignmentMode())
		: static_cast<int>(pixelpart::AlignmentMode::none);
}

Ref<PixelpartStaticPropertyFloat3> PixelpartParticleType::get_pivot() const {
	if(!particleType) {
		return Ref<PixelpartStaticPropertyFloat3>();
	}

	Ref<PixelpartStaticPropertyFloat3> property;
	property.instantiate();
	property->init(&particleType->pivot());

	return property;
}

Ref<PixelpartStaticPropertyFloat> PixelpartParticleType::get_motion_path_force() const {
	if(!particleType) {
		return Ref<PixelpartStaticPropertyFloat>();
	}

	Ref<PixelpartStaticPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->motionPathForce());

	return property;
}

void PixelpartParticleType::set_visible(bool mode) {
	if(!particleType) {
		return;
	}

	particleType->visible(mode);
}
bool PixelpartParticleType::is_visible() const {
	return particleType ? particleType->visible() : false;
}

void PixelpartParticleType::set_layer(int layer) {
	if(!particleType) {
		return;
	}

	particleType->layer(static_cast<uint32_t>(std::max(layer, 0)));
}
int PixelpartParticleType::get_layer() const {
	return particleType ? static_cast<int>(particleType->layer()) : 0;
}

Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleType::get_count() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->count());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleType::get_lifespan() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->lifespan());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleType::get_initial_size() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->initialSize());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat3> PixelpartParticleType::get_initial_rotation() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat3>();
	}

	Ref<PixelpartAnimatedPropertyFloat3> property;
	property.instantiate();
	property->init(&particleType->initialRotation());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleType::get_initial_velocity() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->initialVelocity());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleType::get_inherited_velocity() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->inheritedVelocity());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleType::get_initial_opacity() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->initialOpacity());

	return property;
}

Ref<PixelpartAnimatedPropertyFloat3> PixelpartParticleType::get_position() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat3>();
	}

	Ref<PixelpartAnimatedPropertyFloat3> property;
	property.instantiate();
	property->init(&particleType->motionPath());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat3> PixelpartParticleType::get_size() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat3>();
	}

	Ref<PixelpartAnimatedPropertyFloat3> property;
	property.instantiate();
	property->init(&particleType->size());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat3> PixelpartParticleType::get_stretch() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat3>();
	}

	Ref<PixelpartAnimatedPropertyFloat3> property;
	property.instantiate();
	property->init(&particleType->stretch());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleType::get_physical_size() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->physicalSize());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat3> PixelpartParticleType::get_rotation() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat3>();
	}

	Ref<PixelpartAnimatedPropertyFloat3> property;
	property.instantiate();
	property->init(&particleType->rotation());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat3> PixelpartParticleType::get_rotation_by_speed() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat3>();
	}

	Ref<PixelpartAnimatedPropertyFloat3> property;
	property.instantiate();
	property->init(&particleType->rotationBySpeed());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleType::get_acceleration() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->acceleration());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleType::get_radial_acceleration() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->radialAcceleration());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleType::get_weight() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->weight());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleType::get_bounce() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->bounce());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleType::get_friction() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->friction());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat4> PixelpartParticleType::get_color() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat4>();
	}

	Ref<PixelpartAnimatedPropertyFloat4> property;
	property.instantiate();
	property->init(&particleType->color());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartParticleType::get_opacity() const {
	if(!particleType) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->opacity());

	return property;
}

Ref<PixelpartStaticPropertyFloat> PixelpartParticleType::get_lifespan_variance() const {
	if(!particleType) {
		return Ref<PixelpartStaticPropertyFloat>();
	}

	Ref<PixelpartStaticPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->lifespanVariance());

	return property;
}
Ref<PixelpartStaticPropertyFloat> PixelpartParticleType::get_size_variance() const {
	if(!particleType) {
		return Ref<PixelpartStaticPropertyFloat>();
	}

	Ref<PixelpartStaticPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->sizeVariance());

	return property;
}
Ref<PixelpartStaticPropertyFloat3> PixelpartParticleType::get_rotation_variance() const {
	if(!particleType) {
		return Ref<PixelpartStaticPropertyFloat3>();
	}

	Ref<PixelpartStaticPropertyFloat3> property;
	property.instantiate();
	property->init(&particleType->rotationVariance());

	return property;
}
Ref<PixelpartStaticPropertyFloat3> PixelpartParticleType::get_angular_velocity_variance() const {
	if(!particleType) {
		return Ref<PixelpartStaticPropertyFloat3>();
	}

	Ref<PixelpartStaticPropertyFloat3> property;
	property.instantiate();
	property->init(&particleType->angularVelocityVariance());

	return property;
}
Ref<PixelpartStaticPropertyFloat> PixelpartParticleType::get_velocity_variance() const {
	if(!particleType) {
		return Ref<PixelpartStaticPropertyFloat>();
	}

	Ref<PixelpartStaticPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->motionPathForce());

	return property;
}
Ref<PixelpartStaticPropertyFloat4> PixelpartParticleType::get_color_variance() const {
	if(!particleType) {
		return Ref<PixelpartStaticPropertyFloat4>();
	}

	Ref<PixelpartStaticPropertyFloat4> property;
	property.instantiate();
	property->init(&particleType->colorVariance());

	return property;
}
Ref<PixelpartStaticPropertyFloat> PixelpartParticleType::get_opacity_variance() const {
	if(!particleType) {
		return Ref<PixelpartStaticPropertyFloat>();
	}

	Ref<PixelpartStaticPropertyFloat> property;
	property.instantiate();
	property->init(&particleType->opacityVariance());

	return property;
}

void PixelpartParticleType::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_id"), &PixelpartParticleType::get_id);
	ClassDB::bind_method(D_METHOD("get_parent_id"), &PixelpartParticleType::get_parent_id);
	ClassDB::bind_method(D_METHOD("get_name"), &PixelpartParticleType::get_name);
	ClassDB::bind_method(D_METHOD("set_position_relative", "mode"), &PixelpartParticleType::set_position_relative);
	ClassDB::bind_method(D_METHOD("is_position_relative"), &PixelpartParticleType::is_position_relative);
	ClassDB::bind_method(D_METHOD("set_rotation_mode", "mode"), &PixelpartParticleType::set_rotation_mode);
	ClassDB::bind_method(D_METHOD("get_rotation_mode"), &PixelpartParticleType::get_rotation_mode);
	ClassDB::bind_method(D_METHOD("set_alignment_mode", "mode"), &PixelpartParticleType::set_alignment_mode);
	ClassDB::bind_method(D_METHOD("get_alignment_mode"), &PixelpartParticleType::get_alignment_mode);
	ClassDB::bind_method(D_METHOD("get_pivot"), &PixelpartParticleType::get_pivot);
	ClassDB::bind_method(D_METHOD("get_motion_path_force"), &PixelpartParticleType::get_motion_path_force);
	ClassDB::bind_method(D_METHOD("set_visible", "mode"), &PixelpartParticleType::set_visible);
	ClassDB::bind_method(D_METHOD("is_visible"), &PixelpartParticleType::is_visible);
	ClassDB::bind_method(D_METHOD("set_layer", "layer"), &PixelpartParticleType::set_layer);
	ClassDB::bind_method(D_METHOD("get_layer"), &PixelpartParticleType::get_layer);
	ClassDB::bind_method(D_METHOD("get_count"), &PixelpartParticleType::get_count);
	ClassDB::bind_method(D_METHOD("get_lifespan"), &PixelpartParticleType::get_lifespan);
	ClassDB::bind_method(D_METHOD("get_initial_size"), &PixelpartParticleType::get_initial_size);
	ClassDB::bind_method(D_METHOD("get_initial_rotation"), &PixelpartParticleType::get_initial_rotation);
	ClassDB::bind_method(D_METHOD("get_initial_velocity"), &PixelpartParticleType::get_initial_velocity);
	ClassDB::bind_method(D_METHOD("get_inherited_velocity"), &PixelpartParticleType::get_inherited_velocity);
	ClassDB::bind_method(D_METHOD("get_initial_opacity"), &PixelpartParticleType::get_initial_opacity);
	ClassDB::bind_method(D_METHOD("get_position"), &PixelpartParticleType::get_position);
	ClassDB::bind_method(D_METHOD("get_size"), &PixelpartParticleType::get_size);
	ClassDB::bind_method(D_METHOD("get_stretch"), &PixelpartParticleType::get_stretch);
	ClassDB::bind_method(D_METHOD("get_physical_size"), &PixelpartParticleType::get_physical_size);
	ClassDB::bind_method(D_METHOD("get_rotation"), &PixelpartParticleType::get_rotation);
	ClassDB::bind_method(D_METHOD("get_rotation_by_speed"), &PixelpartParticleType::get_rotation_by_speed);
	ClassDB::bind_method(D_METHOD("get_acceleration"), &PixelpartParticleType::get_acceleration);
	ClassDB::bind_method(D_METHOD("get_radial_acceleration"), &PixelpartParticleType::get_radial_acceleration);
	ClassDB::bind_method(D_METHOD("get_weight"), &PixelpartParticleType::get_weight);
	ClassDB::bind_method(D_METHOD("get_bounce"), &PixelpartParticleType::get_bounce);
	ClassDB::bind_method(D_METHOD("get_friction"), &PixelpartParticleType::get_friction);
	ClassDB::bind_method(D_METHOD("get_color"), &PixelpartParticleType::get_color);
	ClassDB::bind_method(D_METHOD("get_opacity"), &PixelpartParticleType::get_opacity);
	ClassDB::bind_method(D_METHOD("get_lifespan_variance"), &PixelpartParticleType::get_lifespan_variance);
	ClassDB::bind_method(D_METHOD("get_size_variance"), &PixelpartParticleType::get_size_variance);
	ClassDB::bind_method(D_METHOD("get_rotation_variance"), &PixelpartParticleType::get_rotation_variance);
	ClassDB::bind_method(D_METHOD("get_angular_velocity_variance"), &PixelpartParticleType::get_angular_velocity_variance);
	ClassDB::bind_method(D_METHOD("get_velocity_variance"), &PixelpartParticleType::get_velocity_variance);
	ClassDB::bind_method(D_METHOD("get_color_variance"), &PixelpartParticleType::get_color_variance);
	ClassDB::bind_method(D_METHOD("get_opacity_variance"), &PixelpartParticleType::get_opacity_variance);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "position_relative"), "set_position_relative", "is_position_relative");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rotation_mode"), "set_rotation_mode", "get_rotation_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "alignment_mode"), "set_alignment_mode", "get_alignment_mode");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "visible"), "set_visible", "is_visible");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "layer"), "set_layer", "get_layer");

	// Deprecated
	ClassDB::bind_method(D_METHOD("get_num_particles"), &PixelpartParticleType::get_count);
}
}
