#include "PixelpartUtil.h"

namespace godot {
Vector2 toGd(const pixelpart::vec2d& v) {
	return Vector2(v.x, v.y);
}
Vector3 toGd(const pixelpart::vec3d& v) {
	return Vector3(v.x, v.y, v.z);
}
Color toGd(const pixelpart::vec4d& v) {
	return Color(v.r, v.g, v.b, v.a);
}

pixelpart::vec2d fromGd(const Vector2& v) {
	return pixelpart::vec2d(v.x, v.y);
}
pixelpart::vec3d fromGd(const Vector3& v) {
	return pixelpart::vec3d(v.x, v.y, v.z);
}
pixelpart::vec4d fromGd(const Color& v) {
	return pixelpart::vec4d(v.r, v.g, v.b, v.a);
}
}