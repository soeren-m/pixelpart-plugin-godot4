#ifndef PIXELPART_NODE_H
#define PIXELPART_NODE_H

#include "PixelpartEffectResource.h"
#include "../property/PixelpartAnimatedPropertyFloat3.h"
#include <pixelpart-runtime/effect/Node.h>
#include <pixelpart-runtime/engine/EffectEngine.h>
#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
class PixelpartNode : public RefCounted {
	GDCLASS(PixelpartNode, RefCounted)

public:
	PixelpartNode();
    virtual ~PixelpartNode();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr);

	int get_id() const;
	int get_parent_id() const;
	String get_name() const;

	void set_lifetime_start(float time);
	void set_lifetime_duration(float time);
	void set_repeat(bool value);
	float get_lifetime_start() const;
	float get_lifetime_duration() const;
	bool get_repeat() const;
	bool is_active() const;
	float get_local_time() const;

	Ref<PixelpartAnimatedPropertyFloat3> get_position() const;
    Ref<PixelpartAnimatedPropertyFloat3> get_rotation() const;
	Ref<PixelpartAnimatedPropertyFloat3> get_scale() const;

protected:
	static void _bind_methods();

	pixelpart::Node* node = nullptr;
	pixelpart::EffectEngine* effectEngine = nullptr;
};
}

#endif