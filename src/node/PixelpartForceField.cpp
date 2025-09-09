#include "PixelpartForceField.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartForceField::PixelpartForceField() : PixelpartNode() {

}
PixelpartForceField::~PixelpartForceField() {

}

void PixelpartForceField::init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) {
	PixelpartNode::init(internalNode, effectEnginePtr);

	forceField = dynamic_cast<pixelpart::ForceField*>(internalNode);
}

void PixelpartForceField::set_infinite(bool mode) {
	if(!forceField) {
		return;
	}

	forceField->infinite(mode);
}
bool PixelpartForceField::is_infinite() const {
	return forceField ? forceField->infinite() : false;
}

Ref<PixelpartAnimatedPropertyFloat> PixelpartForceField::get_strength() const {
	if(!forceField) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&forceField->strength());

	return property;
}

void PixelpartForceField::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_infinite", "mode"), &PixelpartForceField::set_infinite);
	ClassDB::bind_method(D_METHOD("is_infinite"), &PixelpartForceField::is_infinite);
	ClassDB::bind_method(D_METHOD("get_strength"), &PixelpartForceField::get_strength);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "infinite"), "set_infinite", "is_infinite");
}
}
