#include "PixelpartGpuParticleInstance3D.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <pixelpart-runtime/effect/VariantParameter.h>
#include <pixelpart-runtime/effect/MaterialResource.h>
#include <pixelpart-runtime/effect/MaterialInstance.h>
#include <pixelpart-runtime/effect/ShaderGraph.h>
#include <pixelpart-runtime/effect/ParticleEmitter.h>
#include <pixelpart-runtime/effect/ParticleType.h>
#include <pixelpart-runtime/effect/ParticleRendererType.h>
#include <pixelpart-runtime/vertex/VertexFormat.h>
#include <pixelpart-runtime/vertex/VertexAttribute.h>
#include <pixelpart-runtime/vertex/VertexDataBufferDimensions.h>
#include <pixelpart-runtime/vertex/VertexDataBufferCollection.h>
#include <pixelpart-runtime/vertex/SceneContext.h>
#include <cmath>
#include <string>
#include <unordered_map>
#include <algorithm>

namespace godot {
PixelpartGpuParticleInstance3D::PixelpartGpuParticleInstance3D(VisualInstance3D* parent,
	const PixelpartGpuEffectEngine* engine, pixelpart::id_t emitterId, pixelpart::id_t typeId,
	PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider) :
	parentNode(parent), effectEngine(engine), particleEmitterId(emitterId), particleTypeId(typeId) {
	create_material(resourceProvider, shaderProvider);
	create_instance(resourceProvider);
}
PixelpartGpuParticleInstance3D::~PixelpartGpuParticleInstance3D() {
	RenderingServer* rs = RenderingServer::get_singleton();

	rs->free_rid(instanceRid);
}

void PixelpartGpuParticleInstance3D::draw(pixelpart::float_t scale) {
	RenderingServer* rs = RenderingServer::get_singleton();

	const pixelpart::Effect& effect = effectEngine->effect();
	const pixelpart::ParticleEmitter& particleEmitter = effect.sceneGraph().at<pixelpart::ParticleEmitter>(particleEmitterId);
	const pixelpart::ParticleType& particleType = effect.particleTypes().at(particleTypeId);

	switch(particleType.renderer()) {
		case pixelpart::ParticleRendererType::sprite:
		case pixelpart::ParticleRendererType::mesh:
			multiMesh->set_visible_instance_count(0);
			break;
		default:
			break;
	}

	rs->instance_set_scenario(instanceRid, parentNode->get_world_3d()->get_scenario());
	rs->instance_set_layer_mask(instanceRid, parentNode->get_layer_mask());
	rs->instance_set_pivot_data(instanceRid,
		parentNode->get_sorting_offset() + static_cast<float>(particleType.layer()) * 0.01f, false);

	Camera3D* camera = parentNode->get_viewport()->get_camera_3d();
	if(!camera) {
		return;
	}
	else if(!parentNode->is_visible() || !particleType.visibleAtLod(effectEngine->context().lod())) {
		return;
	}

	PixelpartGpuEffectEngine::ParticleGpuTextureSet gpuTextures = effectEngine->gpuTextures(particleEmitterId, particleTypeId);
	if(!gpuTextures.commonTextureRid.is_valid()) {
		return;
	}

	shaderMaterial->set_shader_parameter("u_ParticleCommonTexture", gpuTextures.commonTextureRid);
	shaderMaterial->set_shader_parameter("u_ParticleGlobalPositionTexture", gpuTextures.globalPositionTextureRid);
	shaderMaterial->set_shader_parameter("u_ParticleColorTexture", gpuTextures.colorTextureRid);
	shaderMaterial->set_shader_parameter("u_ParticleSizeTexture", gpuTextures.sizeTextureRid);

	shaderMaterial->set_shader_parameter("u_EffectTime", static_cast<float>(effectEngine->context().time()));
	shaderMaterial->set_shader_parameter("u_ObjectTime", static_cast<float>(particleEmitter.timeSinceStart(effectEngine->context())));

	std::uint32_t particleCount = 100; // TODO

	switch(particleType.renderer()) {
		case pixelpart::ParticleRendererType::sprite:
		case pixelpart::ParticleRendererType::mesh: {
			multiMesh->set_visible_instance_count(static_cast<std::int32_t>(particleCount));

			// TODO: better aabb
			multiMesh->set_custom_aabb(AABB(
				Vector3(0, 0, 0),
				Vector3(100, 100, 100)));

			break;
		}

		default: {
			break;
		}
	}
}

void PixelpartGpuParticleInstance3D::create_material(PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider) {
	const pixelpart::Effect& effect = effectEngine->effect();
	const pixelpart::ParticleType& particleType = effect.particleTypes().at(particleTypeId);
	const pixelpart::MaterialInstance& materialInstance = particleType.materialInstance();

	std::unordered_map<pixelpart::id_t, std::string> shaderParameterNames;
	std::unordered_map<std::string, std::string> textureResourceIds;

	if(materialInstance.builtInMaterial()) {
		shader = shaderProvider.get_builtin_gpu_spatial_shader(materialInstance.materialId());

		if(shader.is_valid()) {
			const auto& metadata = shaderProvider.get_builtin_spatial_shader_metadata(materialInstance.materialId());
			for(const auto& [parameterId, parameter] : metadata.parameters()) {
				shaderParameterNames[parameterId] = PixelpartShaderProvider::uniformPrefix + parameter.name();
			}
		}
	}
	else if(effect.resources().materials().count(materialInstance.materialId()) != 0) {
		const pixelpart::MaterialResource& materialResource = effect.resources().materials().at(materialInstance.materialId());

		pixelpart::ShaderGraph::BuildResult shaderBuildResult;
		materialResource.shaderGraph().build(shaderBuildResult);

		shaderParameterNames = shaderBuildResult.parameterNames;
		for(std::uint32_t samplerIndex = 0; samplerIndex < shaderBuildResult.textureResourceIds.size(); samplerIndex++) {
			textureResourceIds[pixelpart::ShaderGraph::specification.textureSamplers[samplerIndex]] = shaderBuildResult.textureResourceIds[samplerIndex];
		}

		// TODO: select gpu variant
		shader = shaderProvider.get_custom_spatial_shader(
			shaderBuildResult.mainCode, shaderBuildResult.parameterCode,
			particleType.renderer(),
			materialResource.blendMode(),
			materialResource.lightingMode());
	}

	shaderMaterial.instantiate();

	if(!shader.is_valid()) {
		UtilityFunctions::push_error("Could not create particle material");
		return;
	}

	shaderMaterial->set_shader(shader);

	for(const auto& [samplerName, resourceId] : textureResourceIds) {
		if(resourceProvider.get_textures().count(resourceId) == 0) {
			continue;
		}

		shaderMaterial->set_shader_parameter(
			StringName(samplerName.c_str()), resourceProvider.get_textures().at(resourceId)->get_rid());
	}

	for(const auto& [parameterName, parameterValue] : particleType.materialInstance().materialParameters()) {
		if(shaderParameterNames.count(parameterName) == 0) {
			continue;
		}

		StringName shaderParameterName = StringName(shaderParameterNames.at(parameterName).c_str());

		switch(parameterValue.type()) {
			case pixelpart::VariantParameter::Value::type_int:
				shaderMaterial->set_shader_parameter(shaderParameterName, pxpt_to_gd(parameterValue.valueInt()));
				break;
			case pixelpart::VariantParameter::Value::type_float:
				shaderMaterial->set_shader_parameter(shaderParameterName, pxpt_to_gd(parameterValue.valueFloat()));
				break;
			case pixelpart::VariantParameter::Value::type_float2:
				shaderMaterial->set_shader_parameter(shaderParameterName, pxpt_to_gd(parameterValue.valueFloat2()));
				break;
			case pixelpart::VariantParameter::Value::type_float3:
				shaderMaterial->set_shader_parameter(shaderParameterName, pxpt_to_gd(parameterValue.valueFloat3()));
				break;
			case pixelpart::VariantParameter::Value::type_float4:
				shaderMaterial->set_shader_parameter(shaderParameterName, pxpt_to_gd(parameterValue.valueFloat4()));
				break;
			case pixelpart::VariantParameter::Value::type_bool:
				shaderMaterial->set_shader_parameter(shaderParameterName, parameterValue.valueBool());
				break;
			case pixelpart::VariantParameter::Value::type_enum:
				shaderMaterial->set_shader_parameter(shaderParameterName, pxpt_to_gd(parameterValue.valueEnum()));
				break;
			case pixelpart::VariantParameter::Value::type_color:
				shaderMaterial->set_shader_parameter(shaderParameterName, pxpt_to_gd_color(parameterValue.valueColor()));
				break;
			case pixelpart::VariantParameter::Value::type_resource_image:
				if(resourceProvider.get_textures().count(parameterValue.valueResourceId()) != 0u) {
					shaderMaterial->set_shader_parameter(shaderParameterName, resourceProvider.get_textures().at(parameterValue.valueResourceId())->get_rid());
				}
				break;
			default:
				break;
		}
	}
}
void PixelpartGpuParticleInstance3D::create_instance(PixelpartGraphicsResourceProvider& resourceProvider) {
	RenderingServer* rs = RenderingServer::get_singleton();

	const pixelpart::Effect& effect = effectEngine->effect();
	const pixelpart::ParticleType& particleType = effect.particleTypes().at(particleTypeId);

	instanceRid = rs->instance_create();

	multiMesh.instantiate();
	multiMesh->set_transform_format(MultiMesh::TRANSFORM_3D);
	multiMesh->set_use_colors(false);
	multiMesh->set_use_custom_data(false);

	// TODO: is this necessary?
	std::uint32_t maxParticleCount = 1000;
	multiMesh->set_instance_count(static_cast<std::int32_t>(maxParticleCount));
	for(std::uint32_t i = 0; i < maxParticleCount; i++) {
		multiMesh->set_instance_transform(static_cast<std::int32_t>(i), Transform3D());
	}

	switch(particleType.renderer()) {
		case pixelpart::ParticleRendererType::sprite: {
			PackedVector3Array vertices;
			vertices.push_back(Vector3(-0.5f, -0.5f, 0.0f));
			vertices.push_back(Vector3(0.5f, -0.5f, 0.0f));
			vertices.push_back(Vector3(0.5f, 0.5f, 0.0f));
			vertices.push_back(Vector3(-0.5f, 0.5f, 0.0f));

			PackedVector3Array normals;
			normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
			normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
			normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
			normals.push_back(Vector3(0.0f, 0.0f, 1.0f));

			PackedVector2Array uvs;
			uvs.push_back(Vector2(0.0f, 0.0f));
			uvs.push_back(Vector2(1.0f, 0.0f));
			uvs.push_back(Vector2(1.0f, 1.0f));
			uvs.push_back(Vector2(0.0f, 1.0f));

			PackedInt32Array indices;
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(3);
			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(3);

			Array meshArray;
			meshArray.resize(Mesh::ARRAY_MAX);
			meshArray[Mesh::ARRAY_VERTEX] = vertices;
			meshArray[Mesh::ARRAY_NORMAL] = normals;
			meshArray[Mesh::ARRAY_TEX_UV] = uvs;
			meshArray[Mesh::ARRAY_INDEX] = indices;

			Ref<ArrayMesh> baseMesh;
			baseMesh.instantiate();
			baseMesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, meshArray, Array(), Dictionary(),
				Mesh::ARRAY_FORMAT_VERTEX | Mesh::ARRAY_FORMAT_NORMAL |
				Mesh::ARRAY_FORMAT_TEX_UV | Mesh::ARRAY_FORMAT_INDEX);
			baseMesh->surface_set_material(0, shaderMaterial);

			multiMesh->set_mesh(baseMesh);

			break;
		}

		case pixelpart::ParticleRendererType::mesh: {
			if(resourceProvider.get_meshes().count(particleType.meshRendererSettings().meshResourceId) != 0) {
				Ref<ArrayMesh> baseMesh = resourceProvider.get_meshes().at(particleType.meshRendererSettings().meshResourceId);
				baseMesh->surface_set_material(0, shaderMaterial);

				multiMesh->set_mesh(baseMesh);
			}
			else {
				UtilityFunctions::push_error("Mesh \"", String(particleType.meshRendererSettings().meshResourceId.c_str()), "\" not found");
			}

			break;
		}

		default: {
			break;
		}
	}

	rs->instance_set_base(instanceRid, multiMesh->get_rid());
	rs->instance_geometry_set_flag(instanceRid, RenderingServer::INSTANCE_FLAG_DRAW_NEXT_FRAME_IF_VISIBLE, true);
}
}
