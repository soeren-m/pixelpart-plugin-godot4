#ifndef PIXELPART_VORTEX_FIELD_H
#define PIXELPART_VORTEX_FIELD_H

#include "PixelpartForceField.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include <pixelpart-runtime/effect/VortexField.h>

namespace godot {
/**
 * @brief Force field that moves particles in a vortex around its center.
 */
class PixelpartVortexField : public PixelpartForceField {
	GDCLASS(PixelpartVortexField, PixelpartForceField)

public:
	PixelpartVortexField();
	virtual ~PixelpartVortexField();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	/**
	 * @brief How strongly particles are accelerated outwards.
	 *
	 * @return Tangential strength property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_tangential_strength() const;

	/**
	 * @brief How strongly the vortex field attracts particles.
	 *
	 * @return Radial strength property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_radial_strength() const;

protected:
	static void _bind_methods();

private:
	pixelpart::VortexField* vortexField = nullptr;
};
}

#endif
