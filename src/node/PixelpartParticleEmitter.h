#ifndef PIXELPART_PARTICLE_EMITTER_H
#define PIXELPART_PARTICLE_EMITTER_H

#include "PixelpartNode.h"
#include "../property/PixelpartAnimatedPropertyFloat.h"
#include "../property/PixelpartAnimatedPropertyFloat3.h"
#include <pixelpart-runtime/effect/ParticleEmitter.h>
#include <vector>

namespace godot {
class PixelpartParticleEmitter : public PixelpartNode {
	GDCLASS(PixelpartParticleEmitter, PixelpartNode)

public:
	PixelpartParticleEmitter();
	virtual ~PixelpartParticleEmitter();

	virtual void init(pixelpart::Node* internalNode, pixelpart::EffectEngine* effectEnginePtr) override;

	void set_shape(int type);
	int get_shape() const;
	void add_shape_point(Vector3 point);
	void remove_shape_point(int index);
	void set_shape_point(int index, Vector3 point);
	Vector3 get_shape_point(int index) const;
	int get_shape_point_count() const;

	void set_distribution(int mode);
	int get_distribution() const;
	void set_grid_order(int mode);
	int get_grid_order() const;
	void set_grid_size(int x, int y, int z);
	int get_grid_size_x() const;
	int get_grid_size_y() const;
	int get_grid_size_z() const;
	void set_emission_mode(int mode);
	int get_emission_mode() const;
	void set_direction_mode(int mode);
	int get_direction_mode() const;

	Ref<PixelpartAnimatedPropertyFloat3> get_direction() const;
	Ref<PixelpartAnimatedPropertyFloat> get_spread() const;

protected:
	static void _bind_methods();

private:
	void set_shape_points(const std::vector<pixelpart::float3_t>& points);
	std::vector<pixelpart::float3_t> get_shape_points() const;

	pixelpart::ParticleEmitter* particleEmitter = nullptr;
};
}

#endif