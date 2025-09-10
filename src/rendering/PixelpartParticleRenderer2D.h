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
#include <pixelpart-runtime/vertex/ParticleVertexGenerator.h>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace godot {
class PixelpartParticleRenderer2D {
public:
	PixelpartParticleRenderer2D(PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider,
		const pixelpart::Effect& eff, pixelpart::id_t emitterId, pixelpart::id_t typeId);
	PixelpartParticleRenderer2D(const PixelpartParticleRenderer2D&) = delete;
	~PixelpartParticleRenderer2D();

	PixelpartParticleRenderer2D& operator=(const PixelpartParticleRenderer2D&) = delete;

	void draw(Node2D* parentNode,
		pixelpart::ParticleCollection::ReadPtr particles, std::uint32_t particleCount,
		const pixelpart::RuntimeContext& runtimeContext, pixelpart::float2_t scale);

private:
	PixelpartGraphicsResourceProvider& resources;

	const pixelpart::Effect& effect;
	pixelpart::id_t particleEmitterId;
	pixelpart::id_t particleTypeId;

	RID canvasItemRID;
	RID meshRID;
	RID materialRID;
	Ref<Shader> shader;
	std::unordered_map<pixelpart::id_t, std::string> shaderParameterNames;
	std::unordered_map<std::string, std::string> textureResourceIds;

	std::unique_ptr<pixelpart::ParticleVertexGenerator> vertexGenerator;
	PackedInt32Array indexArray;
	PackedVector2Array vertexArray;
	PackedVector2Array uvArray;
	PackedColorArray colorArray;
	PackedFloat32Array custom0Array;
};
}

#endif
