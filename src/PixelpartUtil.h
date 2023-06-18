#ifndef PIXELPART_UTIL_H
#define PIXELPART_UTIL_H

#include "Types.h"
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/color.hpp>

namespace godot {
Vector2 toGd(const pixelpart::vec2d& v);
Vector3 toGd(const pixelpart::vec3d& v);
Color toGd(const pixelpart::vec4d& v);

pixelpart::vec2d fromGd(const Vector2& v);
pixelpart::vec3d fromGd(const Vector3& v);
pixelpart::vec4d fromGd(const Color& v);
}

#endif