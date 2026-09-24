vec3 pixelpart_linear_to_srgb(vec3 color) {
	const vec3 a = vec3(0.055f);

	return mix(
		(vec3(1.0f) + a) * pow(color.rgb, vec3(1.0f / 2.4f)) - a,
		12.92f * color.rgb,
		lessThan(color.rgb, vec3(0.0031308f)));
}
