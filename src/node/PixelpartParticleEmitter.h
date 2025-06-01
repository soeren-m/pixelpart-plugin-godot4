#ifndef PIXELPART_PARTICLE_EMITTER_H
#define PIXELPART_PARTICLE_EMITTER_H

#include "PixelpartNode.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include "../property/PixelpartAnimatedPropertyFloat3.h"
#include <pixelpart-runtime/effect/ParticleEmitter.h>
#include <vector>

namespace godot {
/**
 * @brief Node that emits particles.
 *
 * Particle emitters continuously spawn new particles of assigned particle types in a defined area.
 * They also define in which direction particles move initially.
 */
class PixelpartParticleEmitter : public PixelpartNode {
	GDCLASS(PixelpartParticleEmitter, PixelpartNode)

public:
	PixelpartParticleEmitter();
	virtual ~PixelpartParticleEmitter();

#ifdef DOXYGEN
	/**
	 * @brief Shape of the emitter area.
	 */
	int shape;

	/**
	 * @brief Where particles are spawned inside the emitter area.
	 */
	int distribution;

	/**
	 * @brief Grid order of grid distribution modes.
	 */
	int grid_order;

	/**
	 * @brief Emission mode to control when particles are created
	 * during the lifetime of the emitter.
	 */
	int emission_mode;

	/**
	 * @brief Mode to control initial particle direction.
	 *
	 * When a particle is created by the particle emitter,
	 * it decides in which direction to emit the particle based on the specified direction and the given direction mode.
	 */
	int direction_mode;
 #endif

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	/**
	 * @brief Set shape of the emitter area.
	 *
	 * @param type Shape of the emitter area
	 */
	void set_shape(int type);

	/**
	 * @brief Return shape of the emitter area.
	 *
	 * @return Shape of the emitter area
	 */
	int get_shape() const;

	/**
	 * @brief Add a new point to the emitter shape.
	 *
	 * Only applicable for emitter shape `Path`.
	 *
	 * @param point Point to add
	 */
	void add_shape_point(Vector3 point);

	/**
	 * @brief Remove the point at the given index from the emitter shape.
	 *
	 * Only applicable for emitter shape `Path`.
	 *
	 * @param index Index of point to remove
	 */
	void remove_shape_point(int index);

	/**
	 * @brief Change the location of the point at the given index in the emitter shape.
	 *
	 * Only applicable for emitter shape `Path`.
	 *
	 * @param index Index of point to change
	 * @param point New location
	 */
	void set_shape_point(int index, Vector3 point);

	/**
	 * @brief Return the location of the point at the given index in the emitter shape.
	 *
	 * @param index Index of point
	 * @return Vector3 Location of point
	 */
	Vector3 get_shape_point(int index) const;

	/**
	 * @brief Return the number of points in the emitter shape.
	 *
	 * Only applicable for emitter shape `Path`.
	 *
	 * @return Number of points
	 */
	int get_shape_point_count() const;

	/**
	 * @brief Set where particles are spawned inside the emitter area.
	 *
	 * When spawning particles inside the emitter shape,
	 * particle emitters can use different techniques to determine the exact points where particles are spawned.
	 *
	 * @param mode Distribution mode
	 */
	void set_distribution(int mode);

	/**
	 * @brief Return where particles are spawned inside the emitter area.
	 *
	 * When spawning particles inside the emitter shape,
	 * particle emitters can use different techniques to determine the exact points where particles are spawned.
	 *
	 * @return Distribution mode
	 */
	int get_distribution() const;

	/**
	 * @brief Set the grid order of grid distribution modes.
	 *
	 * @param mode Grid order
	 */
	void set_grid_order(int mode);

	/**
	 * @brief Get the grid order of grid distribution modes.
	 *
	 * @return Grid order
	 */
	int get_grid_order() const;

	/**
	 * @brief Set the number of grid cells.
	 *
	 * Only applicable for grid distribution modes.
	 *
	 * @param x Number of cells in X direction
	 * @param y Number of cells in Y direction
	 * @param z Number of cells in Z direction
	 */
	void set_grid_size(int x, int y, int z);

	/**
	 * @brief Return the number of grid cells in X direction.
	 *
	 * Only applicable for grid distribution modes.
	 *
	 * @return Number of cells
	 */
	int get_grid_size_x() const;

	/**
	 * @brief Return the number of grid cells in Y direction.
	 *
	 * Only applicable for grid distribution modes.
	 *
	 * @return Number of cells
	 */
	int get_grid_size_y() const;

	/**
	 * @brief Return the number of grid cells in Z direction.
	 *
	 * Only applicable for grid distribution modes.
	 *
	 * @return Number of cells
	 */
	int get_grid_size_z() const;

	/**
	 * @brief Set the emission mode.
	 *
	 * The emission mode of particle emitters controls
	 * when particles are created during the lifetime of the emitter.
	 *
	 * @param mode Emission mode
	 */
	void set_emission_mode(int mode);

	/**
	 * @brief Return the emission mode.
	 *
	 * The emission mode of particle emitters controls
	 * when particles are created during the lifetime of the emitter.
	 *
	 * @return Emission mode
	 */
	int get_emission_mode() const;

	/**
	 * @brief Set the direction mode.
	 *
	 * When a particle is created by the particle emitter,
	 * it decides in which direction to emit the particle based on the specified direction and the given direction mode.
	 *
	 * @param mode Direction mode
	 */
	void set_direction_mode(int mode);

	/**
	 * @brief Get the direction mode
	 *
	 * When a particle is created by the particle emitter,
	 * it decides in which direction to emit the particle based on the specified direction and the given direction mode.
	 *
	 * @return Direction mode
	 */
	int get_direction_mode() const;

	/**
	 * @brief Direction in which particles move when spawned, in degrees.
	 *
	 * For 3D effects, the direction is composed of roll, yaw and pitch.
	 *
	 * @return Direction property
	 */
	Ref<PixelpartAnimatedPropertyFloat3> get_direction() const;

	/**
	 * @brief How much the direction varies.
	 *
	 * A spread of 0° means that particles only move in the specified direction,
	 * while 360° leads to particles shooting off in all directions.
	 * @return Spread property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_spread() const;

protected:
	static void _bind_methods();

private:
	void set_shape_points(const std::vector<pixelpart::float3_t>& points);
	std::vector<pixelpart::float3_t> get_shape_points() const;

	pixelpart::ParticleEmitter* particleEmitter = nullptr;
};
}

#endif