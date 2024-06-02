#include "PixelpartParticleMesh3D.h"
#include "../util/PixelpartSortUtil.h"
#include "../util/PixelpartUtil.h"
#include <glm/glm/gtx/norm.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {
PixelpartParticleMesh3D::PixelpartParticleMesh3D(PixelpartGraphicsResourceStore& resourceStore, PixelpartShaderGenerator& shaderGenerator, const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType) :
	resources(resourceStore) {
	RenderingServer* rs = RenderingServer::get_singleton();

	if(particleType.materialInstance.isBuiltInMaterial && shaderGenerator.has_builtin_spatial_shader(particleType.materialInstance.materialId)) {
		shader = shaderGenerator.get_builtin_spatial_shader(particleType.materialInstance.materialId);

		const PixelpartShaderGenerator::ShaderMetadata& metadata = shaderGenerator.get_builtin_spatial_shader_metadata(particleType.materialInstance.materialId);

		for(const auto& parameterEntry : metadata.parameterNames) {
			shaderParameterNames[parameterEntry.first] = PixelpartShaderGenerator::uniformPrefix + parameterEntry.second;
		}
	}
	else if(effect.resources.materials.count(particleType.materialInstance.materialId) != 0u) {
		const pixelpart::MaterialResource& materialResource = effect.resources.materials.at(particleType.materialInstance.materialId);

		pixelpart::ShaderGraph::BuildResult shaderBuildResult;
		materialResource.shaderGraph.build(shaderBuildResult);

		shaderParameterNames = shaderBuildResult.parameterNames;
		for(uint32_t samplerIndex = 0u; samplerIndex < shaderBuildResult.textureResourceIds.size(); samplerIndex++) {
			textureResourceIds[pixelpart::ShaderGraph::graphLanguage.textureSamplers[samplerIndex]] = shaderBuildResult.textureResourceIds[samplerIndex];
		}

		shader = shaderGenerator.get_custom_spatial_shader(
			shaderBuildResult.mainCode, shaderBuildResult.parameterCode,
			particleType.renderer, materialResource.blendMode, materialResource.lightingMode);
	}

	shaderMaterial.instantiate();

	if(shader.is_valid()) {
		shaderMaterial->set_shader(shader);
	}
	else {
		UtilityFunctions::push_error("Could not create particle material");
	}

	instanceRID = rs->instance_create();

	switch(particleType.renderer) {
		case pixelpart::ParticleRendererType::sprite:
		case pixelpart::ParticleRendererType::trail:
			arrayMesh.instantiate();
			rs->instance_set_base(instanceRID, arrayMesh->get_rid());

			break;

		case pixelpart::ParticleRendererType::mesh:
			multiMesh.instantiate();
			multiMesh->set_transform_format(MultiMesh::TRANSFORM_3D);
			multiMesh->set_use_colors(true);
			multiMesh->set_use_custom_data(true);
			if(resources.meshes.count(particleType.meshRendererSettings.meshResourceId) != 0u) {
				Ref<ArrayMesh> baseMesh = resources.meshes.at(particleType.meshRendererSettings.meshResourceId);
				baseMesh->surface_set_material(0, shaderMaterial);

				multiMesh->set_mesh(baseMesh);
			}
			else {
				UtilityFunctions::push_error("Mesh \"", String(particleType.meshRendererSettings.meshResourceId.c_str()), "\" not found");
			}

			rs->instance_set_base(instanceRID, multiMesh->get_rid());

			break;

		default:
			break;
	}
}
PixelpartParticleMesh3D::~PixelpartParticleMesh3D() {
	RenderingServer* rs = RenderingServer::get_singleton();

	rs->free_rid(instanceRID);
}

