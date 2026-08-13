R"!(
shader_type spatial;

render_mode depth_draw_opaque;

#ifndef PIXELPART_RENDERER_MESH
render_mode cull_disabled;
#endif

#if defined(PIXELPART_BLEND_ALPHA)
render_mode blend_mix;
#elif defined(PIXELPART_BLEND_ADDITIVE)
render_mode blend_add;
#elif defined(PIXELPART_BLEND_SUBTRACTIVE)
render_mode blend_sub;
#endif

#ifdef PIXELPART_LIGHTING_LIT
render_mode diffuse_burley, specular_schlick_ggx;
#else
render_mode unshaded;
#endif

uniform sampler2D u_Texture0 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture1 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture2 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture3 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture4 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture5 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture6 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture7 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_DepthTexture : hint_depth_texture, filter_nearest, repeat_disable;

uniform float u_EffectTime = 0.0;
uniform float u_ObjectTime = 0.0;

{parameter}

varying vec3 VELOCITY;
varying float LIFE;
varying float OBJECT_ID;

#include "res://addons/pixelpart/shaders/PixelpartBlend.gdshaderinc"
#include "res://addons/pixelpart/shaders/PixelpartFade.gdshaderinc"
#include "res://addons/pixelpart/shaders/PixelpartNoise.gdshaderinc"
#include "res://addons/pixelpart/shaders/PixelpartSpriteSheetAnimation.gdshaderinc"

float pixelpart_unpack_life(float value) {
	return fract(value) * 2.0;
}
float pixelpart_unpack_id(float value) {
	return floor(value);
}

void vertex() {
#ifdef PIXELPART_RENDERER_MESH
	VELOCITY = INSTANCE_CUSTOM.xyz;
	LIFE = pixelpart_unpack_life(INSTANCE_CUSTOM.w);
	OBJECT_ID = pixelpart_unpack_id(INSTANCE_CUSTOM.w);
#else
	VELOCITY = CUSTOM0.xyz;
	LIFE = UV2.x;
	OBJECT_ID = UV2.y;
#endif
}

void fragment() {
	{main}

	if(!OUTPUT_IS_SRGB) {
		final_Color.rgb = mix(
			pow((final_Color.rgb + vec3(0.055)) * (1.0 / (1.0 + 0.055)), vec3(2.4)),
			final_Color.rgb * (1.0 / 12.92),
			lessThan(final_Color.rgb, vec3(0.04045)));
	}

	ALBEDO = final_Color.rgb;

#ifndef PIXELPART_BLEND_OFF
	ALPHA = final_Color.a;
#endif

#ifdef PIXELPART_LIGHTING_LIT
	EMISSION = final_Emission;
	ROUGHNESS = final_Roughness;
	METALLIC = final_Metallic;
#else
	ALBEDO += final_Emission;
#endif
}
)!"