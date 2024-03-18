#ifndef PIXELPART_PARTICLE_MESH_3D_H
#define PIXELPART_PARTICLE_MESH_3D_H

#include "PixelpartGraphicsResourceStore.h"
#include <effect/Effect.h>
#include <engine/ParticleData.h>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/camera3d.hpp>

namespace godot {
class PixelpartParticleMesh3D {
public:
	struct ParticleTrail {
		uint32_t numParticles = 0u;
		pixelpart::float_t length = 0.0;

		std::vector<pixelpart::vec3_t> position;
		std::vector<pixelpart::vec3_t> size;
		std::vector<pixelpart::vec4_t> color;
		std::vector<pixelpart::vec3_t> velocity;
		std::vector<pixelpart::vec3_t> force;
		std::vector<pixelpart::float_t> life;
		std::vector<pixelpart::vec3_t> direction;
		std::vector<pixelpart::vec3_t> directionToEdge;
		std::vector<pixelpart::float_t> index;
	};

	PixelpartParticleMesh3D(PixelpartGraphicsResourceStore& resourceStore, const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType);
	PixelpartParticleMesh3D(const PixelpartParticleMesh3D&) = delete;
	~PixelpartParticleMesh3D();

	PixelpartParticleMesh3D& operator=(const PixelpartParticleMesh3D&) = delete;

	void draw(Node3D* parentNode,
		const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
		const pixelpart::ParticleData& particles, uint32_t numParticles,
		pixelpart::float_t scale, pixelpart::float_t t);

private:
	void add_particle_sprites(Node3D* parentNode,
		const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
		const pixelpart::ParticleData& particles, uint32_t numParticles,
		pixelpart::float_t scale, pixelpart::float_t t);
	void add_particle_trails(Node3D* parentNode,
		const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
		const pixelpart::ParticleData& particles, uint32_t numParticles,
		pixelpart::float_t scale, pixelpart::float_t t);
	void add_particle_meshes(Node3D* parentNode,
		const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
		const pixelpart::ParticleData& particles, uint32_t numParticles,
		pixelpart::float_t scale, pixelpart::float_t t);

	const pixelpart::ParticleData* sort_particles(const pixelpart::ParticleData& particles, uint32_t numParticles, pixelpart::ParticleSortCriterion sortCriterion, Camera3D* camera);

	pixelpart::mat3_t rotation3d(const pixelpart::vec3_t& angle);
	pixelpart::mat3_t lookAt(const pixelpart::vec3_t& direction);

	PixelpartGraphicsResourceStore& resources;

	RID instanceRID;

	Ref<Mesh> mesh;
	Ref<Shader> shader;
	Ref<ShaderMaterial> shaderMaterial;
	std::unordered_map<pixelpart::id_t, std::string> shaderParameterNames;
	std::unordered_map<std::string, uint32_t> textureParameterSamplers;
	std::vector<std::string> textureResourceIds;

	pixelpart::ParticleData sortedParticleData;
	std::vector<uint32_t> sortKeys;

	std::unordered_map<uint32_t, ParticleTrail> trails;
};
}

#endif