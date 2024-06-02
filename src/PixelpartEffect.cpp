#include "PixelpartEffect.h"
#include "PixelpartSystem.h"
#include "util/PixelpartUtil.h"
#include <engine/CPUParticleEngine.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {
void PixelpartEffect::_bind_methods() {
	ClassDB::bind_method(D_METHOD("draw"), &PixelpartEffect::draw);
	ClassDB::bind_method(D_METHOD("play", "p"), &PixelpartEffect::play);
	ClassDB::bind_method(D_METHOD("pause"), &PixelpartEffect::pause);
	ClassDB::bind_method(D_METHOD("restart"), &PixelpartEffect::restart);
	ClassDB::bind_method(D_METHOD("reset"), &PixelpartEffect::reset);
	ClassDB::bind_method(D_METHOD("is_playing"), &PixelpartEffect::is_playing);
	ClassDB::bind_method(D_METHOD("get_time"), &PixelpartEffect::get_time);
	ClassDB::bind_method(D_METHOD("set_loop", "l"), &PixelpartEffect::set_loop);
	ClassDB::bind_method(D_METHOD("get_loop"), &PixelpartEffect::get_loop);
	ClassDB::bind_method(D_METHOD("set_loop_time", "l"), &PixelpartEffect::set_loop_time);
	ClassDB::bind_method(D_METHOD("get_loop_time"), &PixelpartEffect::get_loop_time);
	ClassDB::bind_method(D_METHOD("set_speed", "s"), &PixelpartEffect::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &PixelpartEffect::get_speed);
	ClassDB::bind_method(D_METHOD("set_frame_rate", "r"), &PixelpartEffect::set_frame_rate);
	ClassDB::bind_method(D_METHOD("get_frame_rate"), &PixelpartEffect::get_frame_rate);
	ClassDB::bind_method(D_METHOD("set_input_bool", "name", "value"), &PixelpartEffect::set_input_bool);
	ClassDB::bind_method(D_METHOD("set_input_int", "name", "value"), &PixelpartEffect::set_input_int);
	ClassDB::bind_method(D_METHOD("set_input_float", "name", "value"), &PixelpartEffect::set_input_float);
	ClassDB::bind_method(D_METHOD("set_input_float2", "name", "value"), &PixelpartEffect::set_input_float2);
	ClassDB::bind_method(D_METHOD("set_input_float3", "name", "value"), &PixelpartEffect::set_input_float3);
	ClassDB::bind_method(D_METHOD("set_input_float4", "name", "value"), &PixelpartEffect::set_input_float4);
	ClassDB::bind_method(D_METHOD("get_input_bool", "name"), &PixelpartEffect::get_input_bool);
	ClassDB::bind_method(D_METHOD("get_input_int", "name"), &PixelpartEffect::get_input_int);
	ClassDB::bind_method(D_METHOD("get_input_float", "name"), &PixelpartEffect::get_input_float);
	ClassDB::bind_method(D_METHOD("get_input_float2", "name"), &PixelpartEffect::get_input_float2);
	ClassDB::bind_method(D_METHOD("get_input_float3", "name"), &PixelpartEffect::get_input_float3);
	ClassDB::bind_method(D_METHOD("get_input_float4", "name"), &PixelpartEffect::get_input_float4);
	ClassDB::bind_method(D_METHOD("spawn_particles", "particleTypeId", "count"), &PixelpartEffect::spawn_particles);
	ClassDB::bind_method(D_METHOD("get_import_scale"), &PixelpartEffect::get_import_scale);
	ClassDB::bind_method(D_METHOD("set_effect", "effect_res"), &PixelpartEffect::set_effect);
	ClassDB::bind_method(D_METHOD("get_effect"), &PixelpartEffect::get_effect);
	ClassDB::bind_method(D_METHOD("find_particle_emitter"), &PixelpartEffect::find_particle_emitter);
	ClassDB::bind_method(D_METHOD("find_particle_type"), &PixelpartEffect::find_particle_type);
	ClassDB::bind_method(D_METHOD("find_force_field"), &PixelpartEffect::find_force_field);
	ClassDB::bind_method(D_METHOD("find_collider"), &PixelpartEffect::find_collider);
	ClassDB::bind_method(D_METHOD("get_particle_emitter"), &PixelpartEffect::get_particle_emitter);
	ClassDB::bind_method(D_METHOD("get_particle_type"), &PixelpartEffect::get_particle_type);
	ClassDB::bind_method(D_METHOD("get_force_field"), &PixelpartEffect::get_force_field);
	ClassDB::bind_method(D_METHOD("get_collider"), &PixelpartEffect::get_collider);
	ClassDB::bind_method(D_METHOD("get_particle_emitter_at_index"), &PixelpartEffect::get_particle_emitter_at_index);
	ClassDB::bind_method(D_METHOD("get_particle_type_at_index"), &PixelpartEffect::get_particle_type_at_index);
	ClassDB::bind_method(D_METHOD("get_force_field_at_index"), &PixelpartEffect::get_force_field_at_index);
	ClassDB::bind_method(D_METHOD("get_collider_at_index"), &PixelpartEffect::get_collider_at_index);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "effect", PROPERTY_HINT_RESOURCE_TYPE, "PixelpartEffectResource"),
		"set_effect", "get_effect");

	ADD_GROUP("Playback", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "play", "is_playing");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "loop"), "set_loop", "get_loop");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "loop_time", PROPERTY_HINT_RANGE, "0.0,1000.0,0.01"), "set_loop_time", "get_loop_time");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0.0,100.0,0.01"), "set_speed", "get_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "frame_rate", PROPERTY_HINT_RANGE, "1.0,100.0,1.0"), "set_frame_rate", "get_frame_rate");
}

