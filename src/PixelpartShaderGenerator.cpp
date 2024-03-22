#include "PixelpartShaderGenerator.h"
#include "util/PixelpartUtil.h"
#include "common/StringUtil.h"
#include <godot_cpp/classes/rendering_server.hpp>

namespace godot {
const std::string PixelpartShaderGenerator::builtInMaterialPrefix = "builtIn-";
const std::string PixelpartShaderGenerator::uniformPrefix = "u_";

PixelpartShaderGenerator::PixelpartShaderGenerator() {
	// TODO: why soft particles, distance fade for 2d?

	std::vector<pixelpart::VariantParameter> unlitBaseShaderParameters = std::vector<pixelpart::VariantParameter>{
		pixelpart::VariantParameter::createImageResourceParameter("MainTexture"),
		pixelpart::VariantParameter::createFloat3Parameter("Emission", pixelpart::vec3_t(0.0), pixelpart::vec3_t(0.0), pixelpart::vec3_t(1000.0)),
		pixelpart::VariantParameter::createEnumParameter("ColorBlendMode", 0, {
			"blend_multiply",
			"blend_add",
			"blend_subtract",
			"blend_difference",
			"blend_screen",
			"blend_overlay",
			"blend_lighten",
			"blend_darken",
			"blend_coloronly"
		})
	};

	std::vector<pixelpart::VariantParameter> unlitSpriteShaderParameters = concat(unlitBaseShaderParameters, std::vector<pixelpart::VariantParameter>{
		pixelpart::VariantParameter::createIntParameter("SpriteSheetRowNumber", 1, 1, 1000),
		pixelpart::VariantParameter::createIntParameter("SpriteSheetColumnNumber", 1, 1, 1000),
		pixelpart::VariantParameter::createEnumParameter("SpriteSheetOrigin", 0, {
			"origin_bl",
			"origin_br",
			"origin_tl",
			"origin_tr"
		}),
		pixelpart::VariantParameter::createIntParameter("SpriteAnimationNumFrames", 1, 1, 1000),
		pixelpart::VariantParameter::createIntParameter("SpriteAnimationStartFrame", 0, 0, 1000),
		pixelpart::VariantParameter::createFloatParameter("SpriteAnimationDuration", 1.0, 0.0, 1000.0),
		pixelpart::VariantParameter::createBoolParameter("SpriteAnimationLoop", false),
		pixelpart::VariantParameter::createBoolParameter("SoftParticles", false),
		pixelpart::VariantParameter::createFloatParameter("SoftParticleTransition", 0.1, 0.0, 1.0),
		pixelpart::VariantParameter::createBoolParameter("DistanceFade", false),
		pixelpart::VariantParameter::createFloatParameter("DistanceFadeTransition", 0.1, 0.0, 10.0)
	});

	std::vector<pixelpart::VariantParameter> unlitTrailShaderParameters = concat(unlitBaseShaderParameters, std::vector<pixelpart::VariantParameter>{
		pixelpart::VariantParameter::createBoolParameter("SoftParticles", false),
		pixelpart::VariantParameter::createFloatParameter("SoftParticleTransition", 0.1, 0.0, 1.0),
		pixelpart::VariantParameter::createBoolParameter("DistanceFade", false),
		pixelpart::VariantParameter::createFloatParameter("DistanceFadeTransition", 0.1, 0.0, 10.0)
	});

	std::vector<pixelpart::VariantParameter> unlitMeshShaderParameters = unlitBaseShaderParameters;

	std::vector<pixelpart::VariantParameter> litBaseShaderParameters = std::vector<pixelpart::VariantParameter>{
		pixelpart::VariantParameter::createImageResourceParameter("MainTexture"),
		pixelpart::VariantParameter::createFloat3Parameter("Emission", pixelpart::vec3_t(0.0), pixelpart::vec3_t(0.0), pixelpart::vec3_t(1000.0)),
		pixelpart::VariantParameter::createFloatParameter("Roughness", 0.5, 0.0, 1.0),
		pixelpart::VariantParameter::createFloatParameter("Metallic", 0.0, 0.0, 1.0),
		pixelpart::VariantParameter::createEnumParameter("ColorBlendMode", 0, {
			"blend_multiply",
			"blend_add",
			"blend_subtract",
			"blend_difference",
			"blend_screen",
			"blend_overlay",
			"blend_lighten",
			"blend_darken",
			"blend_coloronly"
		})
	};

	std::vector<pixelpart::VariantParameter> litSpriteShaderParameters = concat(litBaseShaderParameters, std::vector<pixelpart::VariantParameter>{
		pixelpart::VariantParameter::createIntParameter("SpriteSheetRowNumber", 1, 1, 1000),
		pixelpart::VariantParameter::createIntParameter("SpriteSheetColumnNumber", 1, 1, 1000),
		pixelpart::VariantParameter::createEnumParameter("SpriteSheetOrigin", 0, {
			"origin_bl",
			"origin_br",
			"origin_tl",
			"origin_tr"
		}),
		pixelpart::VariantParameter::createIntParameter("SpriteAnimationNumFrames", 1, 1, 1000),
		pixelpart::VariantParameter::createIntParameter("SpriteAnimationStartFrame", 0, 0, 1000),
		pixelpart::VariantParameter::createFloatParameter("SpriteAnimationDuration", 1.0, 0.0, 1000.0),
		pixelpart::VariantParameter::createBoolParameter("SpriteAnimationLoop", false),
		pixelpart::VariantParameter::createBoolParameter("SoftParticles", false),
		pixelpart::VariantParameter::createFloatParameter("SoftParticleTransition", 0.1, 0.0, 1.0),
		pixelpart::VariantParameter::createBoolParameter("DistanceFade", false),
		pixelpart::VariantParameter::createFloatParameter("DistanceFadeTransition", 0.1, 0.0, 10.0)
	});

	std::vector<pixelpart::VariantParameter> litTrailShaderParameters = concat(litBaseShaderParameters, std::vector<pixelpart::VariantParameter>{
		pixelpart::VariantParameter::createBoolParameter("SoftParticles", false),
		pixelpart::VariantParameter::createFloatParameter("SoftParticleTransition", 0.1, 0.0, 1.0),
		pixelpart::VariantParameter::createBoolParameter("DistanceFade", false),
		pixelpart::VariantParameter::createFloatParameter("DistanceFadeTransition", 0.1, 0.0, 10.0)
	});

	std::vector<pixelpart::VariantParameter> litMeshShaderParameters = litBaseShaderParameters;

	builtInCanvasItemShaders[builtInMaterialPrefix + "SpriteUnlitNormal"] = BuiltInShaderEntry{
		get_canvas_item_shader(spriteCanvasItemShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::unlit),
		ShaderMetadata{ unlitSpriteShaderParameters }
	};
	builtInCanvasItemShaders[builtInMaterialPrefix + "SpriteUnlitAdditive"] = BuiltInShaderEntry{
		get_canvas_item_shader(spriteCanvasItemShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::unlit),
		ShaderMetadata{ unlitSpriteShaderParameters }
	};
	builtInCanvasItemShaders[builtInMaterialPrefix + "SpriteUnlitSubtractive"] = BuiltInShaderEntry{
		get_canvas_item_shader(spriteCanvasItemShader, "", "", pixelpart::BlendMode::subtractive, pixelpart::LightingMode::unlit),
		ShaderMetadata{ unlitSpriteShaderParameters }
	};
	builtInCanvasItemShaders[builtInMaterialPrefix + "TrailUnlitNormal"] = BuiltInShaderEntry{
		get_canvas_item_shader(trailCanvasItemShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::unlit),
		ShaderMetadata{ unlitTrailShaderParameters }
	};
	builtInCanvasItemShaders[builtInMaterialPrefix + "TrailUnlitAdditive"] = BuiltInShaderEntry{
		get_canvas_item_shader(trailCanvasItemShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::unlit),
		ShaderMetadata{ unlitTrailShaderParameters }
	};
	builtInCanvasItemShaders[builtInMaterialPrefix + "TrailUnlitSubtractive"] = BuiltInShaderEntry{
		get_canvas_item_shader(trailCanvasItemShader, "", "", pixelpart::BlendMode::subtractive, pixelpart::LightingMode::unlit),
		ShaderMetadata{ unlitTrailShaderParameters }
	};
	builtInCanvasItemShaders[builtInMaterialPrefix + "SpriteLitNormal"] = BuiltInShaderEntry{
		get_canvas_item_shader(spriteCanvasItemShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::unlit),
		ShaderMetadata{ litSpriteShaderParameters }
	};
	builtInCanvasItemShaders[builtInMaterialPrefix + "SpriteLitAdditive"] = BuiltInShaderEntry{
		get_canvas_item_shader(spriteCanvasItemShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::unlit),
		ShaderMetadata{ litSpriteShaderParameters }
	};
	builtInCanvasItemShaders[builtInMaterialPrefix + "SpriteLitSubtractive"] = BuiltInShaderEntry{
		get_canvas_item_shader(spriteCanvasItemShader, "", "", pixelpart::BlendMode::subtractive, pixelpart::LightingMode::unlit),
		ShaderMetadata{ litSpriteShaderParameters }
	};
	builtInCanvasItemShaders[builtInMaterialPrefix + "TrailLitNormal"] = BuiltInShaderEntry{
		get_canvas_item_shader(trailCanvasItemShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::unlit),
		ShaderMetadata{ litTrailShaderParameters }
	};
	builtInCanvasItemShaders[builtInMaterialPrefix + "TrailLitAdditive"] = BuiltInShaderEntry{
		get_canvas_item_shader(trailCanvasItemShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::unlit),
		ShaderMetadata{ litTrailShaderParameters }
	};
	builtInCanvasItemShaders[builtInMaterialPrefix + "TrailLitSubtractive"] = BuiltInShaderEntry{
		get_canvas_item_shader(trailCanvasItemShader, "", "", pixelpart::BlendMode::subtractive, pixelpart::LightingMode::unlit),
		ShaderMetadata{ litTrailShaderParameters }
	};

	builtInSpatialShaders[builtInMaterialPrefix + "SpriteUnlitNormal"] = BuiltInShaderEntry{
		get_spatial_shader(spriteSpatialShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::unlit),
		ShaderMetadata{ unlitSpriteShaderParameters }
	};
	builtInSpatialShaders[builtInMaterialPrefix + "SpriteUnlitAdditive"] = BuiltInShaderEntry{
		get_spatial_shader(spriteSpatialShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::unlit),
		ShaderMetadata{ unlitSpriteShaderParameters }
	};
	builtInSpatialShaders[builtInMaterialPrefix + "SpriteUnlitSubtractive"] = BuiltInShaderEntry{
		get_spatial_shader(spriteSpatialShader, "", "", pixelpart::BlendMode::subtractive, pixelpart::LightingMode::unlit),
		ShaderMetadata{ unlitSpriteShaderParameters }
	};
	builtInSpatialShaders[builtInMaterialPrefix + "TrailUnlitNormal"] = BuiltInShaderEntry{
		get_spatial_shader(trailSpatialShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::unlit),
		ShaderMetadata{ unlitTrailShaderParameters }
	};
	builtInSpatialShaders[builtInMaterialPrefix + "TrailUnlitAdditive"] = BuiltInShaderEntry{
		get_spatial_shader(trailSpatialShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::unlit),
		ShaderMetadata{ unlitTrailShaderParameters }
	};
	builtInSpatialShaders[builtInMaterialPrefix + "TrailUnlitSubtractive"] = BuiltInShaderEntry{
		get_spatial_shader(trailSpatialShader, "", "", pixelpart::BlendMode::subtractive, pixelpart::LightingMode::unlit),
		ShaderMetadata{ unlitTrailShaderParameters }
	};
	builtInSpatialShaders[builtInMaterialPrefix + "MeshUnlit"] = BuiltInShaderEntry{
		get_spatial_shader(meshSpatialShader, "", "", pixelpart::BlendMode::off, pixelpart::LightingMode::unlit),
		ShaderMetadata{ unlitMeshShaderParameters }
	};
	builtInSpatialShaders[builtInMaterialPrefix + "SpriteLitNormal"] = BuiltInShaderEntry{
		get_spatial_shader(spriteSpatialShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::lit),
		ShaderMetadata{ litSpriteShaderParameters }
	};
	builtInSpatialShaders[builtInMaterialPrefix + "SpriteLitAdditive"] = BuiltInShaderEntry{
		get_spatial_shader(spriteSpatialShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::lit),
		ShaderMetadata{ litSpriteShaderParameters }
	};
	builtInSpatialShaders[builtInMaterialPrefix + "SpriteLitSubtractive"] = BuiltInShaderEntry{
		get_spatial_shader(spriteSpatialShader, "", "", pixelpart::BlendMode::subtractive, pixelpart::LightingMode::lit),
		ShaderMetadata{ litSpriteShaderParameters }
	};
	builtInSpatialShaders[builtInMaterialPrefix + "TrailLitNormal"] = BuiltInShaderEntry{
		get_spatial_shader(trailSpatialShader, "", "", pixelpart::BlendMode::normal, pixelpart::LightingMode::lit),
		ShaderMetadata{ litTrailShaderParameters }
	};
	builtInSpatialShaders[builtInMaterialPrefix + "TrailLitAdditive"] = BuiltInShaderEntry{
		get_spatial_shader(trailSpatialShader, "", "", pixelpart::BlendMode::additive, pixelpart::LightingMode::lit),
		ShaderMetadata{ litTrailShaderParameters }
	};
	builtInSpatialShaders[builtInMaterialPrefix + "TrailLitSubtractive"] = BuiltInShaderEntry{
		get_spatial_shader(trailSpatialShader, "", "", pixelpart::BlendMode::subtractive, pixelpart::LightingMode::lit),
		ShaderMetadata{ litTrailShaderParameters }
	};
	builtInSpatialShaders[builtInMaterialPrefix + "MeshLit"] = BuiltInShaderEntry{
		get_spatial_shader(meshSpatialShader, "", "", pixelpart::BlendMode::off, pixelpart::LightingMode::lit),
		ShaderMetadata{ litMeshShaderParameters }
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
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode) {
	return get_canvas_item_shader(canvasItemShaderTemplate, mainShaderCode, parameterShaderCode, blendMode, lightingMode);
}
Ref<Shader> PixelpartShaderGenerator::get_custom_spatial_shader(
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode) {
	return get_spatial_shader(spatialShaderTemplate, mainShaderCode, parameterShaderCode, blendMode, lightingMode);
}

Ref<Shader> PixelpartShaderGenerator::get_canvas_item_shader(const std::string& shaderTemplate,
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode) {
	std::string renderMode;

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

	return get_shader(shaderTemplate, mainShaderCode, parameterShaderCode, renderMode);
}
Ref<Shader> PixelpartShaderGenerator::get_spatial_shader(const std::string& shaderTemplate,
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	pixelpart::BlendMode blendMode,
	pixelpart::LightingMode lightingMode) {
	std::string renderMode = "cull_disabled";

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

	return get_shader(shaderTemplate, mainShaderCode, parameterShaderCode, renderMode);
}
Ref<Shader> PixelpartShaderGenerator::get_shader(const std::string& shaderTemplate,
	const std::string& mainShaderCode,
	const std::string& parameterShaderCode,
	const std::string& renderMode) {

	std::string shaderSource = pixelpart::replace(shaderTemplate, renderMode, "{mode}");
	shaderSource = pixelpart::replace(shaderSource, shaderCommonCode, "{common}");
	shaderSource = pixelpart::replace(shaderSource, mainShaderCode, "{main}");
	shaderSource = pixelpart::replace(shaderSource, parameterShaderCode, "{parameter}");

	if(shaders.count(shaderSource) != 0u) {
		return shaders.at(shaderSource);
	}

	Ref<Shader> shader = new Shader();
	shader->set_code(String(shaderSource.c_str()));

	shaders[shaderSource] = shader;

	return shader;
}

const std::string PixelpartShaderGenerator::shaderCommonCode = std::string(
	#include "shaders/PixelpartShaderCommon.glsl"
);

const std::string PixelpartShaderGenerator::spriteCanvasItemShader = std::string(
	#include "shaders/PixelpartSpriteCanvasItemShader.glsl"
);
const std::string PixelpartShaderGenerator::trailCanvasItemShader = std::string(
	#include "shaders/PixelpartTrailCanvasItemShader.glsl"
);
const std::string PixelpartShaderGenerator::spriteSpatialShader = std::string(
	#include "shaders/PixelpartSpriteSpatialShader.glsl"
);
const std::string PixelpartShaderGenerator::trailSpatialShader = std::string(
	#include "shaders/PixelpartTrailSpatialShader.glsl"
);
const std::string PixelpartShaderGenerator::meshSpatialShader = std::string(
	#include "shaders/PixelpartMeshSpatialShader.glsl"
);

const std::string PixelpartShaderGenerator::canvasItemShaderTemplate = std::string(
	#include "shaders/PixelpartCanvasItemShaderTemplate.glsl"
);
const std::string PixelpartShaderGenerator::spatialShaderTemplate = std::string(
	#include "shaders/PixelpartSpatialShaderTemplate.glsl"
);
}