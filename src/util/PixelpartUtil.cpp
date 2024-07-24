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
Variant toGd(const pixelpart::VariantValue& v) {
	switch(v.type) {
		case pixelpart::VariantValue::type_bool:
			return Variant(v.getBool());
		case pixelpart::VariantValue::type_int:
			return Variant(v.getInt());
		case pixelpart::VariantValue::type_float:
			return Variant(v.getFloat());
		case pixelpart::VariantValue::type_float2:
			return Variant(toGd(v.getFloat2()));
		case pixelpart::VariantValue::type_float3:
			return Variant(toGd(v.getFloat3()));
		case pixelpart::VariantValue::type_float4:
			return Variant(toGd(v.getFloat4()));
		default:
			return Variant();
	}
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
pixelpart::VariantValue fromGd(const Variant& v) {
	switch(v.get_type()) {
		case Variant::BOOL:
			return pixelpart::VariantValue::Bool(static_cast<bool>(v));
		case Variant::INT:
			return pixelpart::VariantValue::Int(static_cast<int64_t>(v));
		case Variant::FLOAT:
			return pixelpart::VariantValue::Float(static_cast<double>(v));
		case Variant::VECTOR2:
			return pixelpart::VariantValue::Float2(fromGd(static_cast<Vector2>(v)));
		case Variant::VECTOR3:
			return pixelpart::VariantValue::Float3(fromGd(static_cast<Vector3>(v)));
		case Variant::VECTOR4:
			return pixelpart::VariantValue::Float4(fromGd(static_cast<Vector4>(v)));
		case Variant::COLOR:
			return pixelpart::VariantValue::Float4(fromGd(static_cast<Color>(v)));
		default:
			return pixelpart::VariantValue();
	}
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