#include "PixelpartParticleMesh2D.h"
#include "../util/PixelpartUtil.h"
#include <glm/glm/gtx/vector_angle.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {
PixelpartParticleMesh2D::PixelpartParticleMesh2D(PixelpartGraphicsResourceStore& resourceStore, PixelpartShaderGenerator& shaderGenerator, const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType) :
	resources(resourceStore) {
	RenderingServer* rs = RenderingServer::get_singleton();

	if(particleType.materialInstance.isBuiltInMaterial && shaderGenerator.has_builtin_canvas_item_shader(particleType.materialInstance.materialId)) {
		shader = shaderGenerator.get_builtin_canvas_item_shader(particleType.materialInstance.materialId);

		const PixelpartShaderGenerator::ShaderMetadata& metadata = shaderGenerator.get_builtin_canvas_item_shader_metadata(particleType.materialInstance.materialId);

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

		shader = shaderGenerator.get_custom_canvas_item_shader(
			shaderBuildResult.mainCode, shaderBuildResult.parameterCode,
			particleType.renderer, materialResource.blendMode, materialResource.lightingMode);
	}
	else {
		UtilityFunctions::push_warning("Could not create particle material");
	}

	materialRID = rs->material_create();
	if(shader.is_valid()) {
		rs->material_set_shader(materialRID, shader->get_rid());
	}

	canvasItemRID = rs->canvas_item_create();
	rs->canvas_item_set_material(canvasItemRID, materialRID);
}
PixelpartParticleMesh2D::~PixelpartParticleMesh2D() {
	RenderingServer* rs = RenderingServer::get_singleton();

	rs->free_rid(materialRID);
	rs->free_rid(canvasItemRID);
}

void PixelpartParticleMesh2D::draw(Node2D* parentNode,
	const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
	pixelpart::ParticleReadPtr particles, uint32_t numParticles,
	pixelpart::float_t scaleX, pixelpart::float_t scaleY, pixelpart::float_t t) {
	RenderingServer* rs = RenderingServer::get_singleton();

	rs->canvas_item_clear(canvasItemRID);

	if(!parentNode->is_visible() || !particleType.visible) {
		return;
	}

	const pixelpart::ParticleEmitter& particleEmitter = effect.particleEmitters.get(particleType.parentId);

	for(const auto& textureParameterEntry : textureResourceIds) {
		if(resources.textures.count(textureParameterEntry.second) != 0u) {
			rs->material_set_param(materialRID, StringName(textureParameterEntry.first.c_str()), resources.textures.at(textureParameterEntry.second)->get_rid());
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
				rs->material_set_param(materialRID, paramName, toGd(parameterValue.getInt()));
				break;
			}
			case pixelpart::VariantParameter::Value::type_float: {
				rs->material_set_param(materialRID, paramName, toGd(parameterValue.getFloat()));
				break;
			}
			case pixelpart::VariantParameter::Value::type_float2: {
				rs->material_set_param(materialRID, paramName, toGd(parameterValue.getFloat2()));
				break;
			}
			case pixelpart::VariantParameter::Value::type_float3: {
				rs->material_set_param(materialRID, paramName, toGd(parameterValue.getFloat3()));
				break;
			}
			case pixelpart::VariantParameter::Value::type_float4: {
				rs->material_set_param(materialRID, paramName, toGd(parameterValue.getFloat4()));
				break;
			}
			case pixelpart::VariantParameter::Value::type_bool: {
				rs->material_set_param(materialRID, paramName, parameterValue.getBool());
				break;
			}
			case pixelpart::VariantParameter::Value::type_enum: {
				rs->material_set_param(materialRID, paramName, toGd(parameterValue.getEnum()));
				break;
			}
			case pixelpart::VariantParameter::Value::type_color: {
				rs->material_set_param(materialRID, paramName, toGdColor(parameterValue.getColor()));
				break;
			}
			case pixelpart::VariantParameter::Value::type_resource_image: {
				if(resources.textures.count(parameterValue.getResourceId()) != 0u) {
					rs->material_set_param(materialRID, paramName, resources.textures.at(parameterValue.getResourceId())->get_rid());
				}
				break;
			}
			default: {
				break;
			}
		}
	}

	rs->material_set_param(materialRID, "u_EffectTime", static_cast<float>(t));
	rs->material_set_param(materialRID, "u_ObjectTime", static_cast<float>(t - particleEmitter.lifetimeStart));

	rs->canvas_item_set_parent(canvasItemRID, parentNode->get_canvas_item());
	rs->canvas_item_set_transform(canvasItemRID, Transform2D());

	switch(particleType.renderer) {
		case pixelpart::ParticleRendererType::sprite:
			add_particle_sprites(effect, particleType, particles, numParticles, scaleX, scaleY, t);
			break;
		case pixelpart::ParticleRendererType::trail:
			add_particle_trails(effect, particleType, particles, numParticles, scaleX, scaleY, t);
			break;
		default:
			break;
	}
}

