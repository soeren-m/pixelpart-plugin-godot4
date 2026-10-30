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

/*layout (std430, set = 0, binding = 2) restrict buffer CounterBuffer {
	uint COUNTER;
};*/

layout (binding = 0, std140) uniform SimulationParams {
	float deltaTime;
	uint particleCount;
	uint outTextureWidth;
	uint outTextureHeight;
} u_SimulationParams;

ivec2 toTextureIndex(uint index) {
	return ivec2(
		index % u_SimulationParams.outTextureWidth,
		index / u_SimulationParams.outTextureWidth);
}

void main() {
	uint p = gl_GlobalInvocationID.x;
	if(p >= u_SimulationParams.particleCount) {
		return;
	}

	ivec2 textureIndex = toTextureIndex(p);

	vec4 common = imageLoad(u_ParticleCommonTexture, textureIndex);
	vec4 position = imageLoad(u_ParticlePositionTexture, textureIndex);
	vec4 velocity = imageLoad(u_ParticleVelocityTexture, textureIndex);
	vec4 force = imageLoad(u_ParticleForceTexture, textureIndex);
	vec4 color = imageLoad(u_ParticleColorTexture, textureIndex);
	vec4 size = imageLoad(u_ParticleSizeTexture, textureIndex);

	// TODO
	color = vec4(1.0, 1.0, 1.0, 1.0);
	size = vec4(0.1, 0.1, 0.1, 1.0);

	velocity.xyz += force.xyz * u_SimulationParams.deltaTime;
	position.xyz += velocity.xyz * u_SimulationParams.deltaTime;
	vec4 globalPosition = position;

	common.x -= u_SimulationParams.deltaTime;

	imageStore(u_ParticleCommonTexture, textureIndex, common);
	imageStore(u_ParticlePositionTexture, textureIndex, position);
	imageStore(u_ParticleGlobalPositionTexture, textureIndex, globalPosition);
	imageStore(u_ParticleVelocityTexture, textureIndex, velocity);
	imageStore(u_ParticleForceTexture, textureIndex, force);
	imageStore(u_ParticleColorTexture, textureIndex, color);
	imageStore(u_ParticleSizeTexture, textureIndex, size);
}
