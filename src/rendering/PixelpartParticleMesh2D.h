#ifndef PIXELPART_PARTICLE_MESH_2D_H
#define PIXELPART_PARTICLE_MESH_2D_H

#include "PixelpartGraphicsResourceStore.h"
#include "PixelpartShaderGenerator.h"
#include <effect/Effect.h>
#include <engine/ParticleData.h>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/shader.hpp>

namespace godot {
class PixelpartParticleMesh2D {
public:
	PixelpartParticleMesh2D(PixelpartGraphicsResourceStore& resourceStore, PixelpartShaderGenerator& shaderGenerator, const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType);
	PixelpartParticleMesh2D(const PixelpartParticleMesh2D&) = delete;
	~PixelpartParticleMesh2D();

	PixelpartParticleMesh2D& operator=(const PixelpartParticleMesh2D&) = delete;

	void draw(Node2D* parentNode,
		const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
		pixelpart::ParticleReadPtr particles, uint32_t numParticles,
		pixelpart::float_t scaleX, pixelpart::float_t scaleY, pixelpart::float_t t);

private:
	struct ParticleTrailData {
		uint32_t numParticles = 0u;
		pixelpart::float_t length = 0.0;

		std::vector<pixelpart::vec3_t> position;
		std::vector<pixelpart::vec3_t> size;
		std::vector<pixelpart::vec4_t> color;
		std::vector<pixelpart::vec3_t> velocity;
		std::vector<pixelpart::float_t> life;
		std::vector<pixelpart::vec3_t> direction;
		std::vector<pixelpart::float_t> index;

		PackedInt32Array indexArray;
		PackedVector2Array positionArray;
		PackedVector2Array textureCoordArray;
		PackedColorArray colorArray;
	};

	void add_particle_sprites(const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
		pixelpart::ParticleReadPtr particles, uint32_t numParticles,
		pixelpart::float_t scaleX, pixelpart::float_t scaleY, pixelpart::float_t t);
	void add_particle_trails(const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
		pixelpart::ParticleReadPtr particles, uint32_t numParticles,
		pixelpart::float_t scaleX, pixelpart::float_t scaleY, pixelpart::float_t t);

	pixelpart::vec3_t rotate2d(const pixelpart::vec3_t& p, const pixelpart::vec3_t& o, pixelpart::float_t a);
	pixelpart::mat3_t rotation3d(const pixelpart::vec3_t& angle);

	PixelpartGraphicsResourceStore& resources;

	RID canvasItemRID;

	RID materialRID;
	Ref<Shader> shader;
	std::unordered_map<pixelpart::id_t, std::string> shaderParameterNames;
	std::unordered_map<std::string, std::string> textureResourceIds;

	PackedInt32Array indexArray;
	PackedVector2Array positionArray;
	PackedVector2Array textureCoordArray;
	PackedColorArray colorArray;

	std::unordered_map<uint32_t, ParticleTrailData> trails;
};
}

#endif