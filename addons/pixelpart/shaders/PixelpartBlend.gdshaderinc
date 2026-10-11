vec4 pixelpart_blend(vec4 colorA, vec4 colorB, int blendMode) {
	switch(blendMode) {
		case 0:
			return colorA * colorB;
		case 1:
			return vec4(clamp(colorA.rgb + colorB.rgb, vec3(0.0), vec3(1.0)),
				colorA.a * colorB.a);
		case 2:
			return vec4(clamp(colorA.rgb - colorB.rgb, vec3(0.0), vec3(1.0)),
				colorA.a * colorB.a);
		case 3:
			return vec4(abs(colorA.rgb - colorB.rgb),
				colorA.a * colorB.a);
		case 4:
			return vec4(vec3(1.0) - (vec3(1.0) - colorA.rgb) * (vec3(1.0) - colorB.rgb),
				colorA.a * colorB.a);
		case 5:
			return vec4(
				(colorA.r < 0.5) ? (2.0 * colorA.r * colorB.r) : (1.0 - 2.0 * (1.0 - colorA.r) * (1.0 - colorB.r)),
				(colorA.g < 0.5) ? (2.0 * colorA.g * colorB.g) : (1.0 - 2.0 * (1.0 - colorA.g) * (1.0 - colorB.g)),
				(colorA.b < 0.5) ? (2.0 * colorA.b * colorB.b) : (1.0 - 2.0 * (1.0 - colorA.b) * (1.0 - colorB.b)),
				colorA.a * colorB.a);
		case 6:
			return vec4(max(colorA.rgb, colorB.rgb),
				colorA.a * colorB.a);
		case 7:
			return vec4(min(colorA.rgb, colorB.rgb),
				colorA.a * colorB.a);
		case 8:
			return vec4(colorB.rgb, colorA.a * colorB.a);
		default:
			return colorA;
	}
}
