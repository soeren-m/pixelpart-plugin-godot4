R"!(
shader_type canvas_item;
render_mode {mode};

uniform sampler2D u_MainTexture : hint_default_white, filter_linear, repeat_enable;

uniform float u_EffectTime = 0.0;
uniform float u_ObjectTime = 0.0;

uniform vec3 u_Emission = vec3(0.0, 0.0, 0.0);

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
	VELOCITY = vec3(CUSTOM0.x, CUSTOM0.y, 0.0);
	LIFE = CUSTOM0.z;
	OBJECT_ID = CUSTOM0.w;
}

void fragment() {
	vec4 final_Color = texture(u_MainTexture, UV);
	final_Color = pixelpart_blend(final_Color, COLOR, u_ColorBlendMode);

	vec3 final_Emission = u_Emission;

	{output}
}
)!"