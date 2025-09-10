#include "PixelpartParticleRenderer2D.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <pixelpart-runtime/common/VariantParameter.h>
#include <pixelpart-runtime/effect/MaterialResource.h>
#include <pixelpart-runtime/effect/ShaderGraph.h>
#include <cmath>
#include <algorithm>

namespace godot {
PixelpartParticleRenderer2D::PixelpartParticleRenderer2D(PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider,
	const pixelpart::Effect& eff, pixelpart::id_t emitterId, pixelpart::id_t typeId) :
	resources(resourceProvider), effect(eff), particleEmitterId(emitterId), particleTypeId(typeId) {
	const pixelpart::ParticleType& particleType = effect.particleTypes().at(particleTypeId);
	const std::string& materialId = particleType.materialInstance().materialId();

	if(particleType.materialInstance().builtInMaterial()) {
		shader = shaderProvider.get_builtin_canvas_item_shader(materialId);

		if(shader.is_valid()) {
			const PixelpartShaderProvider::ShaderMetadata& metadata = shaderProvider.get_builtin_canvas_item_shader_metadata(materialId);
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

		shader = shaderProvider.get_custom_canvas_item_shader(
			shaderBuildResult.mainCode, shaderBuildResult.parameterCode,
			particleType.renderer(),
			materialResource.blendMode(),
			materialResource.lightingMode());
	}

	RenderingServer* rs = RenderingServer::get_singleton();
	materialRID = rs->material_create();
	if(shader.is_valid()) {
		rs->material_set_shader(materialRID, shader->get_rid());
	}
	else {
		UtilityFunctions::push_warning("Could not create particle material");
	}

	meshRID = rs->mesh_create();

	canvasItemRID = rs->canvas_item_create();
	rs->canvas_item_set_material(canvasItemRID, materialRID);
	rs->canvas_item_add_mesh(canvasItemRID, meshRID);

	pixelpart::VertexFormat vertexFormat({
			pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::index, pixelpart::VertexDataGenerationMode::vertex, pixelpart::VertexDataType::type_int32, 0, 0, sizeof(std::int32_t)),
			pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::position2d, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 1, 0, sizeof(float) * 2),
			pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::texture_coord, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 2, 0, sizeof(float) * 2),
			pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::color, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 3, 0, sizeof(float) * 4),
			pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::velocity2d, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 4, 0, sizeof(float) * 4),
			pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::life, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 4, sizeof(float) * 2, sizeof(float) * 4),
			pixelpart::VertexAttribute(pixelpart::VertexAttributeUsage::id, pixelpart::VertexDataGenerationMode::element, pixelpart::VertexDataType::type_float, 4, sizeof(float) * 3, sizeof(float) * 4)
		}, pixelpart::VertexWindingOrder::cw);

	vertexGenerator = std::make_unique<pixelpart::ParticleVertexGenerator>(
		effect, particleEmitterId, particleTypeId, vertexFormat);
}
PixelpartParticleRenderer2D::~PixelpartParticleRenderer2D() {
	RenderingServer* rs = RenderingServer::get_singleton();

	rs->free_rid(materialRID);
	rs->free_rid(meshRID);
	rs->free_rid(canvasItemRID);
}

