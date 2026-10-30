shader_type spatial;

render_mode depth_draw_opaque, cull_disabled,
	unshaded,
	blend_mix;

uniform sampler2D u_MainTexture : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_DepthTexture : hint_depth_texture, filter_nearest, repeat_disable;
uniform vec3 u_Emission = vec3(0.0, 0.0, 0.0);
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

uniform sampler2D u_ParticleCommonTexture;
uniform sampler2D u_ParticleGlobalPositionTexture;
uniform sampler2D u_ParticleColorTexture;
uniform sampler2D u_ParticleSizeTexture;

varying float LIFE;

#include "res://addons/pixelpart/shaders/PixelpartBlend.gdshaderinc"
#include "res://addons/pixelpart/shaders/PixelpartFade.gdshaderinc"
#include "res://addons/pixelpart/shaders/PixelpartSpriteSheetAnimation.gdshaderinc"

void vertex() {
	ivec2 dataTextureSize = textureSize(u_ParticleCommonTexture, 0);
	ivec2 dataTextureIndex = ivec2(INSTANCE_ID % dataTextureSize.x, INSTANCE_ID / dataTextureSize.x);

	vec4 particleCommon = texelFetch(u_ParticleCommonTexture, dataTextureIndex, 0);
	vec4 particlePosition = texelFetch(u_ParticlePositionTexture, dataTextureIndex, 0);
	vec4 particleColor = texelFetch(u_ParticleColorTexture, dataTextureIndex, 0);
	vec4 particleSize = texelFetch(u_ParticleSizeTexture, dataTextureIndex, 0);

	VERTEX *= particleSize.xyz;
	// TODO: VERTEX = rotate(VERTEX, rotation);
	VERTEX += particlePosition.xyz;

	// TODO: NORMAL = rotate(NORMAL, rotation);

	COLOR = particleColor;
	LIFE = particleCommon.x;
}

void fragment() {
	vec2 textureCoord = pixelpart_sprite_sheet_animation(UV, LIFE,
		u_SpriteSheetOrigin, u_SpriteAnimationStartFrame, u_SpriteAnimationNumFrames,
		u_SpriteSheetRowNumber, u_SpriteSheetColumnNumber,
		u_SpriteAnimationDuration, u_SpriteAnimationLoop);

	vec4 color = texture(u_MainTexture, textureCoord);
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
