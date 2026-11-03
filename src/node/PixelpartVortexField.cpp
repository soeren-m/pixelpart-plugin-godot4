#include "PixelpartVortexField.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartVortexField::PixelpartVortexField() : PixelpartForceField() {

}
PixelpartVortexField::~PixelpartVortexField() {

}

void PixelpartVortexField::init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) {
	PixelpartForceField::init(internalNode, effectEnginePtr);

	vortexField = dynamic_cast<pixelpart::VortexField*>(internalNode);
}

Ref<PixelpartAnimatedPropertyFloat> PixelpartVortexField::get_tangential_strength() const {
	if(!vortexField) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&vortexField->tangentialStrength());

	return property;
}

Ref<PixelpartAnimatedPropertyFloat> PixelpartVortexField::get_radial_strength() const {
	if(!vortexField) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&vortexField->radialStrength());

	return property;
}

void PixelpartVortexField::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_tangential_strength"), &PixelpartVortexField::get_tangential_strength);
	ClassDB::bind_method(D_METHOD("get_radial_strength"), &PixelpartVortexField::get_radial_strength);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "tangential_strength", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_tangential_strength");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "radial_strength", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_radial_strength");
}
}
