shader_type spatial;

render_mode depth_draw_opaque, cull_disabled,
	diffuse_burley, specular_schlick_ggx,
	blend_mix;

uniform sampler2D u_MainTexture : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_DepthTexture : hint_depth_texture, filter_nearest, repeat_disable;
uniform vec3 u_Emission = vec3(0.0, 0.0, 0.0);
uniform float u_Roughness = 0.5;
uniform float u_Metallic = 0.0;
uniform int u_ColorBlendMode = 0;
uniform int u_SpriteSheetRowNumber = 1;
uniform int u_SpriteSheetColumnNumber = 1;
uniform int u_SpriteSheetOrigin = 0;
uniform int u_SpriteAnimationNumFrames = 1;
uniform int u_SpriteAnimationStartFrame = 0;
uniform float u_SpriteAnimationDuration = 1.0;
uniform bool u_SpriteAnimationLoop = false;
uniform bool u_SoftParticles = false;
uniform float u_SoftParticleTransition = 0.0;
uniform bool u_DistanceFade = false;
uniform float u_DistanceFadeTransition = 0.0;

varying float LIFE;

#include "res://addons/pixelpart/shaders/PixelpartBlend.gdshaderinc"
#include "res://addons/pixelpart/shaders/PixelpartFade.gdshaderinc"
#include "res://addons/pixelpart/shaders/PixelpartSpriteSheetAnimation.gdshaderinc"

void vertex() {
	LIFE = UV2.x;
}

void fragment() {
	vec2 textureCoord = pixelpart_sprite_sheet_animation(UV, LIFE,
		u_SpriteSheetOrigin, u_SpriteAnimationStartFrame, u_SpriteAnimationNumFrames,
		u_SpriteSheetRowNumber, u_SpriteSheetColumnNumber,
		u_SpriteAnimationDuration, u_SpriteAnimationLoop);

	vec4 color = texture(u_MainTexture, textureCoord);
	color = pixelpart_blend(color, COLOR, u_ColorBlendMode);

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
	EMISSION = u_Emission;
	ROUGHNESS = u_Roughness;
	METALLIC = u_Metallic;
}
