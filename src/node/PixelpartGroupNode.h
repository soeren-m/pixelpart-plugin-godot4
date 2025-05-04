#ifndef PIXELPART_GROUP_NODE_H
#define PIXELPART_GROUP_NODE_H

#include "PixelpartNode.h"

namespace godot {
class PixelpartGroupNode : public PixelpartNode {
	GDCLASS(PixelpartGroupNode, PixelpartNode)

public:
	PixelpartGroupNode();
	virtual ~PixelpartGroupNode();

protected:
	static void _bind_methods();
};
}

#endif