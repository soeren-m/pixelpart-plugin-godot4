#include "PixelpartParticleRenderer3D.h"
#include "../util/PixelpartSort.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <pixelpart-runtime/common/VariantParameter.h>
#include <pixelpart-runtime/effect/MaterialResource.h>
#include <pixelpart-runtime/shadergraph/ShaderGraph.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <pixelpart-runtime/glm/gtx/norm.hpp>
#include <pixelpart-runtime/glm/gtx/transform.hpp>
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
		for(uint32_t samplerIndex = 0; samplerIndex < shaderBuildResult.textureResourceIds.size(); samplerIndex++) {
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
}
PixelpartParticleRenderer3D::~PixelpartParticleRenderer3D() {
	RenderingServer* rs = RenderingServer::get_singleton();

	rs->free_rid(instanceRID);
}

void PixelpartParticleRenderer3D::draw(Node3D* parentNode,
	pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
	const pixelpart::RuntimeContext& runtimeContext, pixelpart::float_t scale) {
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
	rs->instance_set_pivot_data(instanceRID, static_cast<double>(particleType.layer()) * 0.01, false);

	switch(particleType.renderer()) {
		case pixelpart::ParticleRendererType::sprite:
			add_particle_sprites(parentNode,
				particleEmitter, particleType,
				particles, particleCount,
				runtimeContext, scale);
			break;
		case pixelpart::ParticleRendererType::trail:
			add_particle_trails(parentNode,
				particleEmitter, particleType,
				particles, particleCount,
				runtimeContext, scale);
			break;
		case pixelpart::ParticleRendererType::mesh:
			add_particle_meshes(parentNode,
				particleEmitter, particleType,
				particles, particleCount,
				runtimeContext, scale);
			break;
		default:
			break;
	}

	switch(particleType.renderer()) {
		case pixelpart::ParticleRendererType::sprite:
		case pixelpart::ParticleRendererType::trail:
			if(arrayMesh->get_surface_count() > 0) {
				arrayMesh->surface_set_material(0, shaderMaterial);
			}
			break;
		default:
			break;
	}
}

