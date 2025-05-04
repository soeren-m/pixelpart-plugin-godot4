#ifndef PIXELPART_DRAG_FIELD_H
#define PIXELPART_DRAG_FIELD_H

#include "PixelpartForceField.h"
#include "../property/PixelpartStaticPropertyFloat.h"
#include <pixelpart-runtime/effect/DragField.h>

namespace godot {
class PixelpartDragField : public PixelpartForceField {
	GDCLASS(PixelpartDragField, PixelpartForceField)

public:
	PixelpartDragField();
	virtual ~PixelpartDragField();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	Ref<PixelpartStaticPropertyFloat> get_velocity_influence() const;
	Ref<PixelpartStaticPropertyFloat> get_size_influence() const;

protected:
	static void _bind_methods();

private:
	pixelpart::DragField* dragField = nullptr;
};
}

#endif