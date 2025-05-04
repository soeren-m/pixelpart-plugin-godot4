#include "PixelpartVectorField.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartVectorField::PixelpartVectorField() : PixelpartForceField() {

}
PixelpartVectorField::~PixelpartVectorField() {

}

void PixelpartVectorField::init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) {
	PixelpartForceField::init(internalNode, effectEnginePtr);

	vectorField = dynamic_cast<pixelpart::VectorField*>(internalNode);
}

void PixelpartVectorField::set_vector_filter(int filter) {
	if(!vectorField) {
		return;
	}

	vectorField->vectorFieldFilter(static_cast<pixelpart::VectorField::Filter>(filter));
}
int PixelpartVectorField::get_vector_filter() const {
	return vectorField
		? static_cast<int>(vectorField->vectorFieldFilter())
		: static_cast<int>(pixelpart::VectorField::Filter::none);
}

Ref<PixelpartAnimatedPropertyFloat> PixelpartVectorField::get_tightness() const {
	if(!vectorField) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&vectorField->tightness());

	return property;
}

void PixelpartVectorField::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_vector_filter", "filter"), &PixelpartVectorField::set_vector_filter);
	ClassDB::bind_method(D_METHOD("get_vector_filter"), &PixelpartVectorField::get_vector_filter);
	ClassDB::bind_method(D_METHOD("get_tightness"), &PixelpartVectorField::get_tightness);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "vector_filter"), "set_vector_filter", "get_vector_filter");

	// Deprecated
	ClassDB::bind_method(D_METHOD("get_vector_tightness"), &PixelpartVectorField::get_tightness);
}
}