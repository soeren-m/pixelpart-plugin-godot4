R"!(
const float PACK_FACTOR = 10000.0;

vec3 simplexPermute(vec3 x) {
	return mod(((x * 34.0) + 1.0) * x, 289.0);
}
float simplexNoise2d(vec2 v) {
	const vec4 C = vec4(0.211324865405187, 0.366025403784439, -0.577350269189626, 0.024390243902439);

	vec2 i = floor(v + dot(v, C.yy));
	vec2 x0 = v - i + dot(i, C.xx);
	vec2 i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
	vec4 x12 = x0.xyxy + C.xxzz;
	x12.xy -= i1;
	i = mod(i, 289.0);

	vec3 p = simplexPermute(simplexPermute(i.y + vec3(0.0, i1.y, 1.0)) + i.x + vec3(0.0, i1.x, 1.0));
	vec3 m = max(0.5 - vec3(dot(x0, x0), dot(x12.xy, x12.xy), dot(x12.zw, x12.zw)), 0.0);
	m = m * m;
	m = m * m;

	vec3 x = 2.0 * fract(p * C.www) - 1.0;
	vec3 h = abs(x) - 0.5;
	vec3 ox = floor(x + 0.5);
	vec3 a0 = x - ox;
	m *= 1.79284291400159 - 0.85373472095314 * (a0 * a0 + h * h);

	vec3 g;
	g.x = a0.x * x0.x + h.x * x0.y;
	g.yz = a0.yz * x12.xz + h.yz * x12.yw;

	return 130.0 * dot(m, g);
}

vec2 spriteSheetAnimation(vec2 textureCoord, float life, int origin, int startFrame, int numFrames, int rowNumber, int columnNumber, float duration, bool loop) {
	int animationFrame = startFrame;
	animationFrame += loop
		? int(mod(life / duration * float(numFrames), float(numFrames)))
		: clamp(int(life / duration * float(numFrames)), 0, numFrames - 1);

	switch(origin) {
		case 0:
			return vec2(
					float((animationFrame % (rowNumber * columnNumber)) % rowNumber) / float(rowNumber),
					float((animationFrame % (rowNumber * columnNumber)) / rowNumber) / float(columnNumber)) +
				vec2(1.0 / float(rowNumber), 1.0 / float(columnNumber)) * textureCoord;
		case 1:
			return vec2(
					1.0 - float((animationFrame % (rowNumber * columnNumber)) % rowNumber) / float(rowNumber) - 1.0 / float(rowNumber),
					float((animationFrame % (rowNumber * columnNumber)) / rowNumber) / float(columnNumber)) +
				vec2(1.0 / float(rowNumber), 1.0 / float(columnNumber)) * textureCoord;
		case 2:
			return vec2(
					float((animationFrame % (rowNumber * columnNumber)) % rowNumber) / float(rowNumber),
					1.0 - float((animationFrame % (rowNumber * columnNumber)) / rowNumber) / float(columnNumber) - 1.0 / float(columnNumber)) +
				vec2(1.0 / float(rowNumber), 1.0 / float(columnNumber)) * textureCoord;
		case 3:
			return vec2(
					1.0 - float((animationFrame % (rowNumber * columnNumber)) % rowNumber) / float(rowNumber) - 1.0 / float(rowNumber),
					1.0 - float((animationFrame % (rowNumber * columnNumber)) / rowNumber) / float(columnNumber) - 1.0 / float(columnNumber)) +
				vec2(1.0 / float(rowNumber), 1.0 / float(columnNumber)) * textureCoord;
		default:
			return textureCoord;
	}
}

vec4 blend(vec4 colorA, vec4 colorB, int blendMode) {
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

vec3 unpackVelocity_CanvasItem(vec4 color) {
	return vec3((floor(color.rg) - vec2(PACK_FACTOR)) / vec2(PACK_FACTOR), 0.0);
}
vec3 unpackForce_CanvasItem(vec4 color) {
	return vec3((floor(color.ba) - vec2(PACK_FACTOR)) / vec2(PACK_FACTOR), 0.0);
}
float unpackLife_CanvasItem(vec2 uv) {
	return floor(uv.x) / PACK_FACTOR;
}
float unpackObjectId_CanvasItem(vec2 uv) {
	return floor(uv.y) - 1.0;
}
vec2 unpackUV_CanvasItem(vec2 uv) {
	return fract(uv) * vec2(10.0);
}
vec4 unpackColor_CanvasItem(vec4 color) {
	return fract(color) * vec4(10.0);
}
)!"