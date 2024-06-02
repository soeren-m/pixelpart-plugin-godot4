#ifndef PIXELPART_STATIC_PROPERTY_FLOAT2_H
#define PIXELPART_STATIC_PROPERTY_FLOAT2_H

#include "engine/ParticleEngine.h"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace godot {
class PixelpartStaticPropertyFloat2 : public RefCounted {
	GDCLASS(PixelpartStaticPropertyFloat2, RefCounted)

public:
	static void _bind_methods();

	PixelpartStaticPropertyFloat2();

	void init(pixelpart::StaticProperty<pixelpart::vec2_t>* prop);

	Vector2 get() const;

	void set_value(Vector2 value);
	Vector2 get_value() const;

private:
	pixelpart::StaticProperty<pixelpart::vec2_t>* property = nullptr;
};
}

#endif