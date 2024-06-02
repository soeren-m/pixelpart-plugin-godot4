R"!(
shader_type spatial;
render_mode {mode};

uniform sampler2D u_Texture0 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture1 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture2 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture3 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture4 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture5 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture6 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture7 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_DepthTexture : hint_depth_texture, filter_nearest, repeat_disable;

uniform float u_EffectTime = 0.0;
uniform float u_ObjectTime = 0.0;

{parameter}

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
	{main}

	if(!OUTPUT_IS_SRGB) {
		final_Color.rgb = mix(
			pow((final_Color.rgb + vec3(0.055)) * (1.0 / (1.0 + 0.055)), vec3(2.4)),
			final_Color.rgb * (1.0 / 12.92),
			lessThan(final_Color.rgb, vec3(0.04045)));
	}

	{output}
}
)!"