#ifndef PIXELPART_SHADER_PROVIDER_H
#define PIXELPART_SHADER_PROVIDER_H

#include <godot_cpp/classes/shader.hpp>
#include <pixelpart-runtime/types/Id.h>
#include <pixelpart-runtime/effect/ParticleRendererType.h>
#include <pixelpart-runtime/effect/BlendMode.h>
#include <pixelpart-runtime/effect/LightingMode.h>
#include <pixelpart-runtime/effect/ShaderGraph.h>
#include <pixelpart-runtime/effect/BuiltInMaterialMetadata.h>
#include <pixelpart-runtime/effect/BuiltInMaterialRepository.h>
#include <string>
#include <vector>
#include <unordered_map>

namespace godot {
class PixelpartShaderProvider {
public:
	static const std::string uniformPrefix;

	PixelpartShaderProvider();

	Ref<Shader> get_builtin_canvas_item_shader(const std::string& shaderId) const;
	Ref<Shader> get_builtin_spatial_shader(const std::string& shaderId) const;
	const pixelpart::BuiltInMaterialMetadata& get_builtin_canvas_item_shader_metadata(const std::string& shaderId) const;
	const pixelpart::BuiltInMaterialMetadata& get_builtin_spatial_shader_metadata(const std::string& shaderId) const;

	Ref<Shader> get_custom_canvas_item_shader(
		const std::string& mainShaderCode,
		const std::string& parameterShaderCode,
		pixelpart::ParticleRendererType renderer,
		pixelpart::BlendMode blendMode,
		pixelpart::LightingMode lightingMode);
	Ref<Shader> get_custom_spatial_shader(
		const std::string& mainShaderCode,
		const std::string& parameterShaderCode,
		pixelpart::ParticleRendererType renderer,
		pixelpart::BlendMode blendMode,
		pixelpart::LightingMode lightingMode);

private:
	static String generate_defines(
		pixelpart::ParticleRendererType renderer,
		pixelpart::BlendMode blendMode,
		pixelpart::LightingMode lightingMode);

	static Ref<Shader> create_shader_from_code(const std::string& shaderTemplate,
		const std::string& mainShaderCode,
		const std::string& parameterShaderCode,
		pixelpart::ParticleRendererType renderer,
		pixelpart::BlendMode blendMode,
		pixelpart::LightingMode lightingMode);

	static const std::string canvasItemShaderTemplate;
	static const std::string spatialShaderTemplate;

	std::unordered_map<std::string, Ref<Shader>> builtInCanvasItemShaders;
	std::unordered_map<std::string, Ref<Shader>> builtInSpatialShaders;

	pixelpart::BuiltInMaterialRepository builtInMaterialRepository;
};
}

#endif
