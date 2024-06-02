#ifndef PIXELPART_PARTICLE_EMITTER_H
#define PIXELPART_PARTICLE_EMITTER_H

#include "PixelpartEffectResource.h"
#include "property/PixelpartAnimatedPropertyFloat.h"
#include "property/PixelpartAnimatedPropertyFloat3.h"

namespace godot {
class PixelpartParticleEmitter : public RefCounted {
	GDCLASS(PixelpartParticleEmitter, RefCounted)

public:
	static void _bind_methods();

	PixelpartParticleEmitter();

	void init(Ref<PixelpartEffectResource> resource, pixelpart::ParticleEmitter* particleEmitterPtr, pixelpart::ParticleEngine* particleEnginePtr);

	int get_id() const;
	int get_parent_id() const;
	String get_name() const;

	void set_lifetime_start(float time);
	void set_lifetime_duration(float time);
	void set_repeat(bool value);
	float get_lifetime_start() const;
	float get_lifetime_duration() const;
	bool get_repeat() const;
	bool is_active() const;
	float get_local_time() const;

	Ref<PixelpartAnimatedPropertyFloat3> get_position() const;

	void set_shape(int type);
	int get_shape() const;
	void add_shape_point(Vector3 point);
	void remove_shape_point(int index);
	void set_shape_point(int index, Vector3 point);
	int get_num_shape_points() const;
	Vector3 get_shape_point(int index) const;

	Ref<PixelpartAnimatedPropertyFloat3> get_size() const;
	Ref<PixelpartAnimatedPropertyFloat3> get_orientation() const;

	void set_distribution(int mode);
	int get_distribution() const;
	void set_grid_order(int mode);
	int get_grid_order() const;
	void set_grid_size(int width, int height, int depth);
	int get_grid_width() const;
	int get_grid_height() const;
	int get_grid_depth() const;
	void set_emission_mode(int mode);
	int get_emission_mode() const;
	void set_direction_mode(int mode);
	int get_direction_mode() const;
	Ref<PixelpartAnimatedPropertyFloat3> get_direction() const;
	Ref<PixelpartAnimatedPropertyFloat> get_spread() const;

private:
	void set_shape_points(const std::vector<pixelpart::vec3_t>& points);
	std::vector<pixelpart::vec3_t> get_shape_points() const;

	Ref<PixelpartEffectResource> effectResource;
	pixelpart::ParticleEmitter* particleEmitter = nullptr;
	pixelpart::ParticleEngine* particleEngine = nullptr;
};
}

#endif