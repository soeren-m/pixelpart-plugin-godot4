#include "PixelpartNodeFactory.h"
#include "PixelpartGroupNode.h"
#include "PixelpartParticleEmitter.h"
#include "PixelpartAttractionField.h"
#include "PixelpartAccelerationField.h"
#include "PixelpartVectorField.h"
#include "PixelpartNoiseField.h"
#include "PixelpartDragField.h"
#include "PixelpartLineCollider.h"
#include "PixelpartPlaneCollider.h"
#include "PixelpartDirectionalLightSource.h"
#include "PixelpartPointLightSource.h"
#include "PixelpartSpotLightSource.h"
#include <pixelpart-runtime/effect/GroupNode.h>
#include <pixelpart-runtime/effect/ParticleEmitter.h>
#include <pixelpart-runtime/effect/AccelerationField.h>
#include <pixelpart-runtime/effect/VectorField.h>
#include <pixelpart-runtime/effect/NoiseField.h>
#include <pixelpart-runtime/effect/DragField.h>
#include <pixelpart-runtime/effect/LineCollider.h>
#include <pixelpart-runtime/effect/PlaneCollider.h>
#include <pixelpart-runtime/effect/AttractionField.h>
#include <pixelpart-runtime/effect/DirectionalLightSource.h>
#include <pixelpart-runtime/effect/PointLightSource.h>
#include <pixelpart-runtime/effect/SpotLightSource.h>

namespace godot {
Ref<PixelpartNode> create_pixelpart_node(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEngine) {
	if(dynamic_cast<pixelpart::GroupNode*>(internalNode)) {
		Ref<PixelpartGroupNode> node;
		node.instantiate();
		node->init(internalNode, effectEngine);

		return node;
	}
	else if(dynamic_cast<pixelpart::ParticleEmitter*>(internalNode)) {
		Ref<PixelpartParticleEmitter> node;
		node.instantiate();
		node->init(internalNode, effectEngine);

		return node;
	}
	else if(dynamic_cast<pixelpart::AttractionField*>(internalNode)) {
		Ref<PixelpartAttractionField> node;
		node.instantiate();
		node->init(internalNode, effectEngine);

		return node;
	}
	else if(dynamic_cast<pixelpart::AccelerationField*>(internalNode)) {
		Ref<PixelpartAccelerationField> node;
		node.instantiate();
		node->init(internalNode, effectEngine);

		return node;
	}
	else if(dynamic_cast<pixelpart::VectorField*>(internalNode)) {
		Ref<PixelpartVectorField> node;
		node.instantiate();
		node->init(internalNode, effectEngine);

		return node;
	}
	else if(dynamic_cast<pixelpart::NoiseField*>(internalNode)) {
		Ref<PixelpartNoiseField> node;
		node.instantiate();
		node->init(internalNode, effectEngine);

		return node;
	}
	else if(dynamic_cast<pixelpart::DragField*>(internalNode)) {
		Ref<PixelpartDragField> node;
		node.instantiate();
		node->init(internalNode, effectEngine);

		return node;
	}
	else if(dynamic_cast<pixelpart::LineCollider*>(internalNode)) {
		Ref<PixelpartLineCollider> node;
		node.instantiate();
		node->init(internalNode, effectEngine);

		return node;
	}
	else if(dynamic_cast<pixelpart::PlaneCollider*>(internalNode)) {
		Ref<PixelpartPlaneCollider> node;
		node.instantiate();
		node->init(internalNode, effectEngine);

		return node;
	}
	else if(dynamic_cast<pixelpart::DirectionalLightSource*>(internalNode)) {
		Ref<PixelpartDirectionalLightSource> node;
		node.instantiate();
		node->init(internalNode, effectEngine);

		return node;
	}
	else if(dynamic_cast<pixelpart::PointLightSource*>(internalNode)) {
		Ref<PixelpartPointLightSource> node;
		node.instantiate();
		node->init(internalNode, effectEngine);

		return node;
	}
	else if(dynamic_cast<pixelpart::SpotLightSource*>(internalNode)) {
		Ref<PixelpartSpotLightSource> node;
		node.instantiate();
		node->init(internalNode, effectEngine);

		return node;
	}

	return Ref<PixelpartNode>();
}
}