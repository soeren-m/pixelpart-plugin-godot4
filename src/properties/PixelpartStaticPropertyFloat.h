#ifndef PIXELPART_STATIC_PROPERTY_FLOAT_H
#define PIXELPART_STATIC_PROPERTY_FLOAT_H

#include "engine/ParticleEngine.h"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace godot {
class PixelpartStaticPropertyFloat : public RefCounted {
	GDCLASS(PixelpartStaticPropertyFloat, RefCounted)

public:
	static void _bind_methods();

	PixelpartStaticPropertyFloat();

	void init(pixelpart::StaticProperty<pixelpart::float_t>* prop, pixelpart::ParticleEngine* engine);

	float get() const;

	void set_value(float value);
	float get_value() const;

private:
	void refresh_solver();

	pixelpart::StaticProperty<pixelpart::float_t>* property = nullptr;
	pixelpart::ParticleEngine* particleEngine = nullptr;
};
}

#endif