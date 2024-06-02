#ifndef PIXELPART_STATIC_PROPERTY_FLOAT4_H
#define PIXELPART_STATIC_PROPERTY_FLOAT4_H

#include "engine/ParticleEngine.h"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace godot {
class PixelpartStaticPropertyFloat4 : public RefCounted {
	GDCLASS(PixelpartStaticPropertyFloat4, RefCounted)

public:
	static void _bind_methods();

	PixelpartStaticPropertyFloat4();

	void init(pixelpart::StaticProperty<pixelpart::vec4_t>* prop);

	Vector4 get() const;

	void set_value(Vector4 value);
	Vector4 get_value() const;

private:
	pixelpart::StaticProperty<pixelpart::vec4_t>* property = nullptr;
};
}

#endif