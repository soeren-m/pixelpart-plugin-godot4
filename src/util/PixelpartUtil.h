#ifndef PIXELPART_UTIL_H
#define PIXELPART_UTIL_H

#include "common/Types.h"
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector4.hpp>
#include <godot_cpp/variant/color.hpp>

namespace godot {
int toGd(pixelpart::int_t v);
float toGd(pixelpart::float_t v);
Vector2 toGd(const pixelpart::vec2_t& v);
Vector3 toGd(const pixelpart::vec3_t& v);
Vector4 toGd(const pixelpart::vec4_t& v);
Color toGdColor(const pixelpart::vec4_t& v);

pixelpart::int_t fromGd(int v);
pixelpart::float_t fromGd(float v);
pixelpart::vec2_t fromGd(const Vector2& v);
pixelpart::vec3_t fromGd(const Vector3& v);
pixelpart::vec4_t fromGd(const Vector4& v);
pixelpart::vec4_t fromGd(const Color& v);

float packFloatsUnsigned(float a, float b, float sa, float sb);
float packFloatsSigned(float a, float b, float sa, float sb);
float packFloatsSignedUnsigned(float a, float b, float sa, float sb);
float packUIntFloat(unsigned int a, float b, float sb);

template <typename T>
std::vector<T> concat(const std::vector<T>& v1, const std::vector<T>& v2) {
	std::vector<T> result = v1;
	result.insert(result.end(), v2.begin(), v2.end());

	return result;
}
}

#endif