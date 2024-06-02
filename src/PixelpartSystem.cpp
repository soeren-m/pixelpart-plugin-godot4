#include "PixelpartSystem.h"
#include "shaders/PixelpartShaderLanguage.h"
#include <computegraph/ComputeGraph.h>

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

#ifndef __EMSCRIPTEN__
	threadPool = std::shared_ptr<pixelpart::ThreadPool>(new pixelpart::ThreadPool(std::thread::hardware_concurrency()));
#endif
}
PixelpartSystem::~PixelpartSystem() {
	instance = nullptr;
}

#ifndef __EMSCRIPTEN__
std::shared_ptr<pixelpart::ThreadPool> PixelpartSystem::get_thread_pool() {
	return threadPool;
}
#endif

PixelpartShaderGenerator& PixelpartSystem::get_shader_generator() {
	return shaderGenerator;
}
}