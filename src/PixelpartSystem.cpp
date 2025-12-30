#include "PixelpartSystem.h"
#include "shaders/PixelpartShaderLanguage.h"
#include <pixelpart-runtime/effect/ComputeGraph.h>
#include <pixelpart-runtime/effect/ShaderGraph.h>
#include <pixelpart-runtime/json/json.hpp>

#ifdef PIXELPART_RUNTIME_MULTITHREADING
#include <pixelpart-runtime/common/StdThreadPool.h>
#endif

namespace godot {
PixelpartSystem* PixelpartSystem::instance = nullptr;
PixelpartSystem* PixelpartSystem::get_instance() {
	return instance;
}

PixelpartSystem::PixelpartSystem() {
	instance = this;

	pixelpart::ComputeGraph::nodeFactory.registerBuiltInNodes();

	nlohmann::ordered_json modelJson = nlohmann::json::parse(
		PixelpartShaderLanguage_json,
		PixelpartShaderLanguage_json + PixelpartShaderLanguage_json_size);
	pixelpart::ShaderGraph::graphLanguage = modelJson;

#ifdef PIXELPART_RUNTIME_MULTITHREADING
	threadPool = std::make_shared<pixelpart::StdThreadPool>(std::thread::hardware_concurrency());
#endif
}
PixelpartSystem::~PixelpartSystem() {
	instance = nullptr;
}

std::shared_ptr<pixelpart::ThreadPool> PixelpartSystem::get_thread_pool() {
	return threadPool;
}
PixelpartShaderProvider& PixelpartSystem::get_shader_provider() {
	return shaderProvider;
}
}
