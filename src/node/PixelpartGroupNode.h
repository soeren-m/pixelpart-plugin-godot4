#ifndef PIXELPART_GROUP_NODE_H
#define PIXELPART_GROUP_NODE_H

#include "PixelpartNode.h"

namespace godot {
/**
 * @brief Node that acts as a parent node for other nodes.
 *
 * Group nodes are nodes with no function other than containing other nodes
 * like emitters and force fields.
 */
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
