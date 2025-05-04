#ifndef PIXELPART_FORCE_FIELD_H
#define PIXELPART_FORCE_FIELD_H

#include "PixelpartNode.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include <pixelpart-runtime/effect/ForceField.h>

namespace godot {
class PixelpartForceField : public PixelpartNode {
	GDCLASS(PixelpartForceField, PixelpartNode)

public:
	PixelpartForceField();
	virtual ~PixelpartForceField();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	void set_infinite(bool mode);
	bool is_infinite() const;

	Ref<PixelpartAnimatedPropertyFloat> get_strength() const;

protected:
	static void _bind_methods();

private:
	pixelpart::ForceField* forceField = nullptr;
};
}

#endif