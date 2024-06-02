#include "PixelpartUtil.h"

namespace godot {
int toGd(pixelpart::int_t v) {
	return static_cast<int>(v);
}
float toGd(pixelpart::float_t v) {
	return static_cast<float>(v);
}
Vector2 toGd(const pixelpart::vec2_t& v) {
	return Vector2(v.x, v.y);
}
Vector3 toGd(const pixelpart::vec3_t& v) {
	return Vector3(v.x, v.y, v.z);
}
Vector4 toGd(const pixelpart::vec4_t& v) {
	return Vector4(v.r, v.g, v.b, v.a);
}
Color toGdColor(const pixelpart::vec4_t& v) {
	return Color(v.r, v.g, v.b, v.a);
}

pixelpart::int_t fromGd(int v) {
	return static_cast<pixelpart::int_t>(v);
}
pixelpart::float_t fromGd(float v) {
	return static_cast<pixelpart::float_t>(v);
}
pixelpart::vec2_t fromGd(const Vector2& v) {
	return pixelpart::vec2_t(v.x, v.y);
}
pixelpart::vec3_t fromGd(const Vector3& v) {
	return pixelpart::vec3_t(v.x, v.y, v.z);
}
pixelpart::vec4_t fromGd(const Vector4& v) {
	return pixelpart::vec4_t(v.x, v.y, v.z, v.w);
}
pixelpart::vec4_t fromGd(const Color& v) {
	return pixelpart::vec4_t(v.r, v.g, v.b, v.a);
}

float packFloatsUnsigned(float a, float b, float sa, float sb) {
	return std::floor(a * sa) + b * sb;
}
float packFloatsSigned(float a, float b, float sa, float sb) {
	return std::floor(a * sa) + sa + b / sb;
}
float packFloatsSignedUnsigned(float a, float b, float sa, float sb) {
	return std::floor(a * sa) + sa + b * sb;
}
float packUIntFloat(unsigned int a, float b, float sb) {
	return static_cast<float>(a) + b * sb;
}
}