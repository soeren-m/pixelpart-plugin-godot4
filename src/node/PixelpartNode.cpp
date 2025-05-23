#include "PixelpartNode.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartNode::PixelpartNode() : RefCounted() {

}
PixelpartNode::~PixelpartNode() {

}

void PixelpartNode::init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) {
	node = internalNode;
	effectEngine = effectEnginePtr;
}

int PixelpartNode::get_id() const {
	return node ? static_cast<int>(node->id().value()) : -1;
}
int PixelpartNode::get_parent_id() const {
	return node && node->parentId().valid() ? static_cast<int>(node->parentId().value()) : -1;
}
String PixelpartNode::get_name() const {
	return node ? String(node->name().c_str()) : String();
}

void PixelpartNode::set_lifetime_start(float time) {
	if(!node) {
		return;
	}

	node->start(time);
}
void PixelpartNode::set_lifetime_duration(float time) {
	if(!node) {
		return;
	}

	node->duration(time);
}
void PixelpartNode::set_repeat(bool value) {
	if(!node) {
		return;
	}

	node->repeat(value);
}
float PixelpartNode::get_lifetime_start() const {
	if(!node) {
		return 0.0f;
	}

	return static_cast<float>(node->start());
}
float PixelpartNode::get_lifetime_duration() const {
	if(!node) {
		return 0.0f;
	}

	return static_cast<float>(node->duration());
}
bool PixelpartNode::get_repeat() const {
	if(!node) {
		return false;
	}

	return node->repeat();
}
bool PixelpartNode::is_active() const {
	if(!node || !effectEngine) {
		return false;
	}

	return node->active(effectEngine->runtimeContext());
}
float PixelpartNode::get_local_time() const {
	if(!node || !effectEngine) {
		return 0.0f;
	}

	return static_cast<float>(node->life(effectEngine->runtimeContext()));
}

Ref<PixelpartAnimatedPropertyFloat3> PixelpartNode::get_position() const {
	if(!node) {
		return Ref<PixelpartAnimatedPropertyFloat3>();
	}

	Ref<PixelpartAnimatedPropertyFloat3> property;
	property.instantiate();
	property->init(&node->position());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat3> PixelpartNode::get_rotation() const {
	if(!node) {
		return Ref<PixelpartAnimatedPropertyFloat3>();
	}

	Ref<PixelpartAnimatedPropertyFloat3> property;
	property.instantiate();
	property->init(&node->rotation());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat3> PixelpartNode::get_scale() const {
	if(!node) {
		return Ref<PixelpartAnimatedPropertyFloat3>();
	}

	Ref<PixelpartAnimatedPropertyFloat3> property;
	property.instantiate();
	property->init(&node->scale());

	return property;
}

void PixelpartNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_id"), &PixelpartNode::get_id);
	ClassDB::bind_method(D_METHOD("get_parent_id"), &PixelpartNode::get_parent_id);
	ClassDB::bind_method(D_METHOD("get_name"), &PixelpartNode::get_name);
	ClassDB::bind_method(D_METHOD("set_lifetime_start", "time"), &PixelpartNode::set_lifetime_start);
	ClassDB::bind_method(D_METHOD("set_lifetime_duration", "time"), &PixelpartNode::set_lifetime_duration);
	ClassDB::bind_method(D_METHOD("set_repeat", "value"), &PixelpartNode::set_repeat);
	ClassDB::bind_method(D_METHOD("get_lifetime_start"), &PixelpartNode::get_lifetime_start);
	ClassDB::bind_method(D_METHOD("get_lifetime_duration"), &PixelpartNode::get_lifetime_duration);
	ClassDB::bind_method(D_METHOD("get_repeat"), &PixelpartNode::get_repeat);
	ClassDB::bind_method(D_METHOD("is_active"), &PixelpartNode::is_active);
	ClassDB::bind_method(D_METHOD("get_local_time"), &PixelpartNode::get_local_time);
	ClassDB::bind_method(D_METHOD("get_position"), &PixelpartNode::get_position);
    ClassDB::bind_method(D_METHOD("get_rotation"), &PixelpartNode::get_rotation);
	ClassDB::bind_method(D_METHOD("get_scale"), &PixelpartNode::get_scale);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_start"), "set_lifetime_start", "get_lifetime_start");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_duration"), "set_lifetime_duration", "get_lifetime_duration");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "repeat"), "set_repeat", "get_repeat");
}
}