#ifndef PIXELPART_SYSTEM_H
#define PIXELPART_SYSTEM_H

#include "PixelpartShaderGenerator.h"
#include <common/ThreadPool.h>

namespace godot {
class PixelpartSystem {
public:
	static PixelpartSystem* get_instance();

	PixelpartSystem();
	~PixelpartSystem();

	std::shared_ptr<pixelpart::ThreadPool> get_thread_pool();
	PixelpartShaderGenerator& get_shader_generator();

private:
	static PixelpartSystem* instance;

	std::shared_ptr<pixelpart::ThreadPool> threadPool;
	PixelpartShaderGenerator shaderGenerator;
};
}

#endif