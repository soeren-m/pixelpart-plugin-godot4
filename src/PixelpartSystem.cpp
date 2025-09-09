#include "PixelpartSystem.h"
#include "shaders/PixelpartShaderLanguage.h"
#include <pixelpart-runtime/computegraph/ComputeGraph.h>
#include <pixelpart-runtime/shadergraph/ShaderGraph.h>
#include <pixelpart-runtime/json/json.hpp>

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
	threadPool = std::shared_ptr<pixelpart::ThreadPool>(new pixelpart::ThreadPool(std::thread::hardware_concurrency()));
#endif
}
PixelpartSystem::~PixelpartSystem() {
	instance = nullptr;
}

#ifdef PIXELPART_RUNTIME_MULTITHREADING
std::shared_ptr<pixelpart::ThreadPool> PixelpartSystem::get_thread_pool() {
	return threadPool;
}
#endif

PixelpartShaderProvider& PixelpartSystem::get_shader_provider() {
	return shaderProvider;
}
}
