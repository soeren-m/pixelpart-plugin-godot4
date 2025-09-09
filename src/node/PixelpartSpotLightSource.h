#ifndef PIXELPART_SPOT_LIGHT_SOURCE_H
#define PIXELPART_SPOT_LIGHT_SOURCE_H

#include "PixelpartLightSource.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include <pixelpart-runtime/effect/SpotLightSource.h>

namespace godot {
/**
 * @brief Light source similar to a point light but emitted light is restricted to a cone shape.
 *
 * The direction is determined by the node's orientation property.
 */
class PixelpartSpotLightSource : public PixelpartLightSource {
	GDCLASS(PixelpartSpotLightSource, PixelpartLightSource)

public:
	PixelpartSpotLightSource();
	virtual ~PixelpartSpotLightSource();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	/**
	 * @brief Width of the light cone in degrees.
	 *
	 * @return Spot angle property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_spot_angle() const;

	/**
	 * @brief How much the light looses intensity near the edge of the light cone.
	 *
	 * @return Spot angle attenuation property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_spot_angle_attenuation() const;

protected:
	static void _bind_methods();

private:
	pixelpart::SpotLightSource* spotLightSource = nullptr;
};
}

#endif
