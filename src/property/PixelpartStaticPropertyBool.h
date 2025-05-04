#ifndef PIXELPART_STATIC_PROPERTY_BOOL_H
#define PIXELPART_STATIC_PROPERTY_BOOL_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <pixelpart-runtime/common/Types.h>
#include <pixelpart-runtime/effect/StaticProperty.h>

namespace godot {
class PixelpartStaticPropertyBool : public RefCounted {
	GDCLASS(PixelpartStaticPropertyBool, RefCounted)

public:
	PixelpartStaticPropertyBool();

	void init(pixelpart::StaticProperty<bool>* prop);

	bool value() const;

	void set_base_value(bool value);
	bool get_base_value() const;

protected:
	static void _bind_methods();

private:
	pixelpart::StaticProperty<bool>* property = nullptr;
};
}

#endif