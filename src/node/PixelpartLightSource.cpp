#include "PixelpartLightSource.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartLightSource::PixelpartLightSource() : PixelpartNode() {

}
PixelpartLightSource::~PixelpartLightSource() {

}

void PixelpartLightSource::init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) {
	PixelpartNode::init(internalNode, effectEnginePtr);

	lightSource = dynamic_cast<pixelpart::LightSource*>(internalNode);
}

Ref<PixelpartAnimatedPropertyFloat> PixelpartLightSource::get_attenuation() const {
	if(!lightSource) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&lightSource->attenuation());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat4> PixelpartLightSource::get_color() const {
	if(!lightSource) {
		return Ref<PixelpartAnimatedPropertyFloat4>();
	}

	Ref<PixelpartAnimatedPropertyFloat4> property;
	property.instantiate();
	property->init(&lightSource->color());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartLightSource::get_intensity() const {
	if(!lightSource) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&lightSource->intensity());

	return property;
}

void PixelpartLightSource::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_attenuation"), &PixelpartLightSource::get_attenuation);
	ClassDB::bind_method(D_METHOD("get_color"), &PixelpartLightSource::get_color);
	ClassDB::bind_method(D_METHOD("get_intensity"), &PixelpartLightSource::get_intensity);
}
}
