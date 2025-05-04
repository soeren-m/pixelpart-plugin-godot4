#ifndef PIXELPART_STATIC_PROPERTY_FLOAT2_H
#define PIXELPART_STATIC_PROPERTY_FLOAT2_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <pixelpart-runtime/common/Math.h>
#include <pixelpart-runtime/effect/StaticProperty.h>

namespace godot {
class PixelpartStaticPropertyFloat2 : public RefCounted {
	GDCLASS(PixelpartStaticPropertyFloat2, RefCounted)

public:
	PixelpartStaticPropertyFloat2();

	void init(pixelpart::StaticProperty<pixelpart::float2_t>* prop);

	Vector2 value() const;

	void set_base_value(Vector2 value);
	Vector2 get_base_value() const;

protected:
	static void _bind_methods();

private:
	pixelpart::StaticProperty<pixelpart::float2_t>* property = nullptr;
};
}

#endif