#ifndef PIXELPART_GPU_EFFECT_ENGINE_H
#define PIXELPART_GPU_EFFECT_ENGINE_H

#include <pixelpart-runtime/engine/EffectEngine.h>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/texture2drd.hpp>
#include <godot_cpp/classes/rd_uniform.hpp>
#include <cstdint>
#include <unordered_map>

namespace godot {
class PixelpartGpuEffectEngine : public pixelpart::EffectEngine {
public:
	struct ParticleGpuTextureSet {
		RID commonTextureRid;
		RID globalPositionTextureRid;
		RID colorTextureRid;
		RID sizeTextureRid;
	};

	PixelpartGpuEffectEngine(const pixelpart::Effect& effect);
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

	ParticleGpuTextureSet gpuTextures(pixelpart::id_t particleEmitterId, pixelpart::id_t particleTypeId) const;

private:
	struct ParticleGpuResourceSet {
		RID commonTextureRid;
		RID positionTextureRid;
		RID globalPositionTextureRid;
		RID velocityTextureRid;
		RID forceTextureRid;
		RID rotationTextureRid;
		RID sizeTextureRid;
		RID colorTextureRid;

		RID generationShaderImageUniformSetRid;
		RID simulationShaderImageUniformSetRid;

		Ref<Texture2DRD> commonTextureWrapper;
		Ref<Texture2DRD> globalPositionTextureWrapper;
		Ref<Texture2DRD> colorTextureWrapper;
		Ref<Texture2DRD> sizeTextureWrapper;

		/*RID aliveListBufferRids[2];
		RID deadListBufferRid;
		RID atomicCounterBufferRid;
		std::uint32_t activeAliveListBufferIndex = 0;*/
	};

	ParticleGpuResourceSet createParticleGpuResources(std::uint32_t width, std::uint32_t height);

	static Ref<Texture2DRD> createTexture2DWrapper(RID textureRid);
	static Ref<RDUniform> createUniformBufferUniform(std::int32_t binding, RID id);
	static Ref<RDUniform> createImageUniform(std::int32_t binding, RID id);

	const pixelpart::Effect& engineEffect;

	RenderingDevice* renderingDevice;
	RID generationComputeShaderRid;
	RID simulationComputeShaderRid;
	RID generationComputePipelineRid;
	RID simulationComputePipelineRid;

	RID generationParamsUniformBufferRid;
	RID generationParamsUniformSetRid;
	RID simulationParamsUniformBufferRid;
	RID simulationParamsUniformSetRid;

	std::unordered_map<pixelpart::ParticleEmissionPair, ParticleGpuResourceSet> particleGpuResources;
	std::unordered_map<pixelpart::ParticleEmissionPair, pixelpart::float_t> particleEmissionCounts;

	pixelpart::EffectRuntimeContext runtimeContext;
};
}

#endif
