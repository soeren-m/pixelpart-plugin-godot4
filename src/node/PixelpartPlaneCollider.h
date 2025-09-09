#ifndef PIXELPART_PLANE_COLLIDER_H
#define PIXELPART_PLANE_COLLIDER_H

#include "PixelpartCollider.h"

namespace godot {
/**
 * @brief Collider consisting of a single line (2D) or plane (3D) particles collide with.
 *
 */
class PixelpartPlaneCollider : public PixelpartCollider {
	GDCLASS(PixelpartPlaneCollider, PixelpartCollider)

public:
	PixelpartPlaneCollider();
	virtual ~PixelpartPlaneCollider();

protected:
	static void _bind_methods();
};
}

#endif