PixelpartEffect::PixelpartEffect() {
	ProjectSettings* settings = ProjectSettings::get_singleton();

	if(settings->has_setting("pixelpart/particle_capacity")) {
		particleCapacity = static_cast<uint32_t>(std::max((int)settings->get_setting("pixelpart/particle_capacity"), 1));
	}
}
PixelpartEffect::~PixelpartEffect() {

}

void PixelpartEffect::_enter_tree() {
	if(!Engine::get_singleton()->is_editor_hint()) {
		RenderingServer::get_singleton()->connect("frame_pre_draw", Callable(this, "draw"));
	}
}
void PixelpartEffect::_exit_tree() {
	if(!Engine::get_singleton()->is_editor_hint()) {
		RenderingServer::get_singleton()->disconnect("frame_pre_draw", Callable(this, "draw"));
	}
}

void PixelpartEffect::_process(double dt) {
	if(Engine::get_singleton()->is_editor_hint() || !particleEngine) {
		return;
	}

	if(playing) {
		simulationTime += glm::clamp(static_cast<float>(dt), 0.0f, 1.0f) * speed;

		while(simulationTime > timeStep) {
			simulationTime -= timeStep;
			particleEngine->step(timeStep);

			if(loop) {
				if(particleEngine->getTime() > loopTime) {
					particleEngine->restart(false);
				}
			}
		}
	}
}
void PixelpartEffect::draw() {
	if(!particleEngine) {
		return;
	}

	Viewport* viewport = get_viewport();
	if(!viewport) {
		UtilityFunctions::printerr("Viewport is null");
		return;
	}

	const pixelpart::Effect& effect = particleEngine->getEffect();

	std::vector<uint32_t> particleTypeIndicesSortedByLayer = effect.particleTypes.getSortedIndices(
		[](const pixelpart::ParticleType& t1, const pixelpart::ParticleType& t2) {
			return t1.layer < t2.layer;
		});

	for(uint32_t particleTypeIndex : particleTypeIndicesSortedByLayer) {
		if(particleTypeIndex >= particleMeshes.size() || !particleMeshes[particleTypeIndex]) {
			return;
		}

		particleMeshes[particleTypeIndex]->draw(this,
			effect,
			effect.particleTypes.getByIndex(particleTypeIndex),
			particleEngine->getParticles(particleTypeIndex),
			particleEngine->getNumParticles(particleTypeIndex),
			static_cast<pixelpart::float_t>(get_import_scale()),
			particleEngine->getTime());
	}
}

