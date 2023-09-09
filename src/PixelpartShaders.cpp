#include "PixelpartShaders.h"
#include "PixelpartShaderGraph.h"
#include "PixelpartUtil.h"
#include "ShaderGraph.h"
#include <godot_cpp/classes/rendering_server.hpp>

namespace godot {
PixelpartShaders* PixelpartShaders::instance = nullptr;
PixelpartShaders* PixelpartShaders::get_instance() {
	return instance;
}

PixelpartShaders::PixelpartShaders() {
	instance = this;

	nlohmann::ordered_json modelJson = nlohmann::json::parse(
		PixelpartShaderGraph_json,
		PixelpartShaderGraph_json + PixelpartShaderGraph_json_size);
	pixelpart::ShaderGraph::graphType = modelJson;
}
PixelpartShaders::~PixelpartShaders() {
	instance = nullptr;
}

Ref<Shader> PixelpartShaders::get_canvas_shader(const std::string& shaderSource,
	pixelpart::BlendMode blendMode,
	CanvasItemMaterial::LightMode lightMode) {
	std::string renderMode;
	switch(blendMode) {
		case pixelpart::BlendMode::additive:
			renderMode = "blend_add";
			break;
		case pixelpart::BlendMode::subtractive:
			renderMode = "blend_sub";
			break;
		default:
			renderMode = "blend_mix";
			break;
	}

	switch(lightMode) {
		case CanvasItemMaterial::LIGHT_MODE_UNSHADED:
			renderMode += ",unshaded";
			break;
		case CanvasItemMaterial::LIGHT_MODE_LIGHT_ONLY:
			renderMode += ",light_only";
			break;
		default:
			break;
	}

	std::string vertexShaderSource = R"!(const float __PACK_FACTOR__ = 10000.0;
		VELOCITY = vec3((floor(COLOR.rg) - vec2(__PACK_FACTOR__)) / vec2(__PACK_FACTOR__), 0.0);
		FORCE = vec3((floor(COLOR.ba) - vec2(__PACK_FACTOR__)) / vec2(__PACK_FACTOR__), 0.0);
		LIFE = floor(UV.x) / __PACK_FACTOR__;
		OBJECT_ID = floor(UV.y) - 1.0;
		UV = fract(UV) * vec2(10.0);
		COLOR = fract(COLOR) * vec4(10.0);)!";

	std::string additionalFragmentShaderSource = R"!(COLOR = __SHADER_COLOR__;)!" ;

	return get_shader(shaderSource, "canvas_item", renderMode, vertexShaderSource, additionalFragmentShaderSource);
}
Ref<Shader> PixelpartShaders::get_spatial_shader(const std::string& shaderSource,
	pixelpart::BlendMode blendMode,
	BaseMaterial3D::ShadingMode shadingMode,
	BaseMaterial3D::DiffuseMode diffuseMode,
	BaseMaterial3D::SpecularMode specularMode,
	ParticleNormalMode normalMode) {
	std::string renderMode = "cull_disabled";

	switch(blendMode) {
		case pixelpart::BlendMode::additive:
			renderMode += ",blend_add";
			break;
		case pixelpart::BlendMode::subtractive:
			renderMode += ",blend_sub";
			break;
		default:
			renderMode += ",blend_mix";
			break;
	}

	switch(diffuseMode) {
		case BaseMaterial3D::DIFFUSE_BURLEY:
			renderMode += ",diffuse_burley";
			break;
		case BaseMaterial3D::DIFFUSE_LAMBERT:
			renderMode += ",diffuse_lambert";
			break;
		case BaseMaterial3D::DIFFUSE_LAMBERT_WRAP:
			renderMode += ",diffuse_lambert_wrap";
			break;
		case BaseMaterial3D::DIFFUSE_TOON:
			renderMode += ",diffuse_toon";
			break;
		default:
			break;
	}

	switch(specularMode) {
		case BaseMaterial3D::SPECULAR_SCHLICK_GGX:
			renderMode += ",specular_schlick_ggx";
			break;
		case BaseMaterial3D::SPECULAR_TOON:
			renderMode += ",specular_toon";
			break;
		case BaseMaterial3D::SPECULAR_DISABLED:
			renderMode += ",specular_disabled";
			break;
		default:
			break;
	}

	switch(shadingMode) {
		case BaseMaterial3D::SHADING_MODE_UNSHADED:
			renderMode += ",unshaded";
			break;
		case BaseMaterial3D::SHADING_MODE_PER_VERTEX:
			renderMode += ",vertex_lighting";
			break;
		default:
			break;
	}

	std::string vertexShaderSource = R"!(VELOCITY = CUSTOM0.xyz;
		FORCE = CUSTOM1.xyz;
		LIFE = UV2.x;
		OBJECT_ID = UV2.y;)!";

	switch(normalMode) {
		case ParticleNormalMode::PARTICLE_NORMAL_MODE_STATIC:
			vertexShaderSource += "\nNORMAL = normalize(STATIC_NORMAL);";
			break;
		default:
			vertexShaderSource += "\nNORMAL = normalize(NORMAL);";
			break;
	}

	std::string additionalFragmentShaderSource = R"!(if(!OUTPUT_IS_SRGB) {
			__SHADER_COLOR__.rgb = mix(pow((__SHADER_COLOR__.rgb + vec3(0.055)) * (1.0 / (1.0 + 0.055)), vec3(2.4)), __SHADER_COLOR__.rgb * (1.0 / 12.92), lessThan(__SHADER_COLOR__.rgb, vec3(0.04045)));
		}
		ALBEDO = __SHADER_COLOR__.rgb;
		ALPHA = __SHADER_COLOR__.a;)!";

	return get_shader(shaderSource, "spatial", renderMode, vertexShaderSource, additionalFragmentShaderSource);
}

Ref<Shader> PixelpartShaders::get_shader(const std::string& shaderSource,
	const std::string& shaderType,
	const std::string& renderMode,
	const std::string& vertexShaderSource,
	const std::string& additionalFragmentShaderSource) {
	std::string finalShaderSource =
		"shader_type " + shaderType +
		";\nrender_mode " + renderMode +
		";\n" + shaderSource;

	finalShaderSource = replace(finalShaderSource, "{SHADER_MAIN_START}", "");
	finalShaderSource = replace(finalShaderSource, "{SHADER_MAIN_END}", additionalFragmentShaderSource);
	finalShaderSource = replace(finalShaderSource, "{SHADER_VERTEX_MAIN}", vertexShaderSource);

	if(shaders.count(finalShaderSource) != 0) {
		return shaders.at(finalShaderSource);
	}

	Ref<Shader> shader = new Shader();
	shader->set_code(String(finalShaderSource.c_str()));

	shaders[finalShaderSource] = shader;

	return shader;
}

std::string PixelpartShaders::replace(std::string str, const std::string& from, const std::string& to) {
	if(from.empty()) {
		return str;
	}

	std::size_t pos = 0;
	while((pos = str.find(from, pos)) != std::string::npos) {
		str.replace(pos, from.length(), to);
		pos += to.length();
	}

	return str;
}
}