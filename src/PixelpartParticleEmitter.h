#ifndef PIXELPART_PARTICLEEMITTER_H
#define PIXELPART_PARTICLEEMITTER_H

#include "PixelpartEffectResource.h"
#include "PixelpartCurve.h"
#include "PixelpartCurve3.h"
#include "PixelpartGradient.h"
#include "ParticleEmitter.h"
#include "ParticleEngine.h"

namespace godot {
class PixelpartParticleEmitter : public RefCounted {
	GDCLASS(PixelpartParticleEmitter, RefCounted)

public:
	static void _bind_methods();

	PixelpartParticleEmitter();

	void init(Ref<PixelpartEffectResource> resource, pixelpart::ParticleEmitter* particleEmitter, pixelpart::ParticleEngine* engine);

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

	Ref<PixelpartCurve3> get_position() const;

	void set_shape(int type);
	int get_shape() const;

	Ref<PixelpartCurve3> get_path() const;
	Ref<PixelpartCurve3> get_size() const;
	Ref<PixelpartCurve3> get_orientation() const;

	void set_distribution(int mode);
	void set_emission_mode(int mode);
	void set_direction_mode(int mode);
	int get_distribution() const;
	int get_emission_mode() const;
	int get_direction_mode() const;

	Ref<PixelpartCurve3> get_direction() const;
	Ref<PixelpartCurve> get_spread() const;

private:
	Ref<PixelpartEffectResource> effectResource;
	pixelpart::ParticleEmitter* nativeParticleEmitter = nullptr;
	pixelpart::ParticleEngine* nativeParticleEngine = nullptr;
};
}

#endif