#include "PixelpartParticleCanvasItem.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <pixelpart-runtime/common/VariantParameter.h>
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
PixelpartParticleCanvasItem::PixelpartParticleCanvasItem(Node2D* parent,
	const pixelpart::Effect& eff, pixelpart::id_t emitterId, pixelpart::id_t typeId,
	PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider,
	std::shared_ptr<pixelpart::ThreadPool> threadPool) :
	parentNode(parent), effect(eff), particleEmitterId(emitterId), particleTypeId(typeId) {
	create_material(resourceProvider, shaderProvider);
	create_canvas_item();
	create_vertex_generator(threadPool);
}
PixelpartParticleCanvasItem::~PixelpartParticleCanvasItem() {
	RenderingServer* rs = RenderingServer::get_singleton();

	rs->free_rid(canvasItemRid);
	rs->free_rid(meshRid);
	rs->free_rid(materialRid);
}

void PixelpartParticleCanvasItem::draw(const pixelpart::ParticleCollection& particles,
	const pixelpart::EffectRuntimeContext& runtimeContext, pixelpart::float2_t scale) {
	RenderingServer* rs = RenderingServer::get_singleton();
	const pixelpart::ParticleEmitter& particleEmitter = effect.sceneGraph().at<pixelpart::ParticleEmitter>(particleEmitterId);
	const pixelpart::ParticleType& particleType = effect.particleTypes().at(particleTypeId);

	rs->mesh_clear(meshRid);

	if(!parentNode->is_visible() || !particleType.visible() || particleType.renderer() == pixelpart::ParticleRendererType::mesh) {
		return;
	}

	rs->material_set_param(materialRid, "u_EffectTime", static_cast<float>(runtimeContext.time()));
	rs->material_set_param(materialRid, "u_ObjectTime", static_cast<float>(particleEmitter.timeSinceStart(runtimeContext)));

	rs->canvas_item_set_transform(canvasItemRid, parentNode->get_global_transform().affine_inverse());
	rs->canvas_item_set_visibility_layer(canvasItemRid, parentNode->get_visibility_layer());
	rs->canvas_item_set_self_modulate(canvasItemRid, parentNode->get_self_modulate());
	rs->canvas_item_set_light_mask(canvasItemRid, parentNode->get_light_mask());

	pixelpart::SceneContext sceneContext;
	sceneContext.effectScale = pixelpart::float3_t(scale.x, scale.y, 1.0);

	pixelpart::VertexDataBufferDimensions bufferDimensions = vertexGenerator->buildGeometry(
		particles.readPtr(), particles.count(),
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
		particles.readPtr(), particles.count(),
		runtimeContext, sceneContext);

	Array arrays;
	arrays.resize(Mesh::ARRAY_MAX);
	arrays[Mesh::ARRAY_VERTEX] = vertexArray;
	arrays[Mesh::ARRAY_COLOR] = colorArray;
	arrays[Mesh::ARRAY_TEX_UV] = uvArray;
	arrays[Mesh::ARRAY_CUSTOM0] = custom0Array;
	arrays[Mesh::ARRAY_INDEX] = indexArray;

	rs->mesh_add_surface_from_arrays(meshRid, RenderingServer::PRIMITIVE_TRIANGLES, arrays, Array(), Dictionary(),
		Mesh::ARRAY_FORMAT_VERTEX | Mesh::ARRAY_FORMAT_COLOR | Mesh::ARRAY_FORMAT_TEX_UV |
		Mesh::ARRAY_FORMAT_CUSTOM0 | Mesh::ARRAY_FORMAT_INDEX |
		(Mesh::ARRAY_CUSTOM_RGBA_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT));
}

