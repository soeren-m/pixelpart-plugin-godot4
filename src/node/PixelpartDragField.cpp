#include "PixelpartDragField.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartDragField::PixelpartDragField() : PixelpartForceField() {

}
PixelpartDragField::~PixelpartDragField() {

}

void PixelpartDragField::init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) {
	PixelpartForceField::init(internalNode, effectEnginePtr);

	dragField = dynamic_cast<pixelpart::DragField*>(internalNode);
}

Ref<PixelpartStaticPropertyFloat> PixelpartDragField::get_velocity_influence() const {
	if(!dragField) {
		return Ref<PixelpartStaticPropertyFloat>();
	}

	Ref<PixelpartStaticPropertyFloat> property;
	property.instantiate();
	property->init(&dragField->velocityInfluence());

	return property;
}
Ref<PixelpartStaticPropertyFloat> PixelpartDragField::get_size_influence() const {
	if(!dragField) {
		return Ref<PixelpartStaticPropertyFloat>();
	}

	Ref<PixelpartStaticPropertyFloat> property;
	property.instantiate();
	property->init(&dragField->sizeInfluence());

	return property;
}

void PixelpartDragField::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_velocity_influence"), &PixelpartDragField::get_velocity_influence);
	ClassDB::bind_method(D_METHOD("get_size_influence"), &PixelpartDragField::get_size_influence);

	// Deprecated
	ClassDB::bind_method(D_METHOD("get_drag_velocity_influence"), &PixelpartDragField::get_velocity_influence);
	ClassDB::bind_method(D_METHOD("get_drag_size_influence"), &PixelpartDragField::get_size_influence);
}
}