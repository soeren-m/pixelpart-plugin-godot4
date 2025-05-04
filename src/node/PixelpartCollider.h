#ifndef PIXELPART_COLLIDER_H
#define PIXELPART_COLLIDER_H

#include "PixelpartNode.h"
#include "../property/PixelpartStaticPropertyBool.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include <pixelpart-runtime/effect/Collider.h>

namespace godot {
class PixelpartCollider : public PixelpartNode {
	GDCLASS(PixelpartCollider, PixelpartNode)

public:
	PixelpartCollider();
	virtual ~PixelpartCollider();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	Ref<PixelpartStaticPropertyBool> get_kill_on_contact() const;
	Ref<PixelpartAnimatedPropertyFloat> get_bounce() const;
	Ref<PixelpartAnimatedPropertyFloat> get_friction() const;

protected:
	static void _bind_methods();

private:
	pixelpart::Collider* collider = nullptr;
};
}

#endif