#ifndef PIXELPART_SYSTEM_H
#define PIXELPART_SYSTEM_H

#include "rendering/PixelpartShaderProvider.h"
#include <pixelpart-runtime/common/ThreadPool.h>

namespace godot {
class PixelpartSystem final {
public:
	static PixelpartSystem* get_instance();

	PixelpartSystem();
	~PixelpartSystem();

#ifdef PIXELPART_RUNTIME_MULTITHREADING
	std::shared_ptr<pixelpart::ThreadPool> get_thread_pool();
#endif

	PixelpartShaderProvider& get_shader_provider();

private:
	static PixelpartSystem* instance;

#ifdef PIXELPART_RUNTIME_MULTITHREADING
	std::shared_ptr<pixelpart::ThreadPool> threadPool;
#endif

	PixelpartShaderProvider shaderProvider;
};
}

#endif
