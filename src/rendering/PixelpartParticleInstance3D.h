#ifndef PIXELPART_PARTICLE_INSTANCE_3D_H
#define PIXELPART_PARTICLE_INSTANCE_3D_H

#include "PixelpartGraphicsResourceProvider.h"
#include "PixelpartShaderProvider.h"
#include <godot_cpp/classes/visual_instance3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <pixelpart-runtime/common/Types.h>
#include <pixelpart-runtime/common/Id.h>
#include <pixelpart-runtime/common/ThreadPool.h>
#include <pixelpart-runtime/effect/Effect.h>
#include <pixelpart-runtime/effect/EffectRuntimeContext.h>
#include <pixelpart-runtime/engine/ParticleCollection.h>
#include <pixelpart-runtime/vertex/ParticleVertexGenerator.h>
#include <memory>
#include <cstdint>

namespace godot {
class PixelpartParticleInstance3D {
public:
	PixelpartParticleInstance3D(VisualInstance3D* parent,
		const pixelpart::Effect& eff, pixelpart::id_t emitterId, pixelpart::id_t typeId,
		PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider,
		std::shared_ptr<pixelpart::ThreadPool> threadPool);
	PixelpartParticleInstance3D(const PixelpartParticleInstance3D&) = delete;
	~PixelpartParticleInstance3D();

	PixelpartParticleInstance3D& operator=(const PixelpartParticleInstance3D&) = delete;

	void draw(const pixelpart::ParticleCollection& particles,
		const pixelpart::EffectRuntimeContext& runtimeContext, pixelpart::float_t scale);

private:
	void create_material(PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider);
	void create_instance(PixelpartGraphicsResourceProvider& resourceProvider);
	void create_vertex_generator(std::shared_ptr<pixelpart::ThreadPool> threadPool);

	static float pack_int_float(float a, float b, float s);

	VisualInstance3D* parentNode = nullptr;

	const pixelpart::Effect& effect;
	pixelpart::id_t particleEmitterId;
	pixelpart::id_t particleTypeId;

	RID instanceRid;
	Ref<ArrayMesh> arrayMesh;
	Ref<MultiMesh> multiMesh;
	Ref<Shader> shader;
	Ref<ShaderMaterial> shaderMaterial;

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
