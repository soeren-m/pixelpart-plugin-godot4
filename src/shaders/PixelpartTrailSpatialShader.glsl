R"!(
shader_type spatial;
render_mode {mode};

uniform sampler2D u_MainTexture : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_DepthTexture : hint_depth_texture, filter_nearest, repeat_disable;

uniform float u_EffectTime = 0.0;
uniform float u_ObjectTime = 0.0;

uniform vec3 u_Emission = vec3(0.0, 0.0, 0.0);
uniform float u_Roughness = 0.5;
uniform float u_Metallic = 0.0;

uniform int u_ColorBlendMode = 0;

uniform bool u_SoftParticles = false;
uniform float u_SoftParticleTransition = 0.0;
uniform bool u_DistanceFade = false;
uniform float u_DistanceFadeTransition = 0.0;

varying vec3 VELOCITY;
varying float LIFE;
varying float OBJECT_ID;

{common}

void vertex() {
	VELOCITY = CUSTOM0.xyz;
	LIFE = UV2.x;
	OBJECT_ID = UV2.y;
}

void fragment() {
	vec4 final_Color = texture(u_MainTexture, UV);
	final_Color = pixelpart_blend(final_Color, COLOR, u_ColorBlendMode);

	float softParticleBlendFactor = u_SoftParticles
		? pixelpart_soft_particle_fade(u_DepthTexture, INV_PROJECTION_MATRIX, SCREEN_UV, VERTEX.z, u_SoftParticleTransition)
		: 1.0;
	final_Color.a *= softParticleBlendFactor;

	float distanceBlendFactor = u_DistanceFade
		? pixelpart_distance_fade(VERTEX.z, u_DistanceFadeTransition)
		: 1.0;
	final_Color.a *= distanceBlendFactor;

	vec3 final_Emission = u_Emission;
	float final_Roughness = u_Roughness;
	float final_Metallic = u_Metallic;

	{output}
}
)!"