void PixelpartParticleMesh3D::draw(Node3D* parentNode,
	const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
	pixelpart::ParticleReadPtr particles, uint32_t numParticles,
	pixelpart::float_t scale, pixelpart::float_t t) {
	switch(particleType.renderer) {
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

	if(!parentNode->is_visible() || !particleType.visible) {
		return;
	}

	const pixelpart::ParticleEmitter& particleEmitter = effect.particleEmitters.get(particleType.parentId);

	for(const auto& textureParameterEntry : textureResourceIds) {
		if(resources.textures.count(textureParameterEntry.second) != 0u) {
			shaderMaterial->set_shader_parameter(StringName(textureParameterEntry.first.c_str()), resources.textures.at(textureParameterEntry.second)->get_rid());
		}
	}

	for(const auto& parameterEntry : particleType.materialInstance.materialParameters) {
		if(shaderParameterNames.count(parameterEntry.first) == 0u) {
			continue;
		}

		const pixelpart::VariantParameter::Value& parameterValue = parameterEntry.second;

		StringName paramName = StringName(shaderParameterNames.at(parameterEntry.first).c_str());

		switch(parameterValue.type) {
			case pixelpart::VariantParameter::Value::type_int: {
				shaderMaterial->set_shader_parameter(paramName, toGd(parameterValue.getInt()));
				break;
			}
			case pixelpart::VariantParameter::Value::type_float: {
				shaderMaterial->set_shader_parameter(paramName, toGd(parameterValue.getFloat()));
				break;
			}
			case pixelpart::VariantParameter::Value::type_float2: {
				shaderMaterial->set_shader_parameter(paramName, toGd(parameterValue.getFloat2()));
				break;
			}
			case pixelpart::VariantParameter::Value::type_float3: {
				shaderMaterial->set_shader_parameter(paramName, toGd(parameterValue.getFloat3()));
				break;
			}
			case pixelpart::VariantParameter::Value::type_float4: {
				shaderMaterial->set_shader_parameter(paramName, toGd(parameterValue.getFloat4()));
				break;
			}
			case pixelpart::VariantParameter::Value::type_bool: {
				shaderMaterial->set_shader_parameter(paramName, parameterValue.getBool());
				break;
			}
			case pixelpart::VariantParameter::Value::type_enum: {
				shaderMaterial->set_shader_parameter(paramName, toGd(parameterValue.getEnum()));
				break;
			}
			case pixelpart::VariantParameter::Value::type_color: {
				shaderMaterial->set_shader_parameter(paramName, toGdColor(parameterValue.getColor()));
				break;
			}
			case pixelpart::VariantParameter::Value::type_resource_image: {
				if(resources.textures.count(parameterValue.getResourceId()) != 0u) {
					shaderMaterial->set_shader_parameter(paramName, resources.textures.at(parameterValue.getResourceId())->get_rid());
				}
				break;
			}
			default: {
				break;
			}
		}
	}

	shaderMaterial->set_shader_parameter("u_EffectTime", static_cast<float>(t));
	shaderMaterial->set_shader_parameter("u_ObjectTime", static_cast<float>(t - particleEmitter.lifetimeStart));

	RenderingServer* rs = RenderingServer::get_singleton();
	rs->instance_set_scenario(instanceRID, parentNode->get_world_3d()->get_scenario());
	rs->instance_set_transform(instanceRID, parentNode->get_global_transform());

	switch(particleType.renderer) {
		case pixelpart::ParticleRendererType::sprite:
			add_particle_sprites(parentNode, effect, particleType, particles, numParticles, scale, t);
			break;
		case pixelpart::ParticleRendererType::trail:
			add_particle_trails(parentNode, effect, particleType, particles, numParticles, scale, t);
			break;
		case pixelpart::ParticleRendererType::mesh:
			add_particle_meshes(parentNode, effect, particleType, particles, numParticles, scale, t);
			break;
		default:
			break;
	}

	switch(particleType.renderer) {
		case pixelpart::ParticleRendererType::sprite:
		case pixelpart::ParticleRendererType::trail:
			if(arrayMesh->get_surface_count() > 0) {
				arrayMesh->surface_set_material(0, shaderMaterial);
			}
			break;
		case pixelpart::ParticleRendererType::mesh:
			break;
		default:
			break;
	}
}

void PixelpartParticleMesh3D::add_particle_sprites(Node3D* parentNode,
	const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
	pixelpart::ParticleReadPtr particles, uint32_t numParticles,
	pixelpart::float_t scale, pixelpart::float_t t) {
	if(numParticles == 0u) {
		return;
	}

	const pixelpart::ParticleEmitter& particleEmitter = effect.particleEmitters.get(particleType.parentId);
	pixelpart::float_t alpha = std::fmod(t - particleEmitter.lifetimeStart, particleEmitter.lifetimeDuration) / particleEmitter.lifetimeDuration;

	Camera3D* camera = parentNode->get_viewport()->get_camera_3d();
	if(!camera) {
		return;
	}

	pixelpart::ParticleReadPtr particleRenderData = particles;
	if(particleType.spriteRendererSettings.sortCriterion != pixelpart::ParticleSortCriterion::none) {
		particleRenderData = sort_particles(particles, numParticles, particleType.spriteRendererSettings.sortCriterion, parentNode, camera);
	}

	pixelpart::vec3_t cameraPosition = fromGd(camera->get_global_transform().origin - parentNode->get_global_position());
	pixelpart::vec3_t cameraRight = fromGd(camera->get_global_transform().basis.get_column(0));
	pixelpart::vec3_t cameraUp = fromGd(camera->get_global_transform().basis.get_column(1));

	indexArray.resize(numParticles * 6);
	vertexArray.resize(numParticles * 4);
	normalArray.resize(numParticles * 4);
	uvArray.resize(numParticles * 4);
	uv2Array.resize(numParticles * 4);
	colorArray.resize(numParticles * 4);
	custom0Array.resize(numParticles * 4 * 4);

	int32_t* indices = indexArray.ptrw();
	for(int32_t p = 0; p < static_cast<int32_t>(numParticles); p++) {
		indices[p * 6 + 0] = p * 4 + 0;
		indices[p * 6 + 1] = p * 4 + 1;
		indices[p * 6 + 2] = p * 4 + 3;
		indices[p * 6 + 3] = p * 4 + 1;
		indices[p * 6 + 4] = p * 4 + 2;
		indices[p * 6 + 5] = p * 4 + 3;
	}

	Vector3* positions = vertexArray.ptrw();
	Vector3* normals = normalArray.ptrw();
	for(uint32_t p = 0u; p < numParticles; p++) {
		pixelpart::mat3_t rotationMatrix = rotation3d(particleRenderData.rotation[p]);
		pixelpart::vec3_t pivot = particleType.pivot.get() * particleRenderData.size[p];
		pixelpart::vec3_t position[4] = {
			rotationMatrix * (pixelpart::vec3_t(-0.5, -0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot,
			rotationMatrix * (pixelpart::vec3_t(-0.5, +0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot,
			rotationMatrix * (pixelpart::vec3_t(+0.5, +0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot,
			rotationMatrix * (pixelpart::vec3_t(+0.5, -0.5, 0.0) * particleRenderData.size[p] - pivot) + pivot };
		pixelpart::vec3_t normal = rotationMatrix * pixelpart::vec3_t(0.0, 0.0, 1.0);

		switch(particleType.alignmentMode) {
			case pixelpart::AlignmentMode::camera: {
				position[0] = particleRenderData.globalPosition[p] + cameraRight * position[0].x + cameraUp * position[0].y;
				position[1] = particleRenderData.globalPosition[p] + cameraRight * position[1].x + cameraUp * position[1].y;
				position[2] = particleRenderData.globalPosition[p] + cameraRight * position[2].x + cameraUp * position[2].y;
				position[3] = particleRenderData.globalPosition[p] + cameraRight * position[3].x + cameraUp * position[3].y;
				normal = cameraPosition - particleRenderData.globalPosition[p];
				break;
			}
			case pixelpart::AlignmentMode::motion: {
				pixelpart::mat3_t lookAtMatrix = lookAt(particleRenderData.velocity[p]);
				position[0] = particleRenderData.globalPosition[p] + lookAtMatrix * position[0];
				position[1] = particleRenderData.globalPosition[p] + lookAtMatrix * position[1];
				position[2] = particleRenderData.globalPosition[p] + lookAtMatrix * position[2];
				position[3] = particleRenderData.globalPosition[p] + lookAtMatrix * position[3];
				normal = lookAtMatrix * normal;
				break;
			}
			case pixelpart::AlignmentMode::emission: {
				pixelpart::mat3_t lookAtMatrix = lookAt(particleEmitter.position.get(alpha) - particleRenderData.globalPosition[p]);
				position[0] = particleRenderData.globalPosition[p] + lookAtMatrix * position[0];
				position[1] = particleRenderData.globalPosition[p] + lookAtMatrix * position[1];
				position[2] = particleRenderData.globalPosition[p] + lookAtMatrix * position[2];
				position[3] = particleRenderData.globalPosition[p] + lookAtMatrix * position[3];
				normal = lookAtMatrix * normal;
				break;
			}
			case pixelpart::AlignmentMode::emitter: {
				pixelpart::mat3_t lookAtMatrix = rotation3d(particleEmitter.orientation.get(alpha));
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

		positions[p * 4 + 0] = toGd(position[0] * scale);
		positions[p * 4 + 1] = toGd(position[1] * scale);
		positions[p * 4 + 2] = toGd(position[2] * scale);
		positions[p * 4 + 3] = toGd(position[3] * scale);
		normals[p * 4 + 0] = toGd(normal);
		normals[p * 4 + 1] = toGd(normal);
		normals[p * 4 + 2] = toGd(normal);
		normals[p * 4 + 3] = toGd(normal);
	}

	Color* colors = colorArray.ptrw();
	for(uint32_t p = 0u; p < numParticles; p++) {
		colors[p * 4 + 0] = toGdColor(particleRenderData.color[p]);
		colors[p * 4 + 1] = toGdColor(particleRenderData.color[p]);
		colors[p * 4 + 2] = toGdColor(particleRenderData.color[p]);
		colors[p * 4 + 3] = toGdColor(particleRenderData.color[p]);
	}

	float* uvs = reinterpret_cast<float*>(uvArray.ptrw());
	for(uint32_t p = 0u; p < numParticles; p++) {
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
	for(uint32_t p = 0u; p < numParticles; p++) {
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
	for(uint32_t p = 0u; p < numParticles; p++) {
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

void PixelpartParticleMesh3D::add_particle_trails(Node3D* parentNode,
	const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
	pixelpart::ParticleReadPtr particles, uint32_t numParticles,
	pixelpart::float_t scale, pixelpart::float_t t) {
	const pixelpart::float_t epsilon = 0.000001;
	if(numParticles < 2u) {
		return;
	}

	Camera3D* camera = parentNode->get_viewport()->get_camera_3d();
	if(!camera) {
		return;
	}

	pixelpart::vec3_t cameraPosition = fromGd(camera->get_global_transform().origin - parentNode->get_global_position());

	std::vector<uint32_t> sortKeys(numParticles);
	std::iota(sortKeys.begin(), sortKeys.end(), 0u);
	std::sort(sortKeys.begin(), sortKeys.end(), [&particles](uint32_t p0, uint32_t p1) {
		return (particles.parentId[p0] != particles.parentId[p1])
			? particles.parentId[p0] > particles.parentId[p1]
			: particles.id[p0] > particles.id[p1];
	});

	for(auto& entry : trails) {
		entry.second.numParticles = 0u;
	}

	ParticleTrailData* currentTrail = nullptr;
	for(uint32_t p = 0u, particleParentId = 0u, trailId = 0xFFFFFFFEu; p < numParticles; p++) {
		particleParentId = particles.parentId[sortKeys[p]];

		if(particleParentId != trailId) {
			trailId = particleParentId;
			currentTrail = &trails[trailId];
		}

		currentTrail->numParticles++;
	}

	for(auto it = trails.begin(); it != trails.end(); ) {
		if(it->second.numParticles == 0u) {
			it = trails.erase(it);
		}
		else {
			it++;
		}
	}

	currentTrail = nullptr;

	switch(particleType.trailRendererSettings.smoothingMethod) {
		case pixelpart::ParticleTrailRendererSettings::SmoothingMethod::spline: {
			for(uint32_t p = 0u, particleParentId = 0u, trailId = 0xFFFFFFFEu; p < numParticles; p++) {
				uint32_t particleIndex = sortKeys[p];
				particleParentId = particles.parentId[particleIndex];

				if(particleParentId != trailId) {
					trailId = particleParentId;
					currentTrail = &trails.at(trailId);

					std::vector<pixelpart::float_t> relativeParticlePosition(currentTrail->numParticles);
					relativeParticlePosition[0u] = 0.0;

					currentTrail->length = 0.0;
					for(uint32_t q = 1u; q < currentTrail->numParticles; q++) {
						currentTrail->length += glm::length(particles.globalPosition[sortKeys[p + q]] - particles.globalPosition[sortKeys[p + q - 1u]]);
						relativeParticlePosition[q] = currentTrail->length;
					}
					for(uint32_t q = 1u; q < currentTrail->numParticles; q++) {
						relativeParticlePosition[q] /= currentTrail->length;
					}

					pixelpart::Curve<pixelpart::vec3_t> positionCurve(pixelpart::CurveInterpolation::spline);
					positionCurve.enableFixedCache(particleType.trailRendererSettings.numSmoothingSegments);
					positionCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.globalPosition, sortKeys.data() + p, currentTrail->numParticles);

					pixelpart::Curve<pixelpart::vec3_t> sizeCurve(pixelpart::CurveInterpolation::spline);
					sizeCurve.enableFixedCache(particleType.trailRendererSettings.numSmoothingSegments);
					sizeCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.size, sortKeys.data() + p, currentTrail->numParticles);

					pixelpart::Curve<pixelpart::vec4_t> colorCurve(pixelpart::CurveInterpolation::spline);
					colorCurve.enableFixedCache(particleType.trailRendererSettings.numSmoothingSegments);
					colorCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.color, sortKeys.data() + p, currentTrail->numParticles);

					pixelpart::Curve<pixelpart::vec3_t> velocityCurve(pixelpart::CurveInterpolation::spline);
					velocityCurve.enableFixedCache(particleType.trailRendererSettings.numSmoothingSegments);
					velocityCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.velocity, sortKeys.data() + p, currentTrail->numParticles);

					pixelpart::Curve<pixelpart::float_t> lifeCurve(pixelpart::CurveInterpolation::spline);
					lifeCurve.enableFixedCache(particleType.trailRendererSettings.numSmoothingSegments);
					lifeCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.life, sortKeys.data() + p, currentTrail->numParticles);

					currentTrail->position = positionCurve.getCache();
					currentTrail->size = sizeCurve.getCache();
					currentTrail->color = colorCurve.getCache();
					currentTrail->velocity = velocityCurve.getCache();
					currentTrail->life = lifeCurve.getCache();
					currentTrail->direction.resize(particleType.trailRendererSettings.numSmoothingSegments);
					currentTrail->directionToEdge.resize(particleType.trailRendererSettings.numSmoothingSegments);
					currentTrail->index.resize(particleType.trailRendererSettings.numSmoothingSegments);
				}
			}

			break;
		}

		default: {
			for(uint32_t p = 0u, q = 0u, particleParentId = 0u, trailId = 0xFFFFFFFEu; p < numParticles; p++, q++) {
				uint32_t particleIndex = sortKeys[p];
				particleParentId = particles.parentId[particleIndex];

				if(particleParentId != trailId) {
					trailId = particleParentId;
					q = 0u;

					currentTrail = &trails.at(trailId);
					currentTrail->position.resize(currentTrail->numParticles);
					currentTrail->size.resize(currentTrail->numParticles);
					currentTrail->color.resize(currentTrail->numParticles);
					currentTrail->velocity.resize(currentTrail->numParticles);
					currentTrail->life.resize(currentTrail->numParticles);
					currentTrail->direction.resize(currentTrail->numParticles);
					currentTrail->directionToEdge.resize(currentTrail->numParticles);
					currentTrail->index.resize(currentTrail->numParticles);
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
		if(trail.position.size() < 2u) {
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

		if(trail.position.size() < 2u) {
			continue;
		}

		std::size_t last = trail.position.size() - 1u;
		for(std::size_t i = 0u; i < last; i++) {
			pixelpart::vec3_t toNext = trail.position[i + 1u] - trail.position[i];
			pixelpart::float_t distance = std::max(glm::length(toNext), epsilon);

			trail.direction[i] = toNext / distance;
			trail.index[i] = trail.length;
			trail.length += distance;
		}

		trail.direction[last] = trail.direction[last - 1u];
		trail.index[last] = trail.length;

		for(std::size_t i = last; i > 0u; i--) {
			pixelpart::vec3_t toEdge = trail.direction[i] - trail.direction[i - 1u];
			pixelpart::float_t toEdgeLength = glm::length(toEdge);
			if(toEdgeLength < epsilon) {
				trail.directionToEdge[i] = pixelpart::vec3_t(
					-trail.direction[i].y,
					trail.direction[i].x,
					trail.direction[i].z);
			}
			else {
				trail.directionToEdge[i] = (glm::dot(glm::cross(trail.direction[i], pixelpart::vec3_t(0.0, 0.0, 1.0)), toEdge / toEdgeLength) < 0.0)
					? +toEdge / toEdgeLength
					: -toEdge / toEdgeLength;
			}
		}

		trail.directionToEdge[0u] = trail.directionToEdge[1u];

		for(pixelpart::float_t& index : trail.index) {
			index /= trail.length;
		}

		for(std::size_t p = 0u; p < trail.position.size() - 1u; p++) {
			pixelpart::vec3_t startToEdgeDirection = trail.directionToEdge[p];
			pixelpart::vec3_t endToEdgeDirection = trail.directionToEdge[p + 1u];

			if(particleType.alignmentMode == pixelpart::AlignmentMode::camera) {
				pixelpart::vec3_t cameraToStart = glm::normalize(cameraPosition - trail.position[p]);
				pixelpart::vec3_t cameraToEnd = glm::normalize(cameraPosition - trail.position[p + 1u]);
				startToEdgeDirection = glm::normalize(glm::cross(cameraToStart, trail.direction[p]));
				endToEdgeDirection = glm::normalize(glm::cross(cameraToEnd, trail.direction[p + 1u]));
			}

			pixelpart::vec3_t startToEdge = startToEdgeDirection * std::max(trail.size[p].x, std::max(trail.size[p].y, trail.size[p].z)) * 0.5;
			pixelpart::vec3_t endToEdge = endToEdgeDirection * std::max(trail.size[p + 1u].x, std::max(trail.size[p + 1u].y, trail.size[p + 1u].z)) * 0.5;

			pixelpart::vec3_t vertexPositions[5] = {
				(trail.position[p] + startToEdge) * scale,
				(trail.position[p] - startToEdge) * scale,
				(trail.position[p + 1u] + endToEdge) * scale,
				(trail.position[p + 1u] - endToEdge) * scale,
				(trail.position[p] + trail.position[p + 1u]) * 0.5 * scale
			};
			pixelpart::vec3_t vertexNormals[2] = {
				glm::cross(startToEdgeDirection, trail.direction[p]),
				glm::cross(endToEdgeDirection, trail.direction[p + 1u])
			};

			pixelpart::vec2_t vertexTextureCoords[5];
			switch(particleType.trailRendererSettings.textureRotation) {
				case pixelpart::ParticleTrailRendererSettings::TextureRotation::left:
					vertexTextureCoords[0] = pixelpart::vec2_t(trail.index[p] * particleType.trailRendererSettings.textureUVFactor, 1.0);
					vertexTextureCoords[1] = pixelpart::vec2_t(trail.index[p] * particleType.trailRendererSettings.textureUVFactor, 0.0);
					vertexTextureCoords[2] = pixelpart::vec2_t(trail.index[p + 1u] * particleType.trailRendererSettings.textureUVFactor, 1.0);
					vertexTextureCoords[3] = pixelpart::vec2_t(trail.index[p + 1u] * particleType.trailRendererSettings.textureUVFactor, 0.0);
					vertexTextureCoords[4] = pixelpart::vec2_t((trail.index[p] + trail.index[p + 1u]) * 0.5 * particleType.trailRendererSettings.textureUVFactor, 0.5);
					break;

				case pixelpart::ParticleTrailRendererSettings::TextureRotation::down:
					vertexTextureCoords[0] = pixelpart::vec2_t(1.0, 1.0 - trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					vertexTextureCoords[1] = pixelpart::vec2_t(0.0, 1.0 - trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					vertexTextureCoords[2] = pixelpart::vec2_t(1.0, 1.0 - trail.index[p + 1u] * particleType.trailRendererSettings.textureUVFactor);
					vertexTextureCoords[3] = pixelpart::vec2_t(0.0, 1.0 - trail.index[p + 1u] * particleType.trailRendererSettings.textureUVFactor);
					vertexTextureCoords[4] = pixelpart::vec2_t(0.5, 1.0 - (trail.index[p] + trail.index[p + 1u]) * 0.5 * particleType.trailRendererSettings.textureUVFactor);
					break;

				case pixelpart::ParticleTrailRendererSettings::TextureRotation::right:
					vertexTextureCoords[0] = pixelpart::vec2_t(1.0 - trail.index[p] * particleType.trailRendererSettings.textureUVFactor, 0.0);
					vertexTextureCoords[1] = pixelpart::vec2_t(1.0 - trail.index[p] * particleType.trailRendererSettings.textureUVFactor, 1.0);
					vertexTextureCoords[2] = pixelpart::vec2_t(1.0 - trail.index[p + 1u] * particleType.trailRendererSettings.textureUVFactor, 0.0);
					vertexTextureCoords[3] = pixelpart::vec2_t(1.0 - trail.index[p + 1u] * particleType.trailRendererSettings.textureUVFactor, 1.0);
					vertexTextureCoords[4] = pixelpart::vec2_t(1.0 - (trail.index[p] + trail.index[p + 1u]) * 0.5 * particleType.trailRendererSettings.textureUVFactor, 0.5);
					break;

				default:
					vertexTextureCoords[0] = pixelpart::vec2_t(0.0, trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					vertexTextureCoords[1] = pixelpart::vec2_t(1.0, trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					vertexTextureCoords[2] = pixelpart::vec2_t(0.0, trail.index[p + 1u] * particleType.trailRendererSettings.textureUVFactor);
					vertexTextureCoords[3] = pixelpart::vec2_t(1.0, trail.index[p + 1u] * particleType.trailRendererSettings.textureUVFactor);
					vertexTextureCoords[4] = pixelpart::vec2_t(0.5, (trail.index[p] + trail.index[p + 1u]) * 0.5 * particleType.trailRendererSettings.textureUVFactor);
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

			positions[vertexIndex * 5 + 0] = toGd(vertexPositions[0]);
			positions[vertexIndex * 5 + 1] = toGd(vertexPositions[1]);
			positions[vertexIndex * 5 + 2] = toGd(vertexPositions[2]);
			positions[vertexIndex * 5 + 3] = toGd(vertexPositions[3]);
			positions[vertexIndex * 5 + 4] = toGd(vertexPositions[4]);

			normals[vertexIndex * 5 + 0] = toGd(vertexNormals[0]);
			normals[vertexIndex * 5 + 1] = toGd(vertexNormals[0]);
			normals[vertexIndex * 5 + 2] = toGd(vertexNormals[1]);
			normals[vertexIndex * 5 + 3] = toGd(vertexNormals[1]);
			normals[vertexIndex * 5 + 4] = toGd((vertexNormals[0] + vertexNormals[1]) * 0.5);

			colors[vertexIndex * 5 + 0] = toGdColor(trail.color[p]);
			colors[vertexIndex * 5 + 1] = toGdColor(trail.color[p]);
			colors[vertexIndex * 5 + 2] = toGdColor(trail.color[p + 1u]);
			colors[vertexIndex * 5 + 3] = toGdColor(trail.color[p + 1u]);
			colors[vertexIndex * 5 + 4] = toGdColor((trail.color[p] + trail.color[p + 1u]) * 0.5);

			uvs[vertexIndex * 5 + 0] = toGd(vertexTextureCoords[0]);
			uvs[vertexIndex * 5 + 1] = toGd(vertexTextureCoords[1]);
			uvs[vertexIndex * 5 + 2] = toGd(vertexTextureCoords[2]);
			uvs[vertexIndex * 5 + 3] = toGd(vertexTextureCoords[3]);
			uvs[vertexIndex * 5 + 4] = toGd(vertexTextureCoords[4]);

			uvs2[vertexIndex * 5 + 0] = Vector2(static_cast<float>(trail.life[p]), static_cast<float>(entry.first));
			uvs2[vertexIndex * 5 + 1] = Vector2(static_cast<float>(trail.life[p]), static_cast<float>(entry.first));
			uvs2[vertexIndex * 5 + 2] = Vector2(static_cast<float>(trail.life[p + 1u]), static_cast<float>(entry.first));
			uvs2[vertexIndex * 5 + 3] = Vector2(static_cast<float>(trail.life[p + 1u]), static_cast<float>(entry.first));
			uvs2[vertexIndex * 5 + 4] = Vector2(static_cast<float>(trail.life[p] + trail.life[p + 1u]) * 0.5f, static_cast<float>(entry.first));

			custom0[vertexIndex * 5 * 4 + 0] = static_cast<float>(trail.velocity[p].x);
			custom0[vertexIndex * 5 * 4 + 1] = static_cast<float>(trail.velocity[p].y);
			custom0[vertexIndex * 5 * 4 + 2] = static_cast<float>(trail.velocity[p].z);
			custom0[vertexIndex * 5 * 4 + 3] = 0.0f;
			custom0[vertexIndex * 5 * 4 + 4] = static_cast<float>(trail.velocity[p].x);
			custom0[vertexIndex * 5 * 4 + 5] = static_cast<float>(trail.velocity[p].y);
			custom0[vertexIndex * 5 * 4 + 6] = static_cast<float>(trail.velocity[p].z);
			custom0[vertexIndex * 5 * 4 + 7] = 0.0f;
			custom0[vertexIndex * 5 * 4 + 8] = static_cast<float>(trail.velocity[p + 1u].x);
			custom0[vertexIndex * 5 * 4 + 9] = static_cast<float>(trail.velocity[p + 1u].y);
			custom0[vertexIndex * 5 * 4 + 10] = static_cast<float>(trail.velocity[p + 1u].z);
			custom0[vertexIndex * 5 * 4 + 11] = 0.0f;
			custom0[vertexIndex * 5 * 4 + 12] = static_cast<float>(trail.velocity[p + 1u].x);
			custom0[vertexIndex * 5 * 4 + 13] = static_cast<float>(trail.velocity[p + 1u].y);
			custom0[vertexIndex * 5 * 4 + 14] = static_cast<float>(trail.velocity[p + 1u].z);
			custom0[vertexIndex * 5 * 4 + 15] = 0.0f;
			custom0[vertexIndex * 5 * 4 + 16] = static_cast<float>(trail.velocity[p].x + trail.velocity[p + 1u].x) * 0.5f;
			custom0[vertexIndex * 5 * 4 + 17] = static_cast<float>(trail.velocity[p].y + trail.velocity[p + 1u].y) * 0.5f;
			custom0[vertexIndex * 5 * 4 + 18] = static_cast<float>(trail.velocity[p].z + trail.velocity[p + 1u].z) * 0.5f;
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

void PixelpartParticleMesh3D::add_particle_meshes(Node3D* parentNode,
	const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
	pixelpart::ParticleReadPtr particles, uint32_t numParticles,
	pixelpart::float_t scale, pixelpart::float_t t) {
	const float packFactorA = 1000.0;
	const float packFactorB = 0.5;

	if(numParticles == 0u) {
		return;
	}

	const pixelpart::ParticleEmitter& particleEmitter = effect.particleEmitters.get(particleType.parentId);
	pixelpart::float_t alpha = std::fmod(t - particleEmitter.lifetimeStart, particleEmitter.lifetimeDuration) / particleEmitter.lifetimeDuration;

	Camera3D* camera = parentNode->get_viewport()->get_camera_3d();
	if(!camera) {
		return;
	}

	pixelpart::ParticleReadPtr particleRenderData = particles;
	if(particleType.meshRendererSettings.sortCriterion != pixelpart::ParticleSortCriterion::none) {
		particleRenderData = sort_particles(particles, numParticles, particleType.meshRendererSettings.sortCriterion, parentNode, camera);
	}

	instanceDataArray.resize(numParticles * 20);
	float* instanceData = instanceDataArray.ptrw();

	for(uint32_t p = 0u; p < numParticles; p++) {
		pixelpart::mat4_t globalScaleMatrix = glm::scale(pixelpart::vec3_t(scale));

		pixelpart::mat4_t alignmentMatrix = pixelpart::mat4_t(1.0);
		switch(particleType.alignmentMode) {
			case pixelpart::AlignmentMode::motion:
				alignmentMatrix = pixelpart::mat4_t(lookAt(particleRenderData.velocity[p]));
				break;
			case pixelpart::AlignmentMode::emission:
				alignmentMatrix = pixelpart::mat4_t(lookAt(particleEmitter.position.get(alpha) - particleRenderData.globalPosition[p]));
				break;
			case pixelpart::AlignmentMode::emitter:
				alignmentMatrix = pixelpart::mat4_t(rotation3d(particleEmitter.orientation.get(alpha)));
				break;
			default:
				break;
		}

		pixelpart::vec3_t pivot = particleType.pivot.get() * particleRenderData.size[p];
		pixelpart::mat4_t rotationMatrix =
			glm::translate(pivot) *
			pixelpart::mat4_t(rotation3d(particleRenderData.rotation[p])) *
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

		instanceData[p * 20 + 16] = toGd(particleRenderData.velocity[p].x);
		instanceData[p * 20 + 17] = toGd(particleRenderData.velocity[p].y);
		instanceData[p * 20 + 18] = toGd(particleRenderData.velocity[p].z);
		instanceData[p * 20 + 19] = packUIntFloat(particleRenderData.id[p], toGd(particleRenderData.life[p]), packFactorB);
	}

	multiMesh->set_visible_instance_count(-1);
	multiMesh->set_instance_count(static_cast<int32_t>(numParticles));
	multiMesh->set_buffer(instanceDataArray);
}

pixelpart::ParticleReadPtr PixelpartParticleMesh3D::sort_particles(pixelpart::ParticleReadPtr particles, uint32_t numParticles, pixelpart::ParticleSortCriterion sortCriterion,
	Node3D* parentNode, Camera3D* camera) {
	if(sortCriterion == pixelpart::ParticleSortCriterion::none || numParticles <= 1u) {
		return particles;
	}

	if(numParticles > sortedParticleContainer.getParticleCapacity()) {
		sortedParticleContainer = pixelpart::ParticleContainer(numParticles);
		sortKeys.resize(numParticles);
	}

	std::iota(sortKeys.begin(), sortKeys.begin() + numParticles, 0u);

	switch(sortCriterion) {
		case pixelpart::ParticleSortCriterion::age: {
			insertionSort(sortKeys.begin(), sortKeys.begin() + numParticles,
				[&particles](uint32_t i, uint32_t j) {
					return particles.id[i] < particles.id[j];
				});

			break;
		}

		case pixelpart::ParticleSortCriterion::distance: {
			pixelpart::vec3_t cameraPosition = fromGd(camera->get_global_transform().origin - parentNode->get_global_position());

			insertionSort(sortKeys.begin(), sortKeys.begin() + numParticles,
				[&particles, cameraPosition](uint32_t i, uint32_t j) {
					return glm::distance2(particles.globalPosition[i], cameraPosition) > glm::distance2(particles.globalPosition[j], cameraPosition);
				});

			break;
		}
	}

	pixelpart::ParticleWritePtr sortedParticleData = sortedParticleContainer.getParticleWritePtr();

	for(uint32_t i = 0u; i < numParticles; i++) {
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

	return sortedParticleContainer.getParticleReadPtr();
}

pixelpart::mat3_t PixelpartParticleMesh3D::rotation3d(const pixelpart::vec3_t& angle) {
	pixelpart::vec3_t rotation = glm::radians(angle);
	pixelpart::float_t cy = std::cos(rotation.y);
	pixelpart::float_t sy = std::sin(rotation.y);
	pixelpart::float_t cp = std::cos(rotation.z);
	pixelpart::float_t sp = std::sin(rotation.z);
	pixelpart::float_t cr = std::cos(rotation.x);
	pixelpart::float_t sr = std::sin(rotation.x);

	return pixelpart::mat3_t(
		pixelpart::vec3_t(cy * cr + sy * sp * sr, sr * cp, -sy * cr + cy * sp * sr),
		pixelpart::vec3_t(-cy * sr + sy * sp * cr, cr * cp, sr * sy + cy * sp * cr),
		pixelpart::vec3_t(sy * cp, -sp, cy * cp));
}
pixelpart::mat3_t PixelpartParticleMesh3D::lookAt(const pixelpart::vec3_t& direction) {
	pixelpart::vec3_t front = glm::normalize(direction);

	pixelpart::vec3_t up = pixelpart::vec3_t(0.0, 1.0, 0.0);
	if(glm::abs(glm::abs(glm::dot(front, up)) - 1.0) < 0.001) {
		up = pixelpart::vec3_t(1.0, 0.0, 0.0);
	}

	pixelpart::vec3_t right = glm::normalize(glm::cross(front, up));
	up = glm::normalize(glm::cross(right, front));

	return pixelpart::mat3_t(right, up, -front);
}
}