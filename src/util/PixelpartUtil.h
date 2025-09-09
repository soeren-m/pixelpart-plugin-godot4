#ifndef PIXELPART_UTIL_H
#define PIXELPART_UTIL_H

#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector4.hpp>
#include <godot_cpp/variant/color.hpp>
#include <pixelpart-runtime/common/Types.h>
#include <pixelpart-runtime/common/Math.h>
#include <pixelpart-runtime/common/VariantValue.h>

namespace godot {
int pxpt_to_gd(pixelpart::int_t v);
float pxpt_to_gd(pixelpart::float_t v);
Vector2 pxpt_to_gd(const pixelpart::float2_t& v);
Vector3 pxpt_to_gd(const pixelpart::float3_t& v);
Vector4 pxpt_to_gd(const pixelpart::float4_t& v);
Color pxpt_to_gd_color(const pixelpart::float4_t& v);
Variant pxpt_to_gd(const pixelpart::VariantValue& v);

pixelpart::int_t gd_to_pxpt(int v);
pixelpart::float_t gd_to_pxpt(float v);
pixelpart::float2_t gd_to_pxpt(const Vector2& v);
pixelpart::float3_t gd_to_pxpt(const Vector3& v);
pixelpart::float4_t gd_to_pxpt(const Vector4& v);
pixelpart::float4_t gd_to_pxpt(const Color& v);
pixelpart::VariantValue gd_to_pxpt(const Variant& v);

float pack_uint_float(unsigned int a, float b, float s);
}

#endif
