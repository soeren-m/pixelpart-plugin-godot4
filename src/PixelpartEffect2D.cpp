#include "PixelpartEffect2D.h"
#include "PixelpartUtil.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {
void PixelpartEffect2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("draw"), &PixelpartEffect2D::draw);
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
	ClassDB::bind_method(D_METHOD("set_light_mode", "mode"), &PixelpartEffect2D::set_light_mode);
	ClassDB::bind_method(D_METHOD("get_light_mode"), &PixelpartEffect2D::get_light_mode);
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

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "effect"), "set_effect", "get_effect");

	ADD_GROUP("Playback", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "play", "is_playing");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "loop"), "set_loop", "get_loop");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "loop_time", PROPERTY_HINT_RANGE, "0.0,1000.0,0.01"), "set_loop_time", "get_loop_time");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0.0,100.0,0.01"), "set_speed", "get_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "frame_rate", PROPERTY_HINT_RANGE, "1.0,100.0,1.0"), "set_frame_rate", "get_frame_rate");

	ADD_GROUP("Shading", "");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "light_mode", PROPERTY_HINT_ENUM, "Normal,Unshaded,Light-Only"), "set_light_mode", "get_light_mode");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_h"), "set_flip_h", "get_flip_h");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_v"), "set_flip_v", "get_flip_v");
}

PixelpartEffect2D::PixelpartEffect2D() {
	auto settings = ProjectSettings::get_singleton();

	uint32_t particleCapacity = 10000;
	if(settings->has_setting("pixelpart/particle_capacity")) {
		particleCapacity = static_cast<uint32_t>(std::max((int)settings->get_setting("pixelpart/particle_capacity"), 1));
	}

	particleEngine = std::make_unique<pixelpart::ParticleEngine>(nullptr, particleCapacity);
}
PixelpartEffect2D::~PixelpartEffect2D() {
	RenderingServer* rs = RenderingServer::get_singleton();

	for(ParticleMeshInstance& inst : particleMeshInstances) {
		rs->free_rid(inst.canvasItem);
		rs->free_rid(inst.material);
	}
}

void PixelpartEffect2D::_enter_tree() {
	if(!Engine::get_singleton()->is_editor_hint()) {
		RenderingServer::get_singleton()->connect("frame_pre_draw", Callable(this, "draw"));
	}
}
void PixelpartEffect2D::_exit_tree() {
	if(!Engine::get_singleton()->is_editor_hint()) {
		RenderingServer::get_singleton()->disconnect("frame_pre_draw", Callable(this, "draw"));
	}
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
}

