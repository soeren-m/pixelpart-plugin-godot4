#ifndef PIXELPART_LIGHT_SOURCE_H
#define PIXELPART_LIGHT_SOURCE_H

#include "PixelpartNode.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include "../property/PixelpartAnimatedPropertyFloat4.h"
#include <pixelpart-runtime/effect/LightSource.h>

namespace godot {
/**
 * @brief Node that emits light.
 *
 * Light sources illuminate the effect and can be used to model phenomena like sunlight, lamps and torches.
 * Particles with a material that has lighting enabled (for example built-in materials with the suffix Lit)
 * are shaded by light from light sources in the scene.
 *
 * The Godot plugin uses the light sources defined in the scene to apply lighting to effects. To use
 * light sources defined in the Pixelpart effect file, you need to sync them manually.
 */
class PixelpartLightSource : public PixelpartNode {
	GDCLASS(PixelpartLightSource, PixelpartNode)

public:
	PixelpartLightSource();
	virtual ~PixelpartLightSource();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	/**
	 * @brief How quickly the light looses intensity on the way to its outer radius.
	 *
	 * @return Attenuation property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_attenuation() const;

	/**
	 * @brief Color of the light emitted by the light source.
	 *
	 * @return Color property
	 */
	Ref<PixelpartAnimatedPropertyFloat4> get_color() const;

	/**
	 * @brief How strongly the light shines.
	 *
	 * @return Intensity property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_intensity() const;

protected:
	static void _bind_methods();

private:
	pixelpart::LightSource* lightSource = nullptr;
};
}

#endif
