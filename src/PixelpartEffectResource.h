#ifndef PIXELPART_EFFECTRESOURCE_H
#define PIXELPART_EFFECTRESOURCE_H

#include "Project.h"
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

namespace godot {
class PixelpartEffectResource : public Resource {
	GDCLASS(PixelpartEffectResource, Resource)

public:
	static void _bind_methods();

	PixelpartEffectResource();
	~PixelpartEffectResource();

	void import(String filepath);
	void load();
	void release();

	void set_data(PackedByteArray bytes);
	PackedByteArray get_data() const;

	void set_scale(float s);
	float get_scale() const;

	const pixelpart::Project& get_project() const;
	const pixelpart::ResourceDatabase& get_project_resources() const;

private:
	pixelpart::Project project;
	pixelpart::ResourceDatabase projectResources;
	bool loaded = false;

	PackedByteArray data;
	float scale = 10.0f;
};
}

#endif