void PixelpartEffect2D::draw() {
	Viewport* viewport = get_viewport();
	if(!particleEngine || !viewport) {
		return;
	}

	const pixelpart::Effect* effect = particleEngine->getEffect();
	if(!effect) {
		return;
	}

	if(shaderDirty) {
		for(uint32_t i = 0; i < nativeEffect.particleTypes.getCount(); i++) {
			const pixelpart::ParticleType& particleType = nativeEffect.particleTypes.getByIndex(i);
			const pixelpart::ShaderGraph::BuildResult& shaderBuildResult = particleTypes[particleType.name]->get_shader_build_result();

			particleMeshInstances[i].shader = PixelpartShaders::get_instance()->get_canvas_shader(shaderBuildResult.code,
				particleType.blendMode, lightMode);
		}

		shaderDirty = false;
	}

	std::vector<uint32_t> particleTypeIndicesSortedByLayer = effect->particleTypes.getSortedIndices(
		[](const pixelpart::ParticleType& t1, const pixelpart::ParticleType& t2) {
			return t1.layer < t2.layer;
		});

	for(uint32_t particleTypeIndex : particleTypeIndicesSortedByLayer) {
		if(particleTypeIndex >= particleMeshInstances.size()) {
			continue;
		}

		draw_particles(effect->particleTypes.getByIndex(particleTypeIndex), particleMeshInstances[particleTypeIndex]);
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

void PixelpartEffect2D::set_light_mode(CanvasItemMaterial::LightMode mode) {
	shaderDirty |= mode != lightMode;
	lightMode = mode;
}
CanvasItemMaterial::LightMode PixelpartEffect2D::get_light_mode() const {
	return lightMode;
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

	for(ParticleMeshInstance& inst : particleMeshInstances) {
		rs->free_rid(inst.canvasItem);
		rs->free_rid(inst.material);
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

			for(const pixelpart::ParticleType& particleType : nativeEffect.particleTypes) {
				const pixelpart::ShaderGraph::BuildResult& shaderBuildResult = particleTypes.at(particleType.name)->get_shader_build_result();
				Ref<Shader> shader = shaders->get_canvas_shader(shaderBuildResult.code,
					particleType.blendMode, lightMode);

				particleMeshInstances.push_back(ParticleMeshInstance{
					rs->canvas_item_create(),
					rs->material_create(),
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

void PixelpartEffect2D::draw_particles(const pixelpart::ParticleType& particleType, ParticleMeshInstance& meshInstance) {
	RenderingServer* rs = RenderingServer::get_singleton();
	rs->canvas_item_clear(meshInstance.canvasItem);

	if(!is_visible() || !particleType.visible) {
		return;
	}

	const pixelpart::Effect* effect = particleEngine->getEffect();
	const pixelpart::ParticleEmitter& particleEmitter = effect->particleEmitters.get(particleType.parentId);
	const uint32_t particleTypeIndex = effect->particleTypes.findById(particleType.id);
	const uint32_t numParticles = particleEngine->getNumParticles(particleTypeIndex);
	const pixelpart::ParticleData& particles = particleEngine->getParticles(particleTypeIndex);
	const pixelpart::floatd scaleX = static_cast<pixelpart::floatd>(get_import_scale()) * (flipH ? -1.0 : +1.0);
	const pixelpart::floatd scaleY = static_cast<pixelpart::floatd>(get_import_scale()) * (flipV ? -1.0 : +1.0);

	rs->material_set_shader(meshInstance.material, meshInstance.shader->get_rid());
	rs->material_set_param(meshInstance.material, "EFFECT_TIME", static_cast<float>(particleEngine->getTime()));
	rs->material_set_param(meshInstance.material, "OBJECT_TIME", static_cast<float>(particleEngine->getTime() - particleEmitter.lifetimeStart));
	for(std::size_t t = 0; t < meshInstance.textures.size(); t++) {
		String samplerName = String("TEXTURE") + String::num_int64(t);
		Ref<ImageTexture> texture = textures.at(meshInstance.textures[t]);

		rs->material_set_param(meshInstance.material, samplerName, texture->get_rid());
	}

	rs->canvas_item_set_parent(meshInstance.canvasItem, get_canvas_item());
	rs->canvas_item_set_transform(meshInstance.canvasItem, Transform2D());
	rs->canvas_item_set_material(meshInstance.canvasItem, meshInstance.material);

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

	if(numParticles == 0) {
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
				worldPosition[1] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(+0.5, -0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[2] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(+0.5, +0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[3] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(-0.5, +0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				break;
			}
			case pixelpart::AlignmentMode::emission: {
				pixelpart::vec3d emissionDirection = particleEmitter.position.get(alpha) - particles.globalPosition[p];
				pixelpart::floatd angle = glm::degrees(glm::orientedAngle(pixelpart::vec2d(0.0, 1.0), (emissionDirection != pixelpart::vec3d(0.0))
						? pixelpart::vec2d(glm::normalize(emissionDirection))
						: pixelpart::vec2d(0.0, 1.0)));
				worldPosition[0] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(-0.5, -0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[1] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(+0.5, -0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[2] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(+0.5, +0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				worldPosition[3] = particles.globalPosition[p] + rotate2d(pixelpart::vec3d(-0.5, +0.5, 0.0) * particles.size[p], particleType.pivot * particles.size[p], particles.rotation[p].x + angle);
				break;
			}
			case pixelpart::AlignmentMode::emitter: {
				pixelpart::mat3d rotationMatrix = rotation3d(particles.rotation[p]);
				pixelpart::mat3d lookAtMatrix = rotation3d(particleEmitter.orientation.get(alpha));
				worldPosition[0] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3d(-0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				worldPosition[1] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3d(+0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				worldPosition[2] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3d(+0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				worldPosition[3] = particles.globalPosition[p] + lookAtMatrix * (rotationMatrix * (pixelpart::vec3d(-0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				break;
			}
			default: {
				pixelpart::mat3d rotationMatrix = rotation3d(particles.rotation[p]);
				worldPosition[0] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3d(-0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				worldPosition[1] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3d(+0.5, -0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				worldPosition[2] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3d(+0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
				worldPosition[3] = particles.globalPosition[p] + (rotationMatrix * (pixelpart::vec3d(-0.5, +0.5, 0.0) * particles.size[p] - particleType.pivot * particles.size[p]) + particleType.pivot * particles.size[p]);
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
		textureCoords[p * 4 * 2 + 2] = 1.0f / 10.0f + packedLifeData;
		textureCoords[p * 4 * 2 + 3] = 0.0f + packedIdData;
		textureCoords[p * 4 * 2 + 4] = 1.0f / 10.0f + packedLifeData;
		textureCoords[p * 4 * 2 + 5] = 1.0f / 10.0f + packedIdData;
		textureCoords[p * 4 * 2 + 6] = 0.0f + packedLifeData;
		textureCoords[p * 4 * 2 + 7] = 1.0f / 10.0f + packedIdData;
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
	rs->canvas_item_add_triangle_array(meshInstance.canvasItem,
		indexArray,
		positionArray,
		colorArray,
		textureCoordArray);
}

void PixelpartEffect2D::add_particle_trails(ParticleMeshInstance& meshInstance, const pixelpart::ParticleType& particleType, const pixelpart::ParticleData& particles, uint32_t numParticles, pixelpart::floatd scaleX, pixelpart::floatd scaleY) {
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

	RenderingServer* rs = RenderingServer::get_singleton();

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

		trail.direction[0] = pixelpart::vec3d(-trail.direction[0].y, trail.direction[0].x, trail.direction[0].z);

		for(std::size_t i = last; i > 0; i--) {
			pixelpart::vec3d h = trail.direction[i] - trail.direction[i - 1];
			pixelpart::floatd l = glm::length(h);
			if(l < 0.0001) {
				trail.direction[i] = pixelpart::vec3d(
					-trail.direction[i].y,
					trail.direction[i].x,
					trail.direction[i].z);
			}
			else {
				trail.direction[i] = (glm::dot(glm::cross(trail.direction[i], pixelpart::vec3d(0.0, 0.0, 1.0)), h / l) < 0.0)
					? +h / l
					: -h / l;
			}
		}

		for(pixelpart::floatd& x : trail.index) {
			x /= trail.length;
		}

		trail.indexArray.resize((trail.numParticles - 1) * 6);
		trail.positionArray.resize((trail.numParticles - 1) * 4);
		trail.textureCoordArray.resize((trail.numParticles - 1) * 4);
		trail.colorArray.resize((trail.numParticles - 1) * 4);

		int* indices = trail.indexArray.ptrw();
		float* positions = reinterpret_cast<float*>(trail.positionArray.ptrw());
		float* textureCoords = reinterpret_cast<float*>(trail.textureCoordArray.ptrw());
		float* colors = reinterpret_cast<float*>(trail.colorArray.ptrw());

		for(int p = 0; p < static_cast<int>(trail.numParticles) - 1; p++) {
			indices[p * 6 + 0] = p * 4 + 0;
			indices[p * 6 + 1] = p * 4 + 2;
			indices[p * 6 + 2] = p * 4 + 1;
			indices[p * 6 + 3] = p * 4 + 2;
			indices[p * 6 + 4] = p * 4 + 3;
			indices[p * 6 + 5] = p * 4 + 1;
		}

		for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
			pixelpart::vec3d n0 = trail.direction[p] * std::max(trail.size[p].x, trail.size[p].y) * 0.5;
			pixelpart::vec3d n1 = trail.direction[p + 1] * std::max(trail.size[p + 1].x, trail.size[p + 1].y) * 0.5;
			pixelpart::vec3d p0 = trail.position[p] + n0;
			pixelpart::vec3d p1 = trail.position[p] - n0;
			pixelpart::vec3d p2 = trail.position[p + 1] + n1;
			pixelpart::vec3d p3 = trail.position[p + 1] - n1;

			positions[p * 4 * 2 + 0] = p0.x * scaleX;
			positions[p * 4 * 2 + 1] = p0.y * scaleY;
			positions[p * 4 * 2 + 2] = p1.x * scaleX;
			positions[p * 4 * 2 + 3] = p1.y * scaleY;
			positions[p * 4 * 2 + 4] = p2.x * scaleX;
			positions[p * 4 * 2 + 5] = p2.y * scaleY;
			positions[p * 4 * 2 + 6] = p3.x * scaleX;
			positions[p * 4 * 2 + 7] = p3.y * scaleY;
		}

		switch(particleType.trailRendererSettings.textureRotation) {
			case 1: {
				for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
					textureCoords[p * 4 * 2 + 0] = trail.index[p] * particleType.trailRendererSettings.textureUVFactor;
					textureCoords[p * 4 * 2 + 1] = 1.0f;
					textureCoords[p * 4 * 2 + 2] = trail.index[p] * particleType.trailRendererSettings.textureUVFactor;
					textureCoords[p * 4 * 2 + 3] = 0.0f;
					textureCoords[p * 4 * 2 + 4] = trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor;
					textureCoords[p * 4 * 2 + 5] = 1.0f;
					textureCoords[p * 4 * 2 + 6] = trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor;
					textureCoords[p * 4 * 2 + 7] = 0.0f;
				}

				break;
			}

			case 2: {
				for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
					textureCoords[p * 4 * 2 + 0] = 1.0f;
					textureCoords[p * 4 * 2 + 1] = 1.0f - trail.index[p] * particleType.trailRendererSettings.textureUVFactor;
					textureCoords[p * 4 * 2 + 2] = 0.0f;
					textureCoords[p * 4 * 2 + 3] = 1.0f - trail.index[p] * particleType.trailRendererSettings.textureUVFactor;
					textureCoords[p * 4 * 2 + 4] = 1.0f;
					textureCoords[p * 4 * 2 + 5] = 1.0f - trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor;
					textureCoords[p * 4 * 2 + 6] = 0.0f;
					textureCoords[p * 4 * 2 + 7] = 1.0f - trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor;
				}

				break;
			}

			case 3: {
				for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
					textureCoords[p * 4 * 2 + 0] = 1.0f - trail.index[p] * particleType.trailRendererSettings.textureUVFactor;
					textureCoords[p * 4 * 2 + 1] = 0.0f;
					textureCoords[p * 4 * 2 + 2] = 1.0f - trail.index[p] * particleType.trailRendererSettings.textureUVFactor;
					textureCoords[p * 4 * 2 + 3] = 1.0f;
					textureCoords[p * 4 * 2 + 4] = 1.0f - trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor;
					textureCoords[p * 4 * 2 + 5] = 0.0f;
					textureCoords[p * 4 * 2 + 6] = 1.0f - trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor;
					textureCoords[p * 4 * 2 + 7] = 1.0f;
				}

				break;
			}

			default: {
				for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
					textureCoords[p * 4 * 2 + 0] = 0.0f;
					textureCoords[p * 4 * 2 + 1] = trail.index[p] * particleType.trailRendererSettings.textureUVFactor;
					textureCoords[p * 4 * 2 + 2] = 1.0f;
					textureCoords[p * 4 * 2 + 3] = trail.index[p] * particleType.trailRendererSettings.textureUVFactor;
					textureCoords[p * 4 * 2 + 4] = 0.0f;
					textureCoords[p * 4 * 2 + 5] = trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor;
					textureCoords[p * 4 * 2 + 6] = 1.0f;
					textureCoords[p * 4 * 2 + 7] = trail.index[p + 1] * particleType.trailRendererSettings.textureUVFactor;
				}

				break;
			}
		}

		for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
			float packedLifeData = static_cast<float>(std::floor(trail.life[p] * packFactor) + packFactor);
			float packedIdData = static_cast<float>(entry.first + packFactor);

			textureCoords[p * 4 * 2 + 0] = textureCoords[p * 4 * 2 + 0] / 10.0f + packedLifeData;
			textureCoords[p * 4 * 2 + 1] = textureCoords[p * 4 * 2 + 1] / 10.0f + packedIdData;
			textureCoords[p * 4 * 2 + 2] = textureCoords[p * 4 * 2 + 2] / 10.0f + packedLifeData;
			textureCoords[p * 4 * 2 + 3] = textureCoords[p * 4 * 2 + 3] / 10.0f + packedIdData;
			textureCoords[p * 4 * 2 + 4] = textureCoords[p * 4 * 2 + 4] / 10.0f + packedLifeData;
			textureCoords[p * 4 * 2 + 5] = textureCoords[p * 4 * 2 + 5] / 10.0f + packedIdData;
			textureCoords[p * 4 * 2 + 6] = textureCoords[p * 4 * 2 + 6] / 10.0f + packedLifeData;
			textureCoords[p * 4 * 2 + 7] = textureCoords[p * 4 * 2 + 7] / 10.0f + packedIdData;
		}

		for(uint32_t p = 0; p < trail.numParticles - 1; p++) {
			float packedVelocityX = static_cast<float>(std::floor(trail.velocity[p].x * packFactor) + packFactor);
			float packedVelocityY = static_cast<float>(std::floor(trail.velocity[p].y * packFactor) + packFactor);
			float packedForceX = static_cast<float>(std::floor(trail.force[p].x * packFactor) + packFactor);
			float packedForceY = static_cast<float>(std::floor(trail.force[p].y * packFactor) + packFactor);

			colors[p * 4 * 4 + 0] = packedVelocityX + static_cast<float>(trail.color[p].r * 0.1);
			colors[p * 4 * 4 + 1] = packedVelocityY + static_cast<float>(trail.color[p].g * 0.1);
			colors[p * 4 * 4 + 2] = packedForceX + static_cast<float>(trail.color[p].b * 0.1);
			colors[p * 4 * 4 + 3] = packedForceY + static_cast<float>(trail.color[p].a * 0.1);
			colors[p * 4 * 4 + 4] = packedVelocityX + static_cast<float>(trail.color[p].r * 0.1);
			colors[p * 4 * 4 + 5] = packedVelocityY + static_cast<float>(trail.color[p].g * 0.1);
			colors[p * 4 * 4 + 6] = packedForceX + static_cast<float>(trail.color[p].b * 0.1);
			colors[p * 4 * 4 + 7] = packedForceY + static_cast<float>(trail.color[p].a * 0.1);
			colors[p * 4 * 4 + 8] = packedVelocityX + static_cast<float>(trail.color[p].r * 0.1);
			colors[p * 4 * 4 + 9] = packedVelocityY + static_cast<float>(trail.color[p].g * 0.1);
			colors[p * 4 * 4 + 10] = packedForceX + static_cast<float>(trail.color[p].b * 0.1);
			colors[p * 4 * 4 + 11] = packedForceY + static_cast<float>(trail.color[p].a * 0.1);
			colors[p * 4 * 4 + 12] = packedVelocityX + static_cast<float>(trail.color[p].r * 0.1);
			colors[p * 4 * 4 + 13] = packedVelocityY + static_cast<float>(trail.color[p].g * 0.1);
			colors[p * 4 * 4 + 14] = packedForceX + static_cast<float>(trail.color[p].b * 0.1);
			colors[p * 4 * 4 + 15] = packedForceY + static_cast<float>(trail.color[p].a * 0.1);
		}

		rs->canvas_item_add_triangle_array(meshInstance.canvasItem,
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