void PixelpartParticleCanvasItem::create_material(PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider) {
	RenderingServer* rs = RenderingServer::get_singleton();
	const pixelpart::ParticleType& particleType = effect.particleTypes().at(particleTypeId);
	const pixelpart::MaterialInstance& materialInstance = particleType.materialInstance();

	std::unordered_map<pixelpart::id_t, std::string> shaderParameterNames;
	std::unordered_map<std::string, std::string> textureResourceIds;

	if(materialInstance.builtInMaterial()) {
		shader = shaderProvider.get_builtin_canvas_item_shader(materialInstance.materialId());

		if(shader.is_valid()) {
			const PixelpartShaderProvider::ShaderMetadata& metadata = shaderProvider.get_builtin_canvas_item_shader_metadata(materialInstance.materialId());
			for(const auto& [parameterId, parameterName] : metadata.parameterNames) {
				shaderParameterNames[parameterId] = PixelpartShaderProvider::uniformPrefix + parameterName;
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

		shader = shaderProvider.get_custom_canvas_item_shader(
			shaderBuildResult.mainCode, shaderBuildResult.parameterCode,
			particleType.renderer(),
			materialResource.blendMode(),
			materialResource.lightingMode());
	}

	materialRid = rs->material_create();

	if(!shader.is_valid()) {
		UtilityFunctions::push_warning("Could not create particle material");
		return;
	}

	rs->material_set_shader(materialRid, shader->get_rid());

	for(const auto& [samplerName, resourceId] : textureResourceIds) {
		if(resourceProvider.get_textures().count(resourceId) == 0) {
			continue;
		}

		rs->material_set_param(materialRid,
			StringName(samplerName.c_str()), resourceProvider.get_textures().at(resourceId)->get_rid());
	}

	for(const auto& [parameterName, parameterValue] : particleType.materialInstance().materialParameters()) {
		if(shaderParameterNames.count(parameterName) == 0) {
			continue;
		}

		StringName shaderParameterName = StringName(shaderParameterNames.at(parameterName).c_str());

		switch(parameterValue.type()) {
			case pixelpart::VariantParameter::Value::type_int:
				rs->material_set_param(materialRid, shaderParameterName, pxpt_to_gd(parameterValue.valueInt()));
				break;
			case pixelpart::VariantParameter::Value::type_float:
				rs->material_set_param(materialRid, shaderParameterName, pxpt_to_gd(parameterValue.valueFloat()));
				break;
			case pixelpart::VariantParameter::Value::type_float2:
				rs->material_set_param(materialRid, shaderParameterName, pxpt_to_gd(parameterValue.valueFloat2()));
				break;
			case pixelpart::VariantParameter::Value::type_float3:
				rs->material_set_param(materialRid, shaderParameterName, pxpt_to_gd(parameterValue.valueFloat3()));
				break;
			case pixelpart::VariantParameter::Value::type_float4:
				rs->material_set_param(materialRid, shaderParameterName, pxpt_to_gd(parameterValue.valueFloat4()));
				break;
			case pixelpart::VariantParameter::Value::type_bool:
				rs->material_set_param(materialRid, shaderParameterName, parameterValue.valueBool());
				break;
			case pixelpart::VariantParameter::Value::type_enum:
				rs->material_set_param(materialRid, shaderParameterName, pxpt_to_gd(parameterValue.valueEnum()));
				break;
			case pixelpart::VariantParameter::Value::type_color:
				rs->material_set_param(materialRid, shaderParameterName, pxpt_to_gd_color(parameterValue.valueColor()));
				break;
			case pixelpart::VariantParameter::Value::type_resource_image:
				if(resourceProvider.get_textures().count(parameterValue.valueResourceId()) != 0) {
					rs->material_set_param(materialRid, shaderParameterName, resourceProvider.get_textures().at(parameterValue.valueResourceId())->get_rid());
				}
				break;
			default:
				break;
		}
	}
}
void PixelpartParticleCanvasItem::create_canvas_item() {
	RenderingServer* rs = RenderingServer::get_singleton();
	const pixelpart::ParticleType& particleType = effect.particleTypes().at(particleTypeId);

	canvasItemRid = rs->canvas_item_create();
	rs->canvas_item_set_parent(canvasItemRid, parentNode->get_canvas_item());

	meshRid = rs->mesh_create();
	rs->canvas_item_add_mesh(canvasItemRid, meshRid);

	rs->canvas_item_set_material(canvasItemRid, materialRid);
	rs->canvas_item_set_z_as_relative_to_parent(canvasItemRid, true);
	rs->canvas_item_set_z_index(canvasItemRid, static_cast<int>(particleType.layer()));
	rs->canvas_item_set_sort_children_by_y(canvasItemRid, false);
}
void PixelpartParticleCanvasItem::create_vertex_generator(std::shared_ptr<pixelpart::ThreadPool> threadPool) {
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
		effect, particleEmitterId, particleTypeId, vertexFormat, threadPool);
}
}
