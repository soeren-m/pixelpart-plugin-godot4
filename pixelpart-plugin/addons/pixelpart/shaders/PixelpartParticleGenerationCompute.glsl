#[compute]
#version 460

layout (local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout (rgba32f, binding = 0) uniform restrict image2D u_ParticleCommonTexture;
layout (rgba32f, binding = 1) uniform restrict image2D u_ParticlePositionTexture;
layout (rgba32f, binding = 2) uniform restrict image2D u_ParticleGlobalPositionTexture;
layout (rgba32f, binding = 3) uniform restrict image2D u_ParticleVelocityTexture;
layout (rgba32f, binding = 4) uniform restrict image2D u_ParticleForceTexture;
layout (rgba32f, binding = 5) uniform restrict image2D u_ParticleRotationTexture;
layout (rgba32f, binding = 6) uniform restrict image2D u_ParticleSizeTexture;
layout (rgba32f, binding = 7) uniform restrict image2D u_ParticleColorTexture;

// TODO: push constants?
layout (binding = 0, std140) uniform GenerationParams {
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

	ivec2 textureIndex = toTextureIndex(p);

	vec4 commonData = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 position = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 velocity = vec4(0.0, 0.1, 0.0, 1.0);
	vec4 force = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 size = vec4(0.1, 0.1, 0.1, 1.0);
	// TODO

	imageStore(u_ParticleCommonTexture, textureIndex, commonData);
	imageStore(u_ParticlePositionTexture, textureIndex, position);
	imageStore(u_ParticleVelocityTexture, textureIndex, velocity);
	imageStore(u_ParticleForceTexture, textureIndex, force);
	imageStore(u_ParticleColorTexture, textureIndex, color);
	imageStore(u_ParticleSizeTexture, textureIndex, size);
}
