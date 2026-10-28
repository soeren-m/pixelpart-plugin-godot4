#ifndef PIXELPART_PARTICLE_INSTANCE_3D_H
#define PIXELPART_PARTICLE_INSTANCE_3D_H

#include <pixelpart-runtime/types/Types.h>

namespace godot {
class PixelpartParticleInstance3D {
public:
	virtual ~PixelpartParticleInstance3D() = default;

	virtual void draw(pixelpart::float_t scale) = 0;
};
}

#endif
