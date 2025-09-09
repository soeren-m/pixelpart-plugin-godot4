#include "PixelpartCollider.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartCollider::PixelpartCollider() : PixelpartNode() {

}
PixelpartCollider::~PixelpartCollider() {

}

void PixelpartCollider::init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) {
	PixelpartNode::init(internalNode, effectEnginePtr);

	collider = dynamic_cast<pixelpart::Collider*>(internalNode);
}

Ref<PixelpartStaticPropertyBool> PixelpartCollider::get_kill_on_contact() const {
	if(!collider) {
		return Ref<PixelpartStaticPropertyBool>();
	}

	Ref<PixelpartStaticPropertyBool> property;
	property.instantiate();
	property->init(&collider->killOnContact());

	return property;
}

Ref<PixelpartAnimatedPropertyFloat> PixelpartCollider::get_bounce() const {
	if(!collider) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&collider->bounce());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartCollider::get_friction() const {
	if(!collider) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&collider->friction());

	return property;
}

void PixelpartCollider::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_kill_on_contact"), &PixelpartCollider::get_kill_on_contact);
	ClassDB::bind_method(D_METHOD("get_bounce"), &PixelpartCollider::get_bounce);
	ClassDB::bind_method(D_METHOD("get_friction"), &PixelpartCollider::get_friction);
}
}
