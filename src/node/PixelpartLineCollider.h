#ifndef PIXELPART_LINE_COLLIDER_H
#define PIXELPART_LINE_COLLIDER_H

#include "PixelpartCollider.h"
#include "../property/PixelpartStaticPropertyBool.h"
#include "../property/PixelpartStaticPropertyFloat.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include <pixelpart-runtime/effect/LineCollider.h>

namespace godot {
class PixelpartLineCollider : public PixelpartCollider {
	GDCLASS(PixelpartLineCollider, PixelpartCollider)

public:
	PixelpartLineCollider();
	virtual ~PixelpartLineCollider();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	void add_point(Vector3 point);
	void set_point(int index, Vector3 point);
	void remove_point(int index);
	Vector3 get_point(int index) const;
	int get_point_count() const;

protected:
	static void _bind_methods();

private:
	pixelpart::LineCollider* lineCollider = nullptr;
};
}

#endif