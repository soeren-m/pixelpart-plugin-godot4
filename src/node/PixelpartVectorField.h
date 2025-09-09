#ifndef PIXELPART_VECTOR_FIELD_H
#define PIXELPART_VECTOR_FIELD_H

#include "PixelpartForceField.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include <pixelpart-runtime/effect/VectorField.h>

namespace godot {
/**
 * @brief Force field that accelerates particles in the direction of a predefined grid of vectors.
 *
 * A particle in the field follows the vector specified in the vector field at that location.
 */
class PixelpartVectorField : public PixelpartForceField {
	GDCLASS(PixelpartVectorField, PixelpartForceField)

public:
	PixelpartVectorField();
	virtual ~PixelpartVectorField();

#ifdef DOXYGEN
	/**
	 * @brief How velocity values are interpolated between the cells of the vector field.
	 */
	int vector_filter;
#endif

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	/**
	 * @brief Set how velocity values are interpolated between the cells of the vector field.
	 *
	 * @param filter Filtering algorithm to use
	 */
	void set_vector_filter(int filter);

	/**
	 * @brief How velocity values are interpolated between the cells of the vector field.
	 *
	 * @return Filtering algorithm used
	 */
	int get_vector_filter() const;

	/**
	 * @brief How directly particles follow the vectors in the force field.
	 *
	 * If set to 0, the field applies forces to the particles over time.
	 * If set to 1, particles follow the vectors in the force field directly.
	 *
	 * @return Tightness property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_tightness() const;

protected:
	static void _bind_methods();

private:
	pixelpart::VectorField* vectorField = nullptr;
};
}

#endif
