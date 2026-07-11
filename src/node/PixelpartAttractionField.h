#ifndef PIXELPART_ATTRACTION_FIELD_H
#define PIXELPART_ATTRACTION_FIELD_H

#include "PixelpartForceField.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include <pixelpart-runtime/effect/AttractionField.h>

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

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	/**
	 * @brief How the strength of the force field decreases based on the distance to the center.
	 *
	 * @return Falloff power property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_falloff_power() const;

protected:
	static void _bind_methods();

private:
	pixelpart::AttractionField* attractionField = nullptr;
};
}

#endif
