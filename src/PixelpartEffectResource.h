#ifndef PIXELPART_EFFECT_RESOURCE_H
#define PIXELPART_EFFECT_RESOURCE_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <pixelpart-runtime/asset/EffectAsset.h>

namespace godot {
class PixelpartEffectResource : public Resource {
	GDCLASS(PixelpartEffectResource, Resource)

public:
	PixelpartEffectResource();
	virtual ~PixelpartEffectResource();

	void import(String filepath);

	void load();
	void release();

	void set_data(PackedByteArray bytes);
	PackedByteArray get_data() const;

	void set_scale(float s);
	float get_scale() const;

	const pixelpart::EffectAsset& get_asset() const;

protected:
	static void _bind_methods();

private:
	PackedByteArray data;
	float scale = 1.0f;

	pixelpart::EffectAsset effectAsset;
	bool loaded = false;
};
}

#endif