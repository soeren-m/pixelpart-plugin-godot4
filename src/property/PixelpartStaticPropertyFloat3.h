#ifndef PIXELPART_STATIC_PROPERTY_FLOAT3_H
#define PIXELPART_STATIC_PROPERTY_FLOAT3_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <pixelpart-runtime/common/Math.h>
#include <pixelpart-runtime/effect/StaticProperty.h>

namespace godot {
class PixelpartStaticPropertyFloat3 : public RefCounted {
	GDCLASS(PixelpartStaticPropertyFloat3, RefCounted)

public:
	PixelpartStaticPropertyFloat3();

	void init(pixelpart::StaticProperty<pixelpart::float3_t>* prop);

	Vector3 value() const;

	void set_base_value(Vector3 value);
	Vector3 get_base_value() const;

protected:
	static void _bind_methods();

private:
	pixelpart::StaticProperty<pixelpart::float3_t>* property = nullptr;
};
}

#endif