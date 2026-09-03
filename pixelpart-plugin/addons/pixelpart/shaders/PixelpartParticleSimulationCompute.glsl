#[compute]
#version 460

layout (local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout (rgb32f, binding = 0) uniform restrict image2D u_ParticleCommon;
layout (rgb32f, binding = 1) uniform restrict image2D u_ParticlePosition;
layout (rgb32f, binding = 2) uniform restrict image2D u_ParticleGlobalPosition;
layout (rgb32f, binding = 3) uniform restrict image2D u_ParticleVelocity;
layout (rgb32f, binding = 4) uniform restrict image2D u_ParticleForce;
layout (rgb32f, binding = 5) uniform restrict image2D u_ParticleRotation;
layout (rgb32f, binding = 6) uniform restrict image2D u_ParticleSize;
layout (rgba32f, binding = 7) uniform restrict image2D u_ParticleColor;

layout (std430, set = 0, binding = 2) restrict buffer CounterBuffer {
	uint COUNTER;
};

layout (push_constant, std430) uniform Params {
	float u_DeltaTime;
	uint u_ParticleCount;
	uint u_OutTextureWidth;
	uint u_OutTextureHeight;
} PARAMS;

ivec2 toTextureIndex(uint index) {
	return ivec2(index % u_OutTextureWidth, index / u_OutTextureWidth);
}

void main() {
	uint p = gl_GlobalInvocationID.x;
	if(p >= u_ParticleCount) {
		return;
	}

	ivec2 textureIndex = toTextureIndex(p);

	vec4 common = imageLoad(u_ParticleCommon, textureIndex);
	vec4 position = imageLoad(u_ParticlePosition, textureIndex); 
	vec4 velocity = imageLoad(u_ParticleVelocity, textureIndex);
	vec4 force = imageLoad(u_ParticleForce, textureIndex);
	vec4 color = imageLoad(u_ParticleColor, textureIndex);
	vec4 size = imageLoad(u_ParticleSize, textureIndex);

	// TODO
	color = vec4(1.0, 1.0, 1.0, 1.0);
	size = vec4(0.1, 0.1, 0.1, 1.0);

	velocity.xyz += force.xyz * u_DeltaTime;
	position.xyz += velocity.xyz * u_DeltaTime;
	vec4 globalPosition = position;

	common.x -= u_DeltaTime;

	imageStore(u_ParticleCommon, textureIndex, common);
	imageStore(u_ParticlePosition, textureIndex, position);
	imageStore(u_ParticleGlobalPosition, textureIndex, globalPosition);
	imageStore(u_ParticleVelocity, textureIndex, velocity);
	imageStore(u_ParticleForce, textureIndex, force);
	imageStore(u_ParticleColor, textureIndex, color);
	imageStore(u_ParticleSize, textureIndex, size);
}