void PixelpartParticleRenderer3D::add_particle_sprites(Node3D* parentNode,
	const pixelpart::ParticleEmitter& particleEmitter,
	const pixelpart::ParticleType& particleType,
	pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
	const pixelpart::RuntimeContext& runtimeContext, pixelpart::float_t scale) {
	if(particleCount == 0) {
		return;
	}

	pixelpart::float_t alpha = particleEmitter.life(runtimeContext);

	Camera3D* camera = parentNode->get_viewport()->get_camera_3d();
	if(!camera) {
		return;
	}

	pixelpart::ParticleCollection::ReadPtr particleRenderData = particles;
	if(particleType.spriteRendererSettings().sortCriterion != pixelpart::ParticleSortCriterion::none) {
		particleRenderData = sort_particles(particles, particleCount, particleType.spriteRendererSettings().sortCriterion, parentNode, camera);
	}

	pixelpart::float3_t cameraPosition = gd_to_pxpt(camera->get_global_transform().origin - parentNode->get_global_position());
	pixelpart::float3_t cameraRight = gd_to_pxpt(camera->get_global_transform().basis.get_column(0));
	pixelpart::float3_t cameraUp = gd_to_pxpt(camera->get_global_transform().basis.get_column(1));

	indexArray.resize(particleCount * 6);
	vertexArray.resize(particleCount * 4);
	normalArray.resize(particleCount * 4);
	uvArray.resize(particleCount * 4);
	uv2Array.resize(particleCount * 4);
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

	Vector3* positions = vertexArray.ptrw();
	Vector3* normals = normalArray.ptrw();
	for(uint32_t p = 0; p < particleCount; p++) {
		pixelpart::mat3_t rotationMatrix = rotation_3d(particleRenderData.rotation[p]);
		pixelpart::float3_t pivot = particleType.pivot().value() * particleRenderData.size[p];
		pixelpart::float3_t position[4] = {
			rotationMatrix * (pixelpart::float3_t(-0.5, -0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot,
			rotationMatrix * (pixelpart::float3_t(-0.5, +0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot,
			rotationMatrix * (pixelpart::float3_t(+0.5, +0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot,
			rotationMatrix * (pixelpart::float3_t(+0.5, -0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot };
		pixelpart::float3_t normal = rotationMatrix * pixelpart::float3_t(0.0, 0.0, 1.0);

		switch(particleType.alignmentMode()) {
			case pixelpart::AlignmentMode::camera: {
				position[0] = particleRenderData.globalPosition[p] + cameraRight * position[0].x + cameraUp * position[0].y;
				position[1] = particleRenderData.globalPosition[p] + cameraRight * position[1].x + cameraUp * position[1].y;
				position[2] = particleRenderData.globalPosition[p] + cameraRight * position[2].x + cameraUp * position[2].y;
				position[3] = particleRenderData.globalPosition[p] + cameraRight * position[3].x + cameraUp * position[3].y;
				normal = cameraPosition - particleRenderData.globalPosition[p];
				break;
			}
			case pixelpart::AlignmentMode::motion: {
				pixelpart::mat3_t lookAtMatrix = look_at(particleRenderData.velocity[p]);
				position[0] = particleRenderData.globalPosition[p] + lookAtMatrix * position[0];
				position[1] = particleRenderData.globalPosition[p] + lookAtMatrix * position[1];
				position[2] = particleRenderData.globalPosition[p] + lookAtMatrix * position[2];
				position[3] = particleRenderData.globalPosition[p] + lookAtMatrix * position[3];
				normal = lookAtMatrix * normal;
				break;
			}
			case pixelpart::AlignmentMode::emission: {
				pixelpart::mat3_t lookAtMatrix = look_at(particleEmitter.position().at(alpha) - particleRenderData.globalPosition[p]);
				position[0] = particleRenderData.globalPosition[p] + lookAtMatrix * position[0];
				position[1] = particleRenderData.globalPosition[p] + lookAtMatrix * position[1];
				position[2] = particleRenderData.globalPosition[p] + lookAtMatrix * position[2];
				position[3] = particleRenderData.globalPosition[p] + lookAtMatrix * position[3];
				normal = lookAtMatrix * normal;
				break;
			}
			case pixelpart::AlignmentMode::emitter: {
				pixelpart::mat3_t lookAtMatrix = rotation_3d(particleEmitter.rotation().at(alpha));
				position[0] = particleRenderData.globalPosition[p] + lookAtMatrix * position[0];
				position[1] = particleRenderData.globalPosition[p] + lookAtMatrix * position[1];
				position[2] = particleRenderData.globalPosition[p] + lookAtMatrix * position[2];
				position[3] = particleRenderData.globalPosition[p] + lookAtMatrix * position[3];
				normal = lookAtMatrix * normal;
				break;
			}
			default: {
				position[0] = particleRenderData.globalPosition[p] + position[0];
				position[1] = particleRenderData.globalPosition[p] + position[1];
				position[2] = particleRenderData.globalPosition[p] + position[2];
				position[3] = particleRenderData.globalPosition[p] + position[3];
				break;
			}
		}

		positions[p * 4 + 0] = pxpt_to_gd(position[0] * scale);
		positions[p * 4 + 1] = pxpt_to_gd(position[1] * scale);
		positions[p * 4 + 2] = pxpt_to_gd(position[2] * scale);
		positions[p * 4 + 3] = pxpt_to_gd(position[3] * scale);
		normals[p * 4 + 0] = pxpt_to_gd(normal);
		normals[p * 4 + 1] = pxpt_to_gd(normal);
		normals[p * 4 + 2] = pxpt_to_gd(normal);
		normals[p * 4 + 3] = pxpt_to_gd(normal);
	}

	Color* colors = colorArray.ptrw();
	for(uint32_t p = 0; p < particleCount; p++) {
		colors[p * 4 + 0] = pxpt_to_gd_color(particleRenderData.color[p]);
		colors[p * 4 + 1] = pxpt_to_gd_color(particleRenderData.color[p]);
		colors[p * 4 + 2] = pxpt_to_gd_color(particleRenderData.color[p]);
		colors[p * 4 + 3] = pxpt_to_gd_color(particleRenderData.color[p]);
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

	float* uvs2 = reinterpret_cast<float*>(uv2Array.ptrw());
	for(uint32_t p = 0; p < particleCount; p++) {
		uvs2[p * 4 * 2 + 0] = static_cast<float>(particleRenderData.life[p]);
		uvs2[p * 4 * 2 + 1] = static_cast<float>(particleRenderData.id[p]);
		uvs2[p * 4 * 2 + 2] = static_cast<float>(particleRenderData.life[p]);
		uvs2[p * 4 * 2 + 3] = static_cast<float>(particleRenderData.id[p]);
		uvs2[p * 4 * 2 + 4] = static_cast<float>(particleRenderData.life[p]);
		uvs2[p * 4 * 2 + 5] = static_cast<float>(particleRenderData.id[p]);
		uvs2[p * 4 * 2 + 6] = static_cast<float>(particleRenderData.life[p]);
		uvs2[p * 4 * 2 + 7] = static_cast<float>(particleRenderData.id[p]);
	}

	float* custom0 = custom0Array.ptrw();
	for(uint32_t p = 0; p < particleCount; p++) {
		custom0[p * 4 * 4 + 0] = static_cast<float>(particleRenderData.velocity[p].x);
		custom0[p * 4 * 4 + 1] = static_cast<float>(particleRenderData.velocity[p].y);
		custom0[p * 4 * 4 + 2] = static_cast<float>(particleRenderData.velocity[p].z);
		custom0[p * 4 * 4 + 3] = 0.0f;
		custom0[p * 4 * 4 + 4] = static_cast<float>(particleRenderData.velocity[p].x);
		custom0[p * 4 * 4 + 5] = static_cast<float>(particleRenderData.velocity[p].y);
		custom0[p * 4 * 4 + 6] = static_cast<float>(particleRenderData.velocity[p].z);
		custom0[p * 4 * 4 + 7] = 0.0f;
		custom0[p * 4 * 4 + 8] = static_cast<float>(particleRenderData.velocity[p].x);
		custom0[p * 4 * 4 + 9] = static_cast<float>(particleRenderData.velocity[p].y);
		custom0[p * 4 * 4 + 10] = static_cast<float>(particleRenderData.velocity[p].z);
		custom0[p * 4 * 4 + 11] = 0.0f;
		custom0[p * 4 * 4 + 12] = static_cast<float>(particleRenderData.velocity[p].x);
		custom0[p * 4 * 4 + 13] = static_cast<float>(particleRenderData.velocity[p].y);
		custom0[p * 4 * 4 + 14] = static_cast<float>(particleRenderData.velocity[p].z);
		custom0[p * 4 * 4 + 15] = 0.0f;
	}

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
}

void PixelpartParticleRenderer3D::add_particle_trails(Node3D* parentNode,
	const pixelpart::ParticleEmitter& particleEmitter,
	const pixelpart::ParticleType& particleType,
	pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
	const pixelpart::RuntimeContext& runtimeContext, pixelpart::float_t scale) {
	const pixelpart::float_t epsilon = 0.000001;

	if(particleCount < 2) {
		return;
	}

	Camera3D* camera = parentNode->get_viewport()->get_camera_3d();
	if(!camera) {
		return;
	}

	pixelpart::float3_t cameraPosition = gd_to_pxpt(camera->get_global_transform().origin - parentNode->get_global_position());

	std::vector<uint32_t> sortKeys(particleCount);
	std::iota(sortKeys.begin(), sortKeys.end(), 0u);
	std::sort(sortKeys.begin(), sortKeys.end(), [&particles](uint32_t p0, uint32_t p1) {
		return (particles.parentId[p0] != particles.parentId[p1])
			? particles.parentId[p0] > particles.parentId[p1]
			: particles.id[p0] > particles.id[p1];
	});

	for(auto& entry : trails) {
		entry.second.particleCount = 0u;
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
					relativeParticlePosition[0u] = 0.0;

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
					currentTrail->directionToEdge.resize(particleType.trailRendererSettings().smoothingSegmentCount);
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
					currentTrail->directionToEdge.resize(currentTrail->particleCount);
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

	int64_t numTotalTrailSegments = 0;
	for(auto& entry : trails) {
		ParticleTrailData& trail = entry.second;
		if(trail.position.size() < 2) {
			continue;
		}

		numTotalTrailSegments += static_cast<int64_t>(trail.position.size()) - 1;
	}

	if(numTotalTrailSegments < 1) {
		return;
	}

	indexArray.resize(numTotalTrailSegments * 12);
	vertexArray.resize(numTotalTrailSegments * 5);
	normalArray.resize(numTotalTrailSegments * 5);
	uvArray.resize(numTotalTrailSegments * 5);
	uv2Array.resize(numTotalTrailSegments * 5);
	colorArray.resize(numTotalTrailSegments * 5);
	custom0Array.resize(numTotalTrailSegments * 5 * 4);

	int64_t vertexIndex = 0;
	int32_t* indices = indexArray.ptrw();
	Vector3* positions = vertexArray.ptrw();
	Vector3* normals = normalArray.ptrw();
	Color* colors = colorArray.ptrw();
	Vector2* uvs = uvArray.ptrw();
	Vector2* uvs2 = uv2Array.ptrw();
	float* custom0 = custom0Array.ptrw();

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
				trail.directionToEdge[i] = pixelpart::float3_t(
					-trail.direction[i].y,
					trail.direction[i].x,
					trail.direction[i].z);
			}
			else {
				trail.directionToEdge[i] = (glm::dot(glm::cross(trail.direction[i], pixelpart::float3_t(0.0, 0.0, 1.0)), toEdge / toEdgeLength) < 0.0)
					? +toEdge / toEdgeLength
					: -toEdge / toEdgeLength;
			}
		}

		trail.directionToEdge[0] = trail.directionToEdge[1];

		for(pixelpart::float_t& index : trail.index) {
			index /= trail.length;
		}

		for(std::size_t p = 0; p < trail.position.size() - 1; p++) {
			pixelpart::float3_t startToEdgeDirection = trail.directionToEdge[p];
			pixelpart::float3_t endToEdgeDirection = trail.directionToEdge[p + 1];

			if(particleType.alignmentMode() == pixelpart::AlignmentMode::camera) {
				pixelpart::float3_t cameraToStart = glm::normalize(cameraPosition - trail.position[p]);
				pixelpart::float3_t cameraToEnd = glm::normalize(cameraPosition - trail.position[p + 1]);
				startToEdgeDirection = glm::normalize(glm::cross(cameraToStart, trail.direction[p]));
				endToEdgeDirection = glm::normalize(glm::cross(cameraToEnd, trail.direction[p + 1]));
			}

			pixelpart::float3_t startToEdge = startToEdgeDirection * std::max(trail.size[p].x, std::max(trail.size[p].y, trail.size[p].z)) * 0.5;
			pixelpart::float3_t endToEdge = endToEdgeDirection * std::max(trail.size[p + 1].x, std::max(trail.size[p + 1].y, trail.size[p + 1].z)) * 0.5;

			pixelpart::float3_t vertexPositions[5] = {
				(trail.position[p] + startToEdge) * scale,
				(trail.position[p] - startToEdge) * scale,
				(trail.position[p + 1] + endToEdge) * scale,
				(trail.position[p + 1] - endToEdge) * scale,
				(trail.position[p] + trail.position[p + 1]) * 0.5 * scale
			};
			pixelpart::float3_t vertexNormals[2] = {
				glm::cross(startToEdgeDirection, trail.direction[p]),
				glm::cross(endToEdgeDirection, trail.direction[p + 1])
			};

			pixelpart::float2_t vertexTextureCoords[5];
			switch(particleType.trailRendererSettings().textureRotation) {
				case pixelpart::ParticleTrailRendererSettings::TextureRotation::left:
					vertexTextureCoords[0] = pixelpart::float2_t(trail.index[p] * particleType.trailRendererSettings().textureUVFactor, 1.0);
					vertexTextureCoords[1] = pixelpart::float2_t(trail.index[p] * particleType.trailRendererSettings().textureUVFactor, 0.0);
					vertexTextureCoords[2] = pixelpart::float2_t(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor, 1.0);
					vertexTextureCoords[3] = pixelpart::float2_t(trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor, 0.0);
					vertexTextureCoords[4] = pixelpart::float2_t((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings().textureUVFactor, 0.5);
					break;

				case pixelpart::ParticleTrailRendererSettings::TextureRotation::down:
					vertexTextureCoords[0] = pixelpart::float2_t(1.0, 1.0 - trail.index[p] * particleType.trailRendererSettings().textureUVFactor);
					vertexTextureCoords[1] = pixelpart::float2_t(0.0, 1.0 - trail.index[p] * particleType.trailRendererSettings().textureUVFactor);
					vertexTextureCoords[2] = pixelpart::float2_t(1.0, 1.0 - trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor);
					vertexTextureCoords[3] = pixelpart::float2_t(0.0, 1.0 - trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor);
					vertexTextureCoords[4] = pixelpart::float2_t(0.5, 1.0 - (trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings().textureUVFactor);
					break;

				case pixelpart::ParticleTrailRendererSettings::TextureRotation::right:
					vertexTextureCoords[0] = pixelpart::float2_t(1.0 - trail.index[p] * particleType.trailRendererSettings().textureUVFactor, 0.0);
					vertexTextureCoords[1] = pixelpart::float2_t(1.0 - trail.index[p] * particleType.trailRendererSettings().textureUVFactor, 1.0);
					vertexTextureCoords[2] = pixelpart::float2_t(1.0 - trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor, 0.0);
					vertexTextureCoords[3] = pixelpart::float2_t(1.0 - trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor, 1.0);
					vertexTextureCoords[4] = pixelpart::float2_t(1.0 - (trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings().textureUVFactor, 0.5);
					break;

				default:
					vertexTextureCoords[0] = pixelpart::float2_t(0.0, trail.index[p] * particleType.trailRendererSettings().textureUVFactor);
					vertexTextureCoords[1] = pixelpart::float2_t(1.0, trail.index[p] * particleType.trailRendererSettings().textureUVFactor);
					vertexTextureCoords[2] = pixelpart::float2_t(0.0, trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor);
					vertexTextureCoords[3] = pixelpart::float2_t(1.0, trail.index[p + 1] * particleType.trailRendererSettings().textureUVFactor);
					vertexTextureCoords[4] = pixelpart::float2_t(0.5, (trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings().textureUVFactor);
					break;
			}

			indices[vertexIndex * 12 + 0] = vertexIndex * 5 + 0;
			indices[vertexIndex * 12 + 1] = vertexIndex * 5 + 1;
			indices[vertexIndex * 12 + 2] = vertexIndex * 5 + 4;
			indices[vertexIndex * 12 + 3] = vertexIndex * 5 + 2;
			indices[vertexIndex * 12 + 4] = vertexIndex * 5 + 0;
			indices[vertexIndex * 12 + 5] = vertexIndex * 5 + 4;
			indices[vertexIndex * 12 + 6] = vertexIndex * 5 + 1;
			indices[vertexIndex * 12 + 7] = vertexIndex * 5 + 3;
			indices[vertexIndex * 12 + 8] = vertexIndex * 5 + 4;
			indices[vertexIndex * 12 + 9] = vertexIndex * 5 + 3;
			indices[vertexIndex * 12 + 10] = vertexIndex * 5 + 2;
			indices[vertexIndex * 12 + 11] = vertexIndex * 5 + 4;

			positions[vertexIndex * 5 + 0] = pxpt_to_gd(vertexPositions[0]);
			positions[vertexIndex * 5 + 1] = pxpt_to_gd(vertexPositions[1]);
			positions[vertexIndex * 5 + 2] = pxpt_to_gd(vertexPositions[2]);
			positions[vertexIndex * 5 + 3] = pxpt_to_gd(vertexPositions[3]);
			positions[vertexIndex * 5 + 4] = pxpt_to_gd(vertexPositions[4]);

			normals[vertexIndex * 5 + 0] = pxpt_to_gd(vertexNormals[0]);
			normals[vertexIndex * 5 + 1] = pxpt_to_gd(vertexNormals[0]);
			normals[vertexIndex * 5 + 2] = pxpt_to_gd(vertexNormals[1]);
			normals[vertexIndex * 5 + 3] = pxpt_to_gd(vertexNormals[1]);
			normals[vertexIndex * 5 + 4] = pxpt_to_gd((vertexNormals[0] + vertexNormals[1]) * 0.5);

			colors[vertexIndex * 5 + 0] = pxpt_to_gd_color(trail.color[p]);
			colors[vertexIndex * 5 + 1] = pxpt_to_gd_color(trail.color[p]);
			colors[vertexIndex * 5 + 2] = pxpt_to_gd_color(trail.color[p + 1]);
			colors[vertexIndex * 5 + 3] = pxpt_to_gd_color(trail.color[p + 1]);
			colors[vertexIndex * 5 + 4] = pxpt_to_gd_color((trail.color[p] + trail.color[p + 1]) * 0.5);

			uvs[vertexIndex * 5 + 0] = pxpt_to_gd(vertexTextureCoords[0]);
			uvs[vertexIndex * 5 + 1] = pxpt_to_gd(vertexTextureCoords[1]);
			uvs[vertexIndex * 5 + 2] = pxpt_to_gd(vertexTextureCoords[2]);
			uvs[vertexIndex * 5 + 3] = pxpt_to_gd(vertexTextureCoords[3]);
			uvs[vertexIndex * 5 + 4] = pxpt_to_gd(vertexTextureCoords[4]);

			uvs2[vertexIndex * 5 + 0] = Vector2(static_cast<float>(trail.life[p]), static_cast<float>(entry.first));
			uvs2[vertexIndex * 5 + 1] = Vector2(static_cast<float>(trail.life[p]), static_cast<float>(entry.first));
			uvs2[vertexIndex * 5 + 2] = Vector2(static_cast<float>(trail.life[p + 1]), static_cast<float>(entry.first));
			uvs2[vertexIndex * 5 + 3] = Vector2(static_cast<float>(trail.life[p + 1]), static_cast<float>(entry.first));
			uvs2[vertexIndex * 5 + 4] = Vector2(static_cast<float>(trail.life[p] + trail.life[p + 1]) * 0.5f, static_cast<float>(entry.first));

			custom0[vertexIndex * 5 * 4 + 0] = static_cast<float>(trail.velocity[p].x);
			custom0[vertexIndex * 5 * 4 + 1] = static_cast<float>(trail.velocity[p].y);
			custom0[vertexIndex * 5 * 4 + 2] = static_cast<float>(trail.velocity[p].z);
			custom0[vertexIndex * 5 * 4 + 3] = 0.0f;
			custom0[vertexIndex * 5 * 4 + 4] = static_cast<float>(trail.velocity[p].x);
			custom0[vertexIndex * 5 * 4 + 5] = static_cast<float>(trail.velocity[p].y);
			custom0[vertexIndex * 5 * 4 + 6] = static_cast<float>(trail.velocity[p].z);
			custom0[vertexIndex * 5 * 4 + 7] = 0.0f;
			custom0[vertexIndex * 5 * 4 + 8] = static_cast<float>(trail.velocity[p + 1].x);
			custom0[vertexIndex * 5 * 4 + 9] = static_cast<float>(trail.velocity[p + 1].y);
			custom0[vertexIndex * 5 * 4 + 10] = static_cast<float>(trail.velocity[p + 1].z);
			custom0[vertexIndex * 5 * 4 + 11] = 0.0f;
			custom0[vertexIndex * 5 * 4 + 12] = static_cast<float>(trail.velocity[p + 1].x);
			custom0[vertexIndex * 5 * 4 + 13] = static_cast<float>(trail.velocity[p + 1].y);
			custom0[vertexIndex * 5 * 4 + 14] = static_cast<float>(trail.velocity[p + 1].z);
			custom0[vertexIndex * 5 * 4 + 15] = 0.0f;
			custom0[vertexIndex * 5 * 4 + 16] = static_cast<float>(trail.velocity[p].x + trail.velocity[p + 1].x) * 0.5f;
			custom0[vertexIndex * 5 * 4 + 17] = static_cast<float>(trail.velocity[p].y + trail.velocity[p + 1].y) * 0.5f;
			custom0[vertexIndex * 5 * 4 + 18] = static_cast<float>(trail.velocity[p].z + trail.velocity[p + 1].z) * 0.5f;
			custom0[vertexIndex * 5 * 4 + 19] = 0.0f;

			vertexIndex++;
		}
	}

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
}

void PixelpartParticleRenderer3D::add_particle_meshes(Node3D* parentNode,
	const pixelpart::ParticleEmitter& particleEmitter,
	const pixelpart::ParticleType& particleType,
	pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
	const pixelpart::RuntimeContext& runtimeContext, pixelpart::float_t scale) {
	const float packFactor = 0.5;

	if(particleCount == 0) {
		return;
	}

	Camera3D* camera = parentNode->get_viewport()->get_camera_3d();
	if(!camera) {
		return;
	}

	pixelpart::ParticleCollection::ReadPtr particleRenderData =
		sort_particles(particles, particleCount, particleType.meshRendererSettings().sortCriterion, parentNode, camera);

	pixelpart::float_t alpha = particleEmitter.life(runtimeContext);
	pixelpart::float3_t emitterPosition = particleEmitter.position().at(alpha);
	pixelpart::mat4_t emitterRotationMatrix = pixelpart::mat4_t(rotation_3d(particleEmitter.rotation().at(alpha)));

	instanceDataArray.resize(particleCount * 20);
	float* instanceData = instanceDataArray.ptrw();

	for(uint32_t p = 0; p < particleCount; p++) {
		pixelpart::mat4_t globalScaleMatrix = glm::scale(pixelpart::float3_t(scale));

		pixelpart::mat4_t alignmentMatrix = pixelpart::mat4_t(1.0);
		switch(particleType.alignmentMode()) {
			case pixelpart::AlignmentMode::motion:
				alignmentMatrix = pixelpart::mat4_t(look_at(particleRenderData.velocity[p]));
				break;
			case pixelpart::AlignmentMode::emission:
				alignmentMatrix = pixelpart::mat4_t(look_at(emitterPosition - particleRenderData.globalPosition[p]));
				break;
			case pixelpart::AlignmentMode::emitter:
				alignmentMatrix = emitterRotationMatrix;
				break;
			default:
				break;
		}

		pixelpart::float3_t pivot = particleType.pivot().value() * particleRenderData.size[p];
		pixelpart::mat4_t rotationMatrix =
			glm::translate(pivot) *
			pixelpart::mat4_t(rotation_3d(particleRenderData.rotation[p])) *
			glm::translate(-pivot);

		pixelpart::mat4_t translationMatrix = glm::translate(particleRenderData.globalPosition[p]);
		pixelpart::mat4_t scaleMatrix = glm::scale(particleRenderData.size[p]);

		pixelpart::mat4_t transformationMatrix =
			globalScaleMatrix * translationMatrix * alignmentMatrix * rotationMatrix * scaleMatrix;

		instanceData[p * 20 + 0] = static_cast<float>(transformationMatrix[0][0]);
		instanceData[p * 20 + 1] = static_cast<float>(transformationMatrix[1][0]);
		instanceData[p * 20 + 2] = static_cast<float>(transformationMatrix[2][0]);
		instanceData[p * 20 + 3] = static_cast<float>(transformationMatrix[3][0]);
		instanceData[p * 20 + 4] = static_cast<float>(transformationMatrix[0][1]);
		instanceData[p * 20 + 5] = static_cast<float>(transformationMatrix[1][1]);
		instanceData[p * 20 + 6] = static_cast<float>(transformationMatrix[2][1]);
		instanceData[p * 20 + 7] = static_cast<float>(transformationMatrix[3][1]);
		instanceData[p * 20 + 8] = static_cast<float>(transformationMatrix[0][2]);
		instanceData[p * 20 + 9] = static_cast<float>(transformationMatrix[1][2]);
		instanceData[p * 20 + 10] = static_cast<float>(transformationMatrix[2][2]);
		instanceData[p * 20 + 11] = static_cast<float>(transformationMatrix[3][2]);

		instanceData[p * 20 + 12] = static_cast<float>(particleRenderData.color[p].r);
		instanceData[p * 20 + 13] = static_cast<float>(particleRenderData.color[p].g);
		instanceData[p * 20 + 14] = static_cast<float>(particleRenderData.color[p].b);
		instanceData[p * 20 + 15] = static_cast<float>(particleRenderData.color[p].a);

		instanceData[p * 20 + 16] = pxpt_to_gd(particleRenderData.velocity[p].x);
		instanceData[p * 20 + 17] = pxpt_to_gd(particleRenderData.velocity[p].y);
		instanceData[p * 20 + 18] = pxpt_to_gd(particleRenderData.velocity[p].z);
		instanceData[p * 20 + 19] = pack_uint_float(particleRenderData.id[p], pxpt_to_gd(particleRenderData.life[p]), packFactor);
	}

	multiMesh->set_visible_instance_count(-1);
	multiMesh->set_instance_count(static_cast<int32_t>(particleCount));
	multiMesh->set_buffer(instanceDataArray);
}

pixelpart::ParticleCollection::ReadPtr PixelpartParticleRenderer3D::sort_particles(
	pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
	pixelpart::ParticleSortCriterion sortCriterion,
	Node3D* parentNode, Camera3D* camera) {
	if(sortCriterion == pixelpart::ParticleSortCriterion::none || particleCount <= 1) {
		return particles;
	}

	if(particleCount > sortedParticleCollection.capacity()) {
		sortedParticleCollection = pixelpart::ParticleCollection(particleCount);
		sortKeys.resize(particleCount);
	}

	std::iota(sortKeys.begin(), sortKeys.begin() + particleCount, 0);

	switch(sortCriterion) {
		case pixelpart::ParticleSortCriterion::age: {
			insertion_sort(sortKeys.begin(), sortKeys.begin() + particleCount,
				[&particles](uint32_t i, uint32_t j) {
					return particles.id[i] < particles.id[j];
				});

			break;
		}

		case pixelpart::ParticleSortCriterion::distance: {
			pixelpart::float3_t cameraPosition = gd_to_pxpt(camera->get_global_transform().origin - parentNode->get_global_position());

			insertion_sort(sortKeys.begin(), sortKeys.begin() + particleCount,
				[&particles, cameraPosition](uint32_t i, uint32_t j) {
					return glm::distance2(particles.globalPosition[i], cameraPosition) > glm::distance2(particles.globalPosition[j], cameraPosition);
				});

			break;
		}
	}

	pixelpart::ParticleCollection::WritePtr sortedParticleData = sortedParticleCollection.writePtr();

	for(uint32_t i = 0; i < particleCount; i++) {
		uint32_t j = sortKeys[i];

		sortedParticleData.id[i] = particles.id[j];
		sortedParticleData.life[i] = particles.life[j];
		sortedParticleData.globalPosition[i] = particles.globalPosition[j];
		sortedParticleData.velocity[i] = particles.velocity[j];
		sortedParticleData.force[i] = particles.force[j];
		sortedParticleData.rotation[i] = particles.rotation[j];
		sortedParticleData.size[i] = particles.size[j];
		sortedParticleData.color[i] = particles.color[j];
	}

	return sortedParticleCollection.readPtr();
}

pixelpart::mat3_t PixelpartParticleRenderer3D::rotation_3d(const pixelpart::float3_t& angle) {
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
pixelpart::mat3_t PixelpartParticleRenderer3D::look_at(const pixelpart::float3_t& direction) {
	pixelpart::float3_t front = glm::normalize(direction);

	pixelpart::float3_t up = pixelpart::float3_t(0.0, 1.0, 0.0);
	if(glm::abs(glm::abs(glm::dot(front, up)) - 1.0) < 0.001) {
		up = pixelpart::float3_t(1.0, 0.0, 0.0);
	}

	pixelpart::float3_t right = glm::normalize(glm::cross(front, up));
	up = glm::normalize(glm::cross(right, front));

	return pixelpart::mat3_t(right, up, -front);
}
}