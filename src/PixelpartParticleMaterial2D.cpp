#include "PixelpartParticleMaterial2D.h"

namespace godot {
void PixelpartParticleMaterial2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_light_mode", "mode"), &PixelpartParticleMaterial2D::set_light_mode);
	ClassDB::bind_method(D_METHOD("get_light_mode"), &PixelpartParticleMaterial2D::get_light_mode);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "light_mode", PROPERTY_HINT_ENUM, "Normal,Unshaded,Light-Only"), "set_light_mode", "get_light_mode");
}

PixelpartParticleMaterial2D::PixelpartParticleMaterial2D() {

}

void PixelpartParticleMaterial2D::set_light_mode(CanvasItemMaterial::LightMode mode) {
	shaderDirty |= mode != lightMode;
	lightMode = mode;
}
CanvasItemMaterial::LightMode PixelpartParticleMaterial2D::get_light_mode() const {
	return lightMode;
}

void PixelpartParticleMaterial2D::set_shader_dirty(bool state) {
	shaderDirty = state;
}
bool PixelpartParticleMaterial2D::is_shader_dirty() const {
	return shaderDirty;
}
}