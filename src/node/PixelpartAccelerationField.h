#ifndef PIXELPART_ACCELERATION_FIELD_H
#define PIXELPART_ACCELERATION_FIELD_H

#include "PixelpartForceField.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include "../property/PixelpartAnimatedPropertyFloat3.h"
#include <pixelpart-runtime/effect/AccelerationField.h>
#include <random>

namespace godot {
class PixelpartAccelerationField : public PixelpartForceField {
	GDCLASS(PixelpartAccelerationField, PixelpartForceField)

public:
	PixelpartAccelerationField();
	virtual ~PixelpartAccelerationField();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	Ref<PixelpartAnimatedPropertyFloat3> get_acceleration_direction() const;
	Ref<PixelpartAnimatedPropertyFloat> get_acceleration_direction_variance() const;
	Ref<PixelpartAnimatedPropertyFloat> get_acceleration_strength_variance() const;

	void set_acceleration_grid_size(int x, int y, int z);
	int get_acceleration_grid_size_x() const;
	int get_acceleration_grid_size_y() const;
	int get_acceleration_grid_size_z() const;

protected:
	static void _bind_methods();

private:
	pixelpart::AccelerationField* accelerationField = nullptr;

	std::mt19937 rng;
};
}

#endif