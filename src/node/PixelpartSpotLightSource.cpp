#include "PixelpartSpotLightSource.h"

namespace godot {
PixelpartSpotLightSource::PixelpartSpotLightSource() : PixelpartLightSource() {

}
PixelpartSpotLightSource::~PixelpartSpotLightSource() {

}

void PixelpartSpotLightSource::init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) {
	PixelpartLightSource::init(internalNode, effectEnginePtr);

	spotLightSource = dynamic_cast<pixelpart::SpotLightSource*>(internalNode);
}

Ref<PixelpartAnimatedPropertyFloat> PixelpartSpotLightSource::get_spot_angle() const {
	if(!spotLightSource) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&spotLightSource->spotAngle());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartSpotLightSource::get_spot_angle_attenuation() const {
	if(!spotLightSource) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&spotLightSource->spotAngleAttenuation());

	return property;
}

void PixelpartSpotLightSource::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_spot_angle"), &PixelpartSpotLightSource::get_spot_angle);
	ClassDB::bind_method(D_METHOD("get_spot_angle_attenuation"), &PixelpartSpotLightSource::get_spot_angle_attenuation);
}
}