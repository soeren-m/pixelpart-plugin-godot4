#ifndef PIXELPART_FORCE_FIELD_H
#define PIXELPART_FORCE_FIELD_H

#include "PixelpartNode.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include <pixelpart-runtime/effect/ForceField.h>

namespace godot {
/**
 * @brief Node that interacts with particles in a given area by applying forces
 * and changing the particles' velocity.
 *
 * Force fields affect particles by attracting, repelling or accelerating them in some direction
 * and can be used to model gravity, wind and other natural effects.
 */
class PixelpartForceField : public PixelpartNode {
	GDCLASS(PixelpartForceField, PixelpartNode)

public:
	PixelpartForceField();
	virtual ~PixelpartForceField();

#ifdef DOXYGEN
	/**
	 * @brief Whether the force field has an infinite area of effect.
	 */
	bool infinite;
#endif

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	/**
	 * @brief Set whether the force field has an infinite area of effect.
	 *
	 * @param mode Whether the force field has an infinite area of effect.
	 */
	void set_infinite(bool mode);

	/**
	 * @brief Whether the force field has an infinite area of effect.
	 *
	 * @return Whether the force field has an infinite area of effect.
	 */
	bool is_infinite() const;

	/**
	 * @brief How strongly particles are affected by the force field.
	 *
	 * Negative values reverse the force direction. Attraction fields then repel particles, for example.
	 *
	 * @return Strength propery
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_strength() const;

protected:
	static void _bind_methods();

private:
	pixelpart::ForceField* forceField = nullptr;
};
}

#endif
