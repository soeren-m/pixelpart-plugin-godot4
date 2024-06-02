R"!(
shader_type spatial;
render_mode {mode};

uniform sampler2D u_MainTexture : hint_default_white, filter_linear, repeat_enable;

uniform float u_EffectTime = 0.0;
uniform float u_ObjectTime = 0.0;

uniform vec3 u_Emission = vec3(0.0, 0.0, 0.0);
uniform float u_Roughness = 0.5;
uniform float u_Metallic = 0.0;

uniform int u_ColorBlendMode = 0;

varying vec3 VELOCITY;
varying float LIFE;
varying float OBJECT_ID;

{common}

void vertex() {
	VELOCITY = INSTANCE_CUSTOM.xyz;
	LIFE = pixelpart_unpack_life_spatialinstance(INSTANCE_CUSTOM);
	OBJECT_ID = pixelpart_unpack_id_spatialinstance(INSTANCE_CUSTOM);
}

void fragment() {
	vec4 final_Color = texture(u_MainTexture, UV);
	final_Color = pixelpart_blend(final_Color, COLOR, u_ColorBlendMode);

	vec3 final_Emission = u_Emission;
	float final_Roughness = u_Roughness;
	float final_Metallic = u_Metallic;

	{output}
}
)!"