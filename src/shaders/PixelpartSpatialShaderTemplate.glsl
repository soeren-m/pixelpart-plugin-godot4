R"!(
shader_type spatial;
render_mode {mode};

uniform sampler2D TEXTURE0 : hint_default_white,filter_linear,repeat_enable;
uniform sampler2D TEXTURE1 : hint_default_white,filter_linear,repeat_enable;
uniform sampler2D TEXTURE2 : hint_default_white,filter_linear,repeat_enable;
uniform sampler2D TEXTURE3 : hint_default_white,filter_linear,repeat_enable;
uniform sampler2D TEXTURE4 : hint_default_white,filter_linear,repeat_enable;
uniform sampler2D TEXTURE5 : hint_default_white,filter_linear,repeat_enable;
uniform sampler2D TEXTURE6 : hint_default_white,filter_linear,repeat_enable;
uniform sampler2D TEXTURE7 : hint_default_white,filter_linear,repeat_enable;

uniform float u_EffectTime = 0.0;
uniform float u_ObjectTime = 0.0;

varying vec3 VELOCITY;
varying vec3 FORCE;
varying float LIFE;
varying float OBJECT_ID;

{common}

void vertex() {
	VELOCITY = CUSTOM0.xyz;
	FORCE = CUSTOM1.xyz;
	LIFE = UV2.x;
	OBJECT_ID = UV2.y;
}

void fragment() {
	{main}

	if(!OUTPUT_IS_SRGB) {
		final_Color.rgb = mix(
			pow((final_Color.rgb + vec3(0.055)) * (1.0 / (1.0 + 0.055)), vec3(2.4)),
			final_Color.rgb * (1.0 / 12.92),
			lessThan(final_Color.rgb, vec3(0.04045)));
	}

	ALBEDO = final_Color.rgb;
	ALPHA = final_Color.a;
	EMISSION = final_Emission;
	ROUGHNESS = final_Roughness;
	METALLIC = final_Metallic;
}
)!"