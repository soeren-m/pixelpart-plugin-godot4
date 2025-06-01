#ifndef PIXELPART_COLLIDER_H
#define PIXELPART_COLLIDER_H

#include "PixelpartNode.h"
#include "../property/PixelpartStaticPropertyBool.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include <pixelpart-runtime/effect/Collider.h>

namespace godot {
/**
 * @brief Node particles collide with.
 *
 * When hitting a collider, particles bounce off its surface.
 */
class PixelpartCollider : public PixelpartNode {
	GDCLASS(PixelpartCollider, PixelpartNode)

public:
	PixelpartCollider();
	virtual ~PixelpartCollider();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	/**
	 * @brief Whether to kill particles on contact with the collider.
	 *
	 * @return Kill-on-contact property
	 */
	Ref<PixelpartStaticPropertyBool> get_kill_on_contact() const;

	/**
	 * @brief How strongly particles bounce back from the collider.
	 *
	 * @return Bounce property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_bounce() const;

	/**
	 * @brief How much particles are slowed down when sliding down the collider.
	 *
	 * @return Friction property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_friction() const;

protected:
	static void _bind_methods();

private:
	pixelpart::Collider* collider = nullptr;
};
}

#endif