#ifndef PIXELPART_LIGHT_SOURCE_H
#define PIXELPART_LIGHT_SOURCE_H

#include "PixelpartNode.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include "../property/PixelpartAnimatedPropertyFloat4.h"
#include <pixelpart-runtime/effect/LightSource.h>

namespace godot {
class PixelpartLightSource : public PixelpartNode {
	GDCLASS(PixelpartLightSource, PixelpartNode)

public:
	PixelpartLightSource();
	virtual ~PixelpartLightSource();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	Ref<PixelpartAnimatedPropertyFloat> get_attenuation() const;
	Ref<PixelpartAnimatedPropertyFloat4> get_color() const;
	Ref<PixelpartAnimatedPropertyFloat> get_intensity() const;

protected:
	static void _bind_methods();

private:
	pixelpart::LightSource* lightSource = nullptr;
};
}

#endif