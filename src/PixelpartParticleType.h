#ifndef PIXELPART_PARTICLETYPE_H
#define PIXELPART_PARTICLETYPE_H

#include "PixelpartEffectResource.h"
#include "PixelpartCurve.h"
#include "PixelpartCurve3.h"
#include "PixelpartGradient.h"
#include "ParticleType.h"
#include "ParticleEngine.h"

namespace godot {
class PixelpartParticleType : public RefCounted {
	GDCLASS(PixelpartParticleType, RefCounted)

public:
	static void _bind_methods();

	PixelpartParticleType();

	void init(Ref<PixelpartEffectResource> resource, pixelpart::ParticleType* particleType, pixelpart::ParticleEngine* engine, const pixelpart::ShaderGraph::BuildResult& shaderGraphResult);

	int get_id() const;
	int get_parent_id() const;
	String get_name() const;

	Ref<PixelpartCurve3> get_position() const;

	Ref<PixelpartCurve> get_num_particles() const;
	Ref<PixelpartCurve> get_lifespan() const;
	void set_lifespan_variance(float variance);
	float get_lifespan_variance() const;

	void set_position_relative(bool mode);
	bool is_position_relative() const;

	void set_motion_path_force(float force);
	float get_motion_path_force() const;

	Ref<PixelpartCurve> get_initial_velocity() const;
	void set_velocity_variance(float variance);
	float get_velocity_variance() const;

	Ref<PixelpartCurve> get_acceleration() const;
	Ref<PixelpartCurve> get_radial_acceleration() const;
	Ref<PixelpartCurve> get_damping() const;

	void set_rotation_mode(int mode);
	void set_alignment_mode(int mode);
	int get_rotation_mode() const;
	int get_alignment_mode() const;

	Ref<PixelpartCurve3> get_initial_rotation() const;
	Ref<PixelpartCurve3> get_rotation() const;
	void set_rotation_variance(Vector3 variance);
	void set_angular_velocity_variance(Vector3 variance);
	Vector3 get_rotation_variance() const;
	Vector3 get_angular_velocity_variance() const;

	void set_pivot(Vector3 pivot);
	Vector3 get_pivot() const;

	Ref<PixelpartCurve> get_weight() const;
	Ref<PixelpartCurve> get_bounce() const;
	Ref<PixelpartCurve> get_friction() const;

	void set_visible(bool mode);
	void set_layer(int layer);
	bool is_visible() const;
	int get_layer() const;
	int get_blend_mode() const;

	Ref<PixelpartCurve> get_initial_size() const;
	Ref<PixelpartCurve3> get_size() const;
	void set_size_variance(float variance);
	float get_size_variance() const;

	Ref<PixelpartGradient> get_color() const;
	void set_color_hue_variance(float variance);
	void set_color_saturation_variance(float variance);
	void set_color_value_variance(float variance);
	float get_color_hue_variance() const;
	float get_color_saturation_variance() const;
	float get_color_value_variance() const;

	Ref<PixelpartCurve> get_initial_opacity() const;
	Ref<PixelpartCurve> get_opacity() const;
	void set_opacity_variance(float variance);
	float get_opacity_variance() const;

	void spawn_particles(int count);

	const pixelpart::ShaderGraph::BuildResult& get_shader_build_result() const;

private:
	Ref<PixelpartEffectResource> effectResource;
	pixelpart::ParticleType* nativeParticleType = nullptr;
	pixelpart::ParticleEngine* nativeParticleEngine = nullptr;

	pixelpart::ShaderGraph::BuildResult shaderBuildResult;
};
}

#endif