#ifndef PIXELPART_PARTICLE_CANVAS_ITEM_H
#define PIXELPART_PARTICLE_CANVAS_ITEM_H

#include "PixelpartGraphicsResourceProvider.h"
#include "PixelpartShaderProvider.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/shader.hpp>
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
class PixelpartParticleCanvasItem {
public:
	PixelpartParticleCanvasItem(Node2D* parent,
		const pixelpart::Effect& eff, pixelpart::id_t emitterId, pixelpart::id_t typeId,
		PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider,
		std::shared_ptr<pixelpart::ThreadPool> threadPool);
	PixelpartParticleCanvasItem(const PixelpartParticleCanvasItem&) = delete;
	~PixelpartParticleCanvasItem();

	PixelpartParticleCanvasItem& operator=(const PixelpartParticleCanvasItem&) = delete;

	void draw(const pixelpart::ParticleCollection& particles,
		const pixelpart::EffectRuntimeContext& runtimeContext, pixelpart::float2_t scale);

private:
	void create_material(PixelpartGraphicsResourceProvider& resourceProvider, PixelpartShaderProvider& shaderProvider);
	void create_canvas_item();
	void create_vertex_generator(std::shared_ptr<pixelpart::ThreadPool> threadPool);

	Node2D* parentNode = nullptr;

	const pixelpart::Effect& effect;
	pixelpart::id_t particleEmitterId;
	pixelpart::id_t particleTypeId;

	RID canvasItemRid;
	RID meshRid;
	RID materialRid;
	Ref<Shader> shader;

	std::unique_ptr<pixelpart::ParticleVertexGenerator> vertexGenerator;
	PackedInt32Array indexArray;
	PackedVector2Array vertexArray;
	PackedVector2Array uvArray;
	PackedColorArray colorArray;
	PackedFloat32Array custom0Array;
};
}

#endif
