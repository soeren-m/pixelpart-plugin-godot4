#ifndef PIXELPART_STATIC_PROPERTY_FLOAT3_H
#define PIXELPART_STATIC_PROPERTY_FLOAT3_H

#include "engine/ParticleEngine.h"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace godot {
class PixelpartStaticPropertyFloat3 : public RefCounted {
	GDCLASS(PixelpartStaticPropertyFloat3, RefCounted)

public:
	static void _bind_methods();

	PixelpartStaticPropertyFloat3();

	void init(pixelpart::StaticProperty<pixelpart::vec3_t>* prop);

	Vector3 get() const;

	void set_value(Vector3 value);
	Vector3 get_value() const;

private:
	pixelpart::StaticProperty<pixelpart::vec3_t>* property = nullptr;
};
}

#endif