#ifndef PIXELPART_ATTRACTION_FIELD_H
#define PIXELPART_ATTRACTION_FIELD_H

#include "PixelpartForceField.h"

namespace godot {
class PixelpartAttractionField : public PixelpartForceField {
	GDCLASS(PixelpartAttractionField, PixelpartForceField)

public:
	PixelpartAttractionField();
	virtual ~PixelpartAttractionField();

protected:
	static void _bind_methods();
};
}

#endif