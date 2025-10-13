#include "PixelpartParticleRenderer3D.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <pixelpart-runtime/common/VariantParameter.h>
#include <pixelpart-runtime/effect/MaterialResource.h>
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
#include <algorithm>

namespace godot {
PixelpartParticleRenderer3D::PixelpartParticleRenderer3D(PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider,
	const pixelpart::Effect& eff, pixelpart::id_t emitterId, pixelpart::id_t typeId) :
	resources(resourceProvider), effect(eff), particleEmitterId(emitterId), particleTypeId(typeId) {
	const pixelpart::ParticleType& particleType = effect.particleTypes().at(particleTypeId);
	const std::string& materialId = particleType.materialInstance().materialId();

	if(particleType.materialInstance().builtInMaterial()) {
		shader = shaderProvider.get_builtin_spatial_shader(materialId);

		if(shader.is_valid()) {
			const PixelpartShaderProvider::ShaderMetadata& metadata = shaderProvider.get_builtin_spatial_shader_metadata(materialId);
			for(const auto& parameterEntry : metadata.parameterNames) {
				shaderParameterNames[parameterEntry.first] = PixelpartShaderProvider::uniformPrefix + parameterEntry.second;
			}
		}
	}
	else if(effect.resources().materials().count(materialId) != 0) {
		const pixelpart::MaterialResource& materialResource = effect.resources().materials().at(materialId);

		pixelpart::ShaderGraph::BuildResult shaderBuildResult;
		materialResource.shaderGraph().build(shaderBuildResult);

		shaderParameterNames = shaderBuildResult.parameterNames;
		for(std::uint32_t samplerIndex = 0; samplerIndex < shaderBuildResult.textureResourceIds.size(); samplerIndex++) {
			textureResourceIds[pixelpart::ShaderGraph::graphLanguage.textureSamplers[samplerIndex]] = shaderBuildResult.textureResourceIds[samplerIndex];
		}

		shader = shaderProvider.get_custom_spatial_shader(
			shaderBuildResult.mainCode, shaderBuildResult.parameterCode,
			particleType.renderer(),
			materialResource.blendMode(),
			materialResource.lightingMode());
	}

	shaderMaterial.instantiate();

	if(shader.is_valid()) {
		shaderMaterial->set_shader(shader);
	}
	else {
		UtilityFunctions::push_error("Could not create particle material");
	}

	RenderingServer* rs = RenderingServer::get_singleton();
	instanceRID = rs->instance_create();

	switch(particleType.renderer()) {
		case pixelpart::ParticleRendererType::sprite:
		case pixelpart::ParticleRendererType::trail: {
			arrayMesh.instantiate();
			rs->instance_set_base(instanceRID, arrayMesh->get_rid());

			break;
		}

		case pixelpart::ParticleRendererType::mesh: {
			multiMesh.instantiate();
			multiMesh->set_transform_format(MultiMesh::TRANSFORM_3D);
			multiMesh->set_use_colors(true);
			multiMesh->set_use_custom_data(true);
			if(resources.meshes().count(particleType.meshRendererSettings().meshResourceId) != 0) {
				Ref<ArrayMesh> baseMesh = resources.meshes().at(particleType.meshRendererSettings().meshResourceId);
				baseMesh->surface_set_material(0, shaderMaterial);

				multiMesh->set_mesh(baseMesh);
			}
			else {
				UtilityFunctions::push_error("Mesh \"", String(particleType.meshRendererSettings().meshResourceId.c_str()), "\" not found");
			}

			rs->instance_set_base(instanceRID, multiMesh->get_rid());

			break;
		}

		default: {
			break;
		}
	}

	pixelpart::VertexFormat vertexFormat;

	switch(particleType.renderer()) {
		case pixelpart::ParticleRendererType::sprite:
		case pixelpart::ParticleRendererType::trail:
			vertexFormat = pixelpart::VertexFormat({
					pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::index, pixelpart::VertexDataGenerationMode::vertex, pixelpart::VertexDataType::type_int32, 0, 0, sizeof(std::int32_t)),
					pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::position3d, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 1, 0, sizeof(float) * 3),
					pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::normal, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 2, 0, sizeof(float) * 3),
					pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::texture_coord, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 3, 0, sizeof(float) * 2),
					pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::life, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 4, 0, sizeof(float) * 2),
					pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::id, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 4, sizeof(float), sizeof(float) * 2),
					pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::color, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 5, 0, sizeof(float) * 4),
					pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::velocity3d, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 6, 0, sizeof(float) * 4)
				},
				pixelpart::VertexWindingOrder::cw);
			break;
		case pixelpart::ParticleRendererType::mesh:
			vertexFormat = pixelpart::VertexFormat({
					pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::matrix4x3_row_major, pixelpart::VertexDataGenerationMode::instance, pixelpart::VertexDataType::type_float, 0, 0, sizeof(float) * 20),
					pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::color, pixelpart::VertexDataGenerationMode::instance, pixelpart::VertexDataType::type_float, 0, sizeof(float) * 12, sizeof(float) * 20),
					pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::velocity3d, pixelpart::VertexDataGenerationMode::instance, pixelpart::VertexDataType::type_float, 0, sizeof(float) * 16, sizeof(float) * 20),
					pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::life, pixelpart::VertexDataGenerationMode::instance, pixelpart::VertexDataType::type_float, 1, 0, sizeof(float) * 2),
					pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::id, pixelpart::VertexDataGenerationMode::instance, pixelpart::VertexDataType::type_float, 1, sizeof(float), sizeof(float) * 2)
				},
				pixelpart::VertexWindingOrder::cw);
			break;
		default:
			break;
	}

	vertexGenerator = std::make_unique<pixelpart::ParticleVertexGenerator>(
		effect, particleEmitterId, particleTypeId, vertexFormat);
}
PixelpartParticleRenderer3D::~PixelpartParticleRenderer3D() {
	RenderingServer* rs = RenderingServer::get_singleton();

	rs->free_rid(instanceRID);
}

