float pixelpart_soft_particle_fade(sampler2D depthTexture, mat4 invProjMatrix, vec2 screenUV, float vertexDepth, float transitionSize) {
	float sceneDepth = texture(depthTexture, screenUV).r;
	vec4 scenePos = invProjMatrix * vec4(screenUV * 2.0 - 1.0, sceneDepth, 1.0);
	scenePos.xyz /= scenePos.w;

	float depthDistance = clamp(vertexDepth - scenePos.z, 0.0, 1.0);

	return smoothstep(0.0, transitionSize, depthDistance);
}

float pixelpart_distance_fade(float vertexDepth, float transitionSize) {
	return smoothstep(0.0, transitionSize, -vertexDepth);
}
