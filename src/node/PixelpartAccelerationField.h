#ifndef PIXELPART_ACCELERATION_FIELD_H
#define PIXELPART_ACCELERATION_FIELD_H

#include "PixelpartForceField.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include "../property/PixelpartAnimatedPropertyFloat3.h"
#include <pixelpart-runtime/effect/AccelerationField.h>
#include <random>

namespace godot {
/**
 * @brief Force field that accelerates particles towards a given direction.
 *
 * Acceleration fields also allow you to subdivide the force area with a grid,
 * where each grid cell has a different force direction and strength.
 * This can be useful for organic looking particle motion.
 */
class PixelpartAccelerationField : public PixelpartForceField {
	GDCLASS(PixelpartAccelerationField, PixelpartForceField)

public:
	PixelpartAccelerationField();
	virtual ~PixelpartAccelerationField();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	/**
	 * @brief Direction in which particles are accelerated, in degrees.
	 * For 3D effects, the direction is composed of roll, yaw and pitch.
	 *
	 * @return Acceleration direction property
	 */
	Ref<PixelpartAnimatedPropertyFloat3> get_acceleration_direction() const;

	/**
	 * @brief How much the force direction varies between cells of the grid.
	 *
	 * @return Acceleration direction variance property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_acceleration_direction_variance() const;

	/**
	 * @brief How much the force strength varies between cells of the grid.
	 *
	 * @return Acceleration strength variance property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_acceleration_strength_variance() const;

	/**
	 * @brief Set the number of cells in each dimension of the force field grid.
	 *
	 * Each cell of the grid varies in direction and strength determined by the direction and strength variance values.
	 *
	 * @param x Size on X axis
	 * @param y Size on Y axis
	 * @param z Size on Z axis
	 */
	void set_acceleration_grid_size(int x, int y, int z);

	/**
	 * @brief The number of grid cells on the X axis of the force field grid.
	 *
	 * @return Size on X axis
	 */
	int get_acceleration_grid_size_x() const;

	/**
	 * @brief The number of grid cells on the Y axis of the force field grid.
	 *
	 * @return Size on Y axis
	 */
	int get_acceleration_grid_size_y() const;

	/**
	 * @brief The number of grid cells on the Z axis of the force field grid.
	 *
	 * @return Size on Z axis
	 */
	int get_acceleration_grid_size_z() const;

protected:
	static void _bind_methods();

private:
	pixelpart::AccelerationField* accelerationField = nullptr;

	std::mt19937 rng;
};
}

#endif
