#ifndef PIXELPART_NOISE_FIELD_H
#define PIXELPART_NOISE_FIELD_H

#include "PixelpartForceField.h"
#include "../property/PixelpartStaticPropertyInt.h"
#include "../property/PixelpartStaticPropertyFloat.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include <pixelpart-runtime/effect/NoiseField.h>

namespace godot {
/**
 * @brief Force field that generates random turbulent motion.
 *
 * Noise fields are perfect for creating organic and unpredictable patterns.
 */
class PixelpartNoiseField : public PixelpartForceField {
	GDCLASS(PixelpartNoiseField, PixelpartForceField)

public:
	PixelpartNoiseField();
	virtual ~PixelpartNoiseField();

#ifdef DOXYGEN
	/**
	 * @brief Whether the noise field is animated.
	 */
	bool noise_animated;
#endif

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	/**
	 * @brief How many layers of noise are blended together to produce the final velocity vector.
	 *
	 * @return Noise octaves property
	 */
	Ref<PixelpartStaticPropertyInt> get_noise_octaves() const;

	/**
	 * @brief Base frequency of the noise generation.
	 *
	 * Higher values produce more turbulent noise fields.
	 *
	 * @return Noise frequency property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_noise_frequency() const;

	/**
	 * @brief How much the amplitude changes after each noise octave.
	 *
	 * @return Noise persistence property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_noise_persistence() const;

	/**
	 * @brief How much the frequency changes after each noise octave.
	 *
	 * @return Noise lacunarity property
	 */
	Ref<PixelpartAnimatedPropertyFloat> get_noise_lacunarity() const;

	/**
	 * @brief Set whether the noise field is animated.
	 *
	 * @param animated Whether the noise field is animated
	 */
	void set_noise_animated(bool animated);

	/**
	 * @brief Whether the noise field is animated.
	 *
	 * @return Whether the noise field is animated
	 */
	bool is_noise_animated() const;

	/**
	 * @brief How fast the noise field changes.
	 *
	 * @return Noise animation time scale property
	 */
	Ref<PixelpartStaticPropertyFloat> get_noise_animation_time_scale() const;

	/**
	 * @brief Time offset of the noise animation.
	 *
	 * @return Noise animation time base property
	 */
	Ref<PixelpartStaticPropertyFloat> get_noise_animation_time_base() const;

protected:
	static void _bind_methods();

private:
	pixelpart::NoiseField* noiseField = nullptr;
};
}

#endif