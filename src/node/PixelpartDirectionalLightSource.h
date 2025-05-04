#ifndef PIXELPART_DIRECTIONAL_LIGHT_SOURCE_H
#define PIXELPART_DIRECTIONAL_LIGHT_SOURCE_H

#include "PixelpartLightSource.h"

namespace godot {
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