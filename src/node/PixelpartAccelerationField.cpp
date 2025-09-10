#include "PixelpartAccelerationField.h"
#include <godot_cpp/core/class_db.hpp>
#include <algorithm>

namespace godot {
PixelpartAccelerationField::PixelpartAccelerationField() : PixelpartForceField() {

}
PixelpartAccelerationField::~PixelpartAccelerationField() {

}

void PixelpartAccelerationField::init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) {
	PixelpartForceField::init(internalNode, effectEnginePtr);

	accelerationField = dynamic_cast<pixelpart::AccelerationField*>(internalNode);
}

Ref<PixelpartAnimatedPropertyFloat3> PixelpartAccelerationField::get_acceleration_direction() const {
	if(!accelerationField) {
		return Ref<PixelpartAnimatedPropertyFloat3>();
	}

	Ref<PixelpartAnimatedPropertyFloat3> property;
	property.instantiate();
	property->init(&accelerationField->accelerationDirection());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartAccelerationField::get_acceleration_direction_variance() const {
	if(!accelerationField) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&accelerationField->accelerationDirectionVariance());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartAccelerationField::get_acceleration_strength_variance() const {
	if(!accelerationField) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&accelerationField->accelerationStrengthVariance());

	return property;
}

void PixelpartAccelerationField::set_acceleration_grid_size(int x, int y, int z) {
	if(!accelerationField) {
		return;
	}

	accelerationField->accelerationGrid(
		static_cast<std::int32_t>(std::max(x, 1)),
		static_cast<std::int32_t>(std::max(y, 1)),
		static_cast<std::int32_t>(std::max(z, 1)));
	accelerationField->randomizeAccelerationGrid(rng);
}
int PixelpartAccelerationField::get_acceleration_grid_size_x() const {
	return accelerationField ? static_cast<int>(accelerationField->accelerationGridSizeX()) : 1;
}
int PixelpartAccelerationField::get_acceleration_grid_size_y() const {
	return accelerationField ? static_cast<int>(accelerationField->accelerationGridSizeY()) : 1;
}
int PixelpartAccelerationField::get_acceleration_grid_size_z() const {
	return accelerationField ? static_cast<int>(accelerationField->accelerationGridSizeZ()) : 1;
}

void PixelpartAccelerationField::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_acceleration_direction"), &PixelpartAccelerationField::get_acceleration_direction);
	ClassDB::bind_method(D_METHOD("get_acceleration_direction_variance"), &PixelpartAccelerationField::get_acceleration_direction_variance);
	ClassDB::bind_method(D_METHOD("get_acceleration_strength_variance"), &PixelpartAccelerationField::get_acceleration_strength_variance);
	ClassDB::bind_method(D_METHOD("set_acceleration_grid_size", "x", "y", "z"), &PixelpartAccelerationField::set_acceleration_grid_size);
	ClassDB::bind_method(D_METHOD("get_acceleration_grid_size_x"), &PixelpartAccelerationField::get_acceleration_grid_size_x);
	ClassDB::bind_method(D_METHOD("get_acceleration_grid_size_y"), &PixelpartAccelerationField::get_acceleration_grid_size_y);
	ClassDB::bind_method(D_METHOD("get_acceleration_grid_size_z"), &PixelpartAccelerationField::get_acceleration_grid_size_z);

	// Deprecated
	ClassDB::bind_method(D_METHOD("get_acceleration_grid_width"), &PixelpartAccelerationField::get_acceleration_grid_size_x);
	ClassDB::bind_method(D_METHOD("get_acceleration_grid_height"), &PixelpartAccelerationField::get_acceleration_grid_size_y);
	ClassDB::bind_method(D_METHOD("get_acceleration_grid_depth"), &PixelpartAccelerationField::get_acceleration_grid_size_z);
}
}
