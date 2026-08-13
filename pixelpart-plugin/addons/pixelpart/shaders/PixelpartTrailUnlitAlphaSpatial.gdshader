shader_type spatial;

render_mode depth_draw_opaque, cull_disabled,
	unshaded,
	blend_mix;

uniform sampler2D u_MainTexture : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_DepthTexture : hint_depth_texture, filter_nearest, repeat_disable;
uniform vec3 u_Emission = vec3(0.0, 0.0, 0.0);
uniform int u_ColorBlendMode = 0;
uniform bool u_SoftParticles = false;
uniform float u_SoftParticleTransition = 0.0;
uniform bool u_DistanceFade = false;
uniform float u_DistanceFadeTransition = 0.0;

#include "res://addons/pixelpart/shaders/PixelpartBlend.gdshaderinc"
#include "res://addons/pixelpart/shaders/PixelpartFade.gdshaderinc"

void vertex() {

}

void fragment() {
	vec4 color = texture(u_MainTexture, UV);
	color = pixelpart_blend(color, COLOR, u_ColorBlendMode);
	color.rgb += u_Emission;

	float softParticleBlendFactor = u_SoftParticles
		? pixelpart_soft_particle_fade(u_DepthTexture, INV_PROJECTION_MATRIX, SCREEN_UV, VERTEX.z, u_SoftParticleTransition)
		: 1.0;
	color.a *= softParticleBlendFactor;

	float distanceBlendFactor = u_DistanceFade
		? pixelpart_distance_fade(VERTEX.z, u_DistanceFadeTransition)
		: 1.0;
	color.a *= distanceBlendFactor;

	ALBEDO = color.rgb;
	ALPHA = color.a;
}
