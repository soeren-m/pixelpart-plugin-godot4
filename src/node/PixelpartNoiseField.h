#ifndef PIXELPART_NOISE_FIELD_H
#define PIXELPART_NOISE_FIELD_H

#include "PixelpartForceField.h"
#include "../property/PixelpartStaticPropertyInt.h"
#include "../property/PixelpartStaticPropertyFloat.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include <pixelpart-runtime/effect/NoiseField.h>

namespace godot {
class PixelpartNoiseField : public PixelpartForceField {
	GDCLASS(PixelpartNoiseField, PixelpartForceField)

public:
	PixelpartNoiseField();
	virtual ~PixelpartNoiseField();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	Ref<PixelpartStaticPropertyInt> get_noise_octaves() const;
	Ref<PixelpartAnimatedPropertyFloat> get_noise_frequency() const;
	Ref<PixelpartAnimatedPropertyFloat> get_noise_persistence() const;
	Ref<PixelpartAnimatedPropertyFloat> get_noise_lacunarity() const;

	void set_noise_animated(bool animated);
	bool is_noise_animated() const;
	Ref<PixelpartStaticPropertyFloat> get_noise_animation_time_scale() const;
	Ref<PixelpartStaticPropertyFloat> get_noise_animation_time_base() const;

protected:
	static void _bind_methods();

private:
	pixelpart::NoiseField* noiseField = nullptr;
};
}

#endif