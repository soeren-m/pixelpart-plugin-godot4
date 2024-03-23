#include "PixelpartSystem.h"
#include "shaders/PixelpartShaderLanguage.h"
#include <property/ComputeGraph.h>

namespace godot {
PixelpartSystem* PixelpartSystem::instance = nullptr;
PixelpartSystem* PixelpartSystem::get_instance() {
	return instance;
}

PixelpartSystem::PixelpartSystem() {
	instance = this;

	pixelpart::ComputeGraph::nodeFactory.registerBuiltInNodes();

	nlohmann::ordered_json modelJson = nlohmann::json::parse(
		PixelpartShaderGraph_json,
		PixelpartShaderGraph_json + PixelpartShaderGraph_json_size);
	pixelpart::ShaderGraph::graphLanguage = modelJson;

	threadPool = std::shared_ptr<pixelpart::ThreadPool>(new pixelpart::ThreadPool(std::thread::hardware_concurrency()));
}
PixelpartSystem::~PixelpartSystem() {
	instance = nullptr;
}

std::shared_ptr<pixelpart::ThreadPool> PixelpartSystem::get_thread_pool() {
	return threadPool;
}
PixelpartShaderGenerator& PixelpartSystem::get_shader_generator() {
	return shaderGenerator;
}
}