void PixelpartParticleRenderer3D::draw(VisualInstance3D* parentNode,
	pixelpart::ParticleCollection::ReadPtr particles, std::uint32_t particleCount,
	const pixelpart::EffectRuntimeContext& runtimeContext, pixelpart::float_t scale) {
	const pixelpart::ParticleEmitter& particleEmitter = effect.sceneGraph().at<pixelpart::ParticleEmitter>(particleEmitterId);
	const pixelpart::ParticleType& particleType = effect.particleTypes().at(particleTypeId);

	switch(particleType.renderer()) {
		case pixelpart::ParticleRendererType::sprite:
		case pixelpart::ParticleRendererType::trail:
			arrayMesh->clear_surfaces();
			break;
		case pixelpart::ParticleRendererType::mesh:
			multiMesh->set_visible_instance_count(0);
			break;
		default:
			break;
	}

	if(!parentNode->is_visible() || !particleType.visible()) {
		return;
	}

	for(const auto& textureParameterEntry : textureResourceIds) {
		if(resources.textures().count(textureParameterEntry.second) == 0) {
			continue;
		}

		shaderMaterial->set_shader_parameter(
			StringName(textureParameterEntry.first.c_str()), resources.textures().at(textureParameterEntry.second)->get_rid());
	}

	for(const auto& parameterEntry : particleType.materialInstance().materialParameters()) {
		if(shaderParameterNames.count(parameterEntry.first) == 0) {
			continue;
		}

		const pixelpart::VariantParameter::Value& parameterValue = parameterEntry.second;
		StringName parameterName = StringName(shaderParameterNames.at(parameterEntry.first).c_str());

		switch(parameterValue.type()) {
			case pixelpart::VariantParameter::Value::type_int:
				shaderMaterial->set_shader_parameter(parameterName, pxpt_to_gd(parameterValue.valueInt()));
				break;
			case pixelpart::VariantParameter::Value::type_float:
				shaderMaterial->set_shader_parameter(parameterName, pxpt_to_gd(parameterValue.valueFloat()));
				break;
			case pixelpart::VariantParameter::Value::type_float2:
				shaderMaterial->set_shader_parameter(parameterName, pxpt_to_gd(parameterValue.valueFloat2()));
				break;
			case pixelpart::VariantParameter::Value::type_float3:
				shaderMaterial->set_shader_parameter(parameterName, pxpt_to_gd(parameterValue.valueFloat3()));
				break;
			case pixelpart::VariantParameter::Value::type_float4:
				shaderMaterial->set_shader_parameter(parameterName, pxpt_to_gd(parameterValue.valueFloat4()));
				break;
			case pixelpart::VariantParameter::Value::type_bool:
				shaderMaterial->set_shader_parameter(parameterName, parameterValue.valueBool());
				break;
			case pixelpart::VariantParameter::Value::type_enum:
				shaderMaterial->set_shader_parameter(parameterName, pxpt_to_gd(parameterValue.valueEnum()));
				break;
			case pixelpart::VariantParameter::Value::type_color:
				shaderMaterial->set_shader_parameter(parameterName, pxpt_to_gd_color(parameterValue.valueColor()));
				break;
			case pixelpart::VariantParameter::Value::type_resource_image:
				if(resources.textures().count(parameterValue.valueResourceId()) != 0u) {
					shaderMaterial->set_shader_parameter(parameterName, resources.textures().at(parameterValue.valueResourceId())->get_rid());
				}
				break;
			default:
				break;
		}
	}

	shaderMaterial->set_shader_parameter("u_EffectTime", static_cast<float>(runtimeContext.time()));
	shaderMaterial->set_shader_parameter("u_ObjectTime", static_cast<float>(particleEmitter.timeSinceStart(runtimeContext)));

	RenderingServer* rs = RenderingServer::get_singleton();
	rs->instance_set_scenario(instanceRID, parentNode->get_world_3d()->get_scenario());
	rs->instance_set_layer_mask(instanceRID, parentNode->get_layer_mask());
	rs->instance_set_pivot_data(instanceRID,
		parentNode->get_sorting_offset() + static_cast<double>(particleType.layer()) * 0.01, false);

	Camera3D* camera = parentNode->get_viewport()->get_camera_3d();
	if(!camera) {
		return;
	}

	pixelpart::SceneContext sceneContext;
	sceneContext.effectScale = pixelpart::float3_t(scale);
	sceneContext.cameraPosition = gd_to_pxpt(camera->get_global_transform().origin);
	sceneContext.cameraForward = gd_to_pxpt(camera->get_global_transform().basis.get_column(2));
	sceneContext.cameraRight = gd_to_pxpt(camera->get_global_transform().basis.get_column(0));
	sceneContext.cameraUp = gd_to_pxpt(camera->get_global_transform().basis.get_column(1));

	pixelpart::VertexDataBufferDimensions bufferDimensions = vertexGenerator->buildGeometry(
		particles, particleCount,
		runtimeContext, sceneContext);
	if(std::any_of(bufferDimensions.begin(), bufferDimensions.end(), [](std::uint32_t c) { return c == 0; })) {
		return;
	}

	switch(particleType.renderer()) {
		case pixelpart::ParticleRendererType::sprite:
		case pixelpart::ParticleRendererType::trail: {
			indexArray.resize(static_cast<std::int64_t>(bufferDimensions[0]));
			vertexArray.resize(static_cast<std::int64_t>(bufferDimensions[1]));
			normalArray.resize(static_cast<std::int64_t>(bufferDimensions[2]));
			uvArray.resize(static_cast<std::int64_t>(bufferDimensions[3]));
			uv2Array.resize(static_cast<std::int64_t>(bufferDimensions[4]));
			colorArray.resize(static_cast<std::int64_t>(bufferDimensions[5]));
			custom0Array.resize(static_cast<std::int64_t>(bufferDimensions[6] * 4));

			vertexGenerator->generateVertexData(pixelpart::VertexDataBufferCollection({
					reinterpret_cast<std::uint8_t*>(indexArray.ptrw()),
					reinterpret_cast<std::uint8_t*>(vertexArray.ptrw()),
					reinterpret_cast<std::uint8_t*>(normalArray.ptrw()),
					reinterpret_cast<std::uint8_t*>(uvArray.ptrw()),
					reinterpret_cast<std::uint8_t*>(uv2Array.ptrw()),
					reinterpret_cast<std::uint8_t*>(colorArray.ptrw()),
					reinterpret_cast<std::uint8_t*>(custom0Array.ptrw())
				}),
				particles, particleCount,
				runtimeContext, sceneContext);

			Array meshArray;
			meshArray.resize(Mesh::ARRAY_MAX);
			meshArray[Mesh::ARRAY_VERTEX] = vertexArray;
			meshArray[Mesh::ARRAY_NORMAL] = normalArray;
			meshArray[Mesh::ARRAY_COLOR] = colorArray;
			meshArray[Mesh::ARRAY_TEX_UV] = uvArray;
			meshArray[Mesh::ARRAY_TEX_UV2] = uv2Array;
			meshArray[Mesh::ARRAY_CUSTOM0] = custom0Array;
			meshArray[Mesh::ARRAY_INDEX] = indexArray;

			arrayMesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, meshArray, Array(), Dictionary(),
				Mesh::ARRAY_FORMAT_VERTEX | Mesh::ARRAY_FORMAT_NORMAL | Mesh::ARRAY_FORMAT_COLOR |
				Mesh::ARRAY_FORMAT_TEX_UV | Mesh::ARRAY_FORMAT_TEX_UV2 |
				Mesh::ARRAY_FORMAT_CUSTOM0 | Mesh::ARRAY_FORMAT_INDEX |
				(Mesh::ARRAY_CUSTOM_RGBA_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT));

			if(arrayMesh->get_surface_count() > 0) {
				arrayMesh->surface_set_material(0, shaderMaterial);
			}

			break;
		}

		case pixelpart::ParticleRendererType::mesh: {
			instanceDataArray.resize(static_cast<std::int64_t>(bufferDimensions[0] * 20));
			custom0Array.resize(static_cast<std::int64_t>(bufferDimensions[1] * 2));

			float* instanceDataPtr = instanceDataArray.ptrw();
			float* custom0Ptr = custom0Array.ptrw();

			vertexGenerator->generateVertexData(pixelpart::VertexDataBufferCollection({
					reinterpret_cast<std::uint8_t*>(instanceDataPtr),
					reinterpret_cast<std::uint8_t*>(custom0Ptr)
				}),
				particles, particleCount,
				runtimeContext, sceneContext);

			const float packFactor = 0.5f;
			for(std::uint32_t p = 0; p < bufferDimensions[1]; p++) {
				instanceDataPtr[p * 20 + 19] = pack_int_float(custom0Ptr[p * 2 + 1], custom0Ptr[p * 2], packFactor);
			}

			multiMesh->set_visible_instance_count(-1);
			multiMesh->set_instance_count(static_cast<std::int32_t>(bufferDimensions[0]));
			multiMesh->set_buffer(instanceDataArray);

			break;
		}

		default: {
			break;
		}
	}
}

float PixelpartParticleRenderer3D::pack_int_float(float a, float b, float s) {
	return a + b * s;
}
}
