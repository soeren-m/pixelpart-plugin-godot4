#ifndef PIXELPART_SPOT_LIGHT_SOURCE_H
#define PIXELPART_SPOT_LIGHT_SOURCE_H

#include "PixelpartLightSource.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include <pixelpart-runtime/effect/SpotLightSource.h>

namespace godot {
class PixelpartSpotLightSource : public PixelpartLightSource {
	GDCLASS(PixelpartSpotLightSource, PixelpartLightSource)

public:
	PixelpartSpotLightSource();
	virtual ~PixelpartSpotLightSource();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	Ref<PixelpartAnimatedPropertyFloat> get_spot_angle() const;
	Ref<PixelpartAnimatedPropertyFloat> get_spot_angle_attenuation() const;

protected:
	static void _bind_methods();

private:
	pixelpart::SpotLightSource* spotLightSource = nullptr;
};
}

#endif