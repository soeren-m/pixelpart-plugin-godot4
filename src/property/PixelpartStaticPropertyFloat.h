#ifndef PIXELPART_STATIC_PROPERTY_FLOAT_H
#define PIXELPART_STATIC_PROPERTY_FLOAT_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <pixelpart-runtime/common/Math.h>
#include <pixelpart-runtime/effect/StaticProperty.h>

namespace godot {
class PixelpartStaticPropertyFloat : public RefCounted {
	GDCLASS(PixelpartStaticPropertyFloat, RefCounted)

public:
	PixelpartStaticPropertyFloat();

	void init(pixelpart::StaticProperty<pixelpart::float_t>* prop);

	float value() const;

	void set_base_value(float value);
	float get_base_value() const;

protected:
	static void _bind_methods();

private:
	pixelpart::StaticProperty<pixelpart::float_t>* property = nullptr;
};
}

#endif