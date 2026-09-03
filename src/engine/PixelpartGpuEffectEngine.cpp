#include "PixelpartGpuEffectEngine.h"
#include <godot_cpp/classes/rendering_server.hpp>

// TODO

namespace godot {
PixelpartGpuEffectEngine::PixelpartGpuEffectEngine(const pixelpart::Effect& eff) : effect(eff) {
	RenderingServer* rs = RenderingServer::get_singleton();
	renderingDevice = rs->get_rendering_device();

	// TODO: load shaders

	generationComputePipelineRid = renderingDevice.compute_pipeline_create(generationComputeShader);
	simulationComputePipelineRid = renderingDevice.compute_pipeline_create(simulationComputeShader);
}
PixelpartGpuEffectEngine::~PixelpartGpuEffectEngine() {
	for(const auto& [emissionPair, textures] : particleGpuTextures) {
		renderingDevice.free_rid(textures.commonTextureRid);
		renderingDevice.free_rid(textures.positionTextureRid);
		renderingDevice.free_rid(textures.globalPositionTextureRid);
		renderingDevice.free_rid(textures.velocityTextureRid);
		renderingDevice.free_rid(textures.forceTextureRid);
		renderingDevice.free_rid(textures.rotationTextureRid);
		renderingDevice.free_rid(textures.sizeTextureRid);
		renderingDevice.free_rid(textures.colorTextureRid);
	}

	renderingDevice.free_rid(generationComputePipelineRid);
	renderingDevice.free_rid(simulationComputePipelineRid);
}

void PixelpartGpuEffectEngine::advance(pixelpart::float_t dt) {
	RenderingServer* rs = RenderingServer::get_singleton();

	dt = std::max(dt, 0.0);
	runtimeContext.deltaTime() = dt;

	// TODO: remove buffers??
	for(pixelpart::ParticleEmissionPair emissionPair : effect.particleEmissionPairs()) {
		if(particleGpuTextures.count(emissionPair) == 0) {
			// TODO: calculate size based on particle count
			particleGpuTextures[emissionPair] = createParticleDataTextures(512, 512);
			particleEmissionCounts[emissionPair] = 0.0;
		}
	}

	// TODO

	for(const auto& [emissionPair, textures] : particleGpuTextures) {
		const pixelpart::ParticleType& particleType = engineEffect.particleTypes().at(emissionPair.typeId);
		const pixelpart::ParticleEmitter& particleEmitter = engineEffect.sceneGraph().at<pixelpart::ParticleEmitter>(emissionPair.emitterId);
		if(!particleEmitter.primary() || !particleEmitter.active(runtimeContext)) {
			continue;
		}

		pixelpart::float_t& emissionCount = engineParticleEmissionCounts.at(emissionPair);

		float_t startTime = particleEmitter.startTrigger()
			? particleEmitter.start() + runtimeContext.triggerActivationTime(particleEmitter.startTrigger())
			: particleEmitter.start();

		float_t emissionTime = particleEmitter.repeat()
			? std::fmod(runtimeContext.time() - startTime, particleEmitter.duration())
			: runtimeContext.time() - startTime;

		switch(particleEmitter.emissionMode()) {
			case pixelpart::ParticleEmitter::EmissionMode::continuous:
				emissionCount += particleType.count().at(emissionTime / particleEmitter.duration()) * runtimeContext.deltaTime();
				break;
			case pixelpart::ParticleEmitter::EmissionMode::burst_start:
				if(emissionTime < runtimeContext.deltaTime()) {
					emissionCount += particleType.count().at(0);
				}
				break;
			case pixelpart::ParticleEmitter::EmissionMode::burst_end:
				if(emissionTime > particleEmitter.duration() - runtimeContext.deltaTime()) {
					emissionCount += particleType.count().at(1);
				}
				break;
			default:
				break;
		}

		std::uint32_t emittedParticleCount = static_cast<std::uint32_t>(std::max(emissionCount, 0.0));
		if(emittedParticleCount == 0) {
			continue;
		}

		emissionCount -= static_cast<pixelpart::float_t>(emittedParticleCount);

		// TODO: set params

		std::uint32_t localWorkgroupSize = 64;
		std::uint32_t workgroupCount = (emittedParticleCount + (localWorkgroupSize - 1)) / localWorkgroupSize;

		auto computeList = renderingDevice.compute_list_begin();
		renderingDevice.compute_list_bind_compute_pipeline(computeList, generationComputePipeline);
		renderingDevice.compute_list_dispatch(computeList, workgroupCount, 1, 1);
		renderingDevice.compute_list_end();
	}

	// TODO: barrier

	for(auto& [emissionPair, textures] : particleGpuTextures) {
		const pixelpart::ParticleType& particleType = engineEffect.particleTypes().at(emissionPair.typeId);
		const pixelpart::ParticleEmitter& particleEmitter = engineEffect.sceneGraph().at<pixelpart::ParticleEmitter>(emissionPair.emitterId);

		// TODO: set params

		std::uint32_t localWorkgroupSize = 64;
		std::uint32_t workgroupCount = (512*512) / localWorkgroupSize;

		auto computeList = renderingDevice.compute_list_begin();
		renderingDevice.compute_list_bind_compute_pipeline(computeList, simulationComputePipeline);
		renderingDevice.compute_list_dispatch(computeList, workgroupCount, 1, 1);
		renderingDevice.compute_list_end();
	}

	runtimeContext.invokedEvents().clear();
	for(const auto& [eventId, event] : effect.events()) {
		if(event.nodeId()) {
			const auto& node = effect.sceneGraph().at(event.nodeId());

			if(node.activatedByTrigger(runtimeContext) &&
				runtimeContext.time() >= node.start() + event.time() &&
				runtimeContext.time() < node.start() + event.time() + dt) {
				runtimeContext.invokedEvents().push_back(eventId);
			}
		}
		else {
			if(runtimeContext.time() >= event.time() &&
				runtimeContext.time() < event.time() + dt) {
				runtimeContext.invokedEvents().push_back(eventId);
			}
		}
	}

	runtimeContext.time() += dt;
}
void PixelpartGpuEffectEngine::restart() {
	runtimeContext.time() = 0.0;
	runtimeContext.deltaTime() = 0.0;
	runtimeContext.triggerActivationTimes().clear();
}
void PixelpartGpuEffectEngine::reset(const pixelpart::EffectRuntimeState& initialState, pixelpart::EffectRuntimeContext initialContext) {
	runtimeContext = initialContext;

	particleGpuTextures.clear();
	for(const auto& [emissionPair, particleCollection] : initialState.particleCollections()) {
		particleGpuTextures[emissionPair] = createParticleDataTextures(particleCollection);
	}
}
void PixelpartGpuEffectEngine::reseed(std::uint32_t seed) {
	
}

void PixelpartGpuEffectEngine::generateParticles(std::uint32_t count, pixelpart::id_t particleEmitterId, pixelpart::id_t particleTypeId, pixelpart::EffectRuntimeContext runtimeContext) {
	// TODO
}
void PixelpartGpuEffectEngine::clearParticles() {
	// TODO
}

void PixelpartGpuEffectEngine::activateTrigger(pixelpart::id_t triggerId) {
	if(engineEffect.triggers().count(triggerId) == 0 ||
		runtimeContext.triggerActivationTimes().count(triggerId) != 0) {
		return;
	}

	runtimeContext.triggerActivationTimes()[triggerId] = runtimeContext.time() + runtimeContext.deltaTime() * 0.5;
}

void PixelpartGpuEffectEngine::selectLod(std::uint32_t lod) {
	runtimeContext.lod() = lod;
}

const pixelpart::Effect& PixelpartGpuEffectEngine::effect() const {
	return effect;
}
const pixelpart::EffectRuntimeContext& PixelpartGpuEffectEngine::context() const {
	return runtimeContext;
}

ParticleGpuTexture PixelpartGpuEffectEngine::createParticleDataTextures(std::uint32_t width, std::uint32_t height) {
	ParticleGpuTexture textures;

	Ref<RDTextureFormat> textureFormat;
	textureFormat.instantiate();
	textureFormat.format = RenderingDevice::DATA_FORMAT_R32G32B32A32_SFLOAT;
	textureFormat.width = width;
	textureFormat.height = height;
	textureFormat.depth = 1;
	textureFormat.usage_bits =
		RenderingDevice::TEXTURE_USAGE_SAMPLING_BIT
		| RenderingDevice::TEXTURE_USAGE_STORAGE_BIT
		| RenderingDevice::TEXTURE_USAGE_CAN_UPDATE_BIT
		| RenderingDevice::TEXTURE_USAGE_CAN_COPY_FROM_BIT
		| RenderingDevice::TEXTURE_USAGE_CAN_COPY_TO_BIT;
	textureFormat.mipmaps = 1;
	textureFormat.samples = RenderingDevice::TEXTURE_SAMPLES_1;

	textures.commonTextureRid = renderingDevice.texture_create(textureFormat, RDTextureView.new());
	textures.positionTextureRid = renderingDevice.texture_create(textureFormat, RDTextureView.new());
	textures.globalPositionTextureRid = renderingDevice.texture_create(textureFormat, RDTextureView.new());
	textures.velocityTextureRid = renderingDevice.texture_create(textureFormat, RDTextureView.new());
	textures.forceTextureRid = renderingDevice.texture_create(textureFormat, RDTextureView.new());
	textures.rotationTextureRid = renderingDevice.texture_create(textureFormat, RDTextureView.new());
	textures.sizeTextureRid = renderingDevice.texture_create(textureFormat, RDTextureView.new());
	textures.colorTextureRid = renderingDevice.texture_create(textureFormat, RDTextureView.new());
	
	//textures.aliveListBufferRids[0] = renderingDevice.storage_buffer_create()

	renderingDevice.texture_clear(textures.commonTextureRid, Color.TRANSPARENT, 0, 1, 0, 1);
	renderingDevice.texture_clear(textures.positionTextureRid, Color.TRANSPARENT, 0, 1, 0, 1);
	renderingDevice.texture_clear(textures.globalPositionTextureRid, Color.TRANSPARENT, 0, 1, 0, 1);
	renderingDevice.texture_clear(textures.velocityTextureRid, Color.TRANSPARENT, 0, 1, 0, 1);
	renderingDevice.texture_clear(textures.forceTextureRid, Color.TRANSPARENT, 0, 1, 0, 1);
	renderingDevice.texture_clear(textures.rotationTextureRid, Color.TRANSPARENT, 0, 1, 0, 1);
	renderingDevice.texture_clear(textures.sizeTextureRid, Color.TRANSPARENT, 0, 1, 0, 1);
	renderingDevice.texture_clear(textures.colorTextureRid, Color.TRANSPARENT, 0, 1, 0, 1);

	return textures;
}
}
