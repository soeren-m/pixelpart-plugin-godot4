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
	VELOCITY = pixelpart_unpack_velocity_canvasitem(COLOR);
	LIFE = pixelpart_unpack_life_canvasitem(UV);
	OBJECT_ID = pixelpart_unpack_id_canvasitem(UV);
	UV = pixelpart_unpack_uv_canvasitem(UV);
	COLOR = pixelpart_unpack_color_canvasitem(COLOR);
}

void fragment() {
	{main}

	{output}
}
)!"