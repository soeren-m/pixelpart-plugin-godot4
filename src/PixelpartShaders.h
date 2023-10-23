#ifndef PIXELPART_SHADERS_H
#define PIXELPART_SHADERS_H

#include "BlendMode.h"
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/base_material3d.hpp>
#include <godot_cpp/classes/canvas_item_material.hpp>
#include <unordered_map>
#include <string>

namespace godot {
class PixelpartShaders {
public:
	static PixelpartShaders* get_instance();

	PixelpartShaders();
	~PixelpartShaders();

	Ref<Shader> get_canvas_shader(const std::string& shaderSource,
		pixelpart::BlendMode blendMode,
		CanvasItemMaterial::LightMode lightMode);
	Ref<Shader> get_spatial_shader(const std::string& shaderSource,
		pixelpart::BlendMode blendMode,
		BaseMaterial3D::ShadingMode shadingMode,
		BaseMaterial3D::DiffuseMode diffuseMode,
		BaseMaterial3D::SpecularMode specularMode);

	Ref<Shader> get_shader(const std::string& shaderSource,
		const std::string& shaderType,
		const std::string& renderMode,
		const std::string& vertexShaderSource,
		const std::string& additionalFragmentShaderSource);

private:
	std::string replace(std::string str, const std::string& from, const std::string& to);

	static PixelpartShaders* instance;

	std::unordered_map<std::string, Ref<Shader>> shaders;
};
}

#endif