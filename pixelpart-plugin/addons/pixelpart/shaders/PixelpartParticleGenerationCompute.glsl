#[compute]
#version 460

layout (local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout (rgb32f, binding = 0) uniform restrict image2D u_ParticlePosition;
layout (rgb32f, binding = 1) uniform restrict image2D u_ParticleVelocity;
layout (rgb32f, binding = 2) uniform restrict image2D u_ParticleForce;
layout (rgba32f, binding = 3) uniform restrict image2D u_ParticleColor;
layout (rgb32f, binding = 4) uniform restrict image2D u_ParticleSize;

ivec2 toTextureIndex(uint index) {
	return ivec2(index % u_OutTextureWidth, index / u_OutTextureWidth);
}

void main() {
	uint p = gl_GlobalInvocationID.x;

	ivec2 textureIndex = toTextureIndex(p);

	vec4 position = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 velocity = vec4(0.0, 0.1, 0.0, 1.0);
	vec4 force = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 size = vec4(0.1, 0.1, 0.1, 1.0);
	// TODO

	imageStore(u_ParticlePosition, textureIndex, position);
	imageStore(u_ParticleVelocity, textureIndex, velocity);
	imageStore(u_ParticleForce, textureIndex, force);
	imageStore(u_ParticleColor, textureIndex, color);
	imageStore(u_ParticleSize, textureIndex, size);
}
