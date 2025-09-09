#ifndef PIXELPART_ATTRACTION_FIELD_H
#define PIXELPART_ATTRACTION_FIELD_H

#include "PixelpartForceField.h"

namespace godot {
/**
 * @brief Force field that accelerates particles towards or away from its center.
 *
 * Attraction fields are spherical force fields that pull particles towards their center.
 * You can also make them repel particles by setting a negative strength value.
 */
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
