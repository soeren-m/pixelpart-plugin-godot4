shader_type canvas_item;

render_mode blend_mix;

uniform sampler2D u_MainTexture : hint_default_white, filter_linear, repeat_enable;
uniform vec3 u_Emission = vec3(0.0, 0.0, 0.0);
uniform int u_ColorBlendMode = 0;

#include "res://addons/pixelpart/shaders/PixelpartBlend.gdshaderinc"

void vertex() {

}

void fragment() {
	vec4 color = texture(u_MainTexture, UV);
	color = pixelpart_blend(color, COLOR, u_ColorBlendMode);
	color.rgb += u_Emission;

	COLOR = color;
}
