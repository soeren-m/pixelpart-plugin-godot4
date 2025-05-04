#ifndef PIXELPART_STATIC_PROPERTY_INT_H
#define PIXELPART_STATIC_PROPERTY_INT_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <pixelpart-runtime/common/Types.h>
#include <pixelpart-runtime/effect/StaticProperty.h>

namespace godot {
class PixelpartStaticPropertyInt : public RefCounted {
	GDCLASS(PixelpartStaticPropertyInt, RefCounted)

public:
	PixelpartStaticPropertyInt();

	void init(pixelpart::StaticProperty<pixelpart::int_t>* prop);

	int value() const;

	void set_base_value(int value);
	int get_base_value() const;

protected:
	static void _bind_methods();

private:
	pixelpart::StaticProperty<pixelpart::int_t>* property = nullptr;
};
}

#endif