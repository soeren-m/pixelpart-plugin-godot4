#include "PixelpartGpuEffectEngine.h"
#include <stdexcept>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rd_shader_file.hpp>
#include <godot_cpp/classes/rd_shader_spirv.hpp>
#include <godot_cpp/classes/rd_uniform.hpp>
#include <godot_cpp/classes/rd_texture_format.hpp>
#include <godot_cpp/classes/rd_texture_view.hpp>
// TODO

namespace godot {
PixelpartGpuEffectEngine::PixelpartGpuEffectEngine(const pixelpart::Effect& effect) : engineEffect(effect) {
	RenderingServer* rs = RenderingServer::get_singleton();
	renderingDevice = rs->get_rendering_device();

	ResourceLoader* resourceLoader = ResourceLoader::get_singleton();

	// TODO: load only once!

	Ref<RDShaderFile> generationComputeShaderFile = resourceLoader->load("res://addons/pixelpart/shaders/PixelpartParticleGenerationCompute.glsl", "RDShaderFile");
	Ref<RDShaderFile> simulationComputeShaderFile = resourceLoader->load("res://addons/pixelpart/shaders/PixelpartParticleSimulationCompute.glsl", "RDShaderFile");
	if(!generationComputeShaderFile.is_valid() || !simulationComputeShaderFile.is_valid()) {
		throw std::runtime_error("Invalid particle compute shader file");
	}

	Ref<RDShaderSPIRV> generationComputeShaderSpirv = generationComputeShaderFile->get_spirv();
	Ref<RDShaderSPIRV> simulationComputeShaderSpirv = simulationComputeShaderFile->get_spirv();

	String generationComputeShaderError = generationComputeShaderSpirv->get_stage_compile_error(RenderingDevice::SHADER_STAGE_COMPUTE);
	if(!generationComputeShaderError.is_empty()) {
		throw std::runtime_error("Compile error in particle generation compute shader: " + std::string(generationComputeShaderError.utf8().get_data()));
	}

	String simulationComputeShaderError = simulationComputeShaderSpirv->get_stage_compile_error(RenderingDevice::SHADER_STAGE_COMPUTE);
	if(!simulationComputeShaderError.is_empty()) {
		throw std::runtime_error("Compile error in particle simulation compute shader: " + std::string(simulationComputeShaderError.utf8().get_data()));
	}

	generationComputeShaderRid = renderingDevice->shader_create_from_spirv(generationComputeShaderSpirv);
	simulationComputeShaderRid = renderingDevice->shader_create_from_spirv(simulationComputeShaderSpirv);

	generationComputePipelineRid = renderingDevice->compute_pipeline_create(generationComputeShaderRid);
	simulationComputePipelineRid = renderingDevice->compute_pipeline_create(simulationComputeShaderRid);

	generationParamsUniformBufferRid = renderingDevice->uniform_buffer_create(4 * 4);

	Ref<RDUniform> generationParamsUniform;
	generationParamsUniform.instantiate();
	generationParamsUniform->set_uniform_type(RenderingDevice::UNIFORM_TYPE_UNIFORM_BUFFER);
	generationParamsUniform->set_binding(0);
	generationParamsUniform->add_id(generationParamsUniformBufferRid);

	generationParamsUniformSetRid = renderingDevice->uniform_set_create(TypedArray<RDUniform>{ generationParamsUniform }, generationComputeShaderRid, 0);

	simulationParamsUniformBufferRid = renderingDevice->uniform_buffer_create(4 * 4);

	Ref<RDUniform> simulationParamsUniform;
	simulationParamsUniform.instantiate();
	simulationParamsUniform->set_uniform_type(RenderingDevice::UNIFORM_TYPE_UNIFORM_BUFFER);
	simulationParamsUniform->set_binding(0);
	simulationParamsUniform->add_id(simulationParamsUniformBufferRid);

	simulationParamsUniformSetRid = renderingDevice->uniform_set_create(TypedArray<RDUniform>{ simulationParamsUniform }, simulationComputeShaderRid, 0);
}
PixelpartGpuEffectEngine::~PixelpartGpuEffectEngine() {
	for(const auto& [emissionPair, textures] : particleGpuTextures) {
		renderingDevice->free_rid(textures.commonTextureRid);
		renderingDevice->free_rid(textures.positionTextureRid);
		renderingDevice->free_rid(textures.globalPositionTextureRid);
		renderingDevice->free_rid(textures.velocityTextureRid);
		renderingDevice->free_rid(textures.forceTextureRid);
		renderingDevice->free_rid(textures.rotationTextureRid);
		renderingDevice->free_rid(textures.sizeTextureRid);
		renderingDevice->free_rid(textures.colorTextureRid);
	}

	renderingDevice->free_rid(generationComputePipelineRid);
	renderingDevice->free_rid(simulationComputePipelineRid);

	renderingDevice->free_rid(generationComputeShaderRid);
	renderingDevice->free_rid(simulationComputeShaderRid);
}

void PixelpartGpuEffectEngine::advance(pixelpart::float_t dt) {
	RenderingServer* rs = RenderingServer::get_singleton();

	dt = std::max(dt, 0.0);
	runtimeContext.deltaTime() = dt;

	// TODO: remove buffers??
	/*for(pixelpart::ParticleEmissionPair emissionPair : engineEffect.particleEmissionPairs()) {
		if(particleGpuTextures.count(emissionPair) == 0) {
			// TODO: calculate size based on particle count
			particleGpuTextures[emissionPair] = createParticleDataTextures(512, 512);
			particleEmissionCounts[emissionPair] = 0.0;
		}
	}*/

	// TODO

	for(const auto& [emissionPair, textures] : particleGpuTextures) {
		const pixelpart::ParticleType& particleType = engineEffect.particleTypes().at(emissionPair.typeId);
		const pixelpart::ParticleEmitter& particleEmitter = engineEffect.sceneGraph().at<pixelpart::ParticleEmitter>(emissionPair.emitterId);
		if(!particleEmitter.primary() || !particleEmitter.active(runtimeContext)) {
			continue;
		}

		//pixelpart::float_t& emissionCount = engineParticleEmissionCounts.at(emissionPair);
		pixelpart::float_t emissionCount = 0.0;

		float_t startTime = particleEmitter.startTrigger()
			? particleEmitter.lifetimeStart() + runtimeContext.triggerActivationTime(particleEmitter.startTrigger())
			: particleEmitter.lifetimeStart();

		float_t emissionTime = particleEmitter.repeat()
			? std::fmod(runtimeContext.time() - startTime, particleEmitter.lifetimeDuration())
			: runtimeContext.time() - startTime;

		switch(particleEmitter.emissionMode()) {
			case pixelpart::ParticleEmitter::EmissionMode::continuous:
				emissionCount += particleType.count().at(emissionTime / particleEmitter.lifetimeDuration()) * runtimeContext.deltaTime();
				break;
			case pixelpart::ParticleEmitter::EmissionMode::burst_start:
				if(emissionTime < runtimeContext.deltaTime()) {
					emissionCount += particleType.count().at(0);
				}
				break;
			case pixelpart::ParticleEmitter::EmissionMode::burst_end:
				if(emissionTime > particleEmitter.lifetimeDuration() - runtimeContext.deltaTime()) {
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

		PackedByteArray generationParamsByteArray;
		generationParamsByteArray.resize(4 * 4);
		generationParamsByteArray.encode_float(0, static_cast<float>(dt));
		generationParamsByteArray.encode_u32(4, emittedParticleCount);
		generationParamsByteArray.encode_u32(8, 512);
		generationParamsByteArray.encode_u32(12, 512);

		renderingDevice->buffer_update(generationParamsUniformBufferRid, 0, generationParamsByteArray.size(), generationParamsByteArray);

		// TODO: set params

		std::uint32_t localWorkgroupSize = 64;
		std::uint32_t workgroupCount = (emittedParticleCount + (localWorkgroupSize - 1)) / localWorkgroupSize;

		auto computeList = renderingDevice->compute_list_begin();
		renderingDevice->compute_list_bind_compute_pipeline(computeList, generationComputePipelineRid);
		renderingDevice->compute_list_dispatch(computeList, workgroupCount, 1, 1);
		renderingDevice->compute_list_end();
	}

	// TODO: barrier

	/*std::uint32_t particleCount = 100; // TODO

	PackedByteArray simulationParamsByteArray;
	simulationParamsByteArray.resize(4 * 4);
	simulationParamsByteArray.encode_float(0, static_cast<float>(dt));
	simulationParamsByteArray.encode_u32(4, particleCount);
	simulationParamsByteArray.encode_u32(8, 512);
	simulationParamsByteArray.encode_u32(12, 512);

	renderingDevice->buffer_update(simulationParamsUniformBufferRid, 0, simulationParamsByteArray.size(), simulationParamsByteArray);

	for(auto& [emissionPair, textures] : particleGpuTextures) {
		const pixelpart::ParticleType& particleType = engineEffect.particleTypes().at(emissionPair.typeId);
		const pixelpart::ParticleEmitter& particleEmitter = engineEffect.sceneGraph().at<pixelpart::ParticleEmitter>(emissionPair.emitterId);

		std::uint32_t localWorkgroupSize = 64;
		std::uint32_t workgroupCount = (512*512) / localWorkgroupSize;

		auto computeList = renderingDevice->compute_list_begin();
		renderingDevice->compute_list_bind_compute_pipeline(computeList, simulationComputePipelineRid);
		renderingDevice->compute_list_bind_uniform_set(computeList, simulationParamsUniformSetRid, 0);
		renderingDevice->compute_list_dispatch(computeList, workgroupCount, 1, 1);
		renderingDevice->compute_list_end();
	}

	runtimeContext.invokedEvents().clear();
	for(const auto& [eventId, event] : engineEffect.events()) {
		if(event.nodeId()) {
			const auto& node = engineEffect.sceneGraph().at(event.nodeId());

			if(node.activatedByTrigger(runtimeContext) &&
				runtimeContext.time() >= node.lifetimeStart() + event.time() &&
				runtimeContext.time() < node.lifetimeStart() + event.time() + dt) {
				runtimeContext.invokedEvents().push_back(eventId);
			}
		}
		else {
			if(runtimeContext.time() >= event.time() &&
				runtimeContext.time() < event.time() + dt) {
				runtimeContext.invokedEvents().push_back(eventId);
			}
		}
	}*/

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
	/*for(const auto& [emissionPair, particleCollection] : initialState.particleCollections()) {
		particleGpuTextures[emissionPair] = createParticleDataTextures(512, 512);
	}*/
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
	return engineEffect;
}
const pixelpart::EffectRuntimeContext& PixelpartGpuEffectEngine::context() const {
	return runtimeContext;
}

PixelpartGpuEffectEngine::ParticleGpuTextureSet PixelpartGpuEffectEngine::gpuTextures(pixelpart::id_t particleEmitterId, pixelpart::id_t particleTypeId) const {
	auto gpuTextureIt = particleGpuTextures.find(pixelpart::ParticleEmissionPair(particleEmitterId, particleTypeId));
	if(gpuTextureIt == particleGpuTextures.end()) {
		return ParticleGpuTextureSet();
	}

	return gpuTextureIt->second;
}

PixelpartGpuEffectEngine::ParticleGpuTextureSet PixelpartGpuEffectEngine::createParticleDataTextures(std::uint32_t width, std::uint32_t height) {
	ParticleGpuTextureSet textures;

	Ref<RDTextureFormat> textureFormat;
	textureFormat.instantiate();
	textureFormat->set_format(RenderingDevice::DATA_FORMAT_R32G32B32A32_SFLOAT);
	textureFormat->set_width(width);
	textureFormat->set_height(height);
	textureFormat->set_depth(1);
	textureFormat->set_usage_bits(
		RenderingDevice::TEXTURE_USAGE_SAMPLING_BIT
		| RenderingDevice::TEXTURE_USAGE_STORAGE_BIT
		| RenderingDevice::TEXTURE_USAGE_CAN_UPDATE_BIT
		| RenderingDevice::TEXTURE_USAGE_CAN_COPY_FROM_BIT
		| RenderingDevice::TEXTURE_USAGE_CAN_COPY_TO_BIT);
	textureFormat->set_mipmaps(1);
	textureFormat->set_samples(RenderingDevice::TEXTURE_SAMPLES_1);

	Ref<RDTextureView> textureView;
	textureView.instantiate();

	textures.commonTextureRid = renderingDevice->texture_create(textureFormat, textureView);
	textures.positionTextureRid = renderingDevice->texture_create(textureFormat, textureView);
	textures.globalPositionTextureRid = renderingDevice->texture_create(textureFormat, textureView);
	textures.velocityTextureRid = renderingDevice->texture_create(textureFormat, textureView);
	textures.forceTextureRid = renderingDevice->texture_create(textureFormat, textureView);
	textures.rotationTextureRid = renderingDevice->texture_create(textureFormat, textureView);
	textures.sizeTextureRid = renderingDevice->texture_create(textureFormat, textureView);
	textures.colorTextureRid = renderingDevice->texture_create(textureFormat, textureView);

	//textures.aliveListBufferRids[0] = renderingDevice->storage_buffer_create()

	const Color transparentColor(0.0f, 0.0f, 0.0f, 1.0f);
	renderingDevice->texture_clear(textures.commonTextureRid, transparentColor, 0, 1, 0, 1);
	renderingDevice->texture_clear(textures.positionTextureRid, transparentColor, 0, 1, 0, 1);
	renderingDevice->texture_clear(textures.globalPositionTextureRid, transparentColor, 0, 1, 0, 1);
	renderingDevice->texture_clear(textures.velocityTextureRid, transparentColor, 0, 1, 0, 1);
	renderingDevice->texture_clear(textures.forceTextureRid, transparentColor, 0, 1, 0, 1);
	renderingDevice->texture_clear(textures.rotationTextureRid, transparentColor, 0, 1, 0, 1);
	renderingDevice->texture_clear(textures.sizeTextureRid, transparentColor, 0, 1, 0, 1);
	renderingDevice->texture_clear(textures.colorTextureRid, transparentColor, 0, 1, 0, 1);

	return textures;
}
}
