#include "PixelpartParticleMaterial3D.h"

namespace godot {
void PixelpartParticleMaterial3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_shading_mode", "mode"), &PixelpartParticleMaterial3D::set_shading_mode);
	ClassDB::bind_method(D_METHOD("get_shading_mode"), &PixelpartParticleMaterial3D::get_shading_mode);
	ClassDB::bind_method(D_METHOD("set_diffuse_mode", "mode"), &PixelpartParticleMaterial3D::set_diffuse_mode);
	ClassDB::bind_method(D_METHOD("get_diffuse_mode"), &PixelpartParticleMaterial3D::get_diffuse_mode);
	ClassDB::bind_method(D_METHOD("set_specular_mode", "mode"), &PixelpartParticleMaterial3D::set_specular_mode);
	ClassDB::bind_method(D_METHOD("get_specular_mode"), &PixelpartParticleMaterial3D::get_specular_mode);
	ClassDB::bind_method(D_METHOD("set_normal_mode", "mode"), &PixelpartParticleMaterial3D::set_normal_mode);
	ClassDB::bind_method(D_METHOD("set_static_normal", "normal"), &PixelpartParticleMaterial3D::set_static_normal);
	ClassDB::bind_method(D_METHOD("get_normal_mode"), &PixelpartParticleMaterial3D::get_normal_mode);
	ClassDB::bind_method(D_METHOD("get_static_normal"), &PixelpartParticleMaterial3D::get_static_normal);
	ClassDB::bind_method(D_METHOD("set_metallic", "value"), &PixelpartParticleMaterial3D::set_metallic);
	ClassDB::bind_method(D_METHOD("set_specular", "value"), &PixelpartParticleMaterial3D::set_specular);
	ClassDB::bind_method(D_METHOD("set_roughness", "value"), &PixelpartParticleMaterial3D::set_roughness);
	ClassDB::bind_method(D_METHOD("get_metallic"), &PixelpartParticleMaterial3D::get_metallic);
	ClassDB::bind_method(D_METHOD("get_specular"), &PixelpartParticleMaterial3D::get_specular);
	ClassDB::bind_method(D_METHOD("get_roughness"), &PixelpartParticleMaterial3D::get_roughness);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "shading_mode", PROPERTY_HINT_ENUM, "Unshaded,Per-Pixel,Per-Vertex"), "set_shading_mode", "get_shading_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "diffuse_mode", PROPERTY_HINT_ENUM, "Burley,Lambert,Lambert Wrap,Toon"), "set_diffuse_mode", "get_diffuse_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "specular_mode", PROPERTY_HINT_ENUM, "SchlickGGX,Toon,Disabled"), "set_specular_mode", "get_specular_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "normal_mode", PROPERTY_HINT_ENUM, "Mesh,Static"), "set_normal_mode", "get_normal_mode");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "static_normal"), "set_static_normal", "get_static_normal");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "metallic", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_metallic", "get_metallic");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "specular", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_specular", "get_specular");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "roughness", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_roughness", "get_roughness");

	BIND_ENUM_CONSTANT(PARTICLE_NORMAL_MODE_MESH);
	BIND_ENUM_CONSTANT(PARTICLE_NORMAL_MODE_STATIC);
}

PixelpartParticleMaterial3D::PixelpartParticleMaterial3D() {

}

void PixelpartParticleMaterial3D::set_shading_mode(BaseMaterial3D::ShadingMode mode) {
	shaderDirty |= mode != shadingMode;
	shadingMode = mode;
}
BaseMaterial3D::ShadingMode PixelpartParticleMaterial3D::get_shading_mode() const {
	return shadingMode;
}

void PixelpartParticleMaterial3D::set_diffuse_mode(BaseMaterial3D::DiffuseMode mode) {
	shaderDirty |= mode != diffuseMode;
	diffuseMode = mode;
}
BaseMaterial3D::DiffuseMode PixelpartParticleMaterial3D::get_diffuse_mode() const {
	return diffuseMode;
}

void PixelpartParticleMaterial3D::set_specular_mode(BaseMaterial3D::SpecularMode mode) {
	shaderDirty |= mode != specularMode;
	specularMode = mode;
}
BaseMaterial3D::SpecularMode PixelpartParticleMaterial3D::get_specular_mode() const {
	return specularMode;
}

void PixelpartParticleMaterial3D::set_normal_mode(ParticleNormalMode mode) {
	shaderDirty |= mode != normalMode;
	normalMode = mode;
}
void PixelpartParticleMaterial3D::set_static_normal(Vector3 normal) {
	staticNormal = normal;
}
ParticleNormalMode PixelpartParticleMaterial3D::get_normal_mode() const {
	return normalMode;
}
Vector3 PixelpartParticleMaterial3D::get_static_normal() const {
	return staticNormal;
}

void PixelpartParticleMaterial3D::set_metallic(float value) {
	metallic = value;
}
void PixelpartParticleMaterial3D::set_specular(float value) {
	specular = value;
}
void PixelpartParticleMaterial3D::set_roughness(float value) {
	roughness = value;
}
float PixelpartParticleMaterial3D::get_metallic() const {
	return metallic;
}
float PixelpartParticleMaterial3D::get_specular() const {
	return specular;
}
float PixelpartParticleMaterial3D::get_roughness() const {
	return roughness;
}

void PixelpartParticleMaterial3D::set_shader_dirty(bool state) {
	shaderDirty = state;
}
bool PixelpartParticleMaterial3D::is_shader_dirty() const {
	return shaderDirty;
}
}