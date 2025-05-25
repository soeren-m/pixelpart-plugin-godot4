#include "PixelpartParticleRenderer2D.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <pixelpart-runtime/common/VariantParameter.h>
#include <pixelpart-runtime/effect/MaterialResource.h>
#include <pixelpart-runtime/shadergraph/ShaderGraph.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <pixelpart-runtime/glm/gtx/vector_angle.hpp>
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
		for(uint32_t samplerIndex = 0; samplerIndex < shaderBuildResult.textureResourceIds.size(); samplerIndex++) {
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
}
PixelpartParticleRenderer2D::~PixelpartParticleRenderer2D() {
	RenderingServer* rs = RenderingServer::get_singleton();

	rs->free_rid(materialRID);
	rs->free_rid(meshRID);
	rs->free_rid(canvasItemRID);
}

void PixelpartParticleRenderer2D::draw(Node2D* parentNode,
	pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
	const pixelpart::RuntimeContext& runtimeContext, pixelpart::float2_t scale) {
	const pixelpart::ParticleEmitter& particleEmitter = effect.sceneGraph().at<pixelpart::ParticleEmitter>(particleEmitterId);
	const pixelpart::ParticleType& particleType = effect.particleTypes().at(particleTypeId);

	RenderingServer* rs = RenderingServer::get_singleton();
	rs->mesh_clear(meshRID);

	if(!parentNode->is_visible() || !particleType.visible()) {
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

	switch(particleType.renderer()) {
		case pixelpart::ParticleRendererType::sprite:
			add_particle_sprites(
				particleEmitter, particleType,
				particles, particleCount,
				runtimeContext, scale);
			break;
		case pixelpart::ParticleRendererType::trail:
			add_particle_trails(
				particleEmitter, particleType,
				particles, particleCount,
				runtimeContext, scale);
			break;
		default:
			break;
	}
}

void PixelpartParticleRenderer2D::add_particle_sprites(
	const pixelpart::ParticleEmitter& particleEmitter,
	const pixelpart::ParticleType& particleType,
	pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
	const pixelpart::RuntimeContext& runtimeContext, pixelpart::float2_t scale) {
	if(particleCount == 0) {
		return;
	}

	pixelpart::float_t alpha = particleEmitter.life(runtimeContext);

	indexArray.resize(particleCount * 6);
	vertexArray.resize(particleCount * 4);
	uvArray.resize(particleCount * 4);
	colorArray.resize(particleCount * 4);
	custom0Array.resize(particleCount * 4 * 4);

	int32_t* indices = indexArray.ptrw();
	for(int32_t p = 0; p < static_cast<int32_t>(particleCount); p++) {
		indices[p * 6 + 0] = p * 4 + 0;
		indices[p * 6 + 1] = p * 4 + 1;
		indices[p * 6 + 2] = p * 4 + 3;
		indices[p * 6 + 3] = p * 4 + 1;
		indices[p * 6 + 4] = p * 4 + 2;
		indices[p * 6 + 5] = p * 4 + 3;
	}

	Vector2* positions = vertexArray.ptrw();
	for(uint32_t p = 0; p < particleCount; p++) {
		pixelpart::float3_t worldPosition[4];

		switch(particleType.alignmentMode()) {
			case pixelpart::AlignmentMode::motion: {
				pixelpart::float_t angle = glm::degrees(glm::orientedAngle(pixelpart::float2_t(0.0, 1.0), (particles.velocity[p] != pixelpart::float3_t(0.0))
					? pixelpart::float2_t(glm::normalize(particles.velocity[p]))
					: pixelpart::float2_t(0.0, 1.0)));
				worldPosition[0] = particles.globalPosition[p] + rotate_2d(pixelpart::float3_t(-0.5, -0.5, 0.0) * particles.size[p], particleType.pivot().value() * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[1] = particles.globalPosition[p] + rotate_2d(pixelpart::float3_t(-0.5, +0.5, 0.0) * particles.size[p], particleType.pivot().value() * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[2] = particles.globalPosition[p] + rotate_2d(pixelpart::float3_t(+0.5, +0.5, 0.0) * particles.size[p], particleType.pivot().value() * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[3] = particles.globalPosition[p] + rotate_2d(pixelpart::float3_t(+0.5, -0.5, 0.0) * particles.size[p], particleType.pivot().value() * particles.size[p], particles.rotation[p].x + angle);
				break;
			}
			case pixelpart::AlignmentMode::emission: {
				pixelpart::float3_t emissionDirection = particleEmitter.position().at(alpha) - particles.globalPosition[p];
				pixelpart::float_t angle = glm::degrees(glm::orientedAngle(pixelpart::float2_t(0.0, 1.0), (emissionDirection != pixelpart::float3_t(0.0))
					? pixelpart::float2_t(glm::normalize(emissionDirection))
					: pixelpart::float2_t(0.0, 1.0)));
				worldPosition[0] = particles.globalPosition[p] + rotate_2d(pixelpart::float3_t(-0.5, -0.5, 0.0) * particles.size[p], particleType.pivot().value() * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[1] = particles.globalPosition[p] + rotate_2d(pixelpart::float3_t(-0.5, +0.5, 0.0) * particles.size[p], particleType.pivot().value() * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[2] = particles.globalPosition[p] + rotate_2d(pixelpart::float3_t(+0.5, +0.5, 0.0) * particles.size[p], particleType.pivot().value() * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[3] = particles.globalPosition[p] + rotate_2d(pixelpart::float3_t(+0.5, -0.5, 0.0) * particles.size[p], particleType.pivot().value() * particles.size[p], particles.rotation[p].x + angle);
				break;
			}
			case pixelpart::AlignmentMode::emitter: {
				pixelpart::mat3_t rotationMatrix = rotation_3d(particles.rotation[p]);
				pixelpart::mat3_t alignmentMatrix = rotation_3d(particleEmitter.rotation().at(alpha));
				worldPosition[0] = particles.globalPosition[p] + alignmentMatrix * (rotationMatrix * (pixelpart::float3_t(-0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot().value() * particles.size[p]) + particleType.pivot().value() * particles.size[p]);
				worldPosition[1] = particles.globalPosition[p] + alignmentMatrix * (rotationMatrix * (pixelpart::float3_t(-0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot().value() * particles.size[p]) + particleType.pivot().value() * particles.size[p]);
				worldPosition[2] = particles.globalPosition[p] + alignmentMatrix * (rotationMatrix * (pixelpart::float3_t(+0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot().value() * particles.size[p]) + particleType.pivot().value() * particles.size[p]);
				worldPosition[3] = particles.globalPosition[p] + alignmentMatrix * (rotationMatrix * (pixelpart::float3_t(+0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot().value() * particles.size[p]) + particleType.pivot().value() * particles.size[p]);
				break;
			}
			default: {
				pixelpart::mat3_t rotationMatrix = rotation_3d(particles.rotation[p]);
				worldPosition[0] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::float3_t(-0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot().value() * particles.size[p]) + particleType.pivot().value() * particles.size[p]);
				worldPosition[1] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::float3_t(-0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot().value() * particles.size[p]) + particleType.pivot().value() * particles.size[p]);
				worldPosition[2] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::float3_t(+0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot().value() * particles.size[p]) + particleType.pivot().value() * particles.size[p]);
				worldPosition[3] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::float3_t(+0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot().value() * particles.size[p]) + particleType.pivot().value() * particles.size[p]);
				break;
			}
		}

		positions[p * 4 + 0] = pxpt_to_gd(pixelpart::float2_t(worldPosition[0]) * scale);
		positions[p * 4 + 1] = pxpt_to_gd(pixelpart::float2_t(worldPosition[1]) * scale);
		positions[p * 4 + 2] = pxpt_to_gd(pixelpart::float2_t(worldPosition[2]) * scale);
		positions[p * 4 + 3] = pxpt_to_gd(pixelpart::float2_t(worldPosition[3]) * scale);
	}

	float* uvs = reinterpret_cast<float*>(uvArray.ptrw());
	for(uint32_t p = 0; p < particleCount; p++) {
		uvs[p * 4 * 2 + 0] = 0.0f;
		uvs[p * 4 * 2 + 1] = 0.0f;
		uvs[p * 4 * 2 + 2] = 0.0f;
		uvs[p * 4 * 2 + 3] = 1.0f;
		uvs[p * 4 * 2 + 4] = 1.0f;
		uvs[p * 4 * 2 + 5] = 1.0f;
		uvs[p * 4 * 2 + 6] = 1.0f;
		uvs[p * 4 * 2 + 7] = 0.0f;
	}

	Color* colors = colorArray.ptrw();
	for(uint32_t p = 0; p < particleCount; p++) {
		colors[p * 4 + 0] = pxpt_to_gd_color(particles.color[p]);
		colors[p * 4 + 1] = pxpt_to_gd_color(particles.color[p]);
		colors[p * 4 + 2] = pxpt_to_gd_color(particles.color[p]);
		colors[p * 4 + 3] = pxpt_to_gd_color(particles.color[p]);
	}

	float* custom0 = custom0Array.ptrw();
	for(uint32_t p = 0; p < particleCount; p++) {
		custom0[p * 4 * 4 + 0] = static_cast<float>(particles.velocity[p].x);
		custom0[p * 4 * 4 + 1] = static_cast<float>(particles.velocity[p].y);
		custom0[p * 4 * 4 + 2] = static_cast<float>(particles.life[p]);
		custom0[p * 4 * 4 + 3] = static_cast<float>(particles.id[p]);
		custom0[p * 4 * 4 + 4] = static_cast<float>(particles.velocity[p].x);
		custom0[p * 4 * 4 + 5] = static_cast<float>(particles.velocity[p].y);
		custom0[p * 4 * 4 + 6] = static_cast<float>(particles.life[p]);
		custom0[p * 4 * 4 + 7] = static_cast<float>(particles.id[p]);
		custom0[p * 4 * 4 + 8] = static_cast<float>(particles.velocity[p].x);
		custom0[p * 4 * 4 + 9] = static_cast<float>(particles.velocity[p].y);
		custom0[p * 4 * 4 + 10] = static_cast<float>(particles.life[p]);
		custom0[p * 4 * 4 + 11] = static_cast<float>(particles.id[p]);
		custom0[p * 4 * 4 + 12] = static_cast<float>(particles.velocity[p].x);
		custom0[p * 4 * 4 + 13] = static_cast<float>(particles.velocity[p].y);
		custom0[p * 4 * 4 + 14] = static_cast<float>(particles.life[p]);
		custom0[p * 4 * 4 + 15] = static_cast<float>(particles.id[p]);
	}

	Array arrays;
	arrays.resize(Mesh::ARRAY_MAX);
	arrays[Mesh::ARRAY_VERTEX] = vertexArray;
	arrays[Mesh::ARRAY_COLOR] = colorArray;
	arrays[Mesh::ARRAY_TEX_UV] = uvArray;
	arrays[Mesh::ARRAY_CUSTOM0] = custom0Array;
	arrays[Mesh::ARRAY_INDEX] = indexArray;

	RenderingServer* rs = RenderingServer::get_singleton();

	rs->mesh_add_surface_from_arrays(meshRID, RenderingServer::PRIMITIVE_TRIANGLES, arrays, Array(), Dictionary(),
		Mesh::ARRAY_FORMAT_VERTEX | Mesh::ARRAY_FORMAT_COLOR | Mesh::ARRAY_FORMAT_TEX_UV |
		Mesh::ARRAY_FORMAT_CUSTOM0 | Mesh::ARRAY_FORMAT_INDEX |
		(Mesh::ARRAY_CUSTOM_RGBA_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT));
}

void PixelpartParticleRenderer2D::add_particle_trails(
	const pixelpart::ParticleEmitter& particleEmitter,
	const pixelpart::ParticleType& particleType,
	pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
	const pixelpart::RuntimeContext& runtimeContext, pixelpart::float2_t scale) {
	const pixelpart::float_t epsilon = 0.000001;
	if(particleCount < 2) {
		return;
	}

	std::vector<uint32_t> sortKeys(particleCount);
	std::iota(sortKeys.begin(), sortKeys.end(), 0);
	std::sort(sortKeys.begin(), sortKeys.end(), [&particles](uint32_t p0, uint32_t p1) {
		return (particles.parentId[p0] != particles.parentId[p1])
			? particles.parentId[p0] > particles.parentId[p1]
			: particles.id[p0] > particles.id[p1];
	});

	for(auto& entry : trails) {
		entry.second.particleCount = 0;
	}

	ParticleTrailData* currentTrail = nullptr;
	for(uint32_t p = 0, particleParentId = 0, trailId = 0xFFFFFFFE; p < particleCount; p++) {
		particleParentId = particles.parentId[sortKeys[p]];

		if(particleParentId != trailId) {
			trailId = particleParentId;
			currentTrail = &trails[trailId];
		}

		currentTrail->particleCount++;
	}

	for(auto it = trails.begin(); it != trails.end(); ) {
		if(it->second.particleCount == 0) {
			it = trails.erase(it);
		}
		else {
			it++;
		}
	}

	currentTrail = nullptr;

	switch(particleType.trailRendererSettings().smoothingMethod) {
		case pixelpart::ParticleTrailRendererSettings::SmoothingMethod::spline: {
			for(uint32_t p = 0, particleParentId = 0, trailId = 0xFFFFFFFE; p < particleCount; p++) {
				uint32_t particleIndex = sortKeys[p];
				particleParentId = particles.parentId[particleIndex];

				if(particleParentId != trailId) {
					trailId = particleParentId;
					currentTrail = &trails.at(trailId);

					std::vector<pixelpart::float_t> relativeParticlePosition(currentTrail->particleCount);
					relativeParticlePosition[0] = 0.0;

					currentTrail->length = 0.0;
					for(uint32_t q = 1; q < currentTrail->particleCount; q++) {
						currentTrail->length += glm::length(particles.globalPosition[sortKeys[p + q]] - particles.globalPosition[sortKeys[p + q - 1]]);
						relativeParticlePosition[q] = currentTrail->length;
					}
					for(uint32_t q = 1; q < currentTrail->particleCount; q++) {
						relativeParticlePosition[q] /= currentTrail->length;
					}

					pixelpart::Curve<pixelpart::float3_t> positionCurve(pixelpart::CurveInterpolation::spline);
					positionCurve.enableFixedCache(particleType.trailRendererSettings().smoothingSegmentCount);
					positionCurve.points(ordered_points(relativeParticlePosition.data(), particles.globalPosition, sortKeys.data() + p, currentTrail->particleCount));

					pixelpart::Curve<pixelpart::float3_t> sizeCurve(pixelpart::CurveInterpolation::spline);
					sizeCurve.enableFixedCache(particleType.trailRendererSettings().smoothingSegmentCount);
					sizeCurve.points(ordered_points(relativeParticlePosition.data(), particles.size, sortKeys.data() + p, currentTrail->particleCount));

					pixelpart::Curve<pixelpart::float4_t> colorCurve(pixelpart::CurveInterpolation::spline);
					colorCurve.enableFixedCache(particleType.trailRendererSettings().smoothingSegmentCount);
					colorCurve.points(ordered_points(relativeParticlePosition.data(), particles.color, sortKeys.data() + p, currentTrail->particleCount));

					pixelpart::Curve<pixelpart::float3_t> velocityCurve(pixelpart::CurveInterpolation::spline);
					velocityCurve.enableFixedCache(particleType.trailRendererSettings().smoothingSegmentCount);
					velocityCurve.points(ordered_points(relativeParticlePosition.data(), particles.velocity, sortKeys.data() + p, currentTrail->particleCount));

					pixelpart::Curve<pixelpart::float_t> lifeCurve(pixelpart::CurveInterpolation::spline);
					lifeCurve.enableFixedCache(particleType.trailRendererSettings().smoothingSegmentCount);
					lifeCurve.points(ordered_points(relativeParticlePosition.data(), particles.life, sortKeys.data() + p, currentTrail->particleCount));

					currentTrail->position = positionCurve.cache();
					currentTrail->size = sizeCurve.cache();
					currentTrail->color = colorCurve.cache();
					currentTrail->velocity = velocityCurve.cache();
					currentTrail->life = lifeCurve.cache();
					currentTrail->direction.resize(particleType.trailRendererSettings().smoothingSegmentCount);
					currentTrail->index.resize(particleType.trailRendererSettings().smoothingSegmentCount);
				}
			}

			break;
		}

		default: {
			for(uint32_t p = 0, q = 0, particleParentId = 0, trailId = 0xFFFFFFFE; p < particleCount; p++, q++) {
				uint32_t particleIndex = sortKeys[p];
				particleParentId = particles.parentId[particleIndex];

				if(particleParentId != trailId) {
					trailId = particleParentId;
					q = 0;

					currentTrail = &trails.at(trailId);
					currentTrail->position.resize(currentTrail->particleCount);
					currentTrail->size.resize(currentTrail->particleCount);
					currentTrail->color.resize(currentTrail->particleCount);
					currentTrail->velocity.resize(currentTrail->particleCount);
					currentTrail->life.resize(currentTrail->particleCount);
					currentTrail->direction.resize(currentTrail->particleCount);
					currentTrail->index.resize(currentTrail->particleCount);
				}

				currentTrail->position[q] = particles.globalPosition[particleIndex];
				currentTrail->size[q] = particles.size[particleIndex];
				currentTrail->color[q] = particles.color[particleIndex];
				currentTrail->velocity[q] = particles.velocity[particleIndex];
				currentTrail->life[q] = particles.life[particleIndex];
			}

			break;
		}
	}

	RenderingServer* rs = RenderingServer::get_singleton();

	for(auto& entry : trails) {
		ParticleTrailData& trail = entry.second;
		trail.length = 0.0;

		if(trail.position.size() < 2) {
			continue;
		}

		std::size_t last = trail.position.size() - 1;
		for(std::size_t i = 0; i < last; i++) {
			pixelpart::float3_t toNext = trail.position[i + 1] - trail.position[i];
			pixelpart::float_t distance = std::max(glm::length(toNext), epsilon);

			trail.direction[i] = toNext / distance;
			trail.index[i] = trail.length;
			trail.length += distance;
		}

		trail.direction[last] = trail.direction[last - 1];
		trail.index[last] = trail.length;

		for(std::size_t i = last; i > 0; i--) {
			pixelpart::float3_t toEdge = trail.direction[i] - trail.direction[i - 1];
			pixelpart::float_t toEdgeLength = glm::length(toEdge);
			if(toEdgeLength < epsilon) {
				trail.direction[i] = pixelpart::float3_t(
					-trail.direction[i].y,
					trail.direction[i].x,
					trail.direction[i].z);
			}
			else {
				trail.direction[i] = (glm::dot(glm::cross(trail.direction[i], pixelpart::float3_t(0.0, 0.0, 1.0)), toEdge / toEdgeLength) < 0.0)
					? +toEdge / toEdgeLength
					: -toEdge / toEdgeLength;
			}
		}

		trail.direction[0] = trail.direction[1];

		for(pixelpart::float_t& index : trail.index) {
			index /= trail.length;
		}

		int32_t trailSegmentCount = static_cast<int32_t>(trail.position.size()) - 1;
		trail.indexArray.resize(trailSegmentCount * 12);
		trail.vertexArray.resize(trailSegmentCount * 5);
		trail.uvArray.resize(trailSegmentCount * 5);
		trail.colorArray.resize(trailSegmentCount * 5);
		trail.custom0Array.resize(trailSegmentCount * 5 * 4);

		int32_t* indices = trail.indexArray.ptrw();
		for(int32_t p = 0; p < trailSegmentCount; p++) {
			indices[p * 12 + 0] = p * 5 + 0;
			indices[p * 12 + 1] = p * 5 + 1;
			indices[p * 12 + 2] = p * 5 + 4;
			indices[p * 12 + 3] = p * 5 + 2;
			indices[p * 12 + 4] = p * 5 + 0;
			indices[p * 12 + 5] = p * 5 + 4;
			indices[p * 12 + 6] = p * 5 + 1;
			indices[p * 12 + 7] = p * 5 + 3;
			indices[p * 12 + 8] = p * 5 + 4;
			indices[p * 12 + 9] = p * 5 + 3;
			indices[p * 12 + 10] = p * 5 + 2;
			indices[p * 12 + 11] = p * 5 + 4;
		}

		Vector2* positions = trail.vertexArray.ptrw();
		for(int32_t p = 0; p < trailSegmentCount; p++) {
			pixelpart::float3_t startToEdge = trail.direction[p] * std::max(trail.size[p].x, trail.size[p].y) * 0.5;
			pixelpart::float3_t endToEdge = trail.direction[p + 1] * std::max(trail.size[p + 1].x, trail.size[p + 1].y) * 0.5;

			pixelpart::float3_t vertexPositions[5] = {
				trail.position[p] + startToEdge,
				trail.position[p] - startToEdge,
				trail.position[p + 1] + endToEdge,
				trail.position[p + 1] - endToEdge,
				(trail.position[p] + trail.position[p + 1]) * 0.5
			};

			positions[p * 5 + 0] = pxpt_to_gd(pixelpart::float2_t(vertexPositions[0]) * scale);
			positions[p * 5 + 1] = pxpt_to_gd(pixelpart::float2_t(vertexPositions[1]) * scale);
			positions[p * 5 + 2] = pxpt_to_gd(pixelpart::float2_t(vertexPositions[2]) * scale);
			positions[p * 5 + 3] = pxpt_to_gd(pixelpart::float2_t(vertexPositions[3]) * scale);
			positions[p * 5 + 4] = pxpt_to_gd(pixelpart::float2_t(vertexPositions[4]) * scale);
		}

		float* uvs = reinterpret_cast<float*>(trail.uvArray.ptrw());
		switch(particleType.trailRendererSettings().textureRotation) {
			case pixelpart::ParticleTrailRendererSettings::TextureRotation::left:
				for(int32_t p = 0; p < trailSegmentCount; p++) {
					uvs[p * 5 * 2 + 0] = pxpt_to_gd(trail.index[p] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 1] = 1.0f;
					uvs[p * 5 * 2 + 2] = pxpt_to_gd(trail.index[p] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 3] = 0.0f;
					uvs[p * 5 * 2 + 4] = pxpt_to_gd(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 5] = 1.0f;
					uvs[p * 5 * 2 + 6] = pxpt_to_gd(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 7] = 0.0f;
					uvs[p * 5 * 2 + 8] = pxpt_to_gd((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 9] = 0.5f;
				}
				break;

			case pixelpart::ParticleTrailRendererSettings::TextureRotation::down:
				for(int32_t p = 0; p < trailSegmentCount; p++) {
					uvs[p * 5 * 2 + 0] = 1.0f;
					uvs[p * 5 * 2 + 1] = 1.0f - pxpt_to_gd(trail.index[p] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 2] = 0.0f;
					uvs[p * 5 * 2 + 3] = 1.0f - pxpt_to_gd(trail.index[p] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 4] = 1.0f;
					uvs[p * 5 * 2 + 5] = 1.0f - pxpt_to_gd(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 6] = 0.0f;
					uvs[p * 5 * 2 + 7] = 1.0f - pxpt_to_gd(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 8] = 0.5f;
					uvs[p * 5 * 2 + 9] = 1.0f - pxpt_to_gd((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings().textureUVFactor);
				}
				break;

			case pixelpart::ParticleTrailRendererSettings::TextureRotation::right:
				for(int32_t p = 0; p < trailSegmentCount; p++) {
					uvs[p * 5 * 2 + 0] = 1.0f - pxpt_to_gd(trail.index[p] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 1] = 0.0f;
					uvs[p * 5 * 2 + 2] = 1.0f - pxpt_to_gd(trail.index[p] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 3] = 1.0f;
					uvs[p * 5 * 2 + 4] = 1.0f - pxpt_to_gd(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 5] = 0.0f;
					uvs[p * 5 * 2 + 6] = 1.0f - pxpt_to_gd(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 7] = 1.0f;
					uvs[p * 5 * 2 + 8] = 1.0f - pxpt_to_gd((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 9] = 0.5f;
				}
				break;

			default:
				for(int32_t p = 0; p < trailSegmentCount; p++) {
					uvs[p * 5 * 2 + 0] = 0.0f;
					uvs[p * 5 * 2 + 1] = pxpt_to_gd(trail.index[p] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 2] = 1.0f;
					uvs[p * 5 * 2 + 3] = pxpt_to_gd(trail.index[p] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 4] = 0.0f;
					uvs[p * 5 * 2 + 5] = pxpt_to_gd(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 6] = 1.0f;
					uvs[p * 5 * 2 + 7] = pxpt_to_gd(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor);
					uvs[p * 5 * 2 + 8] = 0.5f;
					uvs[p * 5 * 2 + 9] = pxpt_to_gd((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings().textureUVFactor);
				}
				break;
		}

		Color* colors = trail.colorArray.ptrw();
		for(uint32_t p = 0; p < trailSegmentCount; p++) {
			colors[p * 5 + 0] = pxpt_to_gd_color(trail.color[p]);
			colors[p * 5 + 1] = pxpt_to_gd_color(trail.color[p]);
			colors[p * 5 + 2] = pxpt_to_gd_color(trail.color[p + 1]);
			colors[p * 5 + 3] = pxpt_to_gd_color(trail.color[p + 1]);
			colors[p * 5 + 4] = pxpt_to_gd_color((trail.color[p] + trail.color[p + 1]) * 0.5);
		}

		float* custom0 = trail.custom0Array.ptrw();
		for(uint32_t p = 0; p < trailSegmentCount; p++) {
			custom0[p * 5 * 4 + 0] = static_cast<float>(trail.velocity[p].x);
			custom0[p * 5 * 4 + 1] = static_cast<float>(trail.velocity[p].y);
			custom0[p * 5 * 4 + 2] = static_cast<float>(trail.life[p]);
			custom0[p * 5 * 4 + 3] = static_cast<float>(entry.first);
			custom0[p * 5 * 4 + 4] = static_cast<float>(trail.velocity[p].x);
			custom0[p * 5 * 4 + 5] = static_cast<float>(trail.velocity[p].y);
			custom0[p * 5 * 4 + 6] = static_cast<float>(trail.life[p]);
			custom0[p * 5 * 4 + 7] = static_cast<float>(entry.first);
			custom0[p * 5 * 4 + 8] = static_cast<float>(trail.velocity[p + 1].x);
			custom0[p * 5 * 4 + 9] = static_cast<float>(trail.velocity[p + 1].y);
			custom0[p * 5 * 4 + 10] = static_cast<float>(trail.life[p + 1]);
			custom0[p * 5 * 4 + 11] = static_cast<float>(entry.first);
			custom0[p * 5 * 4 + 12] = static_cast<float>(trail.velocity[p + 1].x);
			custom0[p * 5 * 4 + 13] = static_cast<float>(trail.velocity[p + 1].y);
			custom0[p * 5 * 4 + 14] = static_cast<float>(trail.life[p + 1]);
			custom0[p * 5 * 4 + 15] = static_cast<float>(entry.first);
			custom0[p * 5 * 4 + 16] = static_cast<float>(trail.velocity[p].x + trail.velocity[p + 1].x) * 0.5f;
			custom0[p * 5 * 4 + 17] = static_cast<float>(trail.velocity[p].y + trail.velocity[p + 1].y) * 0.5f;
			custom0[p * 5 * 4 + 18] = static_cast<float>(trail.life[p] + trail.life[p + 1]) * 0.5f;
			custom0[p * 5 * 4 + 19] = static_cast<float>(entry.first);
		}

		Array arrays;
		arrays.resize(Mesh::ARRAY_MAX);
		arrays[Mesh::ARRAY_VERTEX] = trail.vertexArray;
		arrays[Mesh::ARRAY_COLOR] = trail.colorArray;
		arrays[Mesh::ARRAY_TEX_UV] = trail.uvArray;
		arrays[Mesh::ARRAY_CUSTOM0] = trail.custom0Array;
		arrays[Mesh::ARRAY_INDEX] = trail.indexArray;

		rs->mesh_add_surface_from_arrays(meshRID, RenderingServer::PRIMITIVE_TRIANGLES, arrays, Array(), Dictionary(),
			Mesh::ARRAY_FORMAT_VERTEX | Mesh::ARRAY_FORMAT_COLOR | Mesh::ARRAY_FORMAT_TEX_UV |
			Mesh::ARRAY_FORMAT_CUSTOM0 | Mesh::ARRAY_FORMAT_INDEX |
			(Mesh::ARRAY_CUSTOM_RGBA_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT));
	}
}

pixelpart::float3_t PixelpartParticleRenderer2D::rotate_2d(const pixelpart::float3_t& p, const pixelpart::float3_t& o, pixelpart::float_t a) {
	pixelpart::float_t s = std::sin(glm::radians(a));
	pixelpart::float_t c = std::cos(glm::radians(a));

	return pixelpart::float3_t(
		(p.x - o.x) * c - (p.y - o.y) * s + o.x,
		(p.x - o.x) * s + (p.y - o.y) * c + o.y,
		0.0);
}
pixelpart::mat3_t PixelpartParticleRenderer2D::rotation_3d(const pixelpart::float3_t& angle) {
	pixelpart::float3_t rotation = glm::radians(angle);
	pixelpart::float_t cy = std::cos(rotation.y);
	pixelpart::float_t sy = std::sin(rotation.y);
	pixelpart::float_t cp = std::cos(rotation.z);
	pixelpart::float_t sp = std::sin(rotation.z);
	pixelpart::float_t cr = std::cos(rotation.x);
	pixelpart::float_t sr = std::sin(rotation.x);

	return pixelpart::mat3_t(
		pixelpart::float3_t(cy * cr + sy * sp * sr, sr * cp, -sy * cr + cy * sp * sr),
		pixelpart::float3_t(-cy * sr + sy * sp * cr, cr * cp, sr * sy + cy * sp * cr),
		pixelpart::float3_t(sy * cp, -sp, cy * cp));
}
}