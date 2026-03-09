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
	/**
	 * @brief Types of emitter shapes.
 	 */
	enum ShapeType {
		/**
		 * The emitter generates particles at a single point.
		 */
		SHAPE_POINT = 0,

		/**
		 * Particles are emitted on a straight line.
		 */
		SHAPE_LINE = 1,

		/**
		 * Particles are emitted inside a circular or elliptical shape.
		 */
		SHAPE_ELLIPSE = 2,

		/**
		 * Particles are emitted inside a rectangle.
		 */
		SHAPE_RECTANGLE = 3,

		/**
		 * Particles are emitted on a custom path consisting of connected line segments.
		 */
		SHAPE_PATH = 4,

		/**
		 * Particles are emitted inside a sphere or ellipsoid. Only for 3D effects.
		 */
		SHAPE_ELLIPSOID = 5,

		/**
		 * Particles are emitted inside a cube. Only for 3D effects.
		 */
		SHAPE_CUBOID = 6,

		/**
		 * Particles are emitted inside a cylinder. Only for 3D effects.
		 */
		SHAPE_CYLINDER = 7
	};

	/**
	 * @brief Particle distribution modes.
	 *
	 * When spawning particles inside the emitter shape, particle emitters can use different techniques
	 * to determine the exact points where particles are spawned.
	 * For example, you may want to spawn particles near the edge of the shape or in a grid-like pattern.
 	 */
	enum DistributionType {
		/**
		 * Particles are spawned at randomly generated locations (uniform distribution) inside the emitter area.
		 */
		DISTRIBUTION_UNIFORM = 0,

		/**
		 * Particles are created mostly around the center and less at the boundary.
		 */
		DISTRIBUTION_CENTER = 1,

		/**
		 * More particles spawn near the emitter boundary and less in the center.
		 */
		DISTRIBUTION_HOLE = 2,

		/**
		 * Particles are created only on the emitter boundary.
		 */
		DISTRIBUTION_BOUNDARY = 3,

		/**
		 * Particles spawn at random points of a grid.
		 */
		DISTRIBUTION_GRID_RANDOM = 4,

		/**
		 * Particles are created on points in a grid, one grid point after the other.
		 */
		DISTRIBUTION_GRID_ORDERED = 5
	};

	/**
	 * @brief Order of points for grid distribution mode.
 	 */
	enum GridOrderType {
		/**
		 * X -> Y -> Z
		 */
		GRID_ORDER_X_Y_Z = 0,

		/**
		 * X -> Z -> Y
		 */
		GRID_ORDER_X_Z_Y = 1,

		/**
		 * Y -> X -> Z
		 */
		GRID_ORDER_Y_X_Z = 2,

		/**
		 * Y -> Z -> X
		 */
		GRID_ORDER_Y_Z_X = 3,

		/**
		 * Z -> X -> Y
		 */
		GRID_ORDER_Z_X_Y = 4,

		/**
		 * Z -> Y -> X
		 */
		GRID_ORDER_Z_Y_X = 5
	};

	/**
	 * @brief Modes for particle emission.
	 *
	 * The emission mode of particle emitters controls when particles are created during the lifetime of the emitter.
 	 */
	enum EmissionMode {
		/**
		 * The emitter continuously spawns particles throughout its lifetime.
		 */
		EMISSION_CONTINUOUS = 0,

		/**
		 * The emitter instantiates all particles immediately after being created.
		 */
		EMISSION_BURST_START = 1,

		/**
		 * Particles are spawned at the end of the emitter lifetime, or
		 * - if the emitter is a sub-emitter - when particles of the parent emitter
		 * reach the end of their lifetime.
		 */
		EMISSION_BURST_END = 2
	};

	/**
	 * @brief Modes to determine the initial direction of particles.
	 *
	 * When a particle is created by the particle emitter,
	 * it decides in which direction to emit the particle
	 * based on the specified direction and the given direction mode.
 	 */
	enum DirectionMode {
		/**
		 * Particles shoot off in a common direction.
		 */
		DIRECTION_FIXED = 0,

		/**
		 * Particles move away from the emitter.
		 */
		DIRECTION_OUTWARDS = 1,

		/**
		 * Particles move towards the emitter’s center.
		 */
		DIRECTION_INWARDS = 2,

		/**
		 * Particles move along the direction of the parent particle or emitter.
		 */
		DIRECTION_INHERIT = 3,

		/**
		 * Particles move against the direction of the parent particle or emitter.
		 */
		DIRECTION_INHERIT_INVERSE = 4
	};

	PixelpartParticleEmitter();
	virtual ~PixelpartParticleEmitter();

#ifdef DOXYGEN
	/**
	 * @brief Shape of the emitter area.
	 */
	ShapeType shape;

	/**
	 * @brief Where particles are spawned inside the emitter area.
	 */
	DistributionType distribution;

	/**
	 * @brief Grid order of grid distribution modes.
	 */
	GridOrderType grid_order;

	/**
	 * @brief Emission mode to control when particles are created
	 * during the lifetime of the emitter.
	 */
	EmissionMode emission_mode;

	/**
	 * @brief Mode to control initial particle direction.
	 *
	 * When a particle is created by the particle emitter,
	 * it decides in which direction to emit the particle based on the specified direction and the given direction mode.
	 */
	DirectionMode direction_mode;
 #endif

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	/**
	 * @brief Set shape of the emitter area.
	 *
	 * @param type Shape of the emitter area
	 */
	void set_shape(ShapeType type);

	/**
	 * @brief Return shape of the emitter area.
	 *
	 * @return Shape of the emitter area
	 */
	ShapeType get_shape() const;

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
	void set_distribution(DistributionType mode);

	/**
	 * @brief Return where particles are spawned inside the emitter area.
	 *
	 * When spawning particles inside the emitter shape,
	 * particle emitters can use different techniques to determine the exact points where particles are spawned.
	 *
	 * @return Distribution mode
	 */
	DistributionType get_distribution() const;

	/**
	 * @brief Set the grid order of grid distribution modes.
	 *
	 * @param mode Grid order
	 */
	void set_grid_order(GridOrderType mode);

	/**
	 * @brief Get the grid order of grid distribution modes.
	 *
	 * @return Grid order
	 */
	GridOrderType get_grid_order() const;

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
	void set_emission_mode(EmissionMode mode);

	/**
	 * @brief Return the emission mode.
	 *
	 * The emission mode of particle emitters controls
	 * when particles are created during the lifetime of the emitter.
	 *
	 * @return Emission mode
	 */
	EmissionMode get_emission_mode() const;

	/**
	 * @brief Set the direction mode.
	 *
	 * When a particle is created by the particle emitter,
	 * it decides in which direction to emit the particle based on the specified direction and the given direction mode.
	 *
	 * @param mode Direction mode
	 */
	void set_direction_mode(DirectionMode mode);

	/**
	 * @brief Get the direction mode
	 *
	 * When a particle is created by the particle emitter,
	 * it decides in which direction to emit the particle based on the specified direction and the given direction mode.
	 *
	 * @return Direction mode
	 */
	DirectionMode get_direction_mode() const;

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

VARIANT_ENUM_CAST(PixelpartParticleEmitter::ShapeType);
VARIANT_ENUM_CAST(PixelpartParticleEmitter::DistributionType);
VARIANT_ENUM_CAST(PixelpartParticleEmitter::GridOrderType);
VARIANT_ENUM_CAST(PixelpartParticleEmitter::EmissionMode);
VARIANT_ENUM_CAST(PixelpartParticleEmitter::DirectionMode);

#endif
