#ifndef PIXELPART_PARTICLE_MESH_3D_H
#define PIXELPART_PARTICLE_MESH_3D_H

#include "PixelpartGraphicsResourceStore.h"
#include "PixelpartShaderGenerator.h"
#include <effect/Effect.h>
#include <engine/ParticleContainer.h>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/camera3d.hpp>

namespace godot {
class PixelpartParticleMesh3D {
public:
	PixelpartParticleMesh3D(PixelpartGraphicsResourceStore& resourceStore, PixelpartShaderGenerator& shaderGenerator, const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType);
	PixelpartParticleMesh3D(const PixelpartParticleMesh3D&) = delete;
	~PixelpartParticleMesh3D();

	PixelpartParticleMesh3D& operator=(const PixelpartParticleMesh3D&) = delete;

	void draw(Node3D* parentNode,
		const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
		pixelpart::ParticleReadPtr particles, uint32_t numParticles,
		pixelpart::float_t scale, pixelpart::float_t t);

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
		std::vector<pixelpart::vec3_t> directionToEdge;
		std::vector<pixelpart::float_t> index;
	};

	void add_particle_sprites(Node3D* parentNode,
		const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
		pixelpart::ParticleReadPtr particles, uint32_t numParticles,
		pixelpart::float_t scale, pixelpart::float_t t);
	void add_particle_trails(Node3D* parentNode,
		const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
		pixelpart::ParticleReadPtr particles, uint32_t numParticles,
		pixelpart::float_t scale, pixelpart::float_t t);
	void add_particle_meshes(Node3D* parentNode,
		const pixelpart::Effect& effect, const pixelpart::ParticleType& particleType,
		pixelpart::ParticleReadPtr particles, uint32_t numParticles,
		pixelpart::float_t scale, pixelpart::float_t t);

	pixelpart::ParticleReadPtr sort_particles(pixelpart::ParticleReadPtr particles, uint32_t numParticles, pixelpart::ParticleSortCriterion sortCriterion,
		Node3D* parentNode, Camera3D* camera);

	pixelpart::mat3_t rotation3d(const pixelpart::vec3_t& angle);
	pixelpart::mat3_t lookAt(const pixelpart::vec3_t& direction);

	PixelpartGraphicsResourceStore& resources;

	RID instanceRID;

	Ref<ArrayMesh> arrayMesh;
	Ref<MultiMesh> multiMesh;
	Ref<Shader> shader;
	Ref<ShaderMaterial> shaderMaterial;
	std::unordered_map<pixelpart::id_t, std::string> shaderParameterNames;
	std::unordered_map<std::string, std::string> textureResourceIds;

	pixelpart::ParticleContainer sortedParticleContainer;
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