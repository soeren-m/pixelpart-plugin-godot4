#ifndef PIXELPART_POINT_LIGHT_SOURCE_H
#define PIXELPART_POINT_LIGHT_SOURCE_H

#include "PixelpartLightSource.h"

namespace godot {
/**
 * @brief Light source used to model light coming from a single point emitting light in all directions.
 *
 */
class PixelpartPointLightSource : public PixelpartLightSource {
	GDCLASS(PixelpartPointLightSource, PixelpartLightSource)

public:
	PixelpartPointLightSource();
	virtual ~PixelpartPointLightSource();

protected:
	static void _bind_methods();
};
}

#endif