#ifndef PIXELPART_GPU_EFFECT_ENGINE_H
#define PIXELPART_GPU_EFFECT_ENGINE_H

#include <pixelpart-runtime/engine/EffectEngine.h>

namespace godot {
class PixelpartGpuEffectEngine : public pixelpart::EffectEngine {
public:
	PixelpartGpuEffectEngine();
	virtual ~PixelpartGpuEffectEngine();

	virtual void advance(pixelpart::float_t dt) override;
	virtual void restart() override;
	virtual void reset(const pixelpart::EffectRuntimeState& initialState, pixelpart::EffectRuntimeContext initialContext) override;
	virtual void reseed(std::uint32_t seed) override;

	virtual void generateParticles(std::uint32_t count, pixelpart::id_t particleEmitterId, pixelpart::id_t particleTypeId, pixelpart::EffectRuntimeContext runtimeContext) override;
	virtual void clearParticles() override;

	virtual void activateTrigger(pixelpart::id_t triggerId) override;

	virtual void selectLod(std::uint32_t lod) override;

	virtual const pixelpart::Effect& effect() const override;
	virtual const pixelpart::EffectRuntimeContext& context() const override;

private:
	
};
}

#endif
