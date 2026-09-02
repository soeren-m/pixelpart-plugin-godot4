#include "PixelpartGpuEffectEngine.h"
#include <godot_cpp/classes/rendering_server.hpp>

// TODO

namespace godot {
PixelpartGpuEffectEngine::PixelpartGpuEffectEngine(const pixelpart::Effect& eff) : effect(eff) {
	RenderingServer* rs = RenderingServer::get_singleton();
	renderingDevice = rs->get_rendering_device();

	// TODO: load shaders
}
PixelpartGpuEffectEngine::~PixelpartGpuEffectEngine() {
	
}

void PixelpartGpuEffectEngine::advance(pixelpart::float_t dt) {
	RenderingServer* rs = RenderingServer::get_singleton();

	dt = std::max(dt, 0.0);
	runtimeContext.deltaTime() = dt;

	// TODO

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
	// TODO

	runtimeContext = initialContext;
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

	textures.positionTextureRid = renderingDevice.texture_create(textureFormat, RDTextureView.new());
	textures.velocityTextureRid = renderingDevice.texture_create(textureFormat, RDTextureView.new());
	textures.forceTextureRid = renderingDevice.texture_create(textureFormat, RDTextureView.new());
	textures.colorTextureRid = renderingDevice.texture_create(textureFormat, RDTextureView.new());
	textures.sizeTextureRid = renderingDevice.texture_create(textureFormat, RDTextureView.new());

	renderingDevice.texture_clear(textures.positionTextureRid, Color.TRANSPARENT, 0, 1, 0, 1);
	renderingDevice.texture_clear(textures.velocityTextureRid, Color.TRANSPARENT, 0, 1, 0, 1);
	renderingDevice.texture_clear(textures.forceTextureRid, Color.TRANSPARENT, 0, 1, 0, 1);
	renderingDevice.texture_clear(textures.colorTextureRid, Color.TRANSPARENT, 0, 1, 0, 1);
	renderingDevice.texture_clear(textures.sizeTextureRid, Color.TRANSPARENT, 0, 1, 0, 1);

	return textures;
}
}
