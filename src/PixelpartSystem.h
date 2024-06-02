#ifndef PIXELPART_SYSTEM_H
#define PIXELPART_SYSTEM_H

#include "rendering/PixelpartShaderGenerator.h"
#include <common/ThreadPool.h>

namespace godot {
class PixelpartSystem {
public:
	static PixelpartSystem* get_instance();

	PixelpartSystem();
	~PixelpartSystem();

#ifndef __EMSCRIPTEN__
	std::shared_ptr<pixelpart::ThreadPool> get_thread_pool();
#endif

	PixelpartShaderGenerator& get_shader_generator();

private:
	static PixelpartSystem* instance;

#ifndef __EMSCRIPTEN__
	std::shared_ptr<pixelpart::ThreadPool> threadPool;
#endif

	PixelpartShaderGenerator shaderGenerator;
};
}

#endif