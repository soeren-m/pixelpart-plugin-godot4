R"!(
shader_type canvas_item;
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
	VELOCITY = unpackVelocity_CanvasItem(COLOR);
	FORCE = unpackForce_CanvasItem(COLOR);
	LIFE = unpackLife_CanvasItem(UV);
	OBJECT_ID = unpackObjectId_CanvasItem(UV);
	UV = unpackUV_CanvasItem(UV);
	COLOR = unpackColor_CanvasItem(COLOR);
}

void fragment() {
	{main}

	COLOR = final_Color;
}
)!"