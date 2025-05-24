#include "PixelpartUtil.h"
#include <cmath>

namespace godot {
int pxpt_to_gd(pixelpart::int_t v) {
	return static_cast<int>(v);
}
float pxpt_to_gd(pixelpart::float_t v) {
	return static_cast<float>(v);
}
Vector2 pxpt_to_gd(const pixelpart::float2_t& v) {
	return Vector2(v.x, v.y);
}
Vector3 pxpt_to_gd(const pixelpart::float3_t& v) {
	return Vector3(v.x, v.y, v.z);
}
Vector4 pxpt_to_gd(const pixelpart::float4_t& v) {
	return Vector4(v.r, v.g, v.b, v.a);
}
Color pxpt_to_gd_color(const pixelpart::float4_t& v) {
	return Color(v.r, v.g, v.b, v.a);
}
Variant pxpt_to_gd(const pixelpart::VariantValue& v) {
	switch(v.type()) {
		case pixelpart::VariantValue::type_bool:
			return Variant(v.valueBool());
		case pixelpart::VariantValue::type_int:
			return Variant(v.valueInt());
		case pixelpart::VariantValue::type_float:
			return Variant(v.valueFloat());
		case pixelpart::VariantValue::type_float2:
			return Variant(pxpt_to_gd(v.valueFloat2()));
		case pixelpart::VariantValue::type_float3:
			return Variant(pxpt_to_gd(v.valueFloat3()));
		case pixelpart::VariantValue::type_float4:
			return Variant(pxpt_to_gd(v.valueFloat4()));
		default:
			return Variant();
	}
}

pixelpart::int_t gd_to_pxpt(int v) {
	return static_cast<pixelpart::int_t>(v);
}
pixelpart::float_t gd_to_pxpt(float v) {
	return static_cast<pixelpart::float_t>(v);
}
pixelpart::float2_t gd_to_pxpt(const Vector2& v) {
	return pixelpart::float2_t(v.x, v.y);
}
pixelpart::float3_t gd_to_pxpt(const Vector3& v) {
	return pixelpart::float3_t(v.x, v.y, v.z);
}
pixelpart::float4_t gd_to_pxpt(const Vector4& v) {
	return pixelpart::float4_t(v.x, v.y, v.z, v.w);
}
pixelpart::float4_t gd_to_pxpt(const Color& v) {
	return pixelpart::float4_t(v.r, v.g, v.b, v.a);
}
pixelpart::VariantValue gd_to_pxpt(const Variant& v) {
	switch(v.get_type()) {
		case Variant::BOOL:
			return pixelpart::VariantValue::Bool(static_cast<bool>(v));
		case Variant::INT:
			return pixelpart::VariantValue::Int(static_cast<int64_t>(v));
		case Variant::FLOAT:
			return pixelpart::VariantValue::Float(static_cast<double>(v));
		case Variant::VECTOR2:
			return pixelpart::VariantValue::Float2(gd_to_pxpt(static_cast<Vector2>(v)));
		case Variant::VECTOR3:
			return pixelpart::VariantValue::Float3(gd_to_pxpt(static_cast<Vector3>(v)));
		case Variant::VECTOR4:
			return pixelpart::VariantValue::Float4(gd_to_pxpt(static_cast<Vector4>(v)));
		case Variant::COLOR:
			return pixelpart::VariantValue::Float4(gd_to_pxpt(static_cast<Color>(v)));
		default:
			return pixelpart::VariantValue();
	}
}

float pack_uint_float(unsigned int a, float b, float s) {
	return static_cast<float>(a) + b * s;
}
}