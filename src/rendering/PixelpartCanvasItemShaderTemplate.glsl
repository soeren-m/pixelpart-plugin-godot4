R"!(
shader_type canvas_item;

#if defined(PIXELPART_BLEND_ALPHA)
render_mode blend_mix;
#elif defined(PIXELPART_BLEND_ADDITIVE)
render_mode blend_add;
#elif defined(PIXELPART_BLEND_SUBTRACTIVE)
render_mode blend_sub;
#else
render_mode blend_disabled;
#endif

#ifndef PIXELPART_LIGHTING_LIT
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

void vertex() {
	VELOCITY = vec3(CUSTOM0.x, CUSTOM0.y, 0.0);
	LIFE = CUSTOM0.z;
	OBJECT_ID = CUSTOM0.w;
}

void fragment() {
	{main}

	final_Color.rgb += final_Emission;
	COLOR = final_Color;
}
)!"