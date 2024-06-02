#ifndef PIXELPART_STATIC_PROPERTY_BOOL_H
#define PIXELPART_STATIC_PROPERTY_BOOL_H

#include "engine/ParticleEngine.h"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace godot {
class PixelpartStaticPropertyBool : public RefCounted {
	GDCLASS(PixelpartStaticPropertyBool, RefCounted)

public:
	static void _bind_methods();

	PixelpartStaticPropertyBool();

	void init(pixelpart::StaticProperty<bool>* prop);

	bool get() const;

	void set_value(bool value);
	bool get_value() const;

private:
	pixelpart::StaticProperty<bool>* property = nullptr;
};
}

#endif