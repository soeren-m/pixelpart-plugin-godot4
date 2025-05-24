#ifndef PIXELPART_PARTICLE_RENDERER_3D_H
#define PIXELPART_PARTICLE_RENDERER_3D_H

#include "PixelpartGraphicsResourceProvider.h"
#include "PixelpartShaderProvider.h"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <pixelpart-runtime/common/Math.h>
#include <pixelpart-runtime/common/Id.h>
#include <pixelpart-runtime/effect/Effect.h>
#include <pixelpart-runtime/effect/ParticleEmitter.h>
#include <pixelpart-runtime/effect/ParticleType.h>
#include <pixelpart-runtime/effect/RuntimeContext.h>
#include <pixelpart-runtime/engine/ParticleCollection.h>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

namespace godot {
class PixelpartParticleRenderer3D {
public:
	PixelpartParticleRenderer3D(PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider,
		const pixelpart::Effect& eff, pixelpart::id_t emitterId, pixelpart::id_t typeId);
	PixelpartParticleRenderer3D(const PixelpartParticleRenderer3D&) = delete;
	~PixelpartParticleRenderer3D();

	PixelpartParticleRenderer3D& operator=(const PixelpartParticleRenderer3D&) = delete;

	void draw(Node3D* parentNode,
		pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
		const pixelpart::RuntimeContext& runtimeContext, pixelpart::float_t scale);

private:
	struct ParticleTrailData {
		uint32_t particleCount = 0;
		pixelpart::float_t length = 0.0;

		std::vector<pixelpart::float3_t> position;
		std::vector<pixelpart::float3_t> size;
		std::vector<pixelpart::float4_t> color;
		std::vector<pixelpart::float3_t> velocity;
		std::vector<pixelpart::float_t> life;
		std::vector<pixelpart::float3_t> direction;
		std::vector<pixelpart::float3_t> directionToEdge;
		std::vector<pixelpart::float_t> index;
	};

	void add_particle_sprites(Node3D* parentNode,
		const pixelpart::ParticleEmitter& particleEmitter,
		const pixelpart::ParticleType& particleType,
		pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
		const pixelpart::RuntimeContext& runtimeContext, pixelpart::float_t scale);
	void add_particle_trails(Node3D* parentNode,
		const pixelpart::ParticleEmitter& particleEmitter,
		const pixelpart::ParticleType& particleType,
		pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
		const pixelpart::RuntimeContext& runtimeContext, pixelpart::float_t scale);
	void add_particle_meshes(Node3D* parentNode,
		const pixelpart::ParticleEmitter& particleEmitter,
		const pixelpart::ParticleType& particleType,
		pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
		const pixelpart::RuntimeContext& runtimeContext, pixelpart::float_t scale);

	pixelpart::ParticleCollection::ReadPtr sort_particles(
		pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
		pixelpart::ParticleSortCriterion sortCriterion,
		Node3D* parentNode, Camera3D* camera);

	static pixelpart::mat3_t rotation_3d(const pixelpart::float3_t& angle);
	static pixelpart::mat3_t look_at(const pixelpart::float3_t& direction);

	template <typename T>
	static std::vector<typename pixelpart::Curve<T>::Point> ordered_points(const pixelpart::float_t* positionList, const T* valueList, const uint32_t* order, uint32_t count) {
		std::vector<typename pixelpart::Curve<T>::Point> result;
		result.resize(count);
		for(uint32_t index = 0; index < count; index++) {
			result[index] = typename pixelpart::Curve<T>::Point{ positionList[index], valueList[order[index]] };
		}

		return result;
	}

	PixelpartGraphicsResourceProvider& resources;

	const pixelpart::Effect& effect;
	pixelpart::id_t particleEmitterId;
	pixelpart::id_t particleTypeId;

	RID instanceRID;

	Ref<ArrayMesh> arrayMesh;
	Ref<MultiMesh> multiMesh;
	Ref<Shader> shader;
	Ref<ShaderMaterial> shaderMaterial;
	std::unordered_map<pixelpart::id_t, std::string> shaderParameterNames;
	std::unordered_map<std::string, std::string> textureResourceIds;

	pixelpart::ParticleCollection sortedParticleCollection;
	std::vector<uint32_t> sortKeys;

	PackedInt32Array indexArray;
	PackedVector3Array vertexArray;
	PackedVector3Array normalArray;
	PackedVector2Array uvArray;
	PackedVector2Array uv2Array;
	PackedColorArray colorArray;
	PackedFloat32Array custom0Array;
	PackedFloat32Array instanceDataArray;

	std::unordered_map<uint32_t, ParticleTrailData> trails;
};
}

#endif