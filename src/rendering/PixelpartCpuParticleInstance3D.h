#ifndef PIXELPART_CPU_PARTICLE_INSTANCE_3D_H
#define PIXELPART_CPU_PARTICLE_INSTANCE_3D_H

#include "PixelpartParticleInstance3D.h"
#include "PixelpartGraphicsResourceProvider.h"
#include "PixelpartShaderProvider.h"
#include <godot_cpp/classes/visual_instance3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <pixelpart-runtime/types/Types.h>
#include <pixelpart-runtime/types/Id.h>
#include <pixelpart-runtime/platform/ThreadPool.h>
#include <pixelpart-runtime/effect/Effect.h>
#include <pixelpart-runtime/effect/EffectRuntimeContext.h>
#include <pixelpart-runtime/engine/CpuEffectEngine.h>
#include <pixelpart-runtime/vertex/ParticleVertexGenerator.h>
#include <memory>
#include <cstdint>

namespace godot {
class PixelpartCpuParticleInstance3D : public PixelpartParticleInstance3D {
public:
	PixelpartCpuParticleInstance3D(VisualInstance3D* parent,
		const pixelpart::CpuEffectEngine* engine, pixelpart::id_t emitterId, pixelpart::id_t typeId,
		PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider,
		std::shared_ptr<pixelpart::ThreadPool> threadPool);
	PixelpartCpuParticleInstance3D(const PixelpartCpuParticleInstance3D&) = delete;
	virtual ~PixelpartCpuParticleInstance3D();

	PixelpartCpuParticleInstance3D& operator=(const PixelpartCpuParticleInstance3D&) = delete;

	virtual void draw(pixelpart::float_t scale) override;

private:
	void create_material(PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider);
	void create_instance(PixelpartGraphicsResourceProvider& resourceProvider);
	void create_vertex_generator(std::shared_ptr<pixelpart::ThreadPool> threadPool);

	static float pack_int_float(float a, float b, float s);

	VisualInstance3D* parentNode = nullptr;

	const pixelpart::CpuEffectEngine* effectEngine = nullptr;
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
