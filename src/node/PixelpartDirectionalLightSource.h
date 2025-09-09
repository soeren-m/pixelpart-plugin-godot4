#ifndef PIXELPART_DIRECTIONAL_LIGHT_SOURCE_H
#define PIXELPART_DIRECTIONAL_LIGHT_SOURCE_H

#include "PixelpartLightSource.h"

namespace godot {
/**
 * @brief Light source with light coming from a specific direction.
 *
 * Direction lights mimic light sources that are very far away and consequently contain light rays of almost parallel direction.
 * Such light sources are useful to simulate sunlight, for example.
 * The direction is determined by the node's orientation property.
 */
class PixelpartDirectionalLightSource : public PixelpartLightSource {
	GDCLASS(PixelpartDirectionalLightSource, PixelpartLightSource)

public:
	PixelpartDirectionalLightSource();
	virtual ~PixelpartDirectionalLightSource();

protected:
	static void _bind_methods();
};
}

#endif
