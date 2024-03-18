#ifndef PIXELPART_PARTICLE_EMITTER_H
#define PIXELPART_PARTICLE_EMITTER_H

#include "PixelpartEffectResource.h"
#include "properties/PixelpartAnimatedPropertyFloat.h"
#include "properties/PixelpartAnimatedPropertyFloat3.h"

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
	//Ref<PixelpartAnimatedPropertyFloat3> get_path() const; TODO
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
	Ref<PixelpartEffectResource> effectResource;
	pixelpart::ParticleEmitter* particleEmitter = nullptr;
	pixelpart::ParticleEngine* particleEngine = nullptr;
};
}

#endif