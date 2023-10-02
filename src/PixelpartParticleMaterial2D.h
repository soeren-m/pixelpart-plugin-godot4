#ifndef PIXELPART_PARTICLE_MATERIAL_2D_H
#define PIXELPART_PARTICLE_MATERIAL_2D_H

#include "PixelpartShaders.h"
#include "ParticleType.h"

namespace godot {
class PixelpartParticleMaterial2D : public Resource {
	GDCLASS(PixelpartParticleMaterial2D, Resource)

public:
	static void _bind_methods();

	PixelpartParticleMaterial2D();

	void set_light_mode(CanvasItemMaterial::LightMode mode);
	CanvasItemMaterial::LightMode get_light_mode() const;

	void set_shader_dirty(bool state);
	bool is_shader_dirty() const;

private:
	CanvasItemMaterial::LightMode lightMode = CanvasItemMaterial::LIGHT_MODE_NORMAL;

	bool shaderDirty = true;
};
}

#endif