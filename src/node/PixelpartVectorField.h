#ifndef PIXELPART_VECTOR_FIELD_H
#define PIXELPART_VECTOR_FIELD_H

#include "PixelpartForceField.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include <pixelpart-runtime/effect/VectorField.h>

namespace godot {
class PixelpartVectorField : public PixelpartForceField {
	GDCLASS(PixelpartVectorField, PixelpartForceField)

public:
	PixelpartVectorField();
	virtual ~PixelpartVectorField();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	void set_vector_filter(int filter);
	int get_vector_filter() const;

	Ref<PixelpartAnimatedPropertyFloat> get_tightness() const;

protected:
	static void _bind_methods();

private:
	pixelpart::VectorField* vectorField = nullptr;
};
}

#endif