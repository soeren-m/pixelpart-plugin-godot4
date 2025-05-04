#ifndef PIXELPART_NODE_FACTORY_H
#define PIXELPART_NODE_FACTORY_H

#include "PixelpartNode.h"
#include <pixelpart-runtime/effect/Node.h>
#include <pixelpart-runtime/engine/EffectEngine.h>

namespace godot {
Ref<PixelpartNode> create_pixelpart_node(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEngine);
}

#endif