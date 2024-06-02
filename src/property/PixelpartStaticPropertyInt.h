#ifndef PIXELPART_STATIC_PROPERTY_INT_H
#define PIXELPART_STATIC_PROPERTY_INT_H

#include "engine/ParticleEngine.h"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace godot {
class PixelpartStaticPropertyInt : public RefCounted {
	GDCLASS(PixelpartStaticPropertyInt, RefCounted)

public:
	static void _bind_methods();

	PixelpartStaticPropertyInt();

	void init(pixelpart::StaticProperty<pixelpart::int_t>* prop);

	int get() const;

	void set_value(int value);
	int get_value() const;

private:
	pixelpart::StaticProperty<pixelpart::int_t>* property = nullptr;
};
}

#endif