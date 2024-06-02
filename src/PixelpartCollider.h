#ifndef PIXELPART_COLLIDER_H
#define PIXELPART_COLLIDER_H

#include "PixelpartEffectResource.h"
#include "property/PixelpartStaticPropertyBool.h"
#include "property/PixelpartStaticPropertyFloat.h"
#include "property/PixelpartAnimatedPropertyFloat.h"

namespace godot {
class PixelpartCollider : public RefCounted {
	GDCLASS(PixelpartCollider, RefCounted)

public:
	static void _bind_methods();

	PixelpartCollider();

	void init(Ref<PixelpartEffectResource> resource, pixelpart::Collider* colliderPtr, pixelpart::ParticleEngine* particleEnginePtr);

	int get_id() const;
	String get_name() const;

	void set_lifetime_start(float time);
	void set_lifetime_duration(float time);
	void set_repeat(bool value);
	float get_lifetime_start() const;
	float get_lifetime_duration() const;
	bool get_repeat() const;
	bool is_active() const;
	float get_local_time() const;

	void add_point(Vector3 point);
	void set_point(int index, Vector3 point);
	void remove_point(int index);
	Vector3 get_point(int index) const;
	int get_num_points() const;

	Ref<PixelpartStaticPropertyFloat> get_width() const;
	Ref<PixelpartStaticPropertyFloat> get_orientation() const;
	Ref<PixelpartStaticPropertyBool> get_kill_on_contact() const;

	Ref<PixelpartAnimatedPropertyFloat> get_bounce() const;
	Ref<PixelpartAnimatedPropertyFloat> get_friction() const;

private:
	Ref<PixelpartEffectResource> effectResource;
	pixelpart::Collider* collider = nullptr;
	pixelpart::ParticleEngine* particleEngine = nullptr;
};
}

#endif