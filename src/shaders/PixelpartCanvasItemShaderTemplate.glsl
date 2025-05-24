R"!(
shader_type canvas_item;
render_mode {mode};

uniform sampler2D u_Texture0 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture1 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture2 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture3 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture4 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture5 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture6 : hint_default_white, filter_linear, repeat_enable;
uniform sampler2D u_Texture7 : hint_default_white, filter_linear, repeat_enable;

uniform float u_EffectTime = 0.0;
uniform float u_ObjectTime = 0.0;

{parameter}

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
	{main}

	{output}
}
)!"