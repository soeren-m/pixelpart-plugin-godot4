#include "PixelpartEffectResource.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {
void PixelpartEffectResource::_bind_methods() {
	ClassDB::bind_method(D_METHOD("import", "filepath"), &PixelpartEffectResource::import);
	ClassDB::bind_method(D_METHOD("load"), &PixelpartEffectResource::load);
	ClassDB::bind_method(D_METHOD("release"), &PixelpartEffectResource::release);
	ClassDB::bind_method(D_METHOD("set_data"), &PixelpartEffectResource::set_data);
	ClassDB::bind_method(D_METHOD("get_data"), &PixelpartEffectResource::get_data);
	ClassDB::bind_method(D_METHOD("set_scale"), &PixelpartEffectResource::set_scale);
	ClassDB::bind_method(D_METHOD("get_scale"), &PixelpartEffectResource::get_scale);

	ADD_PROPERTY(PropertyInfo(Variant::PACKED_BYTE_ARRAY, "data"), "set_data", "get_data");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "scale", PROPERTY_HINT_RANGE, "0.0,1000.0,1.0"), "set_scale", "get_scale");
}

PixelpartEffectResource::PixelpartEffectResource() {

}
PixelpartEffectResource::~PixelpartEffectResource() {
	release();
}

void PixelpartEffectResource::import(String filepath) {
	Ref<FileAccess> file = FileAccess::open(filepath, FileAccess::READ);

	if(!file.is_null()) {
		data = file->get_buffer(file->get_length());
	}
	else {
		UtilityFunctions::printerr(String("Failed to open file: ") + filepath);
	}

	loaded = false;
}
void PixelpartEffectResource::load() {
	if(loaded) {
		return;
	}

	try {
		project = pixelpart::deserialize((const char*)data.ptr(), data.size());
	}
	catch(const std::exception& e) {
		UtilityFunctions::printerr(String("Failed to parse effect file: ") + get_path());
	}

	loaded = true;
}
void PixelpartEffectResource::release() {
	project = pixelpart::Project();
	loaded = false;
}

void PixelpartEffectResource::set_data(PackedByteArray bytes) {
	data = bytes;
}
PackedByteArray PixelpartEffectResource::get_data() const {
	return data;
}

void PixelpartEffectResource::set_scale(float s) {
	scale = s;
}
float PixelpartEffectResource::get_scale() const {
	return scale;
}

const pixelpart::Project& PixelpartEffectResource::get_project() const {
	return project;
}
}