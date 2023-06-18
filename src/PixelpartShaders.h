#ifndef PIXELPART_SHADERS_H
#define PIXELPART_SHADERS_H

#include <godot_cpp/classes/shader.hpp>
#include <unordered_map>
#include <string>

namespace godot {
class PixelpartShaders {
public:
	static PixelpartShaders* get_instance();

	PixelpartShaders();
	~PixelpartShaders();

	Ref<Shader> get_shader(const std::string& shaderSource, const std::string& shaderType, const std::string& renderMode);

private:
	std::string replace(std::string str, const std::string& from, const std::string& to);

	static PixelpartShaders* instance;

	std::unordered_map<std::string, Ref<Shader>> shaders;
};
}

#endif