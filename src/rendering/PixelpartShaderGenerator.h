#ifndef PIXELPART_SHADER_GENERATOR_H
#define PIXELPART_SHADER_GENERATOR_H

#include <effect/ParticleRendererType.h>
#include <effect/BlendMode.h>
#include <effect/LightingMode.h>
#include <shadergraph/ShaderGraph.h>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/base_material3d.hpp>
#include <godot_cpp/classes/canvas_item_material.hpp>

namespace godot {
class PixelpartShaderGenerator {
public:
	struct ShaderMetadata {
		static std::unordered_map<std::string, pixelpart::id_t> builtInMaterialParameterIds;

		std::unordered_map<pixelpart::id_t, std::string> parameterNames;

		ShaderMetadata() = default;
		ShaderMetadata(std::vector<std::string> parameterList);
	};

	static const std::string uniformPrefix;

	PixelpartShaderGenerator();

	Ref<Shader> get_builtin_canvas_item_shader(const std::string& shaderId);
	Ref<Shader> get_builtin_spatial_shader(const std::string& shaderId);
	ShaderMetadata get_builtin_canvas_item_shader_metadata(const std::string& shaderId);
	ShaderMetadata get_builtin_spatial_shader_metadata(const std::string& shaderId);

	bool has_builtin_canvas_item_shader(const std::string& shaderId);
	bool has_builtin_spatial_shader(const std::string& shaderId);

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
	struct BuiltInShaderEntry {
		Ref<Shader> shader;
		ShaderMetadata metadata;
	};

	Ref<Shader> get_canvas_item_shader(const std::string& shaderTemplate,
		const std::string& mainShaderCode,
		const std::string& parameterShaderCode,
		pixelpart::BlendMode blendMode,
		pixelpart::LightingMode lightingMode);
	Ref<Shader> get_spatial_shader(const std::string& shaderTemplate,
		const std::string& mainShaderCode,
		const std::string& parameterShaderCode,
		pixelpart::BlendMode blendMode,
		pixelpart::LightingMode lightingMode,
		bool cull);
	Ref<Shader> get_shader(const std::string& shaderTemplate,
		const std::string& mainShaderCode,
		const std::string& parameterShaderCode,
		const std::string& outputCode,
		const std::string& renderMode);

	static const std::string shaderCommonCode;

	static const std::string spriteCanvasItemShader;
	static const std::string trailCanvasItemShader;
	static const std::string spriteSpatialShader;
	static const std::string trailSpatialShader;
	static const std::string meshSpatialShader;

	static const std::string canvasItemShaderTemplate;
	static const std::string spriteSpatialShaderTemplate;
	static const std::string trailSpatialShaderTemplate;
	static const std::string meshSpatialShaderTemplate;

	std::unordered_map<std::string, Ref<Shader>> shaders;

	std::unordered_map<std::string, BuiltInShaderEntry> builtInCanvasItemShaders;
	std::unordered_map<std::string, BuiltInShaderEntry> builtInSpatialShaders;
};
}

#endif