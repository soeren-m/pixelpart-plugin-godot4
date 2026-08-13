shader_type spatial;

render_mode depth_draw_opaque,
	diffuse_burley, specular_schlick_ggx,
	blend_mix;

uniform sampler2D u_MainTexture : hint_default_white, filter_linear, repeat_enable;
uniform vec3 u_Emission = vec3(0.0, 0.0, 0.0);
uniform float u_Roughness = 0.5;
uniform float u_Metallic = 0.0;
uniform int u_ColorBlendMode = 0;

#include "res://addons/pixelpart/shaders/PixelpartBlend.gdshaderinc"

void vertex() {

}

void fragment() {
	vec4 color = texture(u_MainTexture, UV);
	color = pixelpart_blend(color, COLOR, u_ColorBlendMode);

	ALBEDO = color.rgb;
	ALPHA = color.a;
	EMISSION = u_Emission;
	ROUGHNESS = u_Roughness;
	METALLIC = u_Metallic;
}
