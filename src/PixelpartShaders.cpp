#include "PixelpartShaders.h"
#include "PixelpartUtil.h"
#include "ShaderGraph.h"
#include "PixelpartShaderGraph.h"
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

Ref<Shader> PixelpartShaders::get_shader(const std::string& shaderSource, const std::string& shaderType, const std::string& renderMode) {
	std::string finalShaderSource =
		"shader_type " + shaderType +
		";\nrender_mode " + renderMode +
		";\n" + shaderSource;

	if(shaderType == "canvas_item") {
		finalShaderSource = replace(finalShaderSource, "{SHADER_MAIN_START}", "");
		finalShaderSource = replace(finalShaderSource, "{SHADER_MAIN_END}",
			R"!(COLOR = __SHADER_COLOR__;)!");
		finalShaderSource = replace(finalShaderSource, "{SHADER_VERTEX_MAIN}",
			R"!(const float __PACK_FACTOR__ = 10000.0;
			VELOCITY = vec3((floor(COLOR.rg) - vec2(__PACK_FACTOR__)) / vec2(__PACK_FACTOR__), 0.0);
			FORCE = vec3((floor(COLOR.ba) - vec2(__PACK_FACTOR__)) / vec2(__PACK_FACTOR__), 0.0);
			LIFE = floor(UV.x) / __PACK_FACTOR__;
			OBJECT_ID = floor(UV.y) - 1.0;
			UV = fract(UV) * vec2(10.0);
			COLOR = fract(COLOR) * vec4(10.0);)!");
	}
	else if(shaderType == "spatial") {
		finalShaderSource = replace(finalShaderSource, "{SHADER_MAIN_START}", "");
		finalShaderSource = replace(finalShaderSource, "{SHADER_MAIN_END}",
			R"!(if(!OUTPUT_IS_SRGB) {
				__SHADER_COLOR__.rgb = mix(pow((__SHADER_COLOR__.rgb + vec3(0.055)) * (1.0 / (1.0 + 0.055)), vec3(2.4)), __SHADER_COLOR__.rgb * (1.0 / 12.92), lessThan(__SHADER_COLOR__.rgb, vec3(0.04045)));
			}
			ALBEDO = __SHADER_COLOR__.rgb;
			ALPHA = __SHADER_COLOR__.a;)!");
		finalShaderSource = replace(finalShaderSource, "{SHADER_VERTEX_MAIN}",
			R"!(VELOCITY = CUSTOM0.xyz;
			FORCE = CUSTOM1.xyz;
			LIFE = UV2.x;
			OBJECT_ID = UV2.y;)!");
	}
	else {
		return Ref<Shader>();
	}

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