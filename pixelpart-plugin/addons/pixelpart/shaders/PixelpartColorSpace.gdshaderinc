vec4 pixelpart_srgb_to_linear(vec4 color) {
	return vec4(
		mix(
			pow((color.rgb + vec3(0.055)) * (1.0 / (1.0 + 0.055)), vec3(2.4)),
			color.rgb * (1.0 / 12.92),
			lessThan(color.rgb, vec3(0.04045))),
		color.a);
}

vec4 pixelpart_linear_to_srgb(vec4 color) {
	const vec3 a = vec3(0.055);

	return vec4(
		mix(
			(vec3(1.0) + a) * pow(color.rgb, vec3(1.0 / 2.4)) - a,
			12.92 * color.rgb,
			lessThan(color.rgb, vec3(0.0031308))),
		color.a);
}
