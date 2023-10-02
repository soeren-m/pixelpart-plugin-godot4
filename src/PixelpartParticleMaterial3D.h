#ifndef PIXELPART_PARTICLE_MATERIAL_3D_H
#define PIXELPART_PARTICLE_MATERIAL_3D_H

#include "PixelpartShaders.h"
#include "ParticleType.h"

namespace godot {
class PixelpartParticleMaterial3D : public Resource {
	GDCLASS(PixelpartParticleMaterial3D, Resource)

public:
	static void _bind_methods();

	PixelpartParticleMaterial3D();

	void set_shading_mode(BaseMaterial3D::ShadingMode mode);
	BaseMaterial3D::ShadingMode get_shading_mode() const;

	void set_diffuse_mode(BaseMaterial3D::DiffuseMode mode);
	BaseMaterial3D::DiffuseMode get_diffuse_mode() const;

	void set_specular_mode(BaseMaterial3D::SpecularMode mode);
	BaseMaterial3D::SpecularMode get_specular_mode() const;

	void set_normal_mode(ParticleNormalMode mode);
	void set_static_normal(Vector3 normal);
	ParticleNormalMode get_normal_mode() const;
	Vector3 get_static_normal() const;

	void set_metallic(float value);
	void set_specular(float value);
	void set_roughness(float value);
	float get_metallic() const;
	float get_specular() const;
	float get_roughness() const;

	void set_shader_dirty(bool state);
	bool is_shader_dirty() const;

private:
	BaseMaterial3D::ShadingMode shadingMode = BaseMaterial3D::SHADING_MODE_UNSHADED;
	BaseMaterial3D::DiffuseMode diffuseMode = BaseMaterial3D::DIFFUSE_BURLEY;
	BaseMaterial3D::SpecularMode specularMode = BaseMaterial3D::SPECULAR_SCHLICK_GGX;
	ParticleNormalMode normalMode = PARTICLE_NORMAL_MODE_MESH;
	Vector3 staticNormal = Vector3(0.0f, 1.0f, 0.0f);

	float metallic = 0.0f;
	float specular = 0.5f;
	float roughness = 1.0f;

	bool shaderDirty = true;
};
}

VARIANT_ENUM_CAST(ParticleNormalMode);

#endif