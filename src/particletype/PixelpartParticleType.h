#ifndef PIXELPART_PARTICLE_TYPE_H
#define PIXELPART_PARTICLE_TYPE_H

#include "../property/PixelpartStaticPropertyFloat.h"
#include "../property/PixelpartStaticPropertyFloat3.h"
#include "../property/PixelpartStaticPropertyFloat4.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include "../property/PixelpartAnimatedPropertyFloat3.h"
#include "../property/PixelpartAnimatedPropertyFloat4.h"
#include <pixelpart-runtime/effect/ParticleType.h>
#include <pixelpart-runtime/engine/EffectEngine.h>
#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
class PixelpartParticleType : public RefCounted {
	GDCLASS(PixelpartParticleType, RefCounted)

public:
	PixelpartParticleType();
	virtual ~PixelpartParticleType();

	void init(pixelpart::ParticleType* particleTypePtr, pixelpart::EffectEngine* effectEnginePtr);

	int get_id() const;
	int get_parent_id() const;
	String get_name() const;

	void set_position_relative(bool mode);
	bool is_position_relative() const;

	void set_rotation_mode(int mode);
	int get_rotation_mode() const;

	void set_alignment_mode(int mode);
	int get_alignment_mode() const;

	Ref<PixelpartStaticPropertyFloat3> get_pivot() const;

	Ref<PixelpartStaticPropertyFloat> get_motion_path_force() const;

	void set_visible(bool mode);
	bool is_visible() const;

	void set_layer(int layer);
	int get_layer() const;

	Ref<PixelpartAnimatedPropertyFloat> get_count() const;
	Ref<PixelpartAnimatedPropertyFloat> get_lifespan() const;
	Ref<PixelpartAnimatedPropertyFloat> get_initial_size() const;
	Ref<PixelpartAnimatedPropertyFloat3> get_initial_rotation() const;
	Ref<PixelpartAnimatedPropertyFloat> get_initial_velocity() const;
	Ref<PixelpartAnimatedPropertyFloat> get_inherited_velocity() const;
	Ref<PixelpartAnimatedPropertyFloat> get_initial_opacity() const;

	Ref<PixelpartAnimatedPropertyFloat3> get_position() const;
	Ref<PixelpartAnimatedPropertyFloat3> get_size() const;
	Ref<PixelpartAnimatedPropertyFloat3> get_stretch() const;
	Ref<PixelpartAnimatedPropertyFloat> get_physical_size() const;
	Ref<PixelpartAnimatedPropertyFloat3> get_rotation() const;
	Ref<PixelpartAnimatedPropertyFloat3> get_rotation_by_speed() const;
	Ref<PixelpartAnimatedPropertyFloat> get_acceleration() const;
	Ref<PixelpartAnimatedPropertyFloat> get_radial_acceleration() const;
	Ref<PixelpartAnimatedPropertyFloat> get_weight() const;
	Ref<PixelpartAnimatedPropertyFloat> get_bounce() const;
	Ref<PixelpartAnimatedPropertyFloat> get_friction() const;
	Ref<PixelpartAnimatedPropertyFloat4> get_color() const;
	Ref<PixelpartAnimatedPropertyFloat> get_opacity() const;

	Ref<PixelpartStaticPropertyFloat> get_lifespan_variance() const;
	Ref<PixelpartStaticPropertyFloat> get_size_variance() const;
	Ref<PixelpartStaticPropertyFloat3> get_rotation_variance() const;
	Ref<PixelpartStaticPropertyFloat3> get_angular_velocity_variance() const;
	Ref<PixelpartStaticPropertyFloat> get_velocity_variance() const;
	Ref<PixelpartStaticPropertyFloat4> get_color_variance() const;
	Ref<PixelpartStaticPropertyFloat> get_opacity_variance() const;

protected:
	static void _bind_methods();

private:
	pixelpart::ParticleType* particleType = nullptr;
	pixelpart::EffectEngine* effectEngine = nullptr;
};
}

#endif