#include "PixelpartAttractionField.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartAttractionField::PixelpartAttractionField() : PixelpartForceField() {

}
PixelpartAttractionField::~PixelpartAttractionField() {

}

void PixelpartAttractionField::init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) {
	PixelpartForceField::init(internalNode, effectEnginePtr);

	attractionField = dynamic_cast<pixelpart::AttractionField*>(internalNode);
}

Ref<PixelpartAnimatedPropertyFloat> PixelpartAttractionField::get_falloff_power() const {
	if(!attractionField) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&attractionField->falloffPower());

	return property;
}

void PixelpartAttractionField::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_falloff_power"), &PixelpartAttractionField::get_falloff_power);
}
}
