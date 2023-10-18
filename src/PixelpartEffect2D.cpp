#include "PixelpartEffect2D.h"
#include "PixelpartUtil.h"
#include "ParticleSolverCPU.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {
void PixelpartEffect2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("play", "p"), &PixelpartEffect2D::play);
	ClassDB::bind_method(D_METHOD("pause"), &PixelpartEffect2D::pause);
	ClassDB::bind_method(D_METHOD("restart"), &PixelpartEffect2D::restart);
	ClassDB::bind_method(D_METHOD("reset"), &PixelpartEffect2D::reset);
	ClassDB::bind_method(D_METHOD("is_playing"), &PixelpartEffect2D::is_playing);
	ClassDB::bind_method(D_METHOD("get_time"), &PixelpartEffect2D::get_time);
	ClassDB::bind_method(D_METHOD("set_loop", "l"), &PixelpartEffect2D::set_loop);
	ClassDB::bind_method(D_METHOD("get_loop"), &PixelpartEffect2D::get_loop);
	ClassDB::bind_method(D_METHOD("set_loop_time", "l"), &PixelpartEffect2D::set_loop_time);
	ClassDB::bind_method(D_METHOD("get_loop_time"), &PixelpartEffect2D::get_loop_time);
	ClassDB::bind_method(D_METHOD("set_speed", "s"), &PixelpartEffect2D::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &PixelpartEffect2D::get_speed);
	ClassDB::bind_method(D_METHOD("set_frame_rate", "r"), &PixelpartEffect2D::set_frame_rate);
	ClassDB::bind_method(D_METHOD("get_frame_rate"), &PixelpartEffect2D::get_frame_rate);
	ClassDB::bind_method(D_METHOD("set_flip_h", "flip"), &PixelpartEffect2D::set_flip_h);
	ClassDB::bind_method(D_METHOD("set_flip_v", "flip"), &PixelpartEffect2D::set_flip_v);
	ClassDB::bind_method(D_METHOD("get_flip_h"), &PixelpartEffect2D::get_flip_h);
	ClassDB::bind_method(D_METHOD("get_flip_v"), &PixelpartEffect2D::get_flip_v);
	ClassDB::bind_method(D_METHOD("set_particle_materials", "materials"), &PixelpartEffect2D::set_particle_materials);
	ClassDB::bind_method(D_METHOD("get_particle_materials"), &PixelpartEffect2D::get_particle_materials);
	ClassDB::bind_method(D_METHOD("get_import_scale"), &PixelpartEffect2D::get_import_scale);
	ClassDB::bind_method(D_METHOD("set_effect", "effect_res"), &PixelpartEffect2D::set_effect);
	ClassDB::bind_method(D_METHOD("get_effect"), &PixelpartEffect2D::get_effect);
	ClassDB::bind_method(D_METHOD("find_particle_emitter"), &PixelpartEffect2D::find_particle_emitter);
	ClassDB::bind_method(D_METHOD("find_particle_type"), &PixelpartEffect2D::find_particle_type);
	ClassDB::bind_method(D_METHOD("find_force_field"), &PixelpartEffect2D::find_force_field);
	ClassDB::bind_method(D_METHOD("find_collider"), &PixelpartEffect2D::find_collider);
	ClassDB::bind_method(D_METHOD("get_particle_emitter"), &PixelpartEffect2D::get_particle_emitter);
	ClassDB::bind_method(D_METHOD("get_particle_type"), &PixelpartEffect2D::get_particle_type);
	ClassDB::bind_method(D_METHOD("get_force_field"), &PixelpartEffect2D::get_force_field);
	ClassDB::bind_method(D_METHOD("get_collider"), &PixelpartEffect2D::get_collider);
	ClassDB::bind_method(D_METHOD("get_particle_emitter_at_index"), &PixelpartEffect2D::get_particle_emitter_at_index);
	ClassDB::bind_method(D_METHOD("get_particle_type_at_index"), &PixelpartEffect2D::get_particle_type_at_index);
	ClassDB::bind_method(D_METHOD("get_force_field_at_index"), &PixelpartEffect2D::get_force_field_at_index);
	ClassDB::bind_method(D_METHOD("get_collider_at_index"), &PixelpartEffect2D::get_collider_at_index);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "effect", PROPERTY_HINT_RESOURCE_TYPE, "PixelpartEffectResource"), "set_effect", "get_effect");

	ADD_GROUP("Playback", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "play", "is_playing");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "loop"), "set_loop", "get_loop");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "loop_time", PROPERTY_HINT_RANGE, "0.0,1000.0,0.01"), "set_loop_time", "get_loop_time");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0.0,100.0,0.01"), "set_speed", "get_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "frame_rate", PROPERTY_HINT_RANGE, "1.0,100.0,1.0"), "set_frame_rate", "get_frame_rate");

	ADD_GROUP("Shading", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_h"), "set_flip_h", "get_flip_h");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_v"), "set_flip_v", "get_flip_v");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "particle_materials", PROPERTY_HINT_ARRAY_TYPE, vformat("%s/%s:%s", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE, "PixelpartParticleMaterial2D")),
		"set_particle_materials", "get_particle_materials");
}

PixelpartEffect2D::PixelpartEffect2D() {
	auto settings = ProjectSettings::get_singleton();

	uint32_t particleCapacity = 10000;
	if(settings->has_setting("pixelpart/particle_capacity")) {
		particleCapacity = static_cast<uint32_t>(std::max((int)settings->get_setting("pixelpart/particle_capacity"), 1));
	}

	particleEngine = pixelpart::ParticleEngine::createUnique<pixelpart::ParticleSolverCPU>(nullptr, particleCapacity);
}
PixelpartEffect2D::~PixelpartEffect2D() {

}

void PixelpartEffect2D::_process(double dt) {
	if(Engine::get_singleton()->is_editor_hint() || !particleEngine || !particleEngine->getEffect()) {
		return;
	}

	if(playing) {
		simulationTime += std::min(std::max(static_cast<float>(dt), 0.0f), 1.0f) * speed;

		while(simulationTime > timeStep) {
			simulationTime -= timeStep;
			particleEngine->step(timeStep);

			if(loop) {
				if(particleEngine->getTime() > loopTime) {
					particleEngine->reset();
				}
			}
		}
	}

	queue_redraw();
}