void PixelpartParticleMesh2D::add_particle_sprites(const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
	pixelpart::ParticleReadPtr particles, uint32_t numParticles,
	pixelpart::float_t scaleX, pixelpart::float_t scaleY, pixelpart::float_t t) {
	const pixelpart::vec2_t scale = pixelpart::vec2_t(scaleX, scaleY);
	const float packFactorA = 1000.0;
	const float packFactorB = 0.5;

	if(numParticles == 0u) {
		return;
	}

	const pixelpart::ParticleEmitter& particleEmitter = effect.particleEmitters.get(particleType.parentId);
	pixelpart::float_t alpha = std::fmod(t - particleEmitter.lifetimeStart, particleEmitter.lifetimeDuration) / particleEmitter.lifetimeDuration;

	indexArray.resize(numParticles * 6);
	positionArray.resize(numParticles * 4);
	textureCoordArray.resize(numParticles * 4);
	colorArray.resize(numParticles * 4);

	int32_t* indices = indexArray.ptrw();
	for(int32_t p = 0; p < static_cast<int32_t>(numParticles); p++) {
		indices[p * 6 + 0] = p * 4 + 0;
		indices[p * 6 + 1] = p * 4 + 1;
		indices[p * 6 + 2] = p * 4 + 3;
		indices[p * 6 + 3] = p * 4 + 1;
		indices[p * 6 + 4] = p * 4 + 2;
		indices[p * 6 + 5] = p * 4 + 3;
	}

	Vector2* positions = positionArray.ptrw();
	for(uint32_t p = 0u; p < numParticles; p++) {
		pixelpart::vec3_t worldPosition[4];

		switch(particleType.alignmentMode) {
			case pixelpart::AlignmentMode::motion: {
				pixelpart::float_t angle = glm::degrees(glm::orientedAngle(pixelpart::vec2_t(0.0, 1.0), (particles.velocity[p] != pixelpart::vec3_t(0.0))
					? pixelpart::vec2_t(glm::normalize(particles.velocity[p]))
					: pixelpart::vec2_t(0.0, 1.0)));
				worldPosition[0] = particles.globalPosition[p] + rotate2d(pixelpart::vec3_t(-0.5, -0.5, 0.0) * particles.size[p], particleType.pivot.get() * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[1] = particles.globalPosition[p] + rotate2d(pixelpart::vec3_t(-0.5, +0.5, 0.0) * particles.size[p], particleType.pivot.get() * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[2] = particles.globalPosition[p] + rotate2d(pixelpart::vec3_t(+0.5, +0.5, 0.0) * particles.size[p], particleType.pivot.get() * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[3] = particles.globalPosition[p] + rotate2d(pixelpart::vec3_t(+0.5, -0.5, 0.0) * particles.size[p], particleType.pivot.get() * particles.size[p], particles.rotation[p].x + angle);
				break;
			}
			case pixelpart::AlignmentMode::emission: {
				pixelpart::vec3_t emissionDirection = particleEmitter.position.get(alpha) - particles.globalPosition[p];
				pixelpart::float_t angle = glm::degrees(glm::orientedAngle(pixelpart::vec2_t(0.0, 1.0), (emissionDirection != pixelpart::vec3_t(0.0))
					? pixelpart::vec2_t(glm::normalize(emissionDirection))
					: pixelpart::vec2_t(0.0, 1.0)));
				worldPosition[0] = particles.globalPosition[p] + rotate2d(pixelpart::vec3_t(-0.5, -0.5, 0.0) * particles.size[p], particleType.pivot.get() * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[1] = particles.globalPosition[p] + rotate2d(pixelpart::vec3_t(-0.5, +0.5, 0.0) * particles.size[p], particleType.pivot.get() * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[2] = particles.globalPosition[p] + rotate2d(pixelpart::vec3_t(+0.5, +0.5, 0.0) * particles.size[p], particleType.pivot.get() * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[3] = particles.globalPosition[p] + rotate2d(pixelpart::vec3_t(+0.5, -0.5, 0.0) * particles.size[p], particleType.pivot.get() * particles.size[p], particles.rotation[p].x + angle);
				break;
			}
			case pixelpart::AlignmentMode::emitter: {
				pixelpart::mat3_t rotationMatrix = rotation3d(particles.rotation[p]);
				pixelpart::mat3_t alignmentMatrix = rotation3d(particleEmitter.orientation.get(alpha));
				worldPosition[0] = particles.globalPosition[p] + alignmentMatrix * (rotationMatrix * (pixelpart::vec3_t(-0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot.get() * particles.size[p]) + particleType.pivot.get() * particles.size[p]);
				worldPosition[1] = particles.globalPosition[p] + alignmentMatrix * (rotationMatrix * (pixelpart::vec3_t(-0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot.get() * particles.size[p]) + particleType.pivot.get() * particles.size[p]);
				worldPosition[2] = particles.globalPosition[p] + alignmentMatrix * (rotationMatrix * (pixelpart::vec3_t(+0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot.get() * particles.size[p]) + particleType.pivot.get() * particles.size[p]);
				worldPosition[3] = particles.globalPosition[p] + alignmentMatrix * (rotationMatrix * (pixelpart::vec3_t(+0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot.get() * particles.size[p]) + particleType.pivot.get() * particles.size[p]);
				break;
			}
			default: {
				pixelpart::mat3_t rotationMatrix = rotation3d(particles.rotation[p]);
				worldPosition[0] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3_t(-0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot.get() * particles.size[p]) + particleType.pivot.get() * particles.size[p]);
				worldPosition[1] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3_t(-0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot.get() * particles.size[p]) + particleType.pivot.get() * particles.size[p]);
				worldPosition[2] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3_t(+0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot.get() * particles.size[p]) + particleType.pivot.get() * particles.size[p]);
				worldPosition[3] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3_t(+0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot.get() * particles.size[p]) + particleType.pivot.get() * particles.size[p]);
				break;
			}
		}

		positions[p * 4 + 0] = toGd(pixelpart::vec2_t(worldPosition[0]) * scale);
		positions[p * 4 + 1] = toGd(pixelpart::vec2_t(worldPosition[1]) * scale);
		positions[p * 4 + 2] = toGd(pixelpart::vec2_t(worldPosition[2]) * scale);
		positions[p * 4 + 3] = toGd(pixelpart::vec2_t(worldPosition[3]) * scale);
	}

	float* textureCoords = reinterpret_cast<float*>(textureCoordArray.ptrw());
	for(uint32_t p = 0u; p < numParticles; p++) {
		textureCoords[p * 4 * 2 + 0] = packFloatsUnsigned(toGd(particles.life[p]), 0.0f, packFactorA, packFactorB);
		textureCoords[p * 4 * 2 + 1] = packUIntFloat(particles.id[p], 0.0f, packFactorB);
		textureCoords[p * 4 * 2 + 2] = packFloatsUnsigned(toGd(particles.life[p]), 0.0f, packFactorA, packFactorB);
		textureCoords[p * 4 * 2 + 3] = packUIntFloat(particles.id[p], 1.0f, packFactorB);
		textureCoords[p * 4 * 2 + 4] = packFloatsUnsigned(toGd(particles.life[p]), 1.0f, packFactorA, packFactorB);
		textureCoords[p * 4 * 2 + 5] = packUIntFloat(particles.id[p], 1.0f, packFactorB);
		textureCoords[p * 4 * 2 + 6] = packFloatsUnsigned(toGd(particles.life[p]), 1.0f, packFactorA, packFactorB);
		textureCoords[p * 4 * 2 + 7] = packUIntFloat(particles.id[p], 0.0f, packFactorB);
	}

	float* colors = reinterpret_cast<float*>(colorArray.ptrw());
	for(uint32_t p = 0u; p < numParticles; p++) {
		float x = packFloatsSignedUnsigned(toGd(particles.velocity[p].x), toGd(particles.color[p].r), packFactorA, packFactorB);
		float y = packFloatsSignedUnsigned(toGd(particles.velocity[p].y), toGd(particles.color[p].g), packFactorA, packFactorB);
		float z = packFloatsSignedUnsigned(toGd(particles.velocity[p].z), toGd(particles.color[p].b), packFactorA, packFactorB);
		float w = packFloatsSignedUnsigned(0.0f, toGd(particles.color[p].a), packFactorA, packFactorB);

		colors[p * 4 * 4 + 0] = x;
		colors[p * 4 * 4 + 1] = y;
		colors[p * 4 * 4 + 2] = z;
		colors[p * 4 * 4 + 3] = w;
		colors[p * 4 * 4 + 4] = x;
		colors[p * 4 * 4 + 5] = y;
		colors[p * 4 * 4 + 6] = z;
		colors[p * 4 * 4 + 7] = w;
		colors[p * 4 * 4 + 8] = x;
		colors[p * 4 * 4 + 9] = y;
		colors[p * 4 * 4 + 10] = z;
		colors[p * 4 * 4 + 11] = w;
		colors[p * 4 * 4 + 12] = x;
		colors[p * 4 * 4 + 13] = y;
		colors[p * 4 * 4 + 14] = z;
		colors[p * 4 * 4 + 15] = w;
	}

	RenderingServer* rs = RenderingServer::get_singleton();
	rs->canvas_item_add_triangle_array(canvasItemRID,
		indexArray,
		positionArray,
		colorArray,
		textureCoordArray);
}

void PixelpartParticleMesh2D::add_particle_trails(const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
	pixelpart::ParticleReadPtr particles, uint32_t numParticles,
	pixelpart::float_t scaleX, pixelpart::float_t scaleY, pixelpart::float_t t) {
	const pixelpart::float_t epsilon = 0.000001;
	const float packFactorA = 1000.0;
	const float packFactorB = 0.5;

	if(numParticles < 2u) {
		return;
	}

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

	RenderingServer* rs = RenderingServer::get_singleton();

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
				trail.direction[i] = pixelpart::vec3_t(
					-trail.direction[i].y,
					trail.direction[i].x,
					trail.direction[i].z);
			}
			else {
				trail.direction[i] = (glm::dot(glm::cross(trail.direction[i], pixelpart::vec3_t(0.0, 0.0, 1.0)), toEdge / toEdgeLength) < 0.0)
					? +toEdge / toEdgeLength
					: -toEdge / toEdgeLength;
			}
		}

		trail.direction[0u] = trail.direction[1u];

		for(pixelpart::float_t& index : trail.index) {
			index /= trail.length;
		}

		int32_t numTrailSegments = static_cast<int32_t>(trail.position.size()) - 1;
		trail.indexArray.resize(numTrailSegments * 12);
		trail.positionArray.resize(numTrailSegments * 5);
		trail.textureCoordArray.resize(numTrailSegments * 5);
		trail.colorArray.resize(numTrailSegments * 5);

		int32_t* indices = trail.indexArray.ptrw();
		for(int32_t p = 0; p < numTrailSegments; p++) {
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

		float* positions = reinterpret_cast<float*>(trail.positionArray.ptrw());
		for(int32_t p = 0; p < numTrailSegments; p++) {
			pixelpart::vec3_t startToEdge = trail.direction[p] * std::max(trail.size[p].x, trail.size[p].y) * 0.5;
			pixelpart::vec3_t endToEdge = trail.direction[p + 1] * std::max(trail.size[p + 1].x, trail.size[p + 1].y) * 0.5;

			pixelpart::vec3_t vertexPositions[5] = {
				trail.position[p] + startToEdge,
				trail.position[p] - startToEdge,
				trail.position[p + 1] + endToEdge,
				trail.position[p + 1] - endToEdge,
				(trail.position[p] + trail.position[p + 1]) * 0.5
			};

			positions[p * 5 * 2 + 0] = toGd(vertexPositions[0].x * scaleX);
			positions[p * 5 * 2 + 1] = toGd(vertexPositions[0].y * scaleY);
			positions[p * 5 * 2 + 2] = toGd(vertexPositions[1].x * scaleX);
			positions[p * 5 * 2 + 3] = toGd(vertexPositions[1].y * scaleY);
			positions[p * 5 * 2 + 4] = toGd(vertexPositions[2].x * scaleX);
			positions[p * 5 * 2 + 5] = toGd(vertexPositions[2].y * scaleY);
			positions[p * 5 * 2 + 6] = toGd(vertexPositions[3].x * scaleX);
			positions[p * 5 * 2 + 7] = toGd(vertexPositions[3].y * scaleY);
			positions[p * 5 * 2 + 8] = toGd(vertexPositions[4].x * scaleX);
			positions[p * 5 * 2 + 9] = toGd(vertexPositions[4].y * scaleY);
		}

		float* textureCoords = reinterpret_cast<float*>(trail.textureCoordArray.ptrw());
		switch(particleType.trailRendererSettings.textureRotation) {
			case pixelpart::ParticleTrailRendererSettings::TextureRotation::left:
				for(int32_t p = 0; p < numTrailSegments; p++) {
					textureCoords[p * 5 * 2 + 0] = toGd(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 1] = 1.0f;
					textureCoords[p * 5 * 2 + 2] = toGd(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 3] = 0.0f;
					textureCoords[p * 5 * 2 + 4] = toGd(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 5] = 1.0f;
					textureCoords[p * 5 * 2 + 6] = toGd(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 7] = 0.0f;
					textureCoords[p * 5 * 2 + 8] = toGd((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 9] = 0.5f;
				}
				break;

			case pixelpart::ParticleTrailRendererSettings::TextureRotation::down:
				for(int32_t p = 0; p < numTrailSegments; p++) {
					textureCoords[p * 5 * 2 + 0] = 1.0f;
					textureCoords[p * 5 * 2 + 1] = 1.0f - toGd(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 2] = 0.0f;
					textureCoords[p * 5 * 2 + 3] = 1.0f - toGd(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 4] = 1.0f;
					textureCoords[p * 5 * 2 + 5] = 1.0f - toGd(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 6] = 0.0f;
					textureCoords[p * 5 * 2 + 7] = 1.0f - toGd(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 8] = 0.5f;
					textureCoords[p * 5 * 2 + 9] = 1.0f - toGd((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor);
				}
				break;

			case pixelpart::ParticleTrailRendererSettings::TextureRotation::right:
				for(int32_t p = 0; p < numTrailSegments; p++) {
					textureCoords[p * 5 * 2 + 0] = 1.0f - toGd(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 1] = 0.0f;
					textureCoords[p * 5 * 2 + 2] = 1.0f - toGd(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 3] = 1.0f;
					textureCoords[p * 5 * 2 + 4] = 1.0f - toGd(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 5] = 0.0f;
					textureCoords[p * 5 * 2 + 6] = 1.0f - toGd(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 7] = 1.0f;
					textureCoords[p * 5 * 2 + 8] = 1.0f - toGd((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 9] = 0.5f;
				}
				break;

			default:
				for(int32_t p = 0; p < numTrailSegments; p++) {
					textureCoords[p * 5 * 2 + 0] = 0.0f;
					textureCoords[p * 5 * 2 + 1] = toGd(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 2] = 1.0f;
					textureCoords[p * 5 * 2 + 3] = toGd(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 4] = 0.0f;
					textureCoords[p * 5 * 2 + 5] = toGd(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 6] = 1.0f;
					textureCoords[p * 5 * 2 + 7] = toGd(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 8] = 0.5f;
					textureCoords[p * 5 * 2 + 9] = toGd((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor);
				}
				break;
		}

		for(int32_t p = 0; p < numTrailSegments; p++) {
			textureCoords[p * 5 * 2 + 0] = packFloatsUnsigned(toGd(particles.life[p]), textureCoords[p * 5 * 2 + 0], packFactorA, packFactorB);
			textureCoords[p * 5 * 2 + 1] = packUIntFloat(entry.first, textureCoords[p * 5 * 2 + 1], packFactorB);
			textureCoords[p * 5 * 2 + 2] = packFloatsUnsigned(toGd(particles.life[p]), textureCoords[p * 5 * 2 + 2], packFactorA, packFactorB);
			textureCoords[p * 5 * 2 + 3] = packUIntFloat(entry.first, textureCoords[p * 5 * 2 + 3], packFactorB);
			textureCoords[p * 5 * 2 + 4] = packFloatsUnsigned(toGd(particles.life[p + 1]), textureCoords[p * 5 * 2 + 4], packFactorA, packFactorB);
			textureCoords[p * 5 * 2 + 5] = packUIntFloat(entry.first, textureCoords[p * 5 * 2 + 5], packFactorB);
			textureCoords[p * 5 * 2 + 6] = packFloatsUnsigned(toGd(particles.life[p + 1]), textureCoords[p * 5 * 2 + 6], packFactorA, packFactorB);
			textureCoords[p * 5 * 2 + 7] = packUIntFloat(entry.first, textureCoords[p * 5 * 2 + 7], packFactorB);
			textureCoords[p * 5 * 2 + 8] = packFloatsUnsigned(toGd(particles.life[p] + particles.life[p + 1]) * 0.5, textureCoords[p * 5 * 2 + 8], packFactorA, packFactorB);
			textureCoords[p * 5 * 2 + 9] = packUIntFloat(entry.first, textureCoords[p * 5 * 2 + 9], packFactorB);
		}

		float* colors = reinterpret_cast<float*>(trail.colorArray.ptrw());
		for(int32_t p = 0; p < numTrailSegments; p++) {
			float x1 = packFloatsSignedUnsigned(toGd(trail.velocity[p].x), toGd(trail.color[p].r), packFactorA, packFactorB);
			float y1 = packFloatsSignedUnsigned(toGd(trail.velocity[p].y), toGd(trail.color[p].g), packFactorA, packFactorB);
			float z1 = packFloatsSignedUnsigned(toGd(trail.velocity[p].z), toGd(trail.color[p].b), packFactorA, packFactorB);
			float w1 = packFloatsSignedUnsigned(0.0f, toGd(trail.color[p].a), packFactorA, packFactorB);
			float x2 = packFloatsSignedUnsigned(toGd(trail.velocity[p + 1].x), toGd(trail.color[p + 1].r), packFactorA, packFactorB);
			float y2 = packFloatsSignedUnsigned(toGd(trail.velocity[p + 1].y), toGd(trail.color[p + 1].g), packFactorA, packFactorB);
			float z2 = packFloatsSignedUnsigned(toGd(trail.velocity[p + 1].z), toGd(trail.color[p + 1].b), packFactorA, packFactorB);
			float w2 = packFloatsSignedUnsigned(0.0f, toGd(trail.color[p + 1].a), packFactorA, packFactorB);
			float x3 = packFloatsSignedUnsigned(toGd(trail.velocity[p].x + trail.velocity[p + 1].x) * 0.5, toGd(trail.color[p].r + trail.color[p + 1].r) * 0.5, packFactorA, packFactorB);
			float y3 = packFloatsSignedUnsigned(toGd(trail.velocity[p].y + trail.velocity[p + 1].y) * 0.5, toGd(trail.color[p].g + trail.color[p + 1].g) * 0.5, packFactorA, packFactorB);
			float z3 = packFloatsSignedUnsigned(toGd(trail.velocity[p].z + trail.velocity[p + 1].z) * 0.5, toGd(trail.color[p].b + trail.color[p + 1].b) * 0.5, packFactorA, packFactorB);
			float w3 = packFloatsSignedUnsigned(0.0f, toGd(trail.color[p].a + trail.color[p + 1].a) * 0.5, packFactorA, packFactorB);

			colors[p * 5 * 4 + 0] = x1;
			colors[p * 5 * 4 + 1] = y1;
			colors[p * 5 * 4 + 2] = z1;
			colors[p * 5 * 4 + 3] = w1;
			colors[p * 5 * 4 + 4] = x1;
			colors[p * 5 * 4 + 5] = y1;
			colors[p * 5 * 4 + 6] = z1;
			colors[p * 5 * 4 + 7] = w1;
			colors[p * 5 * 4 + 8] = x2;
			colors[p * 5 * 4 + 9] = y2;
			colors[p * 5 * 4 + 10] = z2;
			colors[p * 5 * 4 + 11] = w2;
			colors[p * 5 * 4 + 12] = x2;
			colors[p * 5 * 4 + 13] = y2;
			colors[p * 5 * 4 + 14] = z2;
			colors[p * 5 * 4 + 15] = w2;
			colors[p * 5 * 4 + 16] = x3;
			colors[p * 5 * 4 + 17] = y3;
			colors[p * 5 * 4 + 18] = z3;
			colors[p * 5 * 4 + 19] = w3;
		}

		rs->canvas_item_add_triangle_array(canvasItemRID,
			trail.indexArray,
			trail.positionArray,
			trail.colorArray,
			trail.textureCoordArray);
	}
}

pixelpart::vec3_t PixelpartParticleMesh2D::rotate2d(const pixelpart::vec3_t& p, const pixelpart::vec3_t& o, pixelpart::float_t a) {
	pixelpart::float_t s = std::sin(glm::radians(a));
	pixelpart::float_t c = std::cos(glm::radians(a));

	return pixelpart::vec3_t(
		(p.x - o.x) * c - (p.y - o.y) * s + o.x,
		(p.x - o.x) * s + (p.y - o.y) * c + o.y,
		0.0);
}
pixelpart::mat3_t PixelpartParticleMesh2D::rotation3d(const pixelpart::vec3_t& angle) {
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
}