void PixelpartParticleRenderer2D::draw(Node2D* parentNode,
	pixelpart::ParticleCollection::ReadPtr particles, std::uint32_t particleCount,
	const pixelpart::RuntimeContext& runtimeContext, pixelpart::float2_t scale) {
	const pixelpart::ParticleEmitter& particleEmitter = effect.sceneGraph().at<pixelpart::ParticleEmitter>(particleEmitterId);
	const pixelpart::ParticleType& particleType = effect.particleTypes().at(particleTypeId);

	RenderingServer* rs = RenderingServer::get_singleton();
	rs->mesh_clear(meshRID);

	if(!parentNode->is_visible() || !particleType.visible() || particleType.renderer() == pixelpart::ParticleRendererType::mesh) {
		return;
	}

	for(const auto& textureParameterEntry : textureResourceIds) {
		if(resources.textures().count(textureParameterEntry.second) == 0) {
			continue;
		}

		rs->material_set_param(materialRID,
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
				rs->material_set_param(materialRID, parameterName, pxpt_to_gd(parameterValue.valueInt()));
				break;
			case pixelpart::VariantParameter::Value::type_float:
				rs->material_set_param(materialRID, parameterName, pxpt_to_gd(parameterValue.valueFloat()));
				break;
			case pixelpart::VariantParameter::Value::type_float2:
				rs->material_set_param(materialRID, parameterName, pxpt_to_gd(parameterValue.valueFloat2()));
				break;
			case pixelpart::VariantParameter::Value::type_float3:
				rs->material_set_param(materialRID, parameterName, pxpt_to_gd(parameterValue.valueFloat3()));
				break;
			case pixelpart::VariantParameter::Value::type_float4:
				rs->material_set_param(materialRID, parameterName, pxpt_to_gd(parameterValue.valueFloat4()));
				break;
			case pixelpart::VariantParameter::Value::type_bool:
				rs->material_set_param(materialRID, parameterName, parameterValue.valueBool());
				break;
			case pixelpart::VariantParameter::Value::type_enum:
				rs->material_set_param(materialRID, parameterName, pxpt_to_gd(parameterValue.valueEnum()));
				break;
			case pixelpart::VariantParameter::Value::type_color:
				rs->material_set_param(materialRID, parameterName, pxpt_to_gd_color(parameterValue.valueColor()));
				break;
			case pixelpart::VariantParameter::Value::type_resource_image:
				if(resources.textures().count(parameterValue.valueResourceId()) != 0) {
					rs->material_set_param(materialRID, parameterName, resources.textures().at(parameterValue.valueResourceId())->get_rid());
				}
				break;
			default:
				break;
		}
	}

	rs->material_set_param(materialRID, "u_EffectTime", static_cast<float>(runtimeContext.time()));
	rs->material_set_param(materialRID, "u_ObjectTime", static_cast<float>(particleEmitter.timeSinceStart(runtimeContext)));

	rs->canvas_item_set_parent(canvasItemRID, parentNode->get_canvas_item());
	rs->canvas_item_set_transform(canvasItemRID, parentNode->get_global_transform().affine_inverse());
	rs->canvas_item_set_visibility_layer(canvasItemRID, parentNode->get_visibility_layer());

	pixelpart::SceneContext sceneContext;
	sceneContext.effectScale = pixelpart::float3_t(scale.x, scale.y, 1.0);

	pixelpart::VertexDataBufferDimensions bufferDimensions = vertexGenerator->buildGeometry(
		particles, particleCount,
		runtimeContext, sceneContext);
	if(std::any_of(bufferDimensions.begin(), bufferDimensions.end(), [](std::uint32_t c) { return c == 0; })) {
		return;
	}

	indexArray.resize(static_cast<std::int64_t>(bufferDimensions[0]));
	vertexArray.resize(static_cast<std::int64_t>(bufferDimensions[1]));
	uvArray.resize(static_cast<std::int64_t>(bufferDimensions[2]));
	colorArray.resize(static_cast<std::int64_t>(bufferDimensions[3]));
	custom0Array.resize(static_cast<std::int64_t>(bufferDimensions[4] * 4));

	vertexGenerator->generateVertexData(pixelpart::VertexDataBufferCollection({
			reinterpret_cast<std::uint8_t*>(indexArray.ptrw()),
			reinterpret_cast<std::uint8_t*>(vertexArray.ptrw()),
			reinterpret_cast<std::uint8_t*>(uvArray.ptrw()),
			reinterpret_cast<std::uint8_t*>(colorArray.ptrw()),
			reinterpret_cast<std::uint8_t*>(custom0Array.ptrw())
		}),
		particles, particleCount,
		runtimeContext, sceneContext);

	Array arrays;
	arrays.resize(Mesh::ARRAY_MAX);
	arrays[Mesh::ARRAY_VERTEX] = vertexArray;
	arrays[Mesh::ARRAY_COLOR] = colorArray;
	arrays[Mesh::ARRAY_TEX_UV] = uvArray;
	arrays[Mesh::ARRAY_CUSTOM0] = custom0Array;
	arrays[Mesh::ARRAY_INDEX] = indexArray;

	rs->mesh_add_surface_from_arrays(meshRID, RenderingServer::PRIMITIVE_TRIANGLES, arrays, Array(), Dictionary(),
		Mesh::ARRAY_FORMAT_VERTEX | Mesh::ARRAY_FORMAT_COLOR | Mesh::ARRAY_FORMAT_TEX_UV |
		Mesh::ARRAY_FORMAT_CUSTOM0 | Mesh::ARRAY_FORMAT_INDEX |
		(Mesh::ARRAY_CUSTOM_RGBA_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT));
}
}
