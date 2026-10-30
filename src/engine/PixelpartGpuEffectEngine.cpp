#include "PixelpartGpuEffectEngine.h"
#include <stdexcept>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rd_shader_file.hpp>
#include <godot_cpp/classes/rd_shader_spirv.hpp>
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

	TypedArray<RDUniform> generationParamSetUniforms;
	generationParamSetUniforms.push_back(createUniformBufferUniform(0, generationParamsUniformBufferRid));

	generationParamsUniformSetRid = renderingDevice->uniform_set_create(generationParamSetUniforms, generationComputeShaderRid, 1);

	simulationParamsUniformBufferRid = renderingDevice->uniform_buffer_create(4 * 4);

	TypedArray<RDUniform> simulationParamSetUniforms;
	simulationParamSetUniforms.push_back(createUniformBufferUniform(0, simulationParamsUniformBufferRid));

	simulationParamsUniformSetRid = renderingDevice->uniform_set_create(simulationParamSetUniforms, simulationComputeShaderRid, 1);
}
PixelpartGpuEffectEngine::~PixelpartGpuEffectEngine() {
	for(const auto& [emissionPair, resourceSet] : particleGpuResources) {
		renderingDevice->free_rid(resourceSet.generationShaderImageUniformSetRid);
		renderingDevice->free_rid(resourceSet.simulationShaderImageUniformSetRid);

		renderingDevice->free_rid(resourceSet.commonTextureRid);
		renderingDevice->free_rid(resourceSet.positionTextureRid);
		renderingDevice->free_rid(resourceSet.globalPositionTextureRid);
		renderingDevice->free_rid(resourceSet.velocityTextureRid);
		renderingDevice->free_rid(resourceSet.forceTextureRid);
		renderingDevice->free_rid(resourceSet.rotationTextureRid);
		renderingDevice->free_rid(resourceSet.sizeTextureRid);
		renderingDevice->free_rid(resourceSet.colorTextureRid);
	}

	renderingDevice->free_rid(generationParamsUniformSetRid);
	renderingDevice->free_rid(generationParamsUniformBufferRid);
	renderingDevice->free_rid(simulationParamsUniformSetRid);
	renderingDevice->free_rid(simulationParamsUniformBufferRid);

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
	for(pixelpart::ParticleEmissionPair emissionPair : engineEffect.particleEmissionPairs()) {
		if(particleGpuResources.count(emissionPair) == 0) {
			// TODO: calculate size based on particle count
			particleGpuResources[emissionPair] = createParticleGpuResources(512, 512);
			particleEmissionCounts[emissionPair] = 0.0;
		}
	}

	// TODO

	const std::uint32_t localWorkgroupSize = 64;

	/*for(const auto& [emissionPair, textures] : particleGpuResources) {
		const pixelpart::ParticleType& particleType = engineEffect.particleTypes().at(emissionPair.typeId);
		const pixelpart::ParticleEmitter& particleEmitter = engineEffect.sceneGraph().at<pixelpart::ParticleEmitter>(emissionPair.emitterId);
		if(!particleEmitter.primary() || !particleEmitter.active(runtimeContext)) {
			continue;
		}

		pixelpart::float_t& emissionCount = particleEmissionCounts.at(emissionPair);

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

		std::uint32_t workgroupCount = (emittedParticleCount + (localWorkgroupSize - 1)) / localWorkgroupSize;

		auto computeList = renderingDevice->compute_list_begin();
		renderingDevice->compute_list_bind_compute_pipeline(computeList, generationComputePipelineRid);
		renderingDevice->compute_list_bind_uniform_set(computeList, textures.generationShaderImageUniformSetRid, 0);
		renderingDevice->compute_list_bind_uniform_set(computeList, generationParamsUniformSetRid, 1);
		renderingDevice->compute_list_dispatch(computeList, workgroupCount, 1, 1);
		renderingDevice->compute_list_end();
	}*/

	// TODO: barrier

	std::uint32_t particleCount = 100; // TODO

	for(auto& [emissionPair, textures] : particleGpuResources) {
		const pixelpart::ParticleType& particleType = engineEffect.particleTypes().at(emissionPair.typeId);
		const pixelpart::ParticleEmitter& particleEmitter = engineEffect.sceneGraph().at<pixelpart::ParticleEmitter>(emissionPair.emitterId);

		PackedByteArray simulationParamsByteArray;
		simulationParamsByteArray.resize(4 * 4);
		simulationParamsByteArray.encode_float(0, static_cast<float>(dt));
		simulationParamsByteArray.encode_u32(4, particleCount);
		simulationParamsByteArray.encode_u32(8, 512);
		simulationParamsByteArray.encode_u32(12, 512);

		renderingDevice->buffer_update(simulationParamsUniformBufferRid, 0, simulationParamsByteArray.size(), simulationParamsByteArray);

		std::uint32_t workgroupCount = (particleCount + (localWorkgroupSize - 1)) / localWorkgroupSize;

		auto computeList = renderingDevice->compute_list_begin();
		renderingDevice->compute_list_bind_compute_pipeline(computeList, simulationComputePipelineRid);
		renderingDevice->compute_list_bind_uniform_set(computeList, textures.simulationShaderImageUniformSetRid, 0);
		renderingDevice->compute_list_bind_uniform_set(computeList, simulationParamsUniformSetRid, 1);
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

	particleGpuResources.clear();
	for(const auto& [emissionPair, particleCollection] : initialState.particleCollections()) {
		particleGpuResources[emissionPair] = createParticleGpuResources(512, 512);
		particleEmissionCounts[emissionPair] = 0.0;
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
	return engineEffect;
}
const pixelpart::EffectRuntimeContext& PixelpartGpuEffectEngine::context() const {
	return runtimeContext;
}

PixelpartGpuEffectEngine::ParticleGpuTextureSet PixelpartGpuEffectEngine::gpuTextures(pixelpart::id_t particleEmitterId, pixelpart::id_t particleTypeId) const {
	auto gpuResourceIt = particleGpuResources.find(pixelpart::ParticleEmissionPair(particleEmitterId, particleTypeId));
	if(gpuResourceIt == particleGpuResources.end()) {
		return ParticleGpuTextureSet();
	}

	return ParticleGpuTextureSet{
		gpuResourceIt->second.commonTextureWrapper->get_rid(),
		gpuResourceIt->second.globalPositionTextureWrapper->get_rid(),
		gpuResourceIt->second.colorTextureWrapper->get_rid(),
		gpuResourceIt->second.sizeTextureWrapper->get_rid()
	};
}

PixelpartGpuEffectEngine::ParticleGpuResourceSet PixelpartGpuEffectEngine::createParticleGpuResources(std::uint32_t width, std::uint32_t height) {
	ParticleGpuResourceSet resources;

	Ref<RDTextureFormat> textureFormat;
	textureFormat.instantiate();
	textureFormat->set_width(width);
	textureFormat->set_height(height);
	textureFormat->set_depth(1);
	textureFormat->set_format(RenderingDevice::DATA_FORMAT_R32G32B32A32_SFLOAT);
	textureFormat->set_usage_bits(
		RenderingDevice::TEXTURE_USAGE_SAMPLING_BIT |
		RenderingDevice::TEXTURE_USAGE_STORAGE_BIT |
		RenderingDevice::TEXTURE_USAGE_CAN_COPY_TO_BIT);
	textureFormat->set_mipmaps(1);
	textureFormat->set_samples(RenderingDevice::TEXTURE_SAMPLES_1);

	Ref<RDTextureView> textureView;
	textureView.instantiate();

	resources.commonTextureRid = renderingDevice->texture_create(textureFormat, textureView);
	resources.positionTextureRid = renderingDevice->texture_create(textureFormat, textureView);
	resources.globalPositionTextureRid = renderingDevice->texture_create(textureFormat, textureView);
	resources.velocityTextureRid = renderingDevice->texture_create(textureFormat, textureView);
	resources.forceTextureRid = renderingDevice->texture_create(textureFormat, textureView);
	resources.rotationTextureRid = renderingDevice->texture_create(textureFormat, textureView);
	resources.sizeTextureRid = renderingDevice->texture_create(textureFormat, textureView);
	resources.colorTextureRid = renderingDevice->texture_create(textureFormat, textureView);

	//resources.aliveListBufferRids[0] = renderingDevice->storage_buffer_create()

	const Color transparentColor(0.0f, 0.0f, 0.0f, 1.0f);
	renderingDevice->texture_clear(resources.commonTextureRid, transparentColor, 0, 1, 0, 1);
	renderingDevice->texture_clear(resources.positionTextureRid, transparentColor, 0, 1, 0, 1);
	renderingDevice->texture_clear(resources.globalPositionTextureRid, transparentColor, 0, 1, 0, 1);
	renderingDevice->texture_clear(resources.velocityTextureRid, transparentColor, 0, 1, 0, 1);
	renderingDevice->texture_clear(resources.forceTextureRid, transparentColor, 0, 1, 0, 1);
	renderingDevice->texture_clear(resources.rotationTextureRid, transparentColor, 0, 1, 0, 1);
	renderingDevice->texture_clear(resources.sizeTextureRid, transparentColor, 0, 1, 0, 1);
	renderingDevice->texture_clear(resources.colorTextureRid, transparentColor, 0, 1, 0, 1);

	TypedArray<RDUniform> imageUniforms;
	imageUniforms.push_back(createImageUniform(0, resources.commonTextureRid));
	imageUniforms.push_back(createImageUniform(1, resources.positionTextureRid));
	imageUniforms.push_back(createImageUniform(2, resources.globalPositionTextureRid));
	imageUniforms.push_back(createImageUniform(3, resources.velocityTextureRid));
	imageUniforms.push_back(createImageUniform(4, resources.forceTextureRid));
	imageUniforms.push_back(createImageUniform(5, resources.rotationTextureRid));
	imageUniforms.push_back(createImageUniform(6, resources.sizeTextureRid));
	imageUniforms.push_back(createImageUniform(7, resources.colorTextureRid));

	resources.generationShaderImageUniformSetRid = renderingDevice->uniform_set_create(imageUniforms, generationComputeShaderRid, 0);
	resources.simulationShaderImageUniformSetRid = renderingDevice->uniform_set_create(imageUniforms, simulationComputeShaderRid, 0);

	resources.commonTextureWrapper = createTexture2DWrapper(resources.commonTextureRid);
	resources.globalPositionTextureWrapper = createTexture2DWrapper(resources.globalPositionTextureRid);
	resources.colorTextureWrapper = createTexture2DWrapper(resources.colorTextureRid);
	resources.sizeTextureWrapper = createTexture2DWrapper(resources.sizeTextureRid);

	return resources;
}

Ref<Texture2DRD> PixelpartGpuEffectEngine::createTexture2DWrapper(RID textureRid) {
	Ref<Texture2DRD> textureWrapper;
	textureWrapper.instantiate();
	textureWrapper->set_texture_rd_rid(textureRid);

	return textureWrapper;
}
Ref<RDUniform> PixelpartGpuEffectEngine::createUniformBufferUniform(std::int32_t binding, RID id) {
	Ref<RDUniform> uniform;
	uniform.instantiate();
	uniform->set_uniform_type(RenderingDevice::UNIFORM_TYPE_UNIFORM_BUFFER);
	uniform->set_binding(binding);
	uniform->add_id(id);

	return uniform;
}
Ref<RDUniform> PixelpartGpuEffectEngine::createImageUniform(std::int32_t binding, RID id) {
	Ref<RDUniform> uniform;
	uniform.instantiate();
	uniform->set_uniform_type(RenderingDevice::UNIFORM_TYPE_IMAGE);
	uniform->set_binding(binding);
	uniform->add_id(id);

	return uniform;
}
}
