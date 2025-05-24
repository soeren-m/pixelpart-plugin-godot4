#ifndef PIXELPART_PARTICLE_RENDERER_2D_H
#define PIXELPART_PARTICLE_RENDERER_2D_H

#include "PixelpartGraphicsResourceProvider.h"
#include "PixelpartShaderProvider.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <pixelpart-runtime/common/Math.h>
#include <pixelpart-runtime/common/Id.h>
#include <pixelpart-runtime/common/Curve.h>
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
class PixelpartParticleRenderer2D {
public:
	PixelpartParticleRenderer2D(PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider,
		const pixelpart::Effect& eff, pixelpart::id_t emitterId, pixelpart::id_t typeId);
	PixelpartParticleRenderer2D(const PixelpartParticleRenderer2D&) = delete;
	~PixelpartParticleRenderer2D();

	PixelpartParticleRenderer2D& operator=(const PixelpartParticleRenderer2D&) = delete;

	void draw(Node2D* parentNode,
		pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
		const pixelpart::RuntimeContext& runtimeContext, pixelpart::float2_t scale);

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
		std::vector<pixelpart::float_t> index;

		PackedInt32Array indexArray;
		PackedVector2Array vertexArray;
		PackedVector2Array uvArray;
		PackedColorArray colorArray;
		PackedFloat32Array custom0Array;
	};

	void add_particle_sprites(
		const pixelpart::ParticleEmitter& particleEmitter,
		const pixelpart::ParticleType& particleType,
		pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
		const pixelpart::RuntimeContext& runtimeContext, pixelpart::float2_t scale);
	void add_particle_trails(
		const pixelpart::ParticleEmitter& particleEmitter,
		const pixelpart::ParticleType& particleType,
		pixelpart::ParticleCollection::ReadPtr particles, uint32_t particleCount,
		const pixelpart::RuntimeContext& runtimeContext, pixelpart::float2_t scale);

	static pixelpart::float3_t rotate_2d(const pixelpart::float3_t& p, const pixelpart::float3_t& o, pixelpart::float_t a);
	static pixelpart::mat3_t rotation_3d(const pixelpart::float3_t& angle);

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

	RID canvasItemRID;

	RID materialRID;
	Ref<Shader> shader;
	std::unordered_map<pixelpart::id_t, std::string> shaderParameterNames;
	std::unordered_map<std::string, std::string> textureResourceIds;

	RID meshRID;
	PackedInt32Array indexArray;
	PackedVector2Array vertexArray;
	PackedVector2Array uvArray;
	PackedColorArray colorArray;
	PackedFloat32Array custom0Array;

	std::unordered_map<uint32_t, ParticleTrailData> trails;
};
}

#endif