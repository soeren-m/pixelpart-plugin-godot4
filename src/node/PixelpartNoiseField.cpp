#include "PixelpartNoiseField.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {
PixelpartNoiseField::PixelpartNoiseField() : PixelpartForceField() {

}
PixelpartNoiseField::~PixelpartNoiseField() {

}

void PixelpartNoiseField::init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) {
	PixelpartForceField::init(internalNode, effectEnginePtr);

	noiseField = dynamic_cast<pixelpart::NoiseField*>(internalNode);
}

Ref<PixelpartStaticPropertyInt> PixelpartNoiseField::get_noise_octaves() const {
	if(!noiseField) {
		return Ref<PixelpartStaticPropertyInt>();
	}

	Ref<PixelpartStaticPropertyInt> property;
	property.instantiate();
	property->init(&noiseField->noiseOctaves());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartNoiseField::get_noise_frequency() const {
	if(!noiseField) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&noiseField->noiseFrequency());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartNoiseField::get_noise_persistence() const {
	if(!noiseField) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&noiseField->noisePersistence());

	return property;
}
Ref<PixelpartAnimatedPropertyFloat> PixelpartNoiseField::get_noise_lacunarity() const {
	if(!noiseField) {
		return Ref<PixelpartAnimatedPropertyFloat>();
	}

	Ref<PixelpartAnimatedPropertyFloat> property;
	property.instantiate();
	property->init(&noiseField->noiseLacunarity());

	return property;
}

void PixelpartNoiseField::set_noise_animated(bool animated) {
	if(!noiseField) {
		return;
	}

	noiseField->noiseAnimated(animated);
}
bool PixelpartNoiseField::is_noise_animated() const {
	return noiseField ? noiseField->noiseAnimated() : false;
}
Ref<PixelpartStaticPropertyFloat> PixelpartNoiseField::get_noise_animation_time_scale() const {
	if(!noiseField) {
		return Ref<PixelpartStaticPropertyFloat>();
	}

	Ref<PixelpartStaticPropertyFloat> property;
	property.instantiate();
	property->init(&noiseField->noiseAnimationTimeScale());

	return property;
}
Ref<PixelpartStaticPropertyFloat> PixelpartNoiseField::get_noise_animation_time_base() const {
	if(!noiseField) {
		return Ref<PixelpartStaticPropertyFloat>();
	}

	Ref<PixelpartStaticPropertyFloat> property;
	property.instantiate();
	property->init(&noiseField->noiseAnimationTimeBase());

	return property;
}

void PixelpartNoiseField::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_noise_octaves"), &PixelpartNoiseField::get_noise_octaves);
	ClassDB::bind_method(D_METHOD("get_noise_frequency"), &PixelpartNoiseField::get_noise_frequency);
	ClassDB::bind_method(D_METHOD("get_noise_persistence"), &PixelpartNoiseField::get_noise_persistence);
	ClassDB::bind_method(D_METHOD("get_noise_lacunarity"), &PixelpartNoiseField::get_noise_lacunarity);
	ClassDB::bind_method(D_METHOD("set_noise_animated", "animated"), &PixelpartNoiseField::set_noise_animated);
	ClassDB::bind_method(D_METHOD("is_noise_animated"), &PixelpartNoiseField::is_noise_animated);
	ClassDB::bind_method(D_METHOD("get_noise_animation_time_scale"), &PixelpartNoiseField::get_noise_animation_time_scale);
	ClassDB::bind_method(D_METHOD("get_noise_animation_time_base"), &PixelpartNoiseField::get_noise_animation_time_base);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "noise_animated"), "set_noise_animated", "is_noise_animated");

	// Deprecated
	ClassDB::bind_method(D_METHOD("get_noise_animated"), &PixelpartNoiseField::is_noise_animated);
}
}