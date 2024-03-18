#ifndef PIXELPART_GRAPHICS_RESOURCE_STORE_H
#define PIXELPART_GRAPHICS_RESOURCE_STORE_H

#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <unordered_map>
#include <string>

namespace godot {
struct PixelpartGraphicsResourceStore {
	std::unordered_map<std::string, Ref<ImageTexture>> textures;
	std::unordered_map<std::string, Ref<ArrayMesh>> meshes;
};
}

#endif