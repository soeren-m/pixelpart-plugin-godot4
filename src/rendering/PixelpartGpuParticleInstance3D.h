#ifndef PIXELPART_GPU_PARTICLE_INSTANCE_3D_H
#define PIXELPART_GPU_PARTICLE_INSTANCE_3D_H

#include "PixelpartParticleInstance3D.h"
#include "PixelpartGraphicsResourceProvider.h"
#include "PixelpartShaderProvider.h"
#include "../engine/PixelpartGpuEffectEngine.h"
#include <godot_cpp/classes/visual_instance3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <pixelpart-runtime/types/Types.h>
#include <pixelpart-runtime/types/Id.h>
#include <pixelpart-runtime/effect/Effect.h>
#include <pixelpart-runtime/effect/EffectRuntimeContext.h>
#include <memory>
#include <cstdint>

namespace godot {
class PixelpartGpuParticleInstance3D : public PixelpartParticleInstance3D {
public:
	PixelpartGpuParticleInstance3D(VisualInstance3D* parent,
		const PixelpartGpuEffectEngine* engine, pixelpart::id_t emitterId, pixelpart::id_t typeId,
		PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider);
	PixelpartGpuParticleInstance3D(const PixelpartGpuParticleInstance3D&) = delete;
	virtual ~PixelpartGpuParticleInstance3D();

	PixelpartGpuParticleInstance3D& operator=(const PixelpartGpuParticleInstance3D&) = delete;

	virtual void draw(pixelpart::float_t scale) override;

private:
	void create_material(PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider);
	void create_instance(PixelpartGraphicsResourceProvider& resourceProvider);

	VisualInstance3D* parentNode = nullptr;

	const PixelpartGpuEffectEngine* effectEngine = nullptr;
	pixelpart::id_t particleEmitterId;
	pixelpart::id_t particleTypeId;

	RID instanceRid;
	Ref<ArrayMesh> arrayMesh;
	Ref<MultiMesh> multiMesh;
	Ref<Shader> shader;
	Ref<ShaderMaterial> shaderMaterial;

    PackedInt32Array indexArray;
	PackedVector3Array dummyVertexArray;
};
}

#endif
