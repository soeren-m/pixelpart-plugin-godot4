#ifndef PIXELPART_GRAPHICS_RESOURCE_PROVIDER_H
#define PIXELPART_GRAPHICS_RESOURCE_PROVIDER_H

#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <pixelpart-runtime/effect/Effect.h>
#include <pixelpart-runtime/effect/ImageResource.h>
#include <pixelpart-runtime/effect/MeshResource.h>
#include <string>
#include <unordered_map>

namespace godot {
class PixelpartGraphicsResourceProvider {
public:
	using TextureResourceMap = std::unordered_map<std::string, Ref<ImageTexture>>;
	using MeshResourceMap = std::unordered_map<std::string, Ref<ArrayMesh>>;

	PixelpartGraphicsResourceProvider();

	void load(const pixelpart::Effect& effect);

	void clear();

	const TextureResourceMap& textures() const;
	const MeshResourceMap& meshes() const;

private:
	void load_texture(const std::string& name, const pixelpart::ImageResource& imageResource, bool convertToLinear);
	void load_mesh(const std::string& name, const pixelpart::MeshResource& meshResource);

	TextureResourceMap providerTextures;
	MeshResourceMap providerMeshes;
};
}

#endif
