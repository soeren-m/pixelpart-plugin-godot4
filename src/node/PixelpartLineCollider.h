#ifndef PIXELPART_LINE_COLLIDER_H
#define PIXELPART_LINE_COLLIDER_H

#include "PixelpartCollider.h"
#include "../property/PixelpartStaticPropertyBool.h"
#include "../property/PixelpartStaticPropertyFloat.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include <pixelpart-runtime/effect/LineCollider.h>

namespace godot {
/**
 * @brief Collider consisting of several connected line segments (2D only).
 *
 * Line colliders combine several collision segments together and
 * are defined by a set of points at the vertices of the line segments.
 */
class PixelpartLineCollider : public PixelpartCollider {
	GDCLASS(PixelpartLineCollider, PixelpartCollider)

public:
	PixelpartLineCollider();
	virtual ~PixelpartLineCollider();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	/**
	 * @brief Add a vertex to the end of the line collider.
	 *
	 * @param point Point to add
	 */
	void add_point(Vector3 point);

	/**
	 * @brief Change the location of a vertex in the line collider.
	 *
	 * @param index Index of the vertex
	 * @param point New location
	 */
	void set_point(int index, Vector3 point);

	/**
	 * @brief Remove a vertex from the line collider.
	 *
	 * @param index Index of vertex to remove
	 */
	void remove_point(int index);

	/**
	 * @brief Location of a vertex in the line collider.
	 *
	 * @param index Index Index of the vertex
	 * @return Vertex location
	 */
	Vector3 get_point(int index) const;

	/**
	 * @brief Number of vertices in the line collider.
	 *
	 * @return Number of vertices
	 */
	int get_point_count() const;

protected:
	static void _bind_methods();

private:
	pixelpart::LineCollider* lineCollider = nullptr;
};
}

#endif