void PixelpartEffect::play(bool p) {
	playing = p;
}
void PixelpartEffect::pause() {
	playing = false;
}
void PixelpartEffect::restart() {
	if(particleEngine) {
		particleEngine->restart(true);
	}
}
void PixelpartEffect::reset() {
	if(particleEngine) {
		particleEngine->restart(false);
	}
}
bool PixelpartEffect::is_playing() const {
	return playing;
}
float PixelpartEffect::get_time() const {
	return static_cast<float>(particleEngine->getTime());
}

void PixelpartEffect::set_loop(bool l) {
	loop = l;
}
bool PixelpartEffect::get_loop() const {
	return loop;
}

void PixelpartEffect::set_loop_time(float l) {
	loopTime = std::max(l, 0.0f);
}
float PixelpartEffect::get_loop_time() const {
	return loopTime;
}

void PixelpartEffect::set_speed(float s) {
	speed = std::max(s, 0.0f);
}
float PixelpartEffect::get_speed() const {
	return speed;
}

void PixelpartEffect::set_frame_rate(float r) {
	timeStep = 1.0f / std::min(std::max(r, 1.0f), 100.0f);
}
float PixelpartEffect::get_frame_rate() const {
	return 1.0f / timeStep;
}

void PixelpartEffect::set_input_bool(String name, bool value) {
	pixelpart::EffectInputCollection::iterator inputIt = findInput(name);
	if(inputIt == effect.inputs.end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return;
	}

	inputIt->second.value = pixelpart::VariantValue::Bool(value);

	pixelpart::refreshEffectProperties(effect);
}
void PixelpartEffect::set_input_int(String name, int value) {
	pixelpart::EffectInputCollection::iterator inputIt = findInput(name);
	if(inputIt == effect.inputs.end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return;
	}

	inputIt->second.value = pixelpart::VariantValue::Int(fromGd(value));

	pixelpart::refreshEffectProperties(effect);
}
void PixelpartEffect::set_input_float(String name, float value) {
	pixelpart::EffectInputCollection::iterator inputIt = findInput(name);
	if(inputIt == effect.inputs.end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return;
	}

	inputIt->second.value = pixelpart::VariantValue::Float(fromGd(value));

	pixelpart::refreshEffectProperties(effect);
}
void PixelpartEffect::set_input_float2(String name, Vector2 value) {
	pixelpart::EffectInputCollection::iterator inputIt = findInput(name);
	if(inputIt == effect.inputs.end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return;
	}

	inputIt->second.value = pixelpart::VariantValue::Float2(fromGd(value));

	pixelpart::refreshEffectProperties(effect);
}
void PixelpartEffect::set_input_float3(String name, Vector3 value) {
	pixelpart::EffectInputCollection::iterator inputIt = findInput(name);
	if(inputIt == effect.inputs.end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return;
	}

	inputIt->second.value = pixelpart::VariantValue::Float3(fromGd(value));

	pixelpart::refreshEffectProperties(effect);
}
void PixelpartEffect::set_input_float4(String name, Vector4 value) {
	pixelpart::EffectInputCollection::iterator inputIt = findInput(name);
	if(inputIt == effect.inputs.end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return;
	}

	inputIt->second.value = pixelpart::VariantValue::Float4(fromGd(value));

	pixelpart::refreshEffectProperties(effect);
}
bool PixelpartEffect::get_input_bool(String name) const {
	pixelpart::EffectInputCollection::const_iterator inputIt = findInput(name);
	if(inputIt == effect.inputs.end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return false;
	}

	return inputIt->second.value.toBool();
}
int PixelpartEffect::get_input_int(String name) const {
	pixelpart::EffectInputCollection::const_iterator inputIt = findInput(name);
	if(inputIt == effect.inputs.end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return 0;
	}

	return toGd(inputIt->second.value.toInt());
}
float PixelpartEffect::get_input_float(String name) const {
	pixelpart::EffectInputCollection::const_iterator inputIt = findInput(name);
	if(inputIt == effect.inputs.end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return 0.0f;
	}

	return toGd(inputIt->second.value.toFloat());
}
Vector2 PixelpartEffect::get_input_float2(String name) const {
	pixelpart::EffectInputCollection::const_iterator inputIt = findInput(name);
	if(inputIt == effect.inputs.end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return Vector2(0.0f, 0.0f);
	}

	return toGd(inputIt->second.value.toFloat2());
}
Vector3 PixelpartEffect::get_input_float3(String name) const {
	pixelpart::EffectInputCollection::const_iterator inputIt = findInput(name);
	if(inputIt == effect.inputs.end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	return toGd(inputIt->second.value.toFloat3());
}
Vector4 PixelpartEffect::get_input_float4(String name) const {
	pixelpart::EffectInputCollection::const_iterator inputIt = findInput(name);
	if(inputIt == effect.inputs.end()) {
		UtilityFunctions::push_warning("Unknown effect input \"", name, "\"");
		return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return toGd(inputIt->second.value.toFloat4());
}

void PixelpartEffect::spawn_particles(int particleTypeId, int count) {
	if(particleEngine && count > 0) {
		particleEngine->spawnParticles(static_cast<pixelpart::id_t>(particleTypeId), static_cast<uint32_t>(count));
	}
}


float PixelpartEffect::get_import_scale() const {
	if(!effectResource.is_valid()) {
		return 1.0f;
	}

	return effectResource->get_scale();
}

void PixelpartEffect::set_effect(Ref<PixelpartEffectResource> effectRes) {
	RenderingServer* rs = RenderingServer::get_singleton();

	particleEmitters.clear();
	particleTypes.clear();
	forceFields.clear();
	colliders.clear();

	particleMeshes.clear();
	graphicsResources.textures.clear();
	graphicsResources.meshes.clear();

	effectResource = effectRes;
	if(effectResource.is_null()) {
		particleEngine = nullptr;
		notify_property_list_changed();

		return;
	}

	effectResource->load();

	effect = effectResource->get_project().effect;
	pixelpart::refreshEffectProperties(effect);

#ifndef __EMSCRIPTEN__
	particleEngine = std::unique_ptr<pixelpart::CPUParticleEngine>(
		new pixelpart::CPUParticleEngine(effect, particleCapacity, PixelpartSystem::get_instance()->get_thread_pool()));
#else
	particleEngine = std::unique_ptr<pixelpart::CPUParticleEngine>(
		new pixelpart::CPUParticleEngine(effect, particleCapacity));
#endif

	try {
		for(const auto& resourceEntry : effect.resources.images) {
			const pixelpart::ImageResource& imageResource = resourceEntry.second;

			PackedByteArray imageData;
			imageData.resize(static_cast<int64_t>(imageResource.data.size()));

			std::memcpy(imageData.ptrw(), imageResource.data.data(), imageResource.data.size());

			Ref<Image> image = Image::create_from_data(
				static_cast<int32_t>(imageResource.width),
				static_cast<int32_t>(imageResource.height),
				false, Image::FORMAT_RGBA8, imageData);

			graphicsResources.textures[resourceEntry.first] = ImageTexture::create_from_image(image);
		}

		for(const auto& resourceEntry : effect.resources.meshes) {
			const pixelpart::MeshResource& meshResource = resourceEntry.second;

			PackedInt32Array indexArray;
			PackedVector3Array vertexArray;
			PackedVector3Array normalArray;
			PackedVector2Array uvArray;
			indexArray.resize(meshResource.faces.size());
			vertexArray.resize(meshResource.positions.size());
			normalArray.resize(meshResource.normals.size());
			uvArray.resize(meshResource.textureCoords.size());

			int32_t* indices = indexArray.ptrw();
			for(std::size_t i = 0u; i < meshResource.faces.size(); i++) {
				indices[i] = static_cast<int32_t>(meshResource.faces[i]);
			}

			std::memcpy(vertexArray.ptrw(), &meshResource.positions[0], meshResource.positions.size() * sizeof(glm::vec3));
			std::memcpy(normalArray.ptrw(), &meshResource.normals[0], meshResource.normals.size() * sizeof(glm::vec3));
			std::memcpy(uvArray.ptrw(), &meshResource.textureCoords[0], meshResource.textureCoords.size() * sizeof(glm::vec2));

			Array meshArray;
			meshArray.resize(Mesh::ARRAY_MAX);
			meshArray[Mesh::ARRAY_VERTEX] = vertexArray;
			meshArray[Mesh::ARRAY_NORMAL] = normalArray;
			meshArray[Mesh::ARRAY_TEX_UV] = uvArray;
			meshArray[Mesh::ARRAY_INDEX] = indexArray;

			Ref<ArrayMesh> mesh;
			mesh.instantiate();
			mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, meshArray, Array(), Dictionary(),
				Mesh::ARRAY_FORMAT_VERTEX | Mesh::ARRAY_FORMAT_NORMAL | Mesh::ARRAY_FORMAT_TEX_UV | Mesh::ARRAY_FORMAT_INDEX);

			graphicsResources.meshes[resourceEntry.first] = mesh;
		}

		for(pixelpart::ParticleEmitter& particleEmitter : effect.particleEmitters) {
			Ref<PixelpartParticleEmitter> emitterRef;
			emitterRef.instantiate();
			emitterRef->init(effectResource, &particleEmitter, particleEngine.get());

			particleEmitters[particleEmitter.name] = emitterRef;
		}

		for(pixelpart::ParticleType& particleType : effect.particleTypes) {
			Ref<PixelpartParticleType> particleTypeRef;
			particleTypeRef.instantiate();
			particleTypeRef->init(effectResource, &particleType, particleEngine.get());

			particleTypes[particleType.name] = particleTypeRef;
		}

		for(pixelpart::ForceField& forceField : effect.forceFields) {
			Ref<PixelpartForceField> forceFieldRef;
			forceFieldRef.instantiate();
			forceFieldRef->init(effectResource, &forceField, particleEngine.get());

			forceFields[forceField.name] = forceFieldRef;
		}

		for(pixelpart::Collider& collider : effect.colliders) {
			Ref<PixelpartCollider> colliderRef;
			colliderRef.instantiate();
			colliderRef->init(effectResource, &collider, particleEngine.get());

			colliders[collider.name] = colliderRef;
		}

		for(uint32_t particleTypeIndex = 0u; particleTypeIndex < effect.particleTypes.getCount(); particleTypeIndex++) {
			try {
				particleMeshes.emplace_back(std::make_unique<PixelpartParticleMesh3D>(
					graphicsResources, PixelpartSystem::get_instance()->get_shader_generator(),
					effect, effect.particleTypes.getByIndex(particleTypeIndex)));
			}
			catch(const std::exception& e) {
				particleMeshes.emplace_back(nullptr);

				UtilityFunctions::push_warning("Could not create particle mesh");
			}
		}
	}
	catch(const std::exception& e) {
		particleEngine = nullptr;

		UtilityFunctions::printerr(String(e.what()));
	}

	notify_property_list_changed();
}
Ref<PixelpartEffectResource> PixelpartEffect::get_effect() const {
	return effectResource;
}

Ref<PixelpartParticleEmitter> PixelpartEffect::find_particle_emitter(String name) const {
	CharString nameCharString = name.utf8();
	std::string nameStdString = std::string(nameCharString.get_data(), nameCharString.length());

	if(particleEmitters.count(nameStdString)) {
		return particleEmitters.at(nameStdString);
	}

	return Ref<PixelpartParticleEmitter>();
}
Ref<PixelpartParticleType> PixelpartEffect::find_particle_type(String name) const {
	CharString nameCharString = name.utf8();
	std::string nameStdString = std::string(nameCharString.get_data(), nameCharString.length());

	if(particleTypes.count(nameStdString)) {
		return particleTypes.at(nameStdString);
	}

	return Ref<PixelpartParticleType>();
}
Ref<PixelpartForceField> PixelpartEffect::find_force_field(String name) const {
	CharString nameCharString = name.utf8();
	std::string nameStdString = std::string(nameCharString.get_data(), nameCharString.length());

	if(forceFields.count(nameStdString)) {
		return forceFields.at(nameStdString);
	}

	return Ref<PixelpartForceField>();
}
Ref<PixelpartCollider> PixelpartEffect::find_collider(String name) const {
	CharString nameCharString = name.utf8();
	std::string nameStdString = std::string(nameCharString.get_data(), nameCharString.length());

	if(colliders.count(nameStdString)) {
		return colliders.at(nameStdString);
	}

	return Ref<PixelpartCollider>();
}
Ref<PixelpartParticleEmitter> PixelpartEffect::get_particle_emitter(int id) const {
	if(id >= 0 && effect.particleEmitters.contains(static_cast<uint32_t>(id))) {
		std::string name = effect.particleEmitters.get(static_cast<uint32_t>(id)).name;

		if(particleEmitters.count(name)) {
			return particleEmitters.at(name);
		}
	}

	return Ref<PixelpartParticleEmitter>();
}
Ref<PixelpartParticleType> PixelpartEffect::get_particle_type(int id) const {
	if(id >= 0 && effect.particleTypes.contains(static_cast<uint32_t>(id))) {
		std::string name = effect.particleTypes.get(static_cast<uint32_t>(id)).name;

		if(particleTypes.count(name)) {
			return particleTypes.at(name);
		}
	}

	return Ref<PixelpartParticleType>();
}
Ref<PixelpartForceField> PixelpartEffect::get_force_field(int id) const {
	if(id >= 0 && effect.forceFields.contains(static_cast<uint32_t>(id))) {
		std::string name = effect.forceFields.get(static_cast<uint32_t>(id)).name;

		if(forceFields.count(name)) {
			return forceFields.at(name);
		}
	}

	return Ref<PixelpartForceField>();
}
Ref<PixelpartCollider> PixelpartEffect::get_collider(int id) const {
	if(id >= 0 && effect.colliders.contains(static_cast<uint32_t>(id))) {
		std::string name = effect.colliders.get(static_cast<uint32_t>(id)).name;

		if(colliders.count(name)) {
			return colliders.at(name);
		}
	}

	return Ref<PixelpartCollider>();
}
Ref<PixelpartParticleEmitter> PixelpartEffect::get_particle_emitter_at_index(int index) const {
	if(index >= 0 && effect.particleEmitters.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = effect.particleEmitters.getByIndex(static_cast<uint32_t>(index)).name;

		if(particleEmitters.count(name)) {
			return particleEmitters.at(name);
		}
	}

	return Ref<PixelpartParticleEmitter>();
}
Ref<PixelpartParticleType> PixelpartEffect::get_particle_type_at_index(int index) const {
	if(index >= 0 && effect.particleTypes.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = effect.particleTypes.getByIndex(static_cast<uint32_t>(index)).name;

		if(particleTypes.count(name)) {
			return particleTypes.at(name);
		}
	}

	return Ref<PixelpartParticleType>();
}
Ref<PixelpartForceField> PixelpartEffect::get_force_field_at_index(int index) const {
	if(index >= 0 && effect.forceFields.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = effect.forceFields.getByIndex(static_cast<uint32_t>(index)).name;

		if(forceFields.count(name)) {
			return forceFields.at(name);
		}
	}

	return Ref<PixelpartForceField>();
}
Ref<PixelpartCollider> PixelpartEffect::get_collider_at_index(int index) const {
	if(index >= 0 && effect.colliders.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = effect.colliders.getByIndex(static_cast<uint32_t>(index)).name;

		if(colliders.count(name)) {
			return colliders.at(name);
		}
	}

	return Ref<PixelpartCollider>();
}

pixelpart::EffectInputCollection::iterator PixelpartEffect::findInput(String name) {
	std::string inputName = std::string(name.utf8().get_data());

	return std::find_if(effect.inputs.begin(), effect.inputs.end(), [&inputName](const std::pair<pixelpart::id_t, pixelpart::EffectInput>& entry) {
		return entry.second.name == inputName;
	});
}
pixelpart::EffectInputCollection::const_iterator PixelpartEffect::findInput(String name) const {
	std::string inputName = std::string(name.utf8().get_data());

	return std::find_if(effect.inputs.begin(), effect.inputs.end(), [&inputName](const std::pair<pixelpart::id_t, pixelpart::EffectInput>& entry) {
		return entry.second.name == inputName;
	});
}
}