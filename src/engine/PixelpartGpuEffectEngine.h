#ifndef PIXELPART_GPU_EFFECT_ENGINE_H
#define PIXELPART_GPU_EFFECT_ENGINE_H

#include <pixelpart-runtime/engine/EffectEngine.h>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <unordered_map>

namespace godot {
class PixelpartGpuEffectEngine : public pixelpart::EffectEngine {
public:
	PixelpartGpuEffectEngine(const pixelpart::Effect& eff);
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
	struct ParticleGpuTexture {
		RID commonTextureRid;
		RID positionTextureRid;
		RID globalPositionTextureRid;
		RID velocityTextureRid;
		RID forceTextureRid;
		RID rotationTextureRid;
		RID sizeTextureRid;
		RID colorTextureRid;
		
		/*RID aliveListBufferRids[2];
		RID deadListBufferRid;
		RID atomicCounterBufferRid;
		std::uint32_t activeAliveListBufferIndex = 0;*/
	};

	ParticleGpuTexture createParticleDataTextures(std::uint32_t width, std::uint32_t height);

	const pixelpart::Effect& effect;

	Ref<RenderingDevice> renderingDevice;
	Ref<Shader> generationComputeShader;
	Ref<Shader> simulationComputeShader;
	RID generationComputePipelineRid;
	RID simulationComputePipelineRid;

	std::unordered_map<pixelpart::ParticleEmissionPair, ParticleGpuTexture> particleGpuTextures;
	std::unordered_map<pixelpart::ParticleEmissionPair, pixelpart::float_t> particleEmissionCounts;

	pixelpart::EffectRuntimeContext runtimeContext;
};
}

#endif
