#include "PixelpartShaderGenerator.h"
#include "../util/PixelpartUtil.h"
#include <common/StringUtil.h>
#include <godot_cpp/classes/rendering_server.hpp>

namespace godot {
std::unordered_map<std::string, pixelpart::id_t> PixelpartShaderGenerator::ShaderMetadata::builtInMaterialParameterIds = std::unordered_map<std::string, pixelpart::id_t> {
	{ "MainTexture", 0 },
	{ "ColorBlendMode", 10 },
	{ "Emission", 20 },
	{ "Roughness", 21 },
	{ "Metallic", 22 },
	{ "SpriteSheetRowNumber", 30 },
	{ "SpriteSheetColumnNumber", 31 },
	{ "SpriteSheetOrigin", 32 },
	{ "SpriteAnimationNumFrames", 33 },
	{ "SpriteAnimationStartFrame", 34 },
	{ "SpriteAnimationDuration", 35 },
	{ "SpriteAnimationLoop", 36 },
	{ "SoftParticles", 40 },
	{ "SoftParticleTransition", 41 },
	{ "DistanceFade", 42 },
	{ "DistanceFadeTransition", 43 }
};

PixelpartShaderGenerator::ShaderMetadata::ShaderMetadata(std::vector<std::string> parameterList) {
	for(const std::string& name : parameterList) {
		parameterNames[builtInMaterialParameterIds.at(name)] = name;
	}
}

const std::string PixelpartShaderGenerator::uniformPrefix = "u_";

PixelpartShaderGenerator::PixelpartShaderGenerator() {
	std::vector<std::string> spriteUnlitParameterNames = std::vector<std::string>{
		"MainTexture",
		"Emission",
		"ColorBlendMode",
		"SpriteSheetRowNumber",
		"SpriteSheetColumnNumber",
		"SpriteSheetOrigin",
		"SpriteAnimationNumFrames",
		"SpriteAnimationStartFrame",
		"SpriteAnimationDuration",
		"SpriteAnimationLoop",
		"SoftParticles",
		"SoftParticleTransition",
		"DistanceFade",
		"DistanceFadeTransition"
	};
	std::vector<std::string> spriteLitParameterNames = std::vector<std::string>{
		"MainTexture",
		"Emission",
		"Roughness",
		"Metallic",
		"ColorBlendMode",
		"SpriteSheetRowNumber",
		"SpriteSheetColumnNumber",
		"SpriteSheetOrigin",
		"SpriteAnimationNumFrames",
		"SpriteAnimationStartFrame",
		"SpriteAnimationDuration",
		"SpriteAnimationLoop",
		"SoftParticles",
		"SoftParticleTransition",
		"DistanceFade",
		"DistanceFadeTransition"
	};
	std::vector<std::string> trailUnlitParameterNames = std::vector<std::string>{
		"MainTexture",
		"Emission",
		"ColorBlendMode",
		"SoftParticles",
		"SoftParticleTransition",
		"DistanceFade",
		"DistanceFadeTransition"
	};
	std::vector<std::string> trailLitParameterNames = std::vector<std::string>{
		"MainTexture",
		"Emission",
		"Roughness",
		"Metallic",
		"ColorBlendMode",
		"SoftParticles",
		"SoftParticleTransition",
		"DistanceFade",
		"DistanceFadeTransition"
	};
	std::vector<std::string> meshUnlitParameterNames = std::vector<std::string>{
		"MainTexture",
		"Emission",
		"ColorBlendMode"
	};
	std::vector<std::string> meshUnlitAlphaParameterNames = std::vector<std::string>{
		"MainTexture",
		"Emission",
		"ColorBlendMode",
		"SoftParticles",
		"SoftParticleTransition",
		"DistanceFade",
		"DistanceFadeTransition"
	};
	 std::vector<std::string> meshLitParameterNames = std::vector<std::string>{
		"MainTexture",
		"Emission",
		"Roughness",
		"Metallic",
		"ColorBlendMode"
	};
	 std::vector<std::string> meshLitAlphaParameterNames = std::vector<std::string>{
		"MainTexture",
		"Emission",
		"Roughness",
		"Metallic",
		"ColorBlendMode",
		"SoftParticles",
		"SoftParticleTransition",
		"DistanceFade",
		"DistanceFadeTransition"
	};

	builtInCanvasItemShaders["SpriteUnlitAlpha"] = BuiltInShaderEntry{
		get_canvas_item_shader(spriteCanvasItemShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::unlit),
		ShaderMetadata(spriteUnlitParameterNames)
	};
	builtInCanvasItemShaders["SpriteUnlitAdditive"] = BuiltInShaderEntry{
		get_canvas_item_shader(spriteCanvasItemShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::unlit),
		ShaderMetadata(spriteUnlitParameterNames)
	};
	builtInCanvasItemShaders["TrailUnlitAlpha"] = BuiltInShaderEntry{
		get_canvas_item_shader(trailCanvasItemShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::unlit),
		ShaderMetadata(trailUnlitParameterNames)
	};
	builtInCanvasItemShaders["TrailUnlitAdditive"] = BuiltInShaderEntry{
		get_canvas_item_shader(trailCanvasItemShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::unlit),
		ShaderMetadata(trailUnlitParameterNames)
	};
	builtInCanvasItemShaders["SpriteLitAlpha"] = BuiltInShaderEntry{
		get_canvas_item_shader(spriteCanvasItemShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::unlit),
		ShaderMetadata(spriteLitParameterNames)
	};
	builtInCanvasItemShaders["SpriteLitAdditive"] = BuiltInShaderEntry{
		get_canvas_item_shader(spriteCanvasItemShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::unlit),
		ShaderMetadata(spriteLitParameterNames)
	};
	builtInCanvasItemShaders["TrailLitAlpha"] = BuiltInShaderEntry{
		get_canvas_item_shader(trailCanvasItemShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::unlit),
		ShaderMetadata(trailLitParameterNames)
	};
	builtInCanvasItemShaders["TrailLitAdditive"] = BuiltInShaderEntry{
		get_canvas_item_shader(trailCanvasItemShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::unlit),
		ShaderMetadata(trailLitParameterNames)
	};

	builtInSpatialShaders["SpriteUnlitAlpha"] = BuiltInShaderEntry{
		get_spatial_shader(spriteSpatialShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::unlit, false),
		ShaderMetadata(spriteUnlitParameterNames)
	};
	builtInSpatialShaders["SpriteUnlitAdditive"] = BuiltInShaderEntry{
		get_spatial_shader(spriteSpatialShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::unlit, false),
		ShaderMetadata(spriteUnlitParameterNames)
	};
	builtInSpatialShaders["TrailUnlitAlpha"] = BuiltInShaderEntry{
		get_spatial_shader(trailSpatialShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::unlit, false),
		ShaderMetadata(trailUnlitParameterNames)
	};
	builtInSpatialShaders["TrailUnlitAdditive"] = BuiltInShaderEntry{
		get_spatial_shader(trailSpatialShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::unlit, false),
		ShaderMetadata(trailUnlitParameterNames)
	};
	builtInSpatialShaders["MeshUnlit"] = BuiltInShaderEntry{
		get_spatial_shader(meshSpatialShader, "", "", pixelpart::BlendMode::off, pixelpart::LightingMode::unlit, true),
		ShaderMetadata(meshUnlitParameterNames)
	};
	builtInSpatialShaders["MeshUnlitAlpha"] = BuiltInShaderEntry{
		get_spatial_shader(meshSpatialShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::unlit, true),
		ShaderMetadata(meshUnlitAlphaParameterNames)
	};
	builtInSpatialShaders["SpriteLitAlpha"] = BuiltInShaderEntry{
		get_spatial_shader(spriteSpatialShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::lit, false),
		ShaderMetadata(spriteLitParameterNames)
	};
	builtInSpatialShaders["SpriteLitAdditive"] = BuiltInShaderEntry{
		get_spatial_shader(spriteSpatialShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::lit, false),
		ShaderMetadata(spriteLitParameterNames)
	};
	builtInSpatialShaders["TrailLitAlpha"] = BuiltInShaderEntry{
		get_spatial_shader(trailSpatialShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::lit, false),
		ShaderMetadata(trailLitParameterNames)
	};
	builtInSpatialShaders["TrailLitAdditive"] = BuiltInShaderEntry{
		get_spatial_shader(trailSpatialShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::lit, false),
		ShaderMetadata(trailLitParameterNames)
	};
	builtInSpatialShaders["MeshLit"] = BuiltInShaderEntry{
		get_spatial_shader(meshSpatialShader, "", "", pixelpart::BlendMode::off, pixelpart::LightingMode::lit, true),
		ShaderMetadata(meshLitParameterNames)
	};
	builtInSpatialShaders["MeshLitAlpha"] = BuiltInShaderEntry{
		get_spatial_shader(meshSpatialShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::lit, true),
		ShaderMetadata(meshLitAlphaParameterNames)
	};
}

Ref<Shader> PixelpartShaderGenerator::get_builtin_canvas_item_shader(const std::string& shaderId) {
	return builtInCanvasItemShaders.at(shaderId).shader;
}
Ref<Shader> PixelpartShaderGenerator::get_builtin_spatial_shader(const std::string& shaderId) {
	return builtInSpatialShaders.at(shaderId).shader;
}
PixelpartShaderGenerator::ShaderMetadata PixelpartShaderGenerator::get_builtin_canvas_item_shader_metadata(const std::string& shaderId) {
	return builtInCanvasItemShaders.at(shaderId).metadata;
}
PixelpartShaderGenerator::ShaderMetadata PixelpartShaderGenerator::get_builtin_spatial_shader_metadata(const std::string& shaderId) {
	return builtInSpatialShaders.at(shaderId).metadata;
}

bool PixelpartShaderGenerator::has_builtin_canvas_item_shader(const std::string& shaderId) {
	return builtInCanvasItemShaders.count(shaderId) != 0u;
}
bool PixelpartShaderGenerator::has_builtin_spatial_shader(const std::string& shaderId) {
	return builtInSpatialShaders.count(shaderId) != 0u;
}

Ref<Shader> PixelpartShaderGenerator::get_custom_canvas_item_shader(
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	pixelpart::ParticleRendererType renderer,
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode) {
	return get_canvas_item_shader(canvasItemShaderTemplate, mainShaderCode, parameterShaderCode, blendMode, lightingMode);
}
Ref<Shader> PixelpartShaderGenerator::get_custom_spatial_shader(
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	pixelpart::ParticleRendererType renderer,
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode) {
	switch(renderer) {
		case pixelpart::ParticleRendererType::trail:
			return get_spatial_shader(trailSpatialShaderTemplate, mainShaderCode, parameterShaderCode, blendMode, lightingMode, false);
		case pixelpart::ParticleRendererType::mesh:
			return get_spatial_shader(meshSpatialShaderTemplate, mainShaderCode, parameterShaderCode, blendMode, lightingMode, true);
		default:
			return get_spatial_shader(spriteSpatialShaderTemplate, mainShaderCode, parameterShaderCode, blendMode, lightingMode, false);
	}
}

Ref<Shader> PixelpartShaderGenerator::get_canvas_item_shader(const std::string& shaderTemplate,
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode) {
	std::string renderMode;

	std::string outputCode = "\tfinal_Color.rgb += final_Emission;\n";
	outputCode += "\tCOLOR = final_Color;";

	switch(blendMode) {
		case pixelpart::BlendMode::normal:
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
Ref<Shader> PixelpartShaderGenerator::get_spatial_shader(const std::string& shaderTemplate,
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode,
	bool cull) {
	std::string renderMode = "depth_draw_opaque";

	if(!cull) {
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
		case pixelpart::BlendMode::normal:
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
Ref<Shader> PixelpartShaderGenerator::get_shader(const std::string& shaderTemplate,
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

	Ref<Shader> shader = new Shader();
	shader->set_code(String(shaderSource.c_str()));

	shaders[shaderSource] = shader;

	return shader;
}

const std::string PixelpartShaderGenerator::shaderCommonCode = std::string(
	#include "../shaders/PixelpartShaderCommon.glsl"
);

const std::string PixelpartShaderGenerator::spriteCanvasItemShader = std::string(
	#include "../shaders/PixelpartSpriteCanvasItemShader.glsl"
);
const std::string PixelpartShaderGenerator::trailCanvasItemShader = std::string(
	#include "../shaders/PixelpartTrailCanvasItemShader.glsl"
);
const std::string PixelpartShaderGenerator::spriteSpatialShader = std::string(
	#include "../shaders/PixelpartSpriteSpatialShader.glsl"
);
const std::string PixelpartShaderGenerator::trailSpatialShader = std::string(
	#include "../shaders/PixelpartTrailSpatialShader.glsl"
);
const std::string PixelpartShaderGenerator::meshSpatialShader = std::string(
	#include "../shaders/PixelpartMeshSpatialShader.glsl"
);

const std::string PixelpartShaderGenerator::canvasItemShaderTemplate = std::string(
	#include "../shaders/PixelpartCanvasItemShaderTemplate.glsl"
);
const std::string PixelpartShaderGenerator::spriteSpatialShaderTemplate = std::string(
	#include "../shaders/PixelpartSpriteSpatialShaderTemplate.glsl"
);
const std::string PixelpartShaderGenerator::trailSpatialShaderTemplate = std::string(
	#include "../shaders/PixelpartTrailSpatialShaderTemplate.glsl"
);
const std::string PixelpartShaderGenerator::meshSpatialShaderTemplate = std::string(
	#include "../shaders/PixelpartMeshSpatialShaderTemplate.glsl"
);
}