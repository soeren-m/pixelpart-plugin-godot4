#ifndef PIXELPART_EFFECT2D_H
#define PIXELPART_EFFECT2D_H

#include "PixelpartEffectResource.h"
#include "PixelpartParticleEmitter.h"
#include "PixelpartParticleType.h"
#include "PixelpartForceField.h"
#include "PixelpartCollider.h"
#include "ParticleEngine.h"
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/shader.hpp>

namespace godot {
class PixelpartEffect2D : public Node2D {
	GDCLASS(PixelpartEffect2D, Node2D)

public:
	static void _bind_methods();

	PixelpartEffect2D();
	~PixelpartEffect2D();

	virtual void _process(double dt) override;

	virtual void _draw() override;

	void play(bool p);
	void pause();
	void restart();
	void reset();
	bool is_playing() const;
	float get_time() const;

	void set_loop(bool l);
	bool get_loop() const;

	void set_loop_time(float l);
	float get_loop_time() const;

	void set_speed(float s);
	float get_speed() const;

	void set_frame_rate(float r);
	float get_frame_rate() const;

	void set_flip_h(bool flip);
	void set_flip_v(bool flip);
	bool get_flip_h() const;
	bool get_flip_v() const;

	float get_import_scale() const;

	void set_effect(Ref<PixelpartEffectResource> effectRes);
	Ref<PixelpartEffectResource> get_effect() const;

	Ref<PixelpartParticleEmitter> find_particle_emitter(String name) const;
	Ref<PixelpartParticleType> find_particle_type(String name) const;
	Ref<PixelpartForceField> find_force_field(String name) const;
	Ref<PixelpartCollider> find_collider(String name) const;
	Ref<PixelpartParticleEmitter> get_particle_emitter(int id) const;
	Ref<PixelpartParticleType> get_particle_type(int id) const;
	Ref<PixelpartForceField> get_force_field(int id) const;
	Ref<PixelpartCollider> get_collider(int id) const;
	Ref<PixelpartParticleEmitter> get_particle_emitter_at_index(int index) const;
	Ref<PixelpartParticleType> get_particle_type_at_index(int index) const;
	Ref<PixelpartForceField> get_force_field_at_index(int index) const;
	Ref<PixelpartCollider> get_collider_at_index(int index) const;

private:
	class ParticleMeshInstance {
	public:
		struct ParticleTrail {
			uint32_t numParticles = 0u;
			pixelpart::floatd length = 0.0;

			std::vector<pixelpart::vec3d> position;
			std::vector<pixelpart::vec3d> size;
			std::vector<pixelpart::vec4d> color;
			std::vector<pixelpart::vec3d> velocity;
			std::vector<pixelpart::vec3d> force;
			std::vector<pixelpart::floatd> life;
			std::vector<pixelpart::vec3d> direction;
			std::vector<pixelpart::floatd> index;

			PackedInt32Array indexArray;
			PackedVector2Array positionArray;
			PackedVector2Array textureCoordArray;
			PackedColorArray colorArray;
		};

		ParticleMeshInstance(const pixelpart::ParticleType& particleType);
		ParticleMeshInstance(const ParticleMeshInstance&) = delete;
		~ParticleMeshInstance();

		ParticleMeshInstance& operator=(const ParticleMeshInstance&) = delete;

		void update_shader(const pixelpart::ParticleType& particleType);

		Ref<Shader> get_shader() const;
		RID get_material_rid() const;
		RID get_canvas_item_rid() const;

		std::string get_texture_id(std::size_t index) const;
		std::size_t get_texture_count() const;

		std::unordered_map<uint32_t, ParticleTrail>& get_trails();

	private:
		Ref<Shader> shader;
		RID materialRID;
		RID canvasItemRID;

		pixelpart::ShaderGraph::BuildResult shaderBuildResult;

		std::unordered_map<uint32_t, ParticleTrail> trails;
	};

	void draw_particles(uint32_t particleTypeIndex);

	void add_particle_mesh(ParticleMeshInstance& meshInstance, const pixelpart::ParticleType& particleType, const pixelpart::ParticleData& particles, uint32_t numParticles, pixelpart::floatd scaleX, pixelpart::floatd scaleY);
	void add_particle_sprites(ParticleMeshInstance& meshInstance, const pixelpart::ParticleType& particleType, const pixelpart::ParticleData& particles, uint32_t numParticles, pixelpart::floatd scaleX, pixelpart::floatd scaleY);
	void add_particle_trails(ParticleMeshInstance& meshInstance, const pixelpart::ParticleType& particleType, const pixelpart::ParticleData& particles, uint32_t numParticles, pixelpart::floatd scaleX, pixelpart::floatd scaleY);

	pixelpart::vec3d rotate2d(const pixelpart::vec3d& p, const pixelpart::vec3d& o, pixelpart::floatd a);
	pixelpart::mat3d rotation3d(const pixelpart::vec3d& angle);
	pixelpart::mat3d lookAt(const pixelpart::vec3d& direction);

	Ref<PixelpartEffectResource> effectResource;
	pixelpart::Effect nativeEffect;

	std::unordered_map<std::string, Ref<PixelpartParticleEmitter>> particleEmitters;
	std::unordered_map<std::string, Ref<PixelpartParticleType>> particleTypes;
	std::unordered_map<std::string, Ref<PixelpartForceField>> forceFields;
	std::unordered_map<std::string, Ref<PixelpartCollider>> colliders;

	std::unique_ptr<pixelpart::ParticleEngine> particleEngine;
	float simulationTime = 0.0f;

	bool playing = true;
	bool loop = false;
	float loopTime = 1.0f;
	float speed = 1.0f;
	float timeStep = 1.0f / 60.0f;

	bool flipH = false;
	bool flipV = true;

	std::vector<std::unique_ptr<ParticleMeshInstance>> particleMeshInstances;
	std::unordered_map<std::string, Ref<ImageTexture>> textures;
	bool shaderDirty = false;
};
}

#endif