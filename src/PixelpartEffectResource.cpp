#include "PixelpartEffectResource.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {
PixelpartEffectResource::PixelpartEffectResource() : Resource() {

}
PixelpartEffectResource::~PixelpartEffectResource() {

}

void PixelpartEffectResource::import(String filepath) {
	loaded = false;

	Ref<FileAccess> file = FileAccess::open(filepath, FileAccess::READ);
	if(file.is_null()) {
		UtilityFunctions::push_error(String("Failed to open file: ") + filepath);
		return;
	}

	data = file->get_buffer(file->get_length());
}

void PixelpartEffectResource::load() {
	if(loaded) {
		return;
	}

	try {
		effectAsset = pixelpart::deserializeEffectAsset(
			reinterpret_cast<const char*>(data.ptr()), data.size());

		loaded = true;
	}
	catch(const std::exception& e) {
		UtilityFunctions::push_error(
			String("Failed to parse effect file \"") + get_path() + String("\": ") + String(e.what()));
	}
}
void PixelpartEffectResource::release() {
	effectAsset = pixelpart::EffectAsset();
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

const pixelpart::EffectAsset& PixelpartEffectResource::get_asset() const {
	return effectAsset;
}

void PixelpartEffectResource::_bind_methods() {
	ClassDB::bind_method(D_METHOD("import", "filepath"), &PixelpartEffectResource::import);
	ClassDB::bind_method(D_METHOD("load"), &PixelpartEffectResource::load);
	ClassDB::bind_method(D_METHOD("release"), &PixelpartEffectResource::release);
	ClassDB::bind_method(D_METHOD("set_data", "bytes"), &PixelpartEffectResource::set_data);
	ClassDB::bind_method(D_METHOD("get_data"), &PixelpartEffectResource::get_data);
	ClassDB::bind_method(D_METHOD("set_scale", "s"), &PixelpartEffectResource::set_scale);
	ClassDB::bind_method(D_METHOD("get_scale"), &PixelpartEffectResource::get_scale);

	ADD_PROPERTY(PropertyInfo(Variant::PACKED_BYTE_ARRAY, "data"), "set_data", "get_data");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "scale", PROPERTY_HINT_RANGE, "0.0,1000.0,1.0"), "set_scale", "get_scale");
}
}