#include "PixelpartEffect.h"
#include "PixelpartSortUtil.h"
#include "PixelpartUtil.h"
#include "ParticleSolverCPU.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/camera3d.hpp>
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
	ClassDB::bind_method(D_METHOD("set_shading_mode", "mode"), &PixelpartEffect::set_shading_mode);
	ClassDB::bind_method(D_METHOD("set_diffuse_mode", "mode"), &PixelpartEffect::set_diffuse_mode);
	ClassDB::bind_method(D_METHOD("set_specular_mode", "mode"), &PixelpartEffect::set_specular_mode);
	ClassDB::bind_method(D_METHOD("set_normal_mode", "mode"), &PixelpartEffect::set_normal_mode);
	ClassDB::bind_method(D_METHOD("set_static_normal", "normal"), &PixelpartEffect::set_static_normal);
	ClassDB::bind_method(D_METHOD("get_shading_mode"), &PixelpartEffect::get_shading_mode);
	ClassDB::bind_method(D_METHOD("get_diffuse_mode"), &PixelpartEffect::get_diffuse_mode);
	ClassDB::bind_method(D_METHOD("get_specular_mode"), &PixelpartEffect::get_specular_mode);
	ClassDB::bind_method(D_METHOD("get_normal_mode"), &PixelpartEffect::get_normal_mode);
	ClassDB::bind_method(D_METHOD("get_static_normal"), &PixelpartEffect::get_static_normal);
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

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "effect"), "set_effect", "get_effect");

	ADD_GROUP("Playback", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "play", "is_playing");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "loop"), "set_loop", "get_loop");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "loop_time", PROPERTY_HINT_RANGE, "0.0,1000.0,0.01"), "set_loop_time", "get_loop_time");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0.0,100.0,0.01"), "set_speed", "get_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "frame_rate", PROPERTY_HINT_RANGE, "1.0,100.0,1.0"), "set_frame_rate", "get_frame_rate");

	ADD_GROUP("Shading", "");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "shading_mode", PROPERTY_HINT_ENUM, "Unshaded,Per-Pixel,Per-Vertex"), "set_shading_mode", "get_shading_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "diffuse_mode", PROPERTY_HINT_ENUM, "Burley,Lambert,Lambert Wrap,Toon"), "set_diffuse_mode", "get_diffuse_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "specular_mode", PROPERTY_HINT_ENUM, "SchlickGGX,Toon,Disabled"), "set_specular_mode", "get_specular_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "normal_mode", PROPERTY_HINT_ENUM, "Mesh,Static"), "set_normal_mode", "get_normal_mode");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "static_normal"), "set_static_normal", "get_static_normal");

	BIND_ENUM_CONSTANT(PARTICLE_NORMAL_MODE_MESH);
	BIND_ENUM_CONSTANT(PARTICLE_NORMAL_MODE_STATIC);
}

PixelpartEffect::PixelpartEffect() {
	auto settings = ProjectSettings::get_singleton();

	uint32_t particleCapacity = 10000;
	if(settings->has_setting("pixelpart/particle_capacity")) {
		particleCapacity = static_cast<uint32_t>(std::max((int)settings->get_setting("pixelpart/particle_capacity"), 1));
	}

	particleEngine = pixelpart::ParticleEngine::createUnique<pixelpart::ParticleSolverCPU>(nullptr, particleCapacity);
}
PixelpartEffect::~PixelpartEffect() {
	RenderingServer* rs = RenderingServer::get_singleton();

	for(const ParticleMeshInstance& meshInstance : particleMeshInstances) {
		rs->free_rid(meshInstance.instance);
	}
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
}
void PixelpartEffect::draw() {
	if(!particleEngine) {
		return;
	}

	const pixelpart::Effect* effect = particleEngine->getEffect();
	if(!effect) {
		return;
	}

	Viewport* viewport = get_viewport();
	if(!viewport) {
		UtilityFunctions::printerr("Viewport is null");
		return;
	}

	if(shaderDirty) {
		for(uint32_t i = 0; i < nativeEffect.particleTypes.getCount(); i++) {
			const pixelpart::ParticleType& particleType = nativeEffect.particleTypes.getByIndex(i);
			const pixelpart::ShaderGraph::BuildResult& shaderBuildResult = particleTypes[particleType.name]->get_shader_build_result();

			particleMeshInstances[i].shader = PixelpartShaders::get_instance()->get_spatial_shader(shaderBuildResult.code,
				particleType.blendMode, shadingMode, diffuseMode, specularMode, normalMode);
		}

		shaderDirty = false;
	}

	for(uint32_t particleTypeIndex = 0; particleTypeIndex < effect->particleTypes.getCount(); particleTypeIndex++) {
		draw_particles(effect->particleTypes.getByIndex(particleTypeIndex), particleMeshInstances[particleTypeIndex]);
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
		particleEngine->restart();
	}
}
void PixelpartEffect::reset() {
	if(particleEngine) {
		particleEngine->reset();
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

void PixelpartEffect::set_shading_mode(BaseMaterial3D::ShadingMode mode) {
	shaderDirty |= mode != shadingMode;
	shadingMode = mode;
}
void PixelpartEffect::set_diffuse_mode(BaseMaterial3D::DiffuseMode mode) {
	shaderDirty |= mode != diffuseMode;
	diffuseMode = mode;
}
void PixelpartEffect::set_specular_mode(BaseMaterial3D::SpecularMode mode) {
	shaderDirty |= mode != specularMode;
	specularMode = mode;
}
void PixelpartEffect::set_normal_mode(ParticleNormalMode mode) {
	shaderDirty |= mode != normalMode;
	normalMode = mode;
}
void PixelpartEffect::set_static_normal(Vector3 normal) {
	staticNormal = normal;
}
BaseMaterial3D::ShadingMode PixelpartEffect::get_shading_mode() const {
	return shadingMode;
}
BaseMaterial3D::DiffuseMode PixelpartEffect::get_diffuse_mode() const {
	return diffuseMode;
}
BaseMaterial3D::SpecularMode PixelpartEffect::get_specular_mode() const {
	return specularMode;
}
ParticleNormalMode PixelpartEffect::get_normal_mode() const {
	return normalMode;
}
Vector3 PixelpartEffect::get_static_normal() const {
	return staticNormal;
}

float PixelpartEffect::get_import_scale() const {
	if(!effectResource.is_valid()) {
		return 1.0f;
	}

	return effectResource->get_scale();
}

void PixelpartEffect::set_effect(Ref<PixelpartEffectResource> effectRes) {
	PixelpartShaders* shaders = PixelpartShaders::get_instance();

	RenderingServer* rs = RenderingServer::get_singleton();
	for(const ParticleMeshInstance& meshInstance : particleMeshInstances) {
		rs->free_rid(meshInstance.instance);
	}

	particleMeshInstances.clear();
	particleEmitters.clear();
	particleTypes.clear();
	forceFields.clear();
	colliders.clear();
	textures.clear();

	effectResource = effectRes;

	if(effectResource.is_valid()) {
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
				pixelpart::ShaderGraph::BuildResult buildResult;
				particleType.shader.build(buildResult);

				Ref<PixelpartParticleType> particleTypeRef;
				particleTypeRef.instantiate();
				particleTypeRef->init(effectResource, &particleType, particleEngine.get(), buildResult);

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

			RenderingServer* rs = RenderingServer::get_singleton();

			for(const pixelpart::ParticleType& particleType : nativeEffect.particleTypes) {
				Ref<ArrayMesh> mesh = new ArrayMesh();
				Ref<ShaderMaterial> material = new ShaderMaterial();

				RID instance = rs->instance_create();
				rs->instance_set_base(instance, mesh->get_rid());

				const pixelpart::ShaderGraph::BuildResult& shaderBuildResult = particleTypes.at(particleType.name)->get_shader_build_result();
				Ref<Shader> shader = shaders->get_spatial_shader(shaderBuildResult.code,
					particleType.blendMode, shadingMode, diffuseMode, specularMode, normalMode);

				particleMeshInstances.push_back(ParticleMeshInstance{
					instance,
					mesh,
					material,
					shader,
					shaderBuildResult.textureIds
				});
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
	}
	else {
		particleEngine->setEffect(nullptr);
	}
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
	if(id >= 0 && nativeEffect.particleEmitters.contains(static_cast<uint32_t>(id))) {
		std::string name = nativeEffect.particleEmitters.get(static_cast<uint32_t>(id)).name;

		if(particleEmitters.count(name)) {
			return particleEmitters.at(name);
		}
	}

	return Ref<PixelpartParticleEmitter>();
}
Ref<PixelpartParticleType> PixelpartEffect::get_particle_type(int id) const {
	if(id >= 0 && nativeEffect.particleTypes.contains(static_cast<uint32_t>(id))) {
		std::string name = nativeEffect.particleTypes.get(static_cast<uint32_t>(id)).name;

		if(particleTypes.count(name)) {
			return particleTypes.at(name);
		}
	}

	return Ref<PixelpartParticleType>();
}
Ref<PixelpartForceField> PixelpartEffect::get_force_field(int id) const {
	if(id >= 0 && nativeEffect.forceFields.contains(static_cast<uint32_t>(id))) {
		std::string name = nativeEffect.forceFields.get(static_cast<uint32_t>(id)).name;

		if(forceFields.count(name)) {
			return forceFields.at(name);
		}
	}

	return Ref<PixelpartForceField>();
}
Ref<PixelpartCollider> PixelpartEffect::get_collider(int id) const {
	if(id >= 0 && nativeEffect.colliders.contains(static_cast<uint32_t>(id))) {
		std::string name = nativeEffect.colliders.get(static_cast<uint32_t>(id)).name;

		if(colliders.count(name)) {
			return colliders.at(name);
		}
	}

	return Ref<PixelpartCollider>();
}
Ref<PixelpartParticleEmitter> PixelpartEffect::get_particle_emitter_at_index(int index) const {
	if(index >= 0 && nativeEffect.particleEmitters.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = nativeEffect.particleEmitters.getByIndex(static_cast<uint32_t>(index)).name;

		if(particleEmitters.count(name)) {
			return particleEmitters.at(name);
		}
	}

	return Ref<PixelpartParticleEmitter>();
}
Ref<PixelpartParticleType> PixelpartEffect::get_particle_type_at_index(int index) const {
	if(index >= 0 && nativeEffect.particleTypes.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = nativeEffect.particleTypes.getByIndex(static_cast<uint32_t>(index)).name;

		if(particleTypes.count(name)) {
			return particleTypes.at(name);
		}
	}

	return Ref<PixelpartParticleType>();
}
Ref<PixelpartForceField> PixelpartEffect::get_force_field_at_index(int index) const {
	if(index >= 0 && nativeEffect.forceFields.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = nativeEffect.forceFields.getByIndex(static_cast<uint32_t>(index)).name;

		if(forceFields.count(name)) {
			return forceFields.at(name);
		}
	}

	return Ref<PixelpartForceField>();
}
Ref<PixelpartCollider> PixelpartEffect::get_collider_at_index(int index) const {
	if(index >= 0 && nativeEffect.colliders.containsIndex(static_cast<uint32_t>(index))) {
		std::string name = nativeEffect.colliders.getByIndex(static_cast<uint32_t>(index)).name;

		if(colliders.count(name)) {
			return colliders.at(name);
		}
	}

	return Ref<PixelpartCollider>();
}

void PixelpartEffect::draw_particles(const pixelpart::ParticleType& particleType, ParticleMeshInstance& meshInstance) {
	meshInstance.mesh->clear_surfaces();

	if(!is_visible() || !particleType.visible) {
		return;
	}

	const pixelpart::Effect* effect = particleEngine->getEffect();
	const pixelpart::ParticleEmitter& particleEmitter = effect->particleEmitters.get(particleType.parentId);
	const uint32_t particleTypeIndex = effect->particleTypes.findById(particleType.id);
	const uint32_t numParticles = particleEngine->getNumParticles(particleTypeIndex);
	const pixelpart::ParticleData& particles = particleEngine->getParticles(particleTypeIndex);
	const pixelpart::floatd scale = static_cast<pixelpart::floatd>(get_import_scale());

	meshInstance.material->set_shader(meshInstance.shader);
	meshInstance.material->set_shader_parameter("EFFECT_TIME", static_cast<float>(particleEngine->getTime()));
	meshInstance.material->set_shader_parameter("OBJECT_TIME", static_cast<float>(particleEngine->getTime() - particleEmitter.lifetimeStart));
	meshInstance.material->set_shader_parameter("STATIC_NORMAL", staticNormal);
	for(std::size_t t = 0; t < meshInstance.textures.size(); t++) {
		String samplerName = String("TEXTURE") + String::num_int64(t);
		Ref<ImageTexture> texture = textures.at(meshInstance.textures[t]);

		meshInstance.material->set_shader_parameter(samplerName, texture->get_rid());
	}

	RenderingServer* rs = RenderingServer::get_singleton();
	rs->instance_set_scenario(meshInstance.instance, get_world_3d()->get_scenario());
	rs->instance_set_transform(meshInstance.instance, get_global_transform());

	add_particle_mesh(meshInstance,
		particleType,
		particles,
		numParticles,
		scale);
}

void PixelpartEffect::add_particle_mesh(ParticleMeshInstance& meshInstance, const pixelpart::ParticleType& particleType, const pixelpart::ParticleData& particles, uint32_t numParticles, pixelpart::floatd scale) {
	switch(particleType.renderer) {
		case pixelpart::ParticleType::Renderer::sprite:
			add_particle_sprites(meshInstance, particleType, particles, numParticles, scale);
			break;
		case pixelpart::ParticleType::Renderer::trail:
			add_particle_trails(meshInstance, particleType, particles, numParticles, scale);
			break;
	}
}

void PixelpartEffect::add_particle_sprites(ParticleMeshInstance& meshInstance, const pixelpart::ParticleType& particleType, const pixelpart::ParticleData& particles, uint32_t numParticles, pixelpart::floatd scale) {
	const pixelpart::floatd packFactor = 10000.0;

	if(numParticles == 0) {
		return;
	}

	const pixelpart::ParticleEmitter& particleEmitter = nativeEffect.particleEmitters.get(particleType.parentId);
	pixelpart::floatd alpha = std::fmod(particleEngine->getTime() - particleEmitter.lifetimeStart, particleEmitter.lifetimeDuration) / particleEmitter.lifetimeDuration;

	Camera3D* camera = get_viewport()->get_camera_3d();
	if(!camera) {
		return;
	}

	const pixelpart::ParticleData* particleRenderData = &particles;

	if(particleType.spriteRendererSettings.sortCriterion != pixelpart::ParticleSortCriterion::none && numParticles > 1) {
		meshInstance.sortedParticleData.resize(particles.id.size());
		meshInstance.sortKeys.resize(particles.id.size());
		std::iota(meshInstance.sortKeys.begin(), meshInstance.sortKeys.begin() + numParticles, 0);

		switch(particleType.spriteRendererSettings.sortCriterion) {
			case pixelpart::ParticleSortCriterion::age: {
				insertionSort(meshInstance.sortKeys.begin(), meshInstance.sortKeys.begin() + numParticles,
					[&particles](uint32_t i, uint32_t j) {
						return particles.id[i] < particles.id[j];
					});

				break;
			}

			case pixelpart::ParticleSortCriterion::distance: {
				pixelpart::vec3d cameraPosition = fromGd(camera->get_global_transform().origin);

				insertionSort(meshInstance.sortKeys.begin(), meshInstance.sortKeys.begin() + numParticles,
					[&particles, cameraPosition](uint32_t i, uint32_t j) {
						return glm::distance2(particles.globalPosition[i], cameraPosition) > glm::distance2(particles.globalPosition[j], cameraPosition);
					});

				break;
			}
		}

		for(uint32_t i = 0; i < numParticles; i++) {
			uint32_t j = meshInstance.sortKeys[i];

			meshInstance.sortedParticleData.id[i] = particles.id[j];
			meshInstance.sortedParticleData.life[i] = particles.life[j];
			meshInstance.sortedParticleData.globalPosition[i] = particles.globalPosition[j];
			meshInstance.sortedParticleData.velocity[i] = particles.velocity[j];
			meshInstance.sortedParticleData.force[i] = particles.force[j];
			meshInstance.sortedParticleData.rotation[i] = particles.rotation[j];
			meshInstance.sortedParticleData.size[i] = particles.size[j];
			meshInstance.sortedParticleData.color[i] = particles.color[j];
		}

		particleRenderData = &meshInstance.sortedParticleData;
	}

	pixelpart::vec3d cameraPosition = fromGd(camera->get_global_transform().origin);
	pixelpart::vec3d cameraRight = fromGd(camera->get_global_transform().basis.get_column(0));
	pixelpart::vec3d cameraUp = fromGd(camera->get_global_transform().basis.get_column(1));

	Ref<ArrayMesh> mesh = meshInstance.mesh;

	PackedInt32Array indexArray;
	PackedVector3Array vertexArray;
	PackedVector3Array normalArray;
	PackedVector2Array uvArray;
	PackedVector2Array uv2Array;
	PackedColorArray colorArray;
	PackedFloat32Array custom0Array;
	PackedFloat32Array custom1Array;
	indexArray.resize(numParticles * 6);
	vertexArray.resize(numParticles * 4);
	normalArray.resize(numParticles * 4);
	uvArray.resize(numParticles * 4);
	uv2Array.resize(numParticles * 4);
	colorArray.resize(numParticles * 4);
	custom0Array.resize(numParticles * 4 * 4);
	custom1Array.resize(numParticles * 4 * 4);

	int32_t* indices = indexArray.ptrw();
	Vector3* positions = vertexArray.ptrw();
	Vector3* normals = normalArray.ptrw();
	Color* colors = colorArray.ptrw();
	float* uvs = reinterpret_cast<float*>(uvArray.ptrw());
	float* uvs2 = reinterpret_cast<float*>(uv2Array.ptrw());
	float* custom0 = custom0Array.ptrw();
	float* custom1 = custom1Array.ptrw();

	for(int p = 0; p < static_cast<int>(numParticles); p++) {
		indices[p * 6 + 0] = p * 4 + 0;
		indices[p * 6 + 1] = p * 4 + 1;
		indices[p * 6 + 2] = p * 4 + 3;
		indices[p * 6 + 3] = p * 4 + 1;
		indices[p * 6 + 4] = p * 4 + 2;
		indices[p * 6 + 5] = p * 4 + 3;
	}

	for(uint32_t p = 0; p < numParticles; p++) {
		pixelpart::mat3d rotationMatrix = rotation3d(particleRenderData->rotation[p]);
		pixelpart::vec3d pivot = particleType.pivot * particleRenderData->size[p];
		pixelpart::vec3d position[4] = {
			rotationMatrix * (pixelpart::vec3d(-0.5, -0.5, 0.0) * particleRenderData->size[p] - pivot) + pivot,
			rotationMatrix * (pixelpart::vec3d(-0.5, +0.5, 0.0) * particleRenderData->size[p] - pivot) + pivot,
			rotationMatrix * (pixelpart::vec3d(+0.5, +0.5, 0.0) * particleRenderData->size[p] - pivot) + pivot,
			rotationMatrix * (pixelpart::vec3d(+0.5, -0.5, 0.0) * particleRenderData->size[p] - pivot) + pivot };
		pixelpart::vec3d normal = rotationMatrix * pixelpart::vec3d(0.0, 0.0, 1.0);

		switch(particleType.alignmentMode) {
			case pixelpart::AlignmentMode::camera: {
				position[0] = particleRenderData->globalPosition[p] + cameraRight * position[0].x + cameraUp * position[0].y;
				position[1] = particleRenderData->globalPosition[p] + cameraRight * position[1].x + cameraUp * position[1].y;
				position[2] = particleRenderData->globalPosition[p] + cameraRight * position[2].x + cameraUp * position[2].y;
				position[3] = particleRenderData->globalPosition[p] + cameraRight * position[3].x + cameraUp * position[3].y;
				normal = cameraPosition - particleRenderData->globalPosition[p];
				break;
			}
			case pixelpart::AlignmentMode::motion: {
				pixelpart::mat3d lookAtMatrix = lookAt(particleRenderData->velocity[p]);
				position[0] = particleRenderData->globalPosition[p] + lookAtMatrix * position[0];
				position[1] = particleRenderData->globalPosition[p] + lookAtMatrix * position[1];
				position[2] = particleRenderData->globalPosition[p] + lookAtMatrix * position[2];
				position[3] = particleRenderData->globalPosition[p] + lookAtMatrix * position[3];
				normal = lookAtMatrix * normal;
				break;
			}
			case pixelpart::AlignmentMode::emission: {
				pixelpart::mat3d lookAtMatrix = lookAt(particleEmitter.position.get(alpha) - particleRenderData->globalPosition[p]);
				position[0] = particleRenderData->globalPosition[p] + lookAtMatrix * position[0];
				position[1] = particleRenderData->globalPosition[p] + lookAtMatrix * position[1];
				position[2] = particleRenderData->globalPosition[p] + lookAtMatrix * position[2];
				position[3] = particleRenderData->globalPosition[p] + lookAtMatrix * position[3];
				normal = lookAtMatrix * normal;
				break;
			}
			case pixelpart::AlignmentMode::emitter: {
				pixelpart::mat3d lookAtMatrix = rotation3d(particleEmitter.orientation.get(alpha));
				position[0] = particleRenderData->globalPosition[p] + lookAtMatrix * position[0];
				position[1] = particleRenderData->globalPosition[p] + lookAtMatrix * position[1];
				position[2] = particleRenderData->globalPosition[p] + lookAtMatrix * position[2];
				position[3] = particleRenderData->globalPosition[p] + lookAtMatrix * position[3];
				normal = lookAtMatrix * normal;
				break;
			}
			default: {
				position[0] = particleRenderData->globalPosition[p] + position[0];
				position[1] = particleRenderData->globalPosition[p] + position[1];
				position[2] = particleRenderData->globalPosition[p] + position[2];
				position[3] = particleRenderData->globalPosition[p] + position[3];
				break;
			}
		}

		positions[p * 4 + 0] = toGd(position[0] * scale);
		positions[p * 4 + 1] = toGd(position[1] * scale);
		positions[p * 4 + 2] = toGd(position[2] * scale);
		positions[p * 4 + 3] = toGd(position[3] * scale);
		normals[p * 4 + 0] = toGd(normal);
		normals[p * 4 + 1] = toGd(normal);
		normals[p * 4 + 2] = toGd(normal);
		normals[p * 4 + 3] = toGd(normal);
	}

	for(uint32_t p = 0; p < numParticles; p++) {
		colors[p * 4 + 0] = toGd(particleRenderData->color[p]);
		colors[p * 4 + 1] = toGd(particleRenderData->color[p]);
		colors[p * 4 + 2] = toGd(particleRenderData->color[p]);
		colors[p * 4 + 3] = toGd(particleRenderData->color[p]);
	}

	for(uint32_t p = 0; p < numParticles; p++) {
		uvs[p * 4 * 2 + 0] = 0.0f;
		uvs[p * 4 * 2 + 1] = 0.0f;
		uvs[p * 4 * 2 + 2] = 1.0f;
		uvs[p * 4 * 2 + 3] = 0.0f;
		uvs[p * 4 * 2 + 4] = 1.0f;
		uvs[p * 4 * 2 + 5] = 1.0f;
		uvs[p * 4 * 2 + 6] = 0.0f;
		uvs[p * 4 * 2 + 7] = 1.0f;
	}

	for(uint32_t p = 0; p < numParticles; p++) {
		uvs2[p * 4 * 2 + 0] = static_cast<float>(particleRenderData->life[p]);
		uvs2[p * 4 * 2 + 1] = static_cast<float>(particleRenderData->id[p]);
		uvs2[p * 4 * 2 + 2] = static_cast<float>(particleRenderData->life[p]);
		uvs2[p * 4 * 2 + 3] = static_cast<float>(particleRenderData->id[p]);
		uvs2[p * 4 * 2 + 4] = static_cast<float>(particleRenderData->life[p]);
		uvs2[p * 4 * 2 + 5] = static_cast<float>(particleRenderData->id[p]);
		uvs2[p * 4 * 2 + 6] = static_cast<float>(particleRenderData->life[p]);
		uvs2[p * 4 * 2 + 7] = static_cast<float>(particleRenderData->id[p]);
	}

	for(uint32_t p = 0; p < numParticles; p++) {
		custom0[p * 4 * 4 + 0] = static_cast<float>(particleRenderData->velocity[p].x);
		custom0[p * 4 * 4 + 1] = static_cast<float>(particleRenderData->velocity[p].y);
		custom0[p * 4 * 4 + 2] = static_cast<float>(particleRenderData->velocity[p].z);
		custom0[p * 4 * 4 + 3] = 0.0f;
		custom0[p * 4 * 4 + 4] = static_cast<float>(particleRenderData->velocity[p].x);
		custom0[p * 4 * 4 + 5] = static_cast<float>(particleRenderData->velocity[p].y);
		custom0[p * 4 * 4 + 6] = static_cast<float>(particleRenderData->velocity[p].z);
		custom0[p * 4 * 4 + 7] = 0.0f;
		custom0[p * 4 * 4 + 8] = static_cast<float>(particleRenderData->velocity[p].x);
		custom0[p * 4 * 4 + 9] = static_cast<float>(particleRenderData->velocity[p].y);
		custom0[p * 4 * 4 + 10] = static_cast<float>(particleRenderData->velocity[p].z);
		custom0[p * 4 * 4 + 11] = 0.0f;
		custom0[p * 4 * 4 + 12] = static_cast<float>(particleRenderData->velocity[p].x);
		custom0[p * 4 * 4 + 13] = static_cast<float>(particleRenderData->velocity[p].y);
		custom0[p * 4 * 4 + 14] = static_cast<float>(particleRenderData->velocity[p].z);
		custom0[p * 4 * 4 + 15] = 0.0f;
	}

	for(uint32_t p = 0; p < numParticles; p++) {
		custom1[p * 4 * 4 + 0] = static_cast<float>(particleRenderData->force[p].x);
		custom1[p * 4 * 4 + 1] = static_cast<float>(particleRenderData->force[p].y);
		custom1[p * 4 * 4 + 2] = static_cast<float>(particleRenderData->force[p].z);
		custom1[p * 4 * 4 + 3] = 0.0f;
		custom1[p * 4 * 4 + 4] = static_cast<float>(particleRenderData->force[p].x);
		custom1[p * 4 * 4 + 5] = static_cast<float>(particleRenderData->force[p].y);
		custom1[p * 4 * 4 + 6] = static_cast<float>(particleRenderData->force[p].z);
		custom1[p * 4 * 4 + 7] = 0.0f;
		custom1[p * 4 * 4 + 8] = static_cast<float>(particleRenderData->force[p].x);
		custom1[p * 4 * 4 + 9] = static_cast<float>(particleRenderData->force[p].y);
		custom1[p * 4 * 4 + 10] = static_cast<float>(particleRenderData->force[p].z);
		custom1[p * 4 * 4 + 11] = 0.0f;
		custom1[p * 4 * 4 + 12] = static_cast<float>(particleRenderData->force[p].x);
		custom1[p * 4 * 4 + 13] = static_cast<float>(particleRenderData->force[p].y);
		custom1[p * 4 * 4 + 14] = static_cast<float>(particleRenderData->force[p].z);
		custom1[p * 4 * 4 + 15] = 0.0f;
	}

	Array meshArray;
	meshArray.resize(Mesh::ARRAY_MAX);
	meshArray[Mesh::ARRAY_VERTEX] = vertexArray;
	meshArray[Mesh::ARRAY_NORMAL] = normalArray;
	meshArray[Mesh::ARRAY_COLOR] = colorArray;
	meshArray[Mesh::ARRAY_TEX_UV] = uvArray;
	meshArray[Mesh::ARRAY_TEX_UV2] = uv2Array;
	meshArray[Mesh::ARRAY_CUSTOM0] = custom0Array;
	meshArray[Mesh::ARRAY_CUSTOM1] = custom1Array;
	meshArray[Mesh::ARRAY_INDEX] = indexArray;

	mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, meshArray, Array(), Dictionary(),
		Mesh::ARRAY_FORMAT_VERTEX | Mesh::ARRAY_FORMAT_NORMAL | Mesh::ARRAY_FORMAT_COLOR |
		Mesh::ARRAY_FORMAT_TEX_UV | Mesh::ARRAY_FORMAT_TEX_UV2 |
		Mesh::ARRAY_FORMAT_CUSTOM0 | Mesh::ARRAY_FORMAT_CUSTOM1 |
		Mesh::ARRAY_FORMAT_INDEX |
		(Mesh::ARRAY_CUSTOM_RGBA_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT) |
		(Mesh::ARRAY_CUSTOM_RGBA_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM1_SHIFT));
	mesh->surface_set_material(0, meshInstance.material);
}

void PixelpartEffect::add_particle_trails(ParticleMeshInstance& meshInstance, const pixelpart::ParticleType& particleType, const pixelpart::ParticleData& particles, uint32_t numParticles, pixelpart::floatd scale) {
	const pixelpart::floatd packFactor = 10000.0;

	if(numParticles < 2) {
		return;
	}

	std::vector<uint32_t> sortKeys(numParticles);
	std::iota(sortKeys.begin(), sortKeys.end(), 0);
	std::sort(sortKeys.begin(), sortKeys.end(), [&particles](uint32_t p0, uint32_t p1) {
		return (particles.parentId[p0] != particles.parentId[p1])
			? particles.parentId[p0] > particles.parentId[p1]
			: particles.id[p0] > particles.id[p1];
	});

	for(auto& entry : meshInstance.trails) {
		entry.second.numParticles = 0;
	}

	ParticleMeshInstance::ParticleTrail* trail = nullptr;
	uint32_t trailId = 0xFFFFFFFF - 1;
	uint32_t particleParentId = 0;

	for(uint32_t p = 0; p < numParticles; p++) {
		particleParentId = particles.parentId[sortKeys[p]];

		if(particleParentId != trailId) {
			trailId = particleParentId;

			if(meshInstance.trails.count(trailId) == 0) {
				meshInstance.trails[trailId] = ParticleMeshInstance::ParticleTrail();
			}

			trail = &meshInstance.trails.at(trailId);
		}

		trail->numParticles++;
	}

	for(auto it = meshInstance.trails.begin(); it != meshInstance.trails.end(); ) {
		if(it->second.numParticles == 0) {
			it = meshInstance.trails.erase(it);
		}
		else {
			it++;
		}
	}

	trail = nullptr;
	trailId = 0xFFFFFFFF - 1;
	particleParentId = 0;

	if(particleType.trailRendererSettings.numTrailSegments > 0) {
		for(uint32_t p = 0; p < numParticles; p++) {
			uint32_t particleIndex = sortKeys[p];
			particleParentId = particles.parentId[particleIndex];

			if(particleParentId != trailId) {
				trailId = particleParentId;
				trail = &meshInstance.trails.at(trailId);

				trail->length = 0.0;
				std::vector<pixelpart::floatd> relativeParticlePosition(trail->numParticles, 0.0);
				for(uint32_t q = 1; q < trail->numParticles; q++) {
					trail->length += glm::length(particles.globalPosition[sortKeys[p + q]] - particles.globalPosition[sortKeys[p + q - 1]]);
					relativeParticlePosition[q] = trail->length;
				}
				for(uint32_t q = 0; q < trail->numParticles; q++) {
					relativeParticlePosition[q] /= trail->length;
				}

				pixelpart::Curve<pixelpart::vec3d> positionCurve(pixelpart::CurveInterpolation::spline);
				positionCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
				positionCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.globalPosition.data(), sortKeys.data() + p, trail->numParticles);

				pixelpart::Curve<pixelpart::vec3d> sizeCurve(pixelpart::CurveInterpolation::spline);
				sizeCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
				sizeCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.size.data(), sortKeys.data() + p, trail->numParticles);

				pixelpart::Curve<pixelpart::vec4d> colorCurve(pixelpart::CurveInterpolation::spline);
				colorCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
				colorCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.color.data(), sortKeys.data() + p, trail->numParticles);

				pixelpart::Curve<pixelpart::vec3d> velocityCurve(pixelpart::CurveInterpolation::spline);
				velocityCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
				velocityCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.velocity.data(), sortKeys.data() + p, trail->numParticles);

				pixelpart::Curve<pixelpart::vec3d> forceCurve(pixelpart::CurveInterpolation::spline);
				forceCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
				forceCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.force.data(), sortKeys.data() + p, trail->numParticles);

				pixelpart::Curve<pixelpart::floatd> lifeCurve(pixelpart::CurveInterpolation::spline);
				lifeCurve.enableFixedCache(particleType.trailRendererSettings.numTrailSegments);
				lifeCurve.setPointsOrdered<uint32_t>(relativeParticlePosition.data(), particles.life.data(), sortKeys.data() + p, trail->numParticles);

				trail->position = positionCurve.getCache();
				trail->size = sizeCurve.getCache();
				trail->color = colorCurve.getCache();
				trail->velocity = velocityCurve.getCache();
				trail->force = forceCurve.getCache();
				trail->direction.resize(particleType.trailRendererSettings.numTrailSegments);
				trail->directionToEdge.resize(particleType.trailRendererSettings.numTrailSegments);
				trail->index.resize(particleType.trailRendererSettings.numTrailSegments);
				trail->life = lifeCurve.getCache();
			}
		}
	}
	else {
		for(uint32_t p = 0, q = 0; p < numParticles; p++, q++) {
			uint32_t particleIndex = sortKeys[p];
			particleParentId = particles.parentId[particleIndex];

			if(particleParentId != trailId) {
				trailId = particleParentId;
				q = 0;

				trail = &meshInstance.trails.at(trailId);
				trail->position.resize(trail->numParticles);
				trail->size.resize(trail->numParticles);
				trail->color.resize(trail->numParticles);
				trail->velocity.resize(trail->numParticles);
				trail->force.resize(trail->numParticles);
				trail->direction.resize(trail->numParticles);
				trail->directionToEdge.resize(trail->numParticles);
				trail->index.resize(trail->numParticles);
				trail->life.resize(trail->numParticles);
			}

			trail->position[q] = particles.globalPosition[particleIndex];
			trail->size[q] = particles.size[particleIndex];
			trail->color[q] = particles.color[particleIndex];
			trail->velocity[q] = particles.velocity[particleIndex];
			trail->force[q] = particles.force[particleIndex];
			trail->life[q] = particles.life[particleIndex];
		}
	}

	int64_t numTrailParticles = 0;
	for(auto& entry : meshInstance.trails) {
		ParticleMeshInstance::ParticleTrail& trail = entry.second;
		if(trail.numParticles < 2) {
			continue;
		}

		numTrailParticles += static_cast<int64_t>(trail.numParticles) - 1;
	}

	if(numTrailParticles < 1) {
		return;
	}

	Ref<ArrayMesh> mesh = meshInstance.mesh;

	PackedInt32Array indexArray;
	PackedVector3Array vertexArray;
	PackedVector3Array normalArray;
	PackedVector2Array uvArray;
	PackedVector2Array uv2Array;
	PackedColorArray colorArray;
	PackedFloat32Array custom0Array;
	PackedFloat32Array custom1Array;
	indexArray.resize(numTrailParticles * 6);
	vertexArray.resize(numTrailParticles * 4);
	normalArray.resize(numTrailParticles * 4);
	uvArray.resize(numTrailParticles * 4);
	uv2Array.resize(numTrailParticles * 4);
	colorArray.resize(numTrailParticles * 4);
	custom0Array.resize(numTrailParticles * 4 * 4);
	custom1Array.resize(numTrailParticles * 4 * 4);

	int64_t vertexIndex = 0;
	int32_t* indices = indexArray.ptrw();
	Vector3* positions = vertexArray.ptrw();
	Vector3* normals = normalArray.ptrw();
	Color* colors = colorArray.ptrw();
	Vector2* uvs = uvArray.ptrw();
	Vector2* uvs2 = uv2Array.ptrw();
	float* custom0 = custom0Array.ptrw();
	float* custom1 = custom1Array.ptrw();

	for(auto& entry : meshInstance.trails) {
		ParticleMeshInstance::ParticleTrail& trail = entry.second;
		trail.length = 0.0;

		if(trail.numParticles < 2) {
			continue;
		}

		std::size_t last = trail.numParticles - 1;
		for(std::size_t i = 0; i < last; i++) {
			pixelpart::vec3d toNext = trail.position[i + 1] - trail.position[i];
			pixelpart::floatd distance = std::max(glm::length(toNext), 0.000001);

			trail.direction[i] = toNext / distance;
			trail.index[i] = trail.length;
			trail.length += distance;
		}

		trail.direction[last] = trail.direction[last - 1];
		trail.index[last] = trail.length;

		trail.directionToEdge[0] = pixelpart::vec3d(-trail.direction[0].y, trail.direction[0].x, trail.direction[0].z);

		for(std::size_t i = last; i > 0; i--) {
			pixelpart::vec3d h = trail.direction[i] - trail.direction[i - 1];
			pixelpart::floatd l = glm::length(h);
			if(l < 0.0001) {
				trail.directionToEdge[i] = pixelpart::vec3d(
					-trail.direction[i].y,
					trail.direction[i].x,
					trail.direction[i].z);
			}
			else {
				trail.directionToEdge[i] = (glm::dot(glm::cross(trail.direction[i], pixelpart::vec3d(0.0, 0.0, 1.0)), h / l) < 0.0)
					? +h / l
					: -h / l;
			}
		}

		for(pixelpart::floatd& x : trail.index) {
			x /= trail.length;
		}

		for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
			pixelpart::vec3d n0 = trail.directionToEdge[p] * std::max(trail.size[p].x, trail.size[p].y) * 0.5;
			pixelpart::vec3d n1 = trail.directionToEdge[p + 1] * std::max(trail.size[p + 1].x, trail.size[p + 1].y) * 0.5;
			pixelpart::vec3d p0 = (trail.position[p] + n0) * scale;
			pixelpart::vec3d p1 = (trail.position[p] - n0) * scale;
			pixelpart::vec3d p2 = (trail.position[p + 1] + n1) * scale;
			pixelpart::vec3d p3 = (trail.position[p + 1] - n1) * scale;
			pixelpart::vec3d normal = glm::cross(trail.direction[p], trail.directionToEdge[p]);
			pixelpart::vec2d uv0, uv1, uv2, uv3;

			switch(particleType.trailRendererSettings.textureRotation) {
				case 1:
					uv0 = pixelpart::vec2d(trail.index[p] * particleType.trailRendererSettings.textureUVFactor, 1.0);
					uv1 = pixelpart::vec2d(trail.index[p] * particleType.trailRendererSettings.textureUVFactor, 0.0);
					uv2 = pixelpart::vec2d(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor, 1.0);
					uv3 = pixelpart::vec2d(trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor, 0.0);
					break;

				case 2:
					uv0 = pixelpart::vec2d(1.0, 1.0 - trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					uv1 = pixelpart::vec2d(0.0, 1.0 - trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					uv2 = pixelpart::vec2d(1.0, 1.0 - trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					uv3 = pixelpart::vec2d(0.0, 1.0 - trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					break;

				case 3: {
					uv0 = pixelpart::vec2d(1.0 - trail.index[p] * particleType.trailRendererSettings.textureUVFactor, 0.0);
					uv1 = pixelpart::vec2d(1.0 - trail.index[p] * particleType.trailRendererSettings.textureUVFactor, 1.0);
					uv2 = pixelpart::vec2d(1.0 - trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor, 0.0);
					uv3 = pixelpart::vec2d(1.0 - trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor, 1.0);
					break;
				}

				default: {
					uv0 = pixelpart::vec2d(0.0, trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					uv1 = pixelpart::vec2d(1.0, trail.index[p] * particleType.trailRendererSettings.textureUVFactor);
					uv2 = pixelpart::vec2d(0.0, trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					uv3 = pixelpart::vec2d(1.0, trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor);
					break;
				}
			}

			indices[vertexIndex * 6 + 0] = vertexIndex * 4 + 0;
			indices[vertexIndex * 6 + 1] = vertexIndex * 4 + 2;
			indices[vertexIndex * 6 + 2] = vertexIndex * 4 + 1;
			indices[vertexIndex * 6 + 3] = vertexIndex * 4 + 2;
			indices[vertexIndex * 6 + 4] = vertexIndex * 4 + 3;
			indices[vertexIndex * 6 + 5] = vertexIndex * 4 + 1;

			positions[vertexIndex * 4 + 0] = toGd(p0);
			positions[vertexIndex * 4 + 1] = toGd(p1);
			positions[vertexIndex * 4 + 2] = toGd(p2);
			positions[vertexIndex * 4 + 3] = toGd(p3);

			normals[vertexIndex * 4 + 0] = toGd(normal);
			normals[vertexIndex * 4 + 1] = toGd(normal);
			normals[vertexIndex * 4 + 2] = toGd(normal);
			normals[vertexIndex * 4 + 3] = toGd(normal);

			colors[vertexIndex * 4 + 0] = toGd(trail.color[p]);
			colors[vertexIndex * 4 + 1] = toGd(trail.color[p]);
			colors[vertexIndex * 4 + 2] = toGd(trail.color[p + 1]);
			colors[vertexIndex * 4 + 3] = toGd(trail.color[p + 1]);

			uvs[vertexIndex * 4 + 0] = toGd(uv0);
			uvs[vertexIndex * 4 + 1] = toGd(uv1);
			uvs[vertexIndex * 4 + 2] = toGd(uv2);
			uvs[vertexIndex * 4 + 3] = toGd(uv3);

			uvs2[vertexIndex * 4 + 0] = Vector2(static_cast<float>(trail.life[p]), static_cast<float>(entry.first));
			uvs2[vertexIndex * 4 + 1] = Vector2(static_cast<float>(trail.life[p]), static_cast<float>(entry.first));
			uvs2[vertexIndex * 4 + 2] = Vector2(static_cast<float>(trail.life[p + 1]), static_cast<float>(entry.first));
			uvs2[vertexIndex * 4 + 3] = Vector2(static_cast<float>(trail.life[p + 1]), static_cast<float>(entry.first));

			custom0[vertexIndex * 4 * 4 + 0] = static_cast<float>(trail.velocity[p].x);
			custom0[vertexIndex * 4 * 4 + 1] = static_cast<float>(trail.velocity[p].y);
			custom0[vertexIndex * 4 * 4 + 2] = static_cast<float>(trail.velocity[p].z);
			custom0[vertexIndex * 4 * 4 + 3] = 0.0f;
			custom0[vertexIndex * 4 * 4 + 4] = static_cast<float>(trail.velocity[p].x);
			custom0[vertexIndex * 4 * 4 + 5] = static_cast<float>(trail.velocity[p].y);
			custom0[vertexIndex * 4 * 4 + 6] = static_cast<float>(trail.velocity[p].z);
			custom0[vertexIndex * 4 * 4 + 7] = 0.0f;
			custom0[vertexIndex * 4 * 4 + 8] = static_cast<float>(trail.velocity[p + 1].x);
			custom0[vertexIndex * 4 * 4 + 9] = static_cast<float>(trail.velocity[p + 1].y);
			custom0[vertexIndex * 4 * 4 + 10] = static_cast<float>(trail.velocity[p + 1].z);
			custom0[vertexIndex * 4 * 4 + 11] = 0.0f;
			custom0[vertexIndex * 4 * 4 + 12] = static_cast<float>(trail.velocity[p + 1].x);
			custom0[vertexIndex * 4 * 4 + 13] = static_cast<float>(trail.velocity[p + 1].y);
			custom0[vertexIndex * 4 * 4 + 14] = static_cast<float>(trail.velocity[p + 1].z);
			custom0[vertexIndex * 4 * 4 + 15] = 0.0f;

			custom1[vertexIndex * 4 * 4 + 0] = static_cast<float>(trail.force[p].x);
			custom1[vertexIndex * 4 * 4 + 1] = static_cast<float>(trail.force[p].y);
			custom1[vertexIndex * 4 * 4 + 2] = static_cast<float>(trail.force[p].z);
			custom1[vertexIndex * 4 * 4 + 3] = 0.0f;
			custom1[vertexIndex * 4 * 4 + 4] = static_cast<float>(trail.force[p].x);
			custom1[vertexIndex * 4 * 4 + 5] = static_cast<float>(trail.force[p].y);
			custom1[vertexIndex * 4 * 4 + 6] = static_cast<float>(trail.force[p].z);
			custom1[vertexIndex * 4 * 4 + 7] = 0.0f;
			custom1[vertexIndex * 4 * 4 + 8] = static_cast<float>(trail.force[p + 1].x);
			custom1[vertexIndex * 4 * 4 + 9] = static_cast<float>(trail.force[p + 1].y);
			custom1[vertexIndex * 4 * 4 + 10] = static_cast<float>(trail.force[p + 1].z);
			custom1[vertexIndex * 4 * 4 + 11] = 0.0f;
			custom1[vertexIndex * 4 * 4 + 12] = static_cast<float>(trail.force[p + 1].x);
			custom1[vertexIndex * 4 * 4 + 13] = static_cast<float>(trail.force[p + 1].y);
			custom1[vertexIndex * 4 * 4 + 14] = static_cast<float>(trail.force[p + 1].z);
			custom1[vertexIndex * 4 * 4 + 15] = 0.0f;

			vertexIndex++;
		}
	}

	Array meshArray;
	meshArray.resize(Mesh::ARRAY_MAX);
	meshArray[Mesh::ARRAY_VERTEX] = vertexArray;
	meshArray[Mesh::ARRAY_NORMAL] = normalArray;
	meshArray[Mesh::ARRAY_COLOR] = colorArray;
	meshArray[Mesh::ARRAY_TEX_UV] = uvArray;
	meshArray[Mesh::ARRAY_TEX_UV2] = uv2Array;
	meshArray[Mesh::ARRAY_CUSTOM0] = custom0Array;
	meshArray[Mesh::ARRAY_CUSTOM1] = custom1Array;
	meshArray[Mesh::ARRAY_INDEX] = indexArray;

	mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, meshArray, Array(), Dictionary(),
		Mesh::ARRAY_FORMAT_VERTEX | Mesh::ARRAY_FORMAT_NORMAL | Mesh::ARRAY_FORMAT_COLOR |
		Mesh::ARRAY_FORMAT_TEX_UV | Mesh::ARRAY_FORMAT_TEX_UV2 |
		Mesh::ARRAY_FORMAT_CUSTOM0 | Mesh::ARRAY_FORMAT_CUSTOM1 |
		Mesh::ARRAY_FORMAT_INDEX |
		(Mesh::ARRAY_CUSTOM_RGBA_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT) |
		(Mesh::ARRAY_CUSTOM_RGBA_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM1_SHIFT));
	mesh->surface_set_material(0, meshInstance.material);
}

pixelpart::mat3d PixelpartEffect::rotation3d(const pixelpart::vec3d& angle) {
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
pixelpart::mat3d PixelpartEffect::lookAt(const pixelpart::vec3d& direction) {
	pixelpart::vec3d up = pixelpart::vec3d(0.0, 1.0, 0.0);
	pixelpart::vec3d front = glm::normalize(direction);
	pixelpart::vec3d right = glm::normalize(glm::cross(front, up));
	up = glm::normalize(glm::cross(right, front));

	return pixelpart::mat3d(right, up, front);
}
}