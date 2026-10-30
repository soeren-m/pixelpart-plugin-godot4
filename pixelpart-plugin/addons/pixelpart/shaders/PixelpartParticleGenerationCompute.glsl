#[compute]
#version 460

layout (local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout (rgba32f, set = 0, binding = 0) uniform writeonly image2D u_ParticleCommonTexture;
layout (rgba32f, set = 0, binding = 1) uniform writeonly image2D u_ParticlePositionTexture;
layout (rgba32f, set = 0, binding = 2) uniform writeonly image2D u_ParticleGlobalPositionTexture;
layout (rgba32f, set = 0, binding = 3) uniform writeonly image2D u_ParticleVelocityTexture;
layout (rgba32f, set = 0, binding = 4) uniform writeonly image2D u_ParticleForceTexture;
layout (rgba32f, set = 0, binding = 5) uniform writeonly image2D u_ParticleRotationTexture;
layout (rgba32f, set = 0, binding = 6) uniform writeonly image2D u_ParticleSizeTexture;
layout (rgba32f, set = 0, binding = 7) uniform writeonly image2D u_ParticleColorTexture;

// TODO: push constants?
layout (std140, set = 1, binding = 0) uniform GenerationParams {
	float deltaTime;
	uint particleCount;
	uint outTextureWidth;
	uint outTextureHeight;
} u_GenerationParams;

ivec2 toTextureIndex(uint index) {
	return ivec2(
		index % u_GenerationParams.outTextureWidth,
		index / u_GenerationParams.outTextureHeight);
}

void main() {
	uint p = gl_GlobalInvocationID.x;
	if(p >= u_GenerationParams.particleCount) {
		return;
	}

	ivec2 textureIndex = toTextureIndex(p);

	vec4 commonData = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 position = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 velocity = vec4(0.1, 0.0, 0.0, 1.0);
	vec4 force = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 size = vec4(0.1, 0.1, 0.1, 1.0);
	// TODO

	imageStore(u_ParticleCommonTexture, textureIndex, commonData);
	imageStore(u_ParticlePositionTexture, textureIndex, position);
	imageStore(u_ParticleGlobalPositionTexture, textureIndex, position);
	imageStore(u_ParticleVelocityTexture, textureIndex, velocity);
	imageStore(u_ParticleForceTexture, textureIndex, force);
	imageStore(u_ParticleColorTexture, textureIndex, color);
	imageStore(u_ParticleSizeTexture, textureIndex, size);
}
