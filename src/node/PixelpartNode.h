#ifndef PIXELPART_NODE_H
#define PIXELPART_NODE_H

#include "PixelpartEffectResource.h"
#include "../property/PixelpartAnimatedPropertyFloat3.h"
#include <pixelpart-runtime/effect/Node.h>
#include <pixelpart-runtime/engine/EffectEngine.h>
#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
/**
 * @brief An object in an effect like a particle emitter or a force field.
 *
 * Nodes can be moved, rotated and scaled and move together with their parent node if present.
 */
class PixelpartNode : public RefCounted {
	GDCLASS(PixelpartNode, RefCounted)

public:
	PixelpartNode();
	virtual ~PixelpartNode();

#ifdef DOXYGEN
	/**
	 * @brief Time in seconds until the node becomes active.
	 */
	float lifetime_start;

	/**
	 * @brief How long the node stays active in seconds.
	 */
	float lifetime_duration;

	/**
	 * @brief Wether the node repeats its behavior after its lifetime is over.
	 */
	bool repeat;
#endif

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr);

	/**
	 * @brief ID of the node.
	 *
	 * @return ID
	 */
	int get_id() const;

	/**
	 * @brief ID of the node's parent.
	 *
	 * @return Parent ID or `-1` if the node has no parent
	 */
	int get_parent_id() const;

	/**
	 * @brief Name of the node.
	 *
	 * @return Name
	 */
	String get_name() const;

	/**
	 * @brief Set time until the node becomes active.
	 *
	 * @param time Time in seconds until the node becomes active
	 */
	void set_lifetime_start(float time);

	/**
	 * @brief Set how long the node stays active in seconds.
	 *
	 * @param time How long the node stays active in seconds
	 */
	void set_lifetime_duration(float time);

	/**
	 * @brief Set whether the node repeats its behavior after its lifetime is over.
	 *
	 * @param value Wether the node repeats its behavior after its lifetime is over
	 */
	void set_repeat(bool value);

	/**
	 * @brief Time in seconds until the node becomes active.
	 *
	 * @return Time in seconds until the node becomes active
	 */
	float get_lifetime_start() const;

	/**
	 * @brief How long the node is active in seconds.
	 *
	 * @return How long the node is active in seconds
	 */
	float get_lifetime_duration() const;

	/**
	 * @brief Wether the node repeats its behavior after its lifetime is over.
	 *
	 * @return Wether the node repeats its behavior after its lifetime is over
	 */
	bool get_repeat() const;

	/**
	 * @brief Return whether the node is active at the current point in time.
	 *
	 * @return Whether the node is active at the current point in time.
	 */
	bool is_active() const;

	/**
	 * @brief Return the time fraction the emitter has been active for, in range 0.0 (0%) to 1.0 (100%).
	 *
	 * @return Time fraction the emitter has been active for
	 */
	float get_local_time() const;

	/**
	 * @brief Position of the node.
	 *
	 * @return Position property
	 */
	Ref<PixelpartAnimatedPropertyFloat3> get_position() const;

	/**
	 * @brief Rotation of the node in degrees.
	 *
	 * For 3D effects, the rotation is composed of roll, yaw and pitch.
	 *
	 * @return Rotation property
	 */
	Ref<PixelpartAnimatedPropertyFloat3> get_rotation() const;

	/**
	 * @brief Size of the node.
	 *
	 * @return Scale property
	 */
	Ref<PixelpartAnimatedPropertyFloat3> get_scale() const;

protected:
	static void _bind_methods();

	pixelpart::Node* node = nullptr;
	pixelpart::EffectEngine* effectEngine = nullptr;
};
}

#endif