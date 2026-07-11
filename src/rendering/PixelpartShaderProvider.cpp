#include "PixelpartShaderProvider.h"
#include "../util/PixelpartUtil.h"
#include <godot_cpp/classes/rendering_server.hpp>
#include <pixelpart-runtime/effect/StringFormat.h>

namespace godot {
const std::string PixelpartShaderProvider::uniformPrefix = "u_";

PixelpartShaderProvider::PixelpartShaderProvider() {
	for(const auto& [materialName, materialMetadata] : builtInMaterialRepository.materials()) {
		const std::string& canvasItemShaderCode =
			materialMetadata.rendererType() == pixelpart::ParticleRendererType::trail ? trailCanvasItemShader : spriteCanvasItemShader;
		const std::string& spatialShaderCode =
			materialMetadata.rendererType() == pixelpart::ParticleRendererType::mesh ? meshSpatialShader :
			(materialMetadata.rendererType() == pixelpart::ParticleRendererType::trail ? trailSpatialShader : spriteSpatialShader);

		builtInCanvasItemShaders[materialName] = get_canvas_item_shader(
			canvasItemShaderCode, "", "",
			materialMetadata.rendererType(),
			materialMetadata.blendMode(),
			materialMetadata.lightingMode());

		builtInSpatialShaders[materialName] = get_spatial_shader(
			spatialShaderCode, "", "",
			materialMetadata.rendererType(),
			materialMetadata.blendMode(),
			materialMetadata.lightingMode());
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
	return get_canvas_item_shader(canvasItemShaderTemplate, mainShaderCode, parameterShaderCode, renderer, blendMode, lightingMode);
}
Ref<Shader> PixelpartShaderProvider::get_custom_spatial_shader(
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	pixelpart::ParticleRendererType renderer,
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode) {
	switch(renderer) {
		case pixelpart::ParticleRendererType::trail:
			return get_spatial_shader(trailSpatialShaderTemplate, mainShaderCode, parameterShaderCode, renderer, blendMode, lightingMode);
		case pixelpart::ParticleRendererType::mesh:
			return get_spatial_shader(meshSpatialShaderTemplate, mainShaderCode, parameterShaderCode, renderer, blendMode, lightingMode);
		default:
			return get_spatial_shader(spriteSpatialShaderTemplate, mainShaderCode, parameterShaderCode, renderer, blendMode, lightingMode);
	}
}

Ref<Shader> PixelpartShaderProvider::get_canvas_item_shader(const std::string& shaderTemplate,
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	pixelpart::ParticleRendererType renderer,
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode) {
	std::string renderMode;

	std::string outputCode = "\tfinal_Color.rgb += final_Emission;\n";
	outputCode += "\tCOLOR = final_Color;";

	switch(blendMode) {
		case pixelpart::BlendMode::alpha:
			renderMode = "blend_mix";
			break;
		case pixelpart::BlendMode::additive:
			renderMode = "blend_add";
			break;
		case pixelpart::BlendMode::subtractive:
			renderMode = "blend_sub";
			break;
		default:
			renderMode = "blend_disabled";
			break;
	}

	if(lightingMode == pixelpart::LightingMode::unlit) {
		renderMode += ",unshaded";
	}

	return get_shader(shaderTemplate, mainShaderCode, parameterShaderCode, outputCode, renderMode);
}
Ref<Shader> PixelpartShaderProvider::get_spatial_shader(const std::string& shaderTemplate,
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	pixelpart::ParticleRendererType renderer,
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode) {
	std::string renderMode = "depth_draw_opaque";

	if(renderer != pixelpart::ParticleRendererType::mesh) {
		renderMode += ",cull_disabled";
	}

	std::string outputCode = "\tALBEDO = final_Color.rgb;\n";
	if(blendMode != pixelpart::BlendMode::off) {
		outputCode += "\tALPHA = final_Color.a;\n";
	}

	outputCode += "\tEMISSION = final_Emission;\n";
	outputCode += "\tROUGHNESS = final_Roughness;\n";
	outputCode += "\tMETALLIC = final_Metallic;";

	switch(blendMode) {
		case pixelpart::BlendMode::alpha:
			renderMode += ",blend_mix";
			break;
		case pixelpart::BlendMode::additive:
			renderMode += ",blend_add";
			break;
		case pixelpart::BlendMode::subtractive:
			renderMode += ",blend_sub";
			break;
		default:
			break;
	}

	switch(lightingMode) {
		case pixelpart::LightingMode::lit:
			renderMode += ",diffuse_burley,specular_schlick_ggx";
			break;
		default:
			renderMode += ",unshaded";
			break;
	}

	return get_shader(shaderTemplate, mainShaderCode, parameterShaderCode, outputCode, renderMode);
}
Ref<Shader> PixelpartShaderProvider::get_shader(const std::string& shaderTemplate,
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	const std::string& outputCode,
	const std::string& renderMode) {

	std::string shaderSource = pixelpart::replaceString(shaderTemplate, renderMode, "{mode}");
	shaderSource = pixelpart::replaceString(shaderSource, shaderCommonCode, "{common}");
	shaderSource = pixelpart::replaceString(shaderSource, mainShaderCode, "{main}");
	shaderSource = pixelpart::replaceString(shaderSource, parameterShaderCode, "{parameter}");
	shaderSource = pixelpart::replaceString(shaderSource, outputCode, "{output}");

	if(shaders.count(shaderSource) != 0u) {
		return shaders.at(shaderSource);
	}

	Ref<Shader> shader;
	shader.instantiate();
	shader->set_code(String(shaderSource.c_str()));

	shaders[shaderSource] = shader;

	return shader;
}

const std::string PixelpartShaderProvider::shaderCommonCode = std::string(
	#include "../shaders/PixelpartShaderCommon.glsl"
);

const std::string PixelpartShaderProvider::spriteCanvasItemShader = std::string(
	#include "../shaders/PixelpartSpriteCanvasItemShader.glsl"
);
const std::string PixelpartShaderProvider::trailCanvasItemShader = std::string(
	#include "../shaders/PixelpartTrailCanvasItemShader.glsl"
);
const std::string PixelpartShaderProvider::spriteSpatialShader = std::string(
	#include "../shaders/PixelpartSpriteSpatialShader.glsl"
);
const std::string PixelpartShaderProvider::trailSpatialShader = std::string(
	#include "../shaders/PixelpartTrailSpatialShader.glsl"
);
const std::string PixelpartShaderProvider::meshSpatialShader = std::string(
	#include "../shaders/PixelpartMeshSpatialShader.glsl"
);

const std::string PixelpartShaderProvider::canvasItemShaderTemplate = std::string(
	#include "../shaders/PixelpartCanvasItemShaderTemplate.glsl"
);
const std::string PixelpartShaderProvider::spriteSpatialShaderTemplate = std::string(
	#include "../shaders/PixelpartSpriteSpatialShaderTemplate.glsl"
);
const std::string PixelpartShaderProvider::trailSpatialShaderTemplate = std::string(
	#include "../shaders/PixelpartTrailSpatialShaderTemplate.glsl"
);
const std::string PixelpartShaderProvider::meshSpatialShaderTemplate = std::string(
	#include "../shaders/PixelpartMeshSpatialShaderTemplate.glsl"
);
}
