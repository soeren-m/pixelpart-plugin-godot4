#ifndef PIXELPART_PARTICLE_RENDERER_3D_H
#define PIXELPART_PARTICLE_RENDERER_3D_H

#include "PixelpartGraphicsResourceProvider.h"
#include "PixelpartShaderProvider.h"
#include <godot_cpp/classes/visual_instance3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <pixelpart-runtime/common/Types.h>
#include <pixelpart-runtime/common/Math.h>
#include <pixelpart-runtime/common/Id.h>
#include <pixelpart-runtime/effect/Effect.h>
#include <pixelpart-runtime/effect/EffectRuntimeContext.h>
#include <pixelpart-runtime/engine/ParticleCollection.h>
#include <pixelpart-runtime/vertex/ParticleVertexGenerator.h>
#include <memory>
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

	void draw(VisualInstance3D* parentNode,
		pixelpart::ParticleCollection::ReadPtr particles, std::uint32_t particleCount,
		const pixelpart::EffectRuntimeContext& runtimeContext, pixelpart::float_t scale);

private:
	static float pack_int_float(float a, float b, float s);

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

	std::unique_ptr<pixelpart::ParticleVertexGenerator> vertexGenerator;
	PackedInt32Array indexArray;
	PackedVector3Array vertexArray;
	PackedVector3Array normalArray;
	PackedVector2Array uvArray;
	PackedVector2Array uv2Array;
	PackedColorArray colorArray;
	PackedFloat32Array custom0Array;
	PackedFloat32Array instanceDataArray;
};
}

#endif
