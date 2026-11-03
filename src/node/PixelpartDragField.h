#ifndef PIXELPART_DRAG_FIELD_H
#define PIXELPART_DRAG_FIELD_H

#include "PixelpartForceField.h"
#include "../property/PixelpartStaticPropertyFloat.h"
#include <pixelpart-runtime/effect/DragField.h>

namespace godot {
/**
 * @brief Force field that simulates drag.
 *
 * Drag fields slow down particles mimicking the effects of drag.
 */
class PixelpartDragField : public PixelpartForceField {
	GDCLASS(PixelpartDragField, PixelpartForceField)

public:
	PixelpartDragField();
	virtual ~PixelpartDragField();

#ifdef DOXYGEN
	/**
	 * @brief How strongly the drag force is influenced by the particle velocity (read-only).
	 */
	Ref<PixelpartStaticPropertyFloat> velocity_influence;

	/**
	 * @brief How strongly the drag force is influenced by the particle size (read-only).
	 */
	Ref<PixelpartStaticPropertyFloat> size_influence;
#endif

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	/**
	 * @brief How strongly the drag force is influenced by the particle velocity.
	 *
	 * @return Velocity influence property
	 */
	Ref<PixelpartStaticPropertyFloat> get_velocity_influence() const;

	/**
	 * @brief How strongly the drag force is influenced by the particle size.
	 *
	 * @return Size influence property
	 */
	Ref<PixelpartStaticPropertyFloat> get_size_influence() const;

protected:
	static void _bind_methods();

private:
	pixelpart::DragField* dragField = nullptr;
};
}

#endif
