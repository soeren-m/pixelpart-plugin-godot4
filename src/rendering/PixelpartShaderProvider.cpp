#include "PixelpartShaderProvider.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <pixelpart-runtime/effect/StringFormat.h>

namespace godot {
const std::string PixelpartShaderProvider::uniformPrefix = "u_";

PixelpartShaderProvider::PixelpartShaderProvider() {
	const String shaderBasePath = "res://addons/pixelpart/shaders/";

	ResourceLoader* resourceLoader = ResourceLoader::get_singleton();

	for(const auto& [materialName, materialMetadata] : builtInMaterialRepository.materials()) {
		String canvasItemShaderFile =
			String("Pixelpart") + pxpt_to_gd(materialName) + String("CanvasItem.gdshader");
		String spatialShaderFile =
			String("Pixelpart") + pxpt_to_gd(materialName) + String("Spatial.gdshader");

		if(materialMetadata.rendererType() != pixelpart::ParticleRendererType::mesh) {
			Ref<Shader> canvasItemShader = resourceLoader->load(shaderBasePath + canvasItemShaderFile, "Shader");
			if(canvasItemShader.is_valid()) {
				builtInCanvasItemShaders[materialName] = canvasItemShader;
			}
			else {
				UtilityFunctions::push_error(String("Failed to load built-in shader \"") + canvasItemShaderFile + String("\""));
				continue;
			}
		}

		Ref<Shader> spatialShader = resourceLoader->load(shaderBasePath + spatialShaderFile, "Shader");
		if(spatialShader.is_valid()) {
			builtInSpatialShaders[materialName] = spatialShader;
		}
		else {
			UtilityFunctions::push_error(String("Failed to load built-in shader \"") + spatialShaderFile + String("\""));
			continue;
		}
	}
}

Ref<Shader> PixelpartShaderProvider::get_builtin_canvas_item_shader(const std::string& shaderId) const {
	if(builtInCanvasItemShaders.count(shaderId) == 0) {
		return Ref<Shader>();
	}

	return builtInCanvasItemShaders.at(shaderId);
}
Ref<Shader> PixelpartShaderProvider::get_builtin_spatial_shader(const std::string& shaderId) const {
	if(builtInSpatialShaders.count(shaderId) == 0) {
		return Ref<Shader>();
	}

	return builtInSpatialShaders.at(shaderId);
}
const pixelpart::BuiltInMaterialMetadata& PixelpartShaderProvider::get_builtin_canvas_item_shader_metadata(const std::string& shaderId) const {
	return builtInMaterialRepository.materials().at(shaderId);
}
const pixelpart::BuiltInMaterialMetadata& PixelpartShaderProvider::get_builtin_spatial_shader_metadata(const std::string& shaderId) const {
	return builtInMaterialRepository.materials().at(shaderId);
}

Ref<Shader> PixelpartShaderProvider::get_custom_canvas_item_shader(
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	pixelpart::ParticleRendererType renderer,
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode) {
	return create_shader_from_code(canvasItemShaderTemplate, mainShaderCode, parameterShaderCode, renderer, blendMode, lightingMode);
}
Ref<Shader> PixelpartShaderProvider::get_custom_spatial_shader(
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	pixelpart::ParticleRendererType renderer,
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode) {
	return create_shader_from_code(spatialShaderTemplate, mainShaderCode, parameterShaderCode, renderer, blendMode, lightingMode);
}

String PixelpartShaderProvider::generate_defines(
	pixelpart::ParticleRendererType renderer,
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode) {
	const auto define = [](String& shaderCode, const String& def) {
		shaderCode += "#define ";
		shaderCode += def;
		shaderCode += "\n";
	};

	String shaderCode;

	switch(renderer) {
		case pixelpart::ParticleRendererType::mesh:
			define(shaderCode, "PIXELPART_RENDERER_MESH");
			break;
		case pixelpart::ParticleRendererType::trail:
			define(shaderCode, "PIXELPART_RENDERER_TRAIL");
			break;
		default:
			define(shaderCode, "PIXELPART_RENDERER_SPRITE");
			break;
	}

	switch(blendMode) {
		case pixelpart::BlendMode::alpha:
			define(shaderCode, "PIXELPART_BLEND_ALPHA");
			break;
		case pixelpart::BlendMode::additive:
			define(shaderCode, "PIXELPART_BLEND_ADDITIVE");
			break;
		case pixelpart::BlendMode::subtractive:
			define(shaderCode, "PIXELPART_BLEND_SUBTRACTIVE");
			break;
		default:
			define(shaderCode, "PIXELPART_BLEND_OFF");
			break;
	}

	switch(lightingMode) {
		case pixelpart::LightingMode::lit:
			define(shaderCode, "PIXELPART_LIGHTING_LIT");
			break;
		default:
			define(shaderCode, "PIXELPART_LIGHTING_UNLIT");
			break;
	}

	shaderCode += "\n";

	return shaderCode;
}

Ref<Shader> PixelpartShaderProvider::create_shader_from_code(const std::string& shaderTemplate,
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	pixelpart::ParticleRendererType renderer,
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode) {
	std::string shaderSource = pixelpart::replaceString(shaderTemplate, mainShaderCode, "{main}");
	shaderSource = pixelpart::replaceString(shaderSource, parameterShaderCode, "{parameter}");

	String shaderCode = generate_defines(renderer, blendMode, lightingMode);
	shaderCode += String(shaderSource.c_str());

	Ref<Shader> shader;
	shader.instantiate();
	shader->set_code(shaderCode);

	return shader;
}

const std::string PixelpartShaderProvider::canvasItemShaderTemplate = std::string(
	#include "PixelpartCanvasItemShaderTemplate.glsl"
);
const std::string PixelpartShaderProvider::spatialShaderTemplate = std::string(
	#include "PixelpartSpatialShaderTemplate.glsl"
);
}
