shader_type spatial;

render_mode depth_draw_opaque, cull_disabled,
	diffuse_burley, specular_schlick_ggx;

uniform sampler2D u_MainTexture : hint_default_white, filter_linear, repeat_enable;
uniform vec3 u_Emission = vec3(0.0, 0.0, 0.0);
uniform float u_Roughness = 0.5;
uniform float u_Metallic = 0.0;
uniform int u_ColorBlendMode = 0;
uniform float u_AlphaThreshold = 0.5;
uniform int u_SpriteSheetRowNumber = 1;
uniform int u_SpriteSheetColumnNumber = 1;
uniform int u_SpriteSheetOrigin = 0;
uniform int u_SpriteAnimationNumFrames = 1;
uniform int u_SpriteAnimationStartFrame = 0;
uniform float u_SpriteAnimationDuration = 1.0;
uniform bool u_SpriteAnimationLoop = false;

varying float LIFE;

#include "res://addons/pixelpart/shaders/PixelpartColorSpace.gdshaderinc"
#include "res://addons/pixelpart/shaders/PixelpartBlend.gdshaderinc"
#include "res://addons/pixelpart/shaders/PixelpartSpriteSheetAnimation.gdshaderinc"

void vertex() {
	COLOR = pixelpart_srgb_to_linear(COLOR);
	LIFE = UV2.x;
}

void fragment() {
	vec2 textureCoord = pixelpart_sprite_sheet_animation(UV, LIFE,
		u_SpriteSheetOrigin, u_SpriteAnimationStartFrame, u_SpriteAnimationNumFrames,
		u_SpriteSheetRowNumber, u_SpriteSheetColumnNumber,
		u_SpriteAnimationDuration, u_SpriteAnimationLoop);

	vec4 color = texture(u_MainTexture, textureCoord);
	color = pixelpart_blend(color, COLOR, u_ColorBlendMode);

	if(OUTPUT_IS_SRGB) {
		color = pixelpart_linear_to_srgb(color);
	}

	ALBEDO = color.rgb;
	ALPHA = color.a;
	ALPHA_SCISSOR_THRESHOLD = u_AlphaThreshold;
	EMISSION = u_Emission;
	ROUGHNESS = u_Roughness;
	METALLIC = u_Metallic;
}
