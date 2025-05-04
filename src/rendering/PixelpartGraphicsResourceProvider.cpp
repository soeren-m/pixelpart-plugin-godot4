#include "PixelpartGraphicsResourceProvider.h"
#include <pixelpart-runtime/common/Math.h>
#include <cstdint>
#include <cstring>

namespace godot {
PixelpartGraphicsResourceProvider::PixelpartGraphicsResourceProvider() {

}

void PixelpartGraphicsResourceProvider::load(const pixelpart::Effect& effect) {
	for(const auto& resourceEntry : effect.resources().images()) {
		load_texture(resourceEntry.first, resourceEntry.second, effect.is3d());
	}

	for(const auto& resourceEntry : effect.resources().meshes()) {
		load_mesh(resourceEntry.first, resourceEntry.second);
	}
}

void PixelpartGraphicsResourceProvider::clear() {
	providerTextures.clear();
	providerMeshes.clear();
}

const PixelpartGraphicsResourceProvider::TextureResourceMap& PixelpartGraphicsResourceProvider::textures() const {
	return providerTextures;
}
const PixelpartGraphicsResourceProvider::MeshResourceMap& PixelpartGraphicsResourceProvider::meshes() const {
	return providerMeshes;
}

void PixelpartGraphicsResourceProvider::load_texture(const std::string& name, const pixelpart::ImageResource& imageResource, bool convertToLinear) {
	Image::Format imageFormat = Image::FORMAT_RGBA8;
	switch(imageResource.channels()) {
		case 1:
			imageFormat = Image::FORMAT_R8;
			break;
		case 2:
			imageFormat = Image::FORMAT_RG8;
			break;
		case 3:
			imageFormat = Image::FORMAT_RGB8;
			break;
		default:
			break;
	}

	PackedByteArray imageData;
	imageData.resize(static_cast<int64_t>(imageResource.data().size()));

	std::memcpy(imageData.ptrw(), imageResource.data().data(), imageResource.data().size());

	Ref<Image> image = Image::create_from_data(
		static_cast<int32_t>(imageResource.width()),
		static_cast<int32_t>(imageResource.height()),
		false, imageFormat, imageData);

	if(convertToLinear && imageResource.colorSpace() == pixelpart::ColorSpace::srgb) {
		image->srgb_to_linear();
	}

	providerTextures[name] = ImageTexture::create_from_image(image);
}
void PixelpartGraphicsResourceProvider::load_mesh(const std::string& name, const pixelpart::MeshResource& meshResource) {
	PackedInt32Array indexArray;
	PackedVector3Array vertexArray;
	PackedVector3Array normalArray;
	PackedVector2Array uvArray;
	indexArray.resize(meshResource.faces().size());
	vertexArray.resize(meshResource.positions().size());
	normalArray.resize(meshResource.normals().size());
	uvArray.resize(meshResource.textureCoords().size());

	int32_t* indices = indexArray.ptrw();
	for(std::size_t index = 0; index < meshResource.faces().size(); index++) {
		indices[index] = static_cast<int32_t>(meshResource.faces()[index]);
	}

	std::memcpy(vertexArray.ptrw(), meshResource.positions().data(), meshResource.positions().size() * sizeof(glm::vec3));
	std::memcpy(normalArray.ptrw(), meshResource.normals().data(), meshResource.normals().size() * sizeof(glm::vec3));
	std::memcpy(uvArray.ptrw(), meshResource.textureCoords().data(), meshResource.textureCoords().size() * sizeof(glm::vec2));

	Array meshArray;
	meshArray.resize(Mesh::ARRAY_MAX);
	meshArray[Mesh::ARRAY_VERTEX] = vertexArray;
	meshArray[Mesh::ARRAY_NORMAL] = normalArray;
	meshArray[Mesh::ARRAY_TEX_UV] = uvArray;
	meshArray[Mesh::ARRAY_INDEX] = indexArray;

	Ref<ArrayMesh> mesh;
	mesh.instantiate();
	mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, meshArray, Array(), Dictionary(),
		Mesh::ARRAY_FORMAT_VERTEX | Mesh::ARRAY_FORMAT_NORMAL | Mesh::ARRAY_FORMAT_TEX_UV | Mesh::ARRAY_FORMAT_INDEX);

	providerMeshes[name] = mesh;
}
}