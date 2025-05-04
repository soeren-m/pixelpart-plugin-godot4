#ifndef PIXELPART_STATIC_PROPERTY_FLOAT4_H
#define PIXELPART_STATIC_PROPERTY_FLOAT4_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <pixelpart-runtime/common/Math.h>
#include <pixelpart-runtime/effect/StaticProperty.h>

namespace godot {
class PixelpartStaticPropertyFloat4 : public RefCounted {
	GDCLASS(PixelpartStaticPropertyFloat4, RefCounted)

public:
	PixelpartStaticPropertyFloat4();

	void init(pixelpart::StaticProperty<pixelpart::float4_t>* prop);

	Vector4 value() const;

	void set_base_value(Vector4 value);
	Vector4 get_base_value() const;

protected:
	static void _bind_methods();

private:
	pixelpart::StaticProperty<pixelpart::float4_t>* property = nullptr;
};
}

#endif