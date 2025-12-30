#ifndef PIXELPART_SYSTEM_H
#define PIXELPART_SYSTEM_H

#include "rendering/PixelpartShaderProvider.h"
#include <pixelpart-runtime/common/ThreadPool.h>
#include <memory>

namespace godot {
class PixelpartSystem final {
public:
	static PixelpartSystem* get_instance();

	PixelpartSystem();
	~PixelpartSystem();

	std::shared_ptr<pixelpart::ThreadPool> get_thread_pool();
	PixelpartShaderProvider& get_shader_provider();

private:
	static PixelpartSystem* instance;

	std::shared_ptr<pixelpart::ThreadPool> threadPool;
	PixelpartShaderProvider shaderProvider;
};
}

#endif