void PixelpartEffect2D::_draw() {
	if(Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	Viewport* viewport = get_viewport();
	if(!particleEngine || !viewport) {
		return;
	}

	const pixelpart::Effect* effect = particleEngine->getEffect();
	if(!effect) {
		return;
	}

	std::vector<uint32_t> particleTypeIndicesSortedByLayer = effect->particleTypes.getSortedIndices(
		[](const pixelpart::ParticleType& t1, const pixelpart::ParticleType& t2) {
			return t1.layer < t2.layer;
		});

	for(uint32_t particleTypeIndex : particleTypeIndicesSortedByLayer) {
		if(particleTypeIndex >= particleMeshInstances.size()) {
			continue;
		}

		if(static_cast<int64_t>(particleTypeIndex) < particleMaterials.size()) {
			Ref<PixelpartParticleMaterial2D> particleMaterial = (Ref<PixelpartParticleMaterial2D>)particleMaterials[particleTypeIndex];

			if(particleMaterial.is_valid() && particleMaterial->is_shader_dirty()) {
				particleMeshInstances[particleTypeIndex]->update_shader(
					nativeEffect.particleTypes.getByIndex(particleTypeIndex),
					particleMaterial);

				particleMaterial->set_shader_dirty(false);
			}
		}

		draw_particles(particleTypeIndex);
	}
}

void PixelpartEffect2D::play(bool p) {
	playing = p;
}
void PixelpartEffect2D::pause() {
	playing = false;
}
void PixelpartEffect2D::restart() {
	particleEngine->restart();
}
void PixelpartEffect2D::reset() {
	particleEngine->reset();
}
bool PixelpartEffect2D::is_playing() const {
	return playing;
}
float PixelpartEffect2D::get_time() const {
	return static_cast<float>(particleEngine->getTime());
}

void PixelpartEffect2D::set_loop(bool l) {
	loop = l;
}
bool PixelpartEffect2D::get_loop() const {
	return loop;
}

void PixelpartEffect2D::set_loop_time(float l) {
	loopTime = std::max(l, 0.0f);
}
float PixelpartEffect2D::get_loop_time() const {
	return loopTime;
}

void PixelpartEffect2D::set_speed(float s) {
	speed = std::max(s, 0.0f);
}
float PixelpartEffect2D::get_speed() const {
	return speed;
}

void PixelpartEffect2D::set_frame_rate(float r) {
	timeStep = 1.0f / std::min(std::max(r, 1.0f), 100.0f);
}
float PixelpartEffect2D::get_frame_rate() const {
	return 1.0f / timeStep;
}

void PixelpartEffect2D::set_flip_h(bool flip) {
	flipH = flip;
}
void PixelpartEffect2D::set_flip_v(bool flip) {
	flipV = flip;
}
bool PixelpartEffect2D::get_flip_h() const {
	return flipH;
}
bool PixelpartEffect2D::get_flip_v() const {
	return flipV;
}

void PixelpartEffect2D::set_particle_materials(Array materials) {
	particleMaterials = materials;
}
Array PixelpartEffect2D::get_particle_materials() const {
	return particleMaterials;
}

float PixelpartEffect2D::get_import_scale() const {
	if(!effectResource.is_valid()) {
		return 1.0f;
	}

	return effectResource->get_scale();
}

void PixelpartEffect2D::set_effect(Ref<PixelpartEffectResource> effectRes) {
	RenderingServer* rs = RenderingServer::get_singleton();
	PixelpartShaders* shaders = PixelpartShaders::get_instance();

	particleMeshInstances.clear();
	particleEmitters.clear();
	particleTypes.clear();
	forceFields.clear();
	colliders.clear();
	textures.clear();

	if(Engine::get_singleton()->is_editor_hint()) {
		particleMaterials.clear();
	}

	effectResource = effectRes;
	if(effectResource.is_null()) {
		particleEngine->setEffect(nullptr);
		notify_property_list_changed();

		return;
	}

	effectResource->load();

	nativeEffect = effectResource->get_project().effect;
	particleEngine->setEffect(&nativeEffect);

	try {
		for(pixelpart::ParticleEmitter& particleEmitter : nativeEffect.particleEmitters) {
			Ref<PixelpartParticleEmitter> emitterRef;
			emitterRef.instantiate();
			emitterRef->init(effectResource, &particleEmitter, particleEngine.get());

			particleEmitters[particleEmitter.name] = emitterRef;
		}

		for(pixelpart::ParticleType& particleType : nativeEffect.particleTypes) {
			Ref<PixelpartParticleType> particleTypeRef;
			particleTypeRef.instantiate();
			particleTypeRef->init(effectResource, &particleType, particleEngine.get());

			particleTypes[particleType.name] = particleTypeRef;
		}

		for(pixelpart::ForceField& forceField : nativeEffect.forceFields) {
			Ref<PixelpartForceField> forceFieldRef;
			forceFieldRef.instantiate();
			forceFieldRef->init(effectResource, &forceField, particleEngine.get());

			forceFields[forceField.name] = forceFieldRef;
		}

		for(pixelpart::Collider& collider : nativeEffect.colliders) {
			Ref<PixelpartCollider> colliderRef;
			colliderRef.instantiate();
			colliderRef->init(effectResource, &collider, particleEngine.get());

			colliders[collider.name] = colliderRef;
		}

		for(uint32_t particleTypeIndex = 0u; particleTypeIndex < nativeEffect.particleTypes.getCount(); particleTypeIndex++) {
			if(Engine::get_singleton()->is_editor_hint()) {
				Ref<PixelpartParticleMaterial2D> particleMaterial;
				particleMaterial.instantiate();
				particleMaterials.append(particleMaterial);
			}

			Ref<PixelpartParticleMaterial2D> particleMaterial;
			if(static_cast<int64_t>(particleTypeIndex) < particleMaterials.size()) {
				particleMaterial = (Ref<PixelpartParticleMaterial2D>)particleMaterials[particleTypeIndex];
			}

			particleMeshInstances.push_back(std::make_unique<ParticleMeshInstance>(
				nativeEffect.particleTypes.getByIndex(particleTypeIndex),
				particleMaterial));
		}

		for(const auto& resource : effectResource->get_project_resources().images) {
			PackedByteArray imageData;
			imageData.resize(static_cast<int64_t>(resource.second.data.size()));

			std::memcpy(imageData.ptrw(), resource.second.data.data(), resource.second.data.size());

			Ref<Image> image = Image::create_from_data(
				static_cast<int32_t>(resource.second.width),
				static_cast<int32_t>(resource.second.height),
				false, Image::FORMAT_RGBA8, imageData);

			textures[resource.first] = ImageTexture::create_from_image(image);
		}
	}
	catch(const std::exception& e) {
		particleEngine->setEffect(nullptr);

		UtilityFunctions::printerr(String(e.what()));
	}

	notify_property_list_changed();
}
Ref<PixelpartEffectResource> PixelpartEffect2D::get_effect() const {
	return effectResource;
}

Ref<PixelpartParticleEmitter> PixelpartEffect2D::find_particle_emitter(String name) const {
	CharString nameCharString = name.utf8();
	std::string nameStdString = std::string(nameCharString.get_data(), nameCharString.length());

	if(particleEmitters.count(nameStdString)) {
		return particleEmitters.at(nameStdString);
	}

	return Ref<PixelpartParticleEmitter>();
}
Ref<PixelpartParticleType> PixelpartEffect2D::find_particle_type(String name) const {
	CharString nameCharString = name.utf8();
	std::string nameStdString = std::string(nameCharString.get_data(), nameCharString.length());

	if(particleTypes.count(nameStdString)) {
		return particleTypes.at(nameStdString);
	}

	return Ref<PixelpartParticleType>();
}
Ref<PixelpartForceField> PixelpartEffect2D::find_force_field(String name) const {
	CharString nameCharString = name.utf8();
	std::string nameStdString = std::string(nameCharString.get_data(), nameCharString.length());

	if(forceFields.count(nameStdString)) {
		return forceFields.at(nameStdString);
	}

	return Ref<PixelpartForceField>();
}
Ref<PixelpartCollider> PixelpartEffect2D::find_collider(String name) const {
	CharString nameCharString = name.utf8();
	std::string nameStdString = std::string(nameCharString.get_data(), nameCharString.length());

	if(colliders.count(nameStdString)) {
		return colliders.at(nameStdString);
	}

	return Ref<PixelpartCollider>();
}
Ref<PixelpartParticleEmitter> PixelpartEffect2D::get_particle_emitter(int id) const {
	if(id >= 0 && nativeEffect.particleEmitters.contains(static_cast<uint32_t>(id))) {
		std::string name = nativeEffect.particleEmitters.get(static_cast<uint32_t>(id)).name;

		if(particleEmitters.count(name)) {
			return particleEmitters.at(name);
		}
	}

	return Ref<PixelpartParticleEmitter>();
}
Ref<PixelpartParticleType> PixelpartEffect2D::get_particle_type(int id) const {
	if(id >= 0 && nativeEffect.particleTypes.contains(static_cast<uint32_t>(id))) {
		std::string name = nativeEffect.particleTypes.get(static_cast<uint32_t>(id)).name;

		if(particleTypes.count(name)) {
			return particleTypes.at(name);
		}
	}

	return Ref<PixelpartParticleType>();
}
Ref<PixelpartForceField> PixelpartEffect2D::get_force_field(int id) const {
	if(id >= 0 && nativeEffect.forceFields.contains(static_cast<uint32_t>(id))) {
		std::string name = nativeEffect.forceFields.get(static_cast<uint32_t>(id)).name;

		if(forceFields.count(name)) {
			return forceFields.at(name);
		}
	}

	return Ref<PixelpartForceField>();
}
Ref<PixelpartCollider> PixelpartEffect2D::get_collider(int id) const {
	if(id >= 0 && nativeEffect.colliders.contains(static_cast<uint32_t>(id))) {
		std::string name = nativeEffect.colliders.get(static_cast<uint32_t>(id)).name;

		if(colliders.count(name)) {
			return colliders.at(name);
		}
	}

	return Ref<PixelpartCollider>();
}
Ref<PixelpartParticleEmitter> PixelpartEffect2D::get_particle_emitter_at_index(int index) const {
	if(index >= 0 && nativeEffect.particleEmitters.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = nativeEffect.particleEmitters.getByIndex(static_cast<uint32_t>(index)).name;

		if(particleEmitters.count(name)) {
			return particleEmitters.at(name);
		}
	}

	return Ref<PixelpartParticleEmitter>();
}
Ref<PixelpartParticleType> PixelpartEffect2D::get_particle_type_at_index(int index) const {
	if(index >= 0 && nativeEffect.particleTypes.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = nativeEffect.particleTypes.getByIndex(static_cast<uint32_t>(index)).name;

		if(particleTypes.count(name)) {
			return particleTypes.at(name);
		}
	}

	return Ref<PixelpartParticleType>();
}
Ref<PixelpartForceField> PixelpartEffect2D::get_force_field_at_index(int index) const {
	if(index >= 0 && nativeEffect.forceFields.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = nativeEffect.forceFields.getByIndex(static_cast<uint32_t>(index)).name;

		if(forceFields.count(name)) {
			return forceFields.at(name);
		}
	}

	return Ref<PixelpartForceField>();
}
Ref<PixelpartCollider> PixelpartEffect2D::get_collider_at_index(int index) const {
	if(index >= 0 && nativeEffect.colliders.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = nativeEffect.colliders.getByIndex(static_cast<uint32_t>(index)).name;

		if(colliders.count(name)) {
			return colliders.at(name);
		}
	}

	return Ref<PixelpartCollider>();
}

PixelpartEffect2D::ParticleMeshInstance::ParticleMeshInstance(const pixelpart::ParticleType& particleType, Ref<PixelpartParticleMaterial2D> particleMaterial) {
	RenderingServer* rs = RenderingServer::get_singleton();

	canvasItemRID = rs->canvas_item_create();
	materialRID = rs->material_create();

	particleType.shader.build(shaderBuildResult);
	update_shader(particleType, particleMaterial);
}
PixelpartEffect2D::ParticleMeshInstance::~ParticleMeshInstance() {
	RenderingServer* rs = RenderingServer::get_singleton();

	rs->free_rid(materialRID);
	rs->free_rid(canvasItemRID);
}

void PixelpartEffect2D::ParticleMeshInstance::update_shader(const pixelpart::ParticleType& particleType, Ref<PixelpartParticleMaterial2D> particleMaterial) {
	shader = PixelpartShaders::get_instance()->get_canvas_shader(shaderBuildResult.code,
		particleType.blendMode,
		particleMaterial.is_valid() ? particleMaterial->get_light_mode() : CanvasItemMaterial::LIGHT_MODE_NORMAL);
}

Ref<Shader> PixelpartEffect2D::ParticleMeshInstance::get_shader() const {
	return shader;
}
RID PixelpartEffect2D::ParticleMeshInstance::get_material_rid() const {
	return materialRID;
}
RID PixelpartEffect2D::ParticleMeshInstance::get_canvas_item_rid() const {
	return canvasItemRID;
}

std::string PixelpartEffect2D::ParticleMeshInstance::get_texture_id(std::size_t index) const {
	return index < shaderBuildResult.textureIds.size()
		? shaderBuildResult.textureIds.at(index)
		: "";
}
std::size_t PixelpartEffect2D::ParticleMeshInstance::get_texture_count() const {
	return shaderBuildResult.textureIds.size();
}

std::unordered_map<uint32_t, PixelpartEffect2D::ParticleMeshInstance::ParticleTrail>& PixelpartEffect2D::ParticleMeshInstance::get_trails() {
	return trails;
}

void PixelpartEffect2D::draw_particles(uint32_t particleTypeIndex) {
	RenderingServer* rs = RenderingServer::get_singleton();

	const pixelpart::Effect* effect = particleEngine->getEffect();
	const pixelpart::ParticleType& particleType = effect->particleTypes.getByIndex(particleTypeIndex);
	const pixelpart::ParticleEmitter& particleEmitter = effect->particleEmitters.get(particleType.parentId);

	ParticleMeshInstance& meshInstance = *(particleMeshInstances.at(particleTypeIndex));
	rs->canvas_item_clear(meshInstance.get_canvas_item_rid());

	if(!is_visible() || !particleType.visible) {
		return;
	}

	const pixelpart::ParticleData& particles = particleEngine->getParticles(particleTypeIndex);
	uint32_t numParticles = particleEngine->getNumParticles(particleTypeIndex);
	pixelpart::floatd scaleX = static_cast<pixelpart::floatd>(get_import_scale()) * (flipH ? -1.0 : +1.0);
	pixelpart::floatd scaleY = static_cast<pixelpart::floatd>(get_import_scale()) * (flipV ? -1.0 : +1.0);

	rs->material_set_shader(meshInstance.get_material_rid(), meshInstance.get_shader()->get_rid());
	rs->material_set_param(meshInstance.get_material_rid(), "u_EffectTime", static_cast<float>(particleEngine->getTime()));
	rs->material_set_param(meshInstance.get_material_rid(), "u_ObjectTime", static_cast<float>(particleEngine->getTime() - particleEmitter.lifetimeStart));
	for(std::size_t textureIndex = 0; textureIndex < meshInstance.get_texture_count(); textureIndex++) {
		String samplerName = String("TEXTURE") + String::num_int64(textureIndex);
		Ref<ImageTexture> texture = textures.at(meshInstance.get_texture_id(textureIndex));

		rs->material_set_param(meshInstance.get_material_rid(), samplerName, texture->get_rid());
	}

	rs->canvas_item_set_parent(meshInstance.get_canvas_item_rid(), get_canvas_item());
	rs->canvas_item_set_transform(meshInstance.get_canvas_item_rid(), Transform2D());
	rs->canvas_item_set_material(meshInstance.get_canvas_item_rid(), meshInstance.get_material_rid());

	add_particle_mesh(meshInstance,
		particleType,
		particles,
		numParticles,
		scaleX,
		scaleY);
}

void PixelpartEffect2D::add_particle_mesh(ParticleMeshInstance& meshInstance, const pixelpart::ParticleType& particleType, const pixelpart::ParticleData& particles, uint32_t numParticles, pixelpart::floatd scaleX, pixelpart::floatd scaleY) {
	switch(particleType.renderer) {
		case pixelpart::ParticleType::Renderer::sprite:
			add_particle_sprites(meshInstance, particleType, particles, numParticles, scaleX, scaleY);
			break;
		case pixelpart::ParticleType::Renderer::trail:
			add_particle_trails(meshInstance, particleType, particles, numParticles, scaleX, scaleY);
			break;
	}
}

void PixelpartEffect2D::add_particle_sprites(ParticleMeshInstance& meshInstance, const pixelpart::ParticleType& particleType, const pixelpart::ParticleData& particles, uint32_t numParticles, pixelpart::floatd scaleX, pixelpart::floatd scaleY) {
	const pixelpart::floatd packFactor = 10000.0;
	const pixelpart::vec2d scale = pixelpart::vec2d(scaleX, scaleY);

	if(numParticles == 0u) {
		return;
	}

	const pixelpart::ParticleEmitter& particleEmitter = nativeEffect.particleEmitters.get(particleType.parentId);
	pixelpart::floatd alpha = std::fmod(particleEngine->getTime() - particleEmitter.lifetimeStart, particleEmitter.lifetimeDuration) / particleEmitter.lifetimeDuration;

	PackedInt32Array indexArray;
	PackedVector2Array positionArray;
	PackedVector2Array textureCoordArray;
	PackedColorArray colorArray;
	indexArray.resize(numParticles * 6);
	positionArray.resize(numParticles * 4);
	textureCoordArray.resize(numParticles * 4);
	colorArray.resize(numParticles * 4);

	int* indices = indexArray.ptrw();
	Vector2* positions = positionArray.ptrw();
	float* textureCoords = reinterpret_cast<float*>(textureCoordArray.ptrw());
	float* colors = reinterpret_cast<float*>(colorArray.ptrw());

	for(int p = 0; p < static_cast<int>(numParticles); p++) {
		indices[p * 6 + 0] = p * 4 + 0;
		indices[p * 6 + 1] = p * 4 + 1;
		indices[p * 6 + 2] = p * 4 + 3;
		indices[p * 6 + 3] = p * 4 + 1;
		indices[p * 6 + 4] = p * 4 + 2;
		indices[p * 6 + 5] = p * 4 + 3;
	}

	for(uint32_t p = 0; p < numParticles; p++) {
		pixelpart::vec3d worldPosition[4];

		switch(particleType.alignmentMode) {
			case pixelpart::AlignmentMode::motion: {
				pixelpart::floatd angle = glm::degrees(glm::orientedAngle(pixelpart::vec2d(0.0, 1.0), (particles.velocity[p] != pixelpart::vec3d(0.0))
						? pixelpart::vec2d(glm::normalize(particles.velocity[p]))
						: pixelpart::vec2d(0.0, 1.0)));
				worldPosition[0] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(-0.5, -0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[1] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(-0.5, +0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[2] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(+0.5, +0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[3] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(+0.5, -0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				break;
			}
			case pixelpart::AlignmentMode::emission: {
				pixelpart::vec3d emissionDirection = particleEmitter.position.get(alpha) - particles.globalPosition[p];
				pixelpart::floatd angle = glm::degrees(glm::orientedAngle(pixelpart::vec2d(0.0, 1.0), (emissionDirection != pixelpart::vec3d(0.0))
						? pixelpart::vec2d(glm::normalize(emissionDirection))
						: pixelpart::vec2d(0.0, 1.0)));
				worldPosition[0] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(-0.5, -0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[1] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(-0.5, +0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[2] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(+0.5, +0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[3] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(+0.5, -0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				break;
			}
			case pixelpart::AlignmentMode::emitter: {
				pixelpart::mat3d rotationMatrix = rotation3d(particles.rotation[p]);
				pixelpart::mat3d lookAtMatrix = rotation3d(particleEmitter.orientation.get(alpha));
				worldPosition[0] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3d(-0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				worldPosition[1] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3d(-0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				worldPosition[2] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3d(+0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				worldPosition[3] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3d(+0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				break;
			}
			default: {
				pixelpart::mat3d rotationMatrix = rotation3d(particles.rotation[p]);
				worldPosition[0] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3d(-0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				worldPosition[1] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3d(-0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				worldPosition[2] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3d(+0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				worldPosition[3] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3d(+0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				break;
			}
		}

		positions[p * 4 + 0] = toGd(pixelpart::vec2d(worldPosition[0]) * scale);
		positions[p * 4 + 1] = toGd(pixelpart::vec2d(worldPosition[1]) * scale);
		positions[p * 4 + 2] = toGd(pixelpart::vec2d(worldPosition[2]) * scale);
		positions[p * 4 + 3] = toGd(pixelpart::vec2d(worldPosition[3]) * scale);
	}

	for(uint32_t p = 0; p < numParticles; p++) {
		float packedLifeData = static_cast<float>(std::floor(particles.life[p] * packFactor));
		float packedIdData = static_cast<float>(particles.id[p] + 1u);

		textureCoords[p * 4 * 2 + 0] = 0.0f + packedLifeData;
		textureCoords[p * 4 * 2 + 1] = 0.0f + packedIdData;
		textureCoords[p * 4 * 2 + 2] = 0.0f + packedLifeData;
		textureCoords[p * 4 * 2 + 3] = 1.0f / 10.0f + packedIdData;
		textureCoords[p * 4 * 2 + 4] = 1.0f / 10.0f + packedLifeData;
		textureCoords[p * 4 * 2 + 5] = 1.0f / 10.0f + packedIdData;
		textureCoords[p * 4 * 2 + 6] = 1.0f / 10.0f + packedLifeData;
		textureCoords[p * 4 * 2 + 7] = 0.0f + packedIdData;
	}

	for(uint32_t p = 0; p < numParticles; p++) {
		float packedVelocityX = static_cast<float>(std::floor(particles.velocity[p].x * packFactor) + packFactor);
		float packedVelocityY = static_cast<float>(std::floor(particles.velocity[p].y * packFactor) + packFactor);
		float packedForceX = static_cast<float>(std::floor(particles.force[p].x * packFactor) + packFactor);
		float packedForceY = static_cast<float>(std::floor(particles.force[p].y * packFactor) + packFactor);

		colors[p * 4 * 4 + 0] = packedVelocityX + static_cast<float>(particles.color[p].r * 0.1);
		colors[p * 4 * 4 + 1] = packedVelocityY + static_cast<float>(particles.color[p].g * 0.1);
		colors[p * 4 * 4 + 2] = packedForceX + static_cast<float>(particles.color[p].b * 0.1);
		colors[p * 4 * 4 + 3] = packedForceY + static_cast<float>(particles.color[p].a * 0.1);
		colors[p * 4 * 4 + 4] = packedVelocityX + static_cast<float>(particles.color[p].r * 0.1);
		colors[p * 4 * 4 + 5] = packedVelocityY + static_cast<float>(particles.color[p].g * 0.1);
		colors[p * 4 * 4 + 6] = packedForceX + static_cast<float>(particles.color[p].b * 0.1);
		colors[p * 4 * 4 + 7] = packedForceY + static_cast<float>(particles.color[p].a * 0.1);
		colors[p * 4 * 4 + 8] = packedVelocityX + static_cast<float>(particles.color[p].r * 0.1);
		colors[p * 4 * 4 + 9] = packedVelocityY + static_cast<float>(particles.color[p].g * 0.1);
		colors[p * 4 * 4 + 10] = packedForceX + static_cast<float>(particles.color[p].b * 0.1);
		colors[p * 4 * 4 + 11] = packedForceY + static_cast<float>(particles.color[p].a * 0.1);
		colors[p * 4 * 4 + 12] = packedVelocityX + static_cast<float>(particles.color[p].r * 0.1);
		colors[p * 4 * 4 + 13] = packedVelocityY + static_cast<float>(particles.color[p].g * 0.1);
		colors[p * 4 * 4 + 14] = packedForceX + static_cast<float>(particles.color[p].b * 0.1);
		colors[p * 4 * 4 + 15] = packedForceY + static_cast<float>(particles.color[p].a * 0.1);
	}

	RenderingServer* rs = RenderingServer::get_singleton();
	rs->canvas_item_add_triangle_array(meshInstance.get_canvas_item_rid(),
		indexArray,
		positionArray,
		colorArray,
		textureCoordArray);
}

void PixelpartEffect2D::add_particle_trails(ParticleMeshInstance& meshInstance, const pixelpart::ParticleType& particleType, const pixelpart::ParticleData& particles, uint32_t numParticles, pixelpart::floatd scaleX, pixelpart::floatd scaleY) {
	const pixelpart::floatd epsilon = 0.000001;
	const pixelpart::floatd packFactor = 10000.0;

	if(numParticles < 2u) {
		return;
	}

	std::vector<uint32_t> sortKeys(numParticles);
	std::iota(sortKeys.begin(), sortKeys.end(), 0u);
	std::sort(sortKeys.begin(), sortKeys.end(), [&particles](uint32_t p0, uint32_t p1) {
		return (particles.parentId[p0] != particles.parentId[p1])
			? particles.parentId[p0] > particles.parentId[p1]
			: particles.id[p0] > particles.id[p1];
	});

	std::unordered_map<uint32_t, ParticleMeshInstance::ParticleTrail>& trails = meshInstance.get_trails();

	for(auto& entry : trails) {
		entry.second.numParticles = 0u;
	}

	ParticleMeshInstance::ParticleTrail* currentTrail = nullptr;
	for(uint32_t p = 0u, particleParentId = 0u, trailId = 0xFFFFFFFEu; p < numParticles; p++) {
		particleParentId = particles.parentId[sortKeys[p]];

		if(particleParentId != trailId) {
			trailId = particleParentId;
			currentTrail = &trails[trailId];
		}

		currentTrail->numParticles++;
	}

	for(auto it = trails.begin(); it != trails.end(); ) {
		if(it->second.numParticles == 0u) {
			it = trails.erase(it);
		}
		else {
			it++;
		}
	}

	currentTrail = nullptr;

	if(particleType.trailRendererSettings.numTrailSegments > 0u) {
		for(uint32_t p = 0u, particleParentId = 0u, trailId = 0xFFFFFFFEu; p < numParticles; p++) {
			uint32_t particleIndex = sortKeys[p];
			particleParentId = particles.parentId[particleIndex];

			if(particleParentId != trailId) {
				trailId = particleParentId;
				currentTrail = &trails.at(trailId);

				std::vector<pixelpart::floatd> relativeParticlePosition(currentTrail->numParticles);
				relativeParticlePosition[0u] = 0.0;

				currentTrail->length = 0.0;
				for(uint32_t q = 1u; q < currentTrail->numParticles; q++) {
					currentTrail->length += glm::length(particles.globalPosition[sortKeys[p + q]] - particles.globalPosition[sortKeys[p + q - 1u]]);
					relativeParticlePosition[q] = currentTrail->length;
				}
				for(uint32_t q = 1u; q < currentTrail->numParticles; q++) {
					relativeParticlePosition[q] /= currentTrail->length;
				}

				pixelpart::Curve<pixelpart::vec3d> positionCurve(pixelpart::CurveInterpolation::spline);
				positionCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
				positionCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.globalPosition.data(), sortKeys.data() + p, currentTrail->numParticles);

				pixelpart::Curve<pixelpart::vec3d> sizeCurve(pixelpart::CurveInterpolation::spline);
				sizeCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
				sizeCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.size.data(), sortKeys.data() + p, currentTrail->numParticles);

				pixelpart::Curve<pixelpart::vec4d> colorCurve(pixelpart::CurveInterpolation::spline);
				colorCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
				colorCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.color.data(), sortKeys.data() + p, currentTrail->numParticles);

				pixelpart::Curve<pixelpart::vec3d> velocityCurve(pixelpart::CurveInterpolation::spline);
				velocityCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
				velocityCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.velocity.data(), sortKeys.data() + p, currentTrail->numParticles);

				pixelpart::Curve<pixelpart::vec3d> forceCurve(pixelpart::CurveInterpolation::spline);
				forceCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
				forceCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.force.data(), sortKeys.data() + p, currentTrail->numParticles);

				pixelpart::Curve<pixelpart::floatd> lifeCurve(pixelpart::CurveInterpolation::spline);
				lifeCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
				lifeCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.life.data(), sortKeys.data() + p, currentTrail->numParticles);

				currentTrail->position = positionCurve.getCache();
				currentTrail->size = sizeCurve.getCache();
				currentTrail->color = colorCurve.getCache();
				currentTrail->velocity = velocityCurve.getCache();
				currentTrail->force = forceCurve.getCache();
				currentTrail->life = lifeCurve.getCache();
				currentTrail->direction.resize(particleType.trailRendererSettings.numTrailSegments);
				currentTrail->index.resize(particleType.trailRendererSettings.numTrailSegments);
			}
		}
	}
	else {
		for(uint32_t p = 0u, q = 0u, particleParentId = 0u, trailId = 0xFFFFFFFEu; p < numParticles; p++, q++) {
			uint32_t particleIndex = sortKeys[p];
			particleParentId = particles.parentId[particleIndex];

			if(particleParentId != trailId) {
				trailId = particleParentId;
				q = 0u;

				currentTrail = &trails.at(trailId);
				currentTrail->position.resize(currentTrail->numParticles);
				currentTrail->size.resize(currentTrail->numParticles);
				currentTrail->color.resize(currentTrail->numParticles);
				currentTrail->velocity.resize(currentTrail->numParticles);
				currentTrail->force.resize(currentTrail->numParticles);
				currentTrail->life.resize(currentTrail->numParticles);
				currentTrail->direction.resize(currentTrail->numParticles);
				currentTrail->index.resize(currentTrail->numParticles);
			}

			currentTrail->position[q] = particles.globalPosition[particleIndex];
			currentTrail->size[q] = particles.size[particleIndex];
			currentTrail->color[q] = particles.color[particleIndex];
			currentTrail->velocity[q] = particles.velocity[particleIndex];
			currentTrail->force[q] = particles.force[particleIndex];
			currentTrail->life[q] = particles.life[particleIndex];
		}
	}

	RenderingServer* rs = RenderingServer::get_singleton();

	for(auto& entry : trails) {
		ParticleMeshInstance::ParticleTrail& trail = entry.second;
		trail.length = 0.0;

		if(trail.position.size() < 2u) {
			continue;
		}

		std::size_t last = trail.position.size() - 1u;
		for(std::size_t i = 0u; i < last; i++) {
			pixelpart::vec3d toNext = trail.position[i + 1u] - trail.position[i];
			pixelpart::floatd distance = std::max(glm::length(toNext), epsilon);

			trail.direction[i] = toNext / distance;
			trail.index[i] = trail.length;
			trail.length += distance;
		}

		trail.direction[last] = trail.direction[last - 1u];
		trail.index[last] = trail.length;

		for(std::size_t i = last; i > 0u; i--) {
			pixelpart::vec3d toEdge = trail.direction[i] - trail.direction[i - 1u];
			pixelpart::floatd toEdgeLength = glm::length(toEdge);
			if(toEdgeLength < epsilon) {
				trail.direction[i] = pixelpart::vec3d(
					-trail.direction[i].y,
					trail.direction[i].x,
					trail.direction[i].z);
			}
			else {
				trail.direction[i] = (glm::dot(glm::cross(trail.direction[i], pixelpart::vec3d(0.0, 0.0, 1.0)), toEdge / toEdgeLength) < 0.0)
					? +toEdge / toEdgeLength
					: -toEdge / toEdgeLength;
			}
		}

		trail.direction[0u] = pixelpart::vec3d(-trail.direction[0u].y, trail.direction[0u].x, trail.direction[0u].z);

		for(pixelpart::floatd& index : trail.index) {
			index /= trail.length;
		}

		int32_t numTrailSegments = static_cast<int32_t>(trail.position.size()) - 1;
		trail.indexArray.resize(numTrailSegments * 12);
		trail.positionArray.resize(numTrailSegments * 5);
		trail.textureCoordArray.resize(numTrailSegments * 5);
		trail.colorArray.resize(numTrailSegments * 5);

		int32_t* indices = trail.indexArray.ptrw();
		float* positions = reinterpret_cast<float*>(trail.positionArray.ptrw());
		float* textureCoords = reinterpret_cast<float*>(trail.textureCoordArray.ptrw());
		float* colors = reinterpret_cast<float*>(trail.colorArray.ptrw());

		for(int32_t p = 0; p < numTrailSegments; p++) {
			indices[p * 12 + 0] = p * 5 + 0;
			indices[p * 12 + 1] = p * 5 + 1;
			indices[p * 12 + 2] = p * 5 + 4;
			indices[p * 12 + 3] = p * 5 + 2;
			indices[p * 12 + 4] = p * 5 + 0;
			indices[p * 12 + 5] = p * 5 + 4;
			indices[p * 12 + 6] = p * 5 + 1;
			indices[p * 12 + 7] = p * 5 + 3;
			indices[p * 12 + 8] = p * 5 + 4;
			indices[p * 12 + 9] = p * 5 + 3;
			indices[p * 12 + 10] = p * 5 + 2;
			indices[p * 12 + 11] = p * 5 + 4;
		}

		for(int32_t p = 0; p < numTrailSegments; p++) {
			pixelpart::vec3d startToEdge = trail.direction[p] * std::max(trail.size[p].x, trail.size[p].y) * 0.5;
			pixelpart::vec3d endToEdge = trail.direction[p + 1] * std::max(trail.size[p + 1].x, trail.size[p + 1].y) * 0.5;

			pixelpart::vec3d vertexPositions[5] = {
				trail.position[p] + startToEdge,
				trail.position[p] - startToEdge,
				trail.position[p + 1] + endToEdge,
				trail.position[p + 1] - endToEdge,
				(trail.position[p] + trail.position[p + 1u]) * 0.5
			};

			positions[p * 5 * 2 + 0] = static_cast<float>(vertexPositions[0].x * scaleX);
			positions[p * 5 * 2 + 1] = static_cast<float>(vertexPositions[0].y * scaleY);
			positions[p * 5 * 2 + 2] = static_cast<float>(vertexPositions[1].x * scaleX);
			positions[p * 5 * 2 + 3] = static_cast<float>(vertexPositions[1].y * scaleY);
			positions[p * 5 * 2 + 4] = static_cast<float>(vertexPositions[2].x * scaleX);
			positions[p * 5 * 2 + 5] = static_cast<float>(vertexPositions[2].y * scaleY);
			positions[p * 5 * 2 + 6] = static_cast<float>(vertexPositions[3].x * scaleX);
			positions[p * 5 * 2 + 7] = static_cast<float>(vertexPositions[3].y * scaleY);
			positions[p * 5 * 2 + 8] = static_cast<float>(vertexPositions[4].x * scaleX);
			positions[p * 5 * 2 + 9] = static_cast<float>(vertexPositions[4].y * scaleY);
		}

		switch(particleType.trailRendererSettings.textureRotation) {
			case 1u:
				for(int32_t p = 0; p < numTrailSegments; p++) {
					textureCoords[p * 5 * 2 + 0] = static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 1] = 1.0f;
					textureCoords[p * 5 * 2 + 2] = static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 3] = 0.0f;
					textureCoords[p * 5 * 2 + 4] = static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 5] = 1.0f;
					textureCoords[p * 5 * 2 + 6] = static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 7] = 0.0f;
					textureCoords[p * 5 * 2 + 8] = static_cast<float>((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 9] = 0.5f;
				}
				break;

			case 2u:
				for(int32_t p = 0; p < numTrailSegments; p++) {
					textureCoords[p * 5 * 2 + 0] = 1.0f;
					textureCoords[p * 5 * 2 + 1] = 1.0f - static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 2] = 0.0f;
					textureCoords[p * 5 * 2 + 3] = 1.0f - static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 4] = 1.0f;
					textureCoords[p * 5 * 2 + 5] = 1.0f - static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 6] = 0.0f;
					textureCoords[p * 5 * 2 + 7] = 1.0f - static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 8] = 0.5f;
					textureCoords[p * 5 * 2 + 9] = 1.0f - static_cast<float>((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor);
				}
				break;

			case 3u:
				for(int32_t p = 0; p < numTrailSegments; p++) {
					textureCoords[p * 5 * 2 + 0] = 1.0f - static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 1] = 0.0f;
					textureCoords[p * 5 * 2 + 2] = 1.0f - static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 3] = 1.0f;
					textureCoords[p * 5 * 2 + 4] = 1.0f - static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 5] = 0.0f;
					textureCoords[p * 5 * 2 + 6] = 1.0f - static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 7] = 1.0f;
					textureCoords[p * 5 * 2 + 8] = 1.0f - static_cast<float>((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 9] = 0.5f;
				}
				break;

			default:
				for(int32_t p = 0; p < numTrailSegments; p++) {
					textureCoords[p * 5 * 2 + 0] = 0.0f;
					textureCoords[p * 5 * 2 + 1] = static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 2] = 1.0f;
					textureCoords[p * 5 * 2 + 3] = static_cast<float>(trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 4] = 0.0f;
					textureCoords[p * 5 * 2 + 5] = static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 6] = 1.0f;
					textureCoords[p * 5 * 2 + 7] = static_cast<float>(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					textureCoords[p * 5 * 2 + 8] = 0.5f;
					textureCoords[p * 5 * 2 + 9] = static_cast<float>((trail.index[p] + trail.index[p + 1]) * 0.5 * particleType.trailRendererSettings.textureUVFactor);
				}
				break;
		}

		float packedIdData = static_cast<float>(entry.first + packFactor);
		for(int32_t p = 0; p < numTrailSegments; p++) {
			float packedLifeDataStart = static_cast<float>(std::floor(trail.life[p] * packFactor) + packFactor);
			float packedLifeDataEnd = static_cast<float>(std::floor(trail.life[p + 1] * packFactor) + packFactor);
			float packedLifeDataCenter = static_cast<float>(std::floor((trail.life[p] + trail.life[p + 1]) * 0.5 * packFactor) + packFactor);

			textureCoords[p * 5 * 2 + 0] = textureCoords[p * 5 * 2 + 0] / 10.0f + packedLifeDataStart;
			textureCoords[p * 5 * 2 + 1] = textureCoords[p * 5 * 2 + 1] / 10.0f + packedIdData;
			textureCoords[p * 5 * 2 + 2] = textureCoords[p * 5 * 2 + 2] / 10.0f + packedLifeDataStart;
			textureCoords[p * 5 * 2 + 3] = textureCoords[p * 5 * 2 + 3] / 10.0f + packedIdData;
			textureCoords[p * 5 * 2 + 4] = textureCoords[p * 5 * 2 + 4] / 10.0f + packedLifeDataEnd;
			textureCoords[p * 5 * 2 + 5] = textureCoords[p * 5 * 2 + 5] / 10.0f + packedIdData;
			textureCoords[p * 5 * 2 + 6] = textureCoords[p * 5 * 2 + 6] / 10.0f + packedLifeDataEnd;
			textureCoords[p * 5 * 2 + 7] = textureCoords[p * 5 * 2 + 7] / 10.0f + packedIdData;
			textureCoords[p * 5 * 2 + 8] = textureCoords[p * 5 * 2 + 8] / 10.0f + packedLifeDataCenter;
			textureCoords[p * 5 * 2 + 9] = textureCoords[p * 5 * 2 + 9] / 10.0f + packedIdData;
		}

		for(int32_t p = 0; p < numTrailSegments; p++) {
			float packedVelocityXStart = static_cast<float>(std::floor(trail.velocity[p].x * packFactor) + packFactor);
			float packedVelocityYStart = static_cast<float>(std::floor(trail.velocity[p].y * packFactor) + packFactor);
			float packedVelocityXEnd = static_cast<float>(std::floor(trail.velocity[p + 1].x * packFactor) + packFactor);
			float packedVelocityYEnd = static_cast<float>(std::floor(trail.velocity[p + 1].y * packFactor) + packFactor);
			float packedVelocityXCenter = static_cast<float>(std::floor((trail.velocity[p].x + trail.velocity[p + 1].x) * 0.5 * packFactor) + packFactor);
			float packedVelocityYCenter = static_cast<float>(std::floor((trail.velocity[p].y + trail.velocity[p + 1].y) * 0.5 * packFactor) + packFactor);
			float packedForceXStart = static_cast<float>(std::floor(trail.force[p].x * packFactor) + packFactor);
			float packedForceYStart = static_cast<float>(std::floor(trail.force[p].y * packFactor) + packFactor);
			float packedForceXEnd = static_cast<float>(std::floor(trail.force[p + 1].x * packFactor) + packFactor);
			float packedForceYEnd = static_cast<float>(std::floor(trail.force[p + 1].y * packFactor) + packFactor);
			float packedForceXCenter = static_cast<float>(std::floor((trail.force[p].x + trail.force[p + 1].x) * 0.5 * packFactor) + packFactor);
			float packedForceYCenter = static_cast<float>(std::floor((trail.force[p].y + trail.force[p + 1].y) * 0.5 * packFactor) + packFactor);

			colors[p * 5 * 4 + 0] = packedVelocityXStart + static_cast<float>(trail.color[p].r * 0.1);
			colors[p * 5 * 4 + 1] = packedVelocityYStart + static_cast<float>(trail.color[p].g * 0.1);
			colors[p * 5 * 4 + 2] = packedForceXStart + static_cast<float>(trail.color[p].b * 0.1);
			colors[p * 5 * 4 + 3] = packedForceYStart + static_cast<float>(trail.color[p].a * 0.1);
			colors[p * 5 * 4 + 4] = packedVelocityXStart + static_cast<float>(trail.color[p].r * 0.1);
			colors[p * 5 * 4 + 5] = packedVelocityYStart + static_cast<float>(trail.color[p].g * 0.1);
			colors[p * 5 * 4 + 6] = packedForceXStart + static_cast<float>(trail.color[p].b * 0.1);
			colors[p * 5 * 4 + 7] = packedForceYStart + static_cast<float>(trail.color[p].a * 0.1);
			colors[p * 5 * 4 + 8] = packedVelocityXEnd + static_cast<float>(trail.color[p].r * 0.1);
			colors[p * 5 * 4 + 9] = packedVelocityYEnd + static_cast<float>(trail.color[p].g * 0.1);
			colors[p * 5 * 4 + 10] = packedForceXEnd + static_cast<float>(trail.color[p].b * 0.1);
			colors[p * 5 * 4 + 11] = packedForceYEnd + static_cast<float>(trail.color[p].a * 0.1);
			colors[p * 5 * 4 + 12] = packedVelocityXEnd + static_cast<float>(trail.color[p].r * 0.1);
			colors[p * 5 * 4 + 13] = packedVelocityYEnd + static_cast<float>(trail.color[p].g * 0.1);
			colors[p * 5 * 4 + 14] = packedForceXEnd + static_cast<float>(trail.color[p].b * 0.1);
			colors[p * 5 * 4 + 15] = packedForceYEnd + static_cast<float>(trail.color[p].a * 0.1);
			colors[p * 5 * 4 + 16] = packedVelocityXCenter + static_cast<float>(trail.color[p].r * 0.1);
			colors[p * 5 * 4 + 17] = packedVelocityYCenter + static_cast<float>(trail.color[p].g * 0.1);
			colors[p * 5 * 4 + 18] = packedForceXCenter + static_cast<float>(trail.color[p].b * 0.1);
			colors[p * 5 * 4 + 19] = packedForceYCenter + static_cast<float>(trail.color[p].a * 0.1);
		}

		rs->canvas_item_add_triangle_array(meshInstance.get_canvas_item_rid(),
			trail.indexArray,
			trail.positionArray,
			trail.colorArray,
			trail.textureCoordArray);
	}
}

pixelpart::vec3d PixelpartEffect2D::rotate2d(const pixelpart::vec3d& p, const pixelpart::vec3d& o, pixelpart::floatd a) {
	pixelpart::floatd s = std::sin(glm::radians(a));
	pixelpart::floatd c = std::cos(glm::radians(a));

	return pixelpart::vec3d(
		(p.x - o.x) * c - (p.y - o.y) * s + o.x,
		(p.x - o.x) * s + (p.y - o.y) * c + o.y,
		0.0);
}
pixelpart::mat3d PixelpartEffect2D::rotation3d(const pixelpart::vec3d& angle) {
	pixelpart::vec3d rotation = glm::radians(angle);
	pixelpart::floatd cy = std::cos(rotation.y);
	pixelpart::floatd sy = std::sin(rotation.y);
	pixelpart::floatd cp = std::cos(rotation.z);
	pixelpart::floatd sp = std::sin(rotation.z);
	pixelpart::floatd cr = std::cos(rotation.x);
	pixelpart::floatd sr = std::sin(rotation.x);

	return pixelpart::mat3d(
		pixelpart::vec3d(cy * cr + sy * sp * sr, sr * cp, -sy * cr + cy * sp * sr),
		pixelpart::vec3d(-cy * sr + sy * sp * cr, cr * cp, sr * sy + cy * sp * cr),
		pixelpart::vec3d(sy * cp, -sp, cy * cp));
}
pixelpart::mat3d PixelpartEffect2D::lookAt(const pixelpart::vec3d& direction) {
	pixelpart::vec3d up = pixelpart::vec3d(0.0, 1.0, 0.0);
	pixelpart::vec3d front = glm::normalize(direction);
	pixelpart::vec3d right = glm::normalize(glm::cross(front, up));
	up = glm::normalize(glm::cross(right, front));

	return pixelpart::mat3d(right, up, front);
}
}