R"!(
shader_type canvas_item;
render_mode {mode};

uniform sampler2D u_MainTexture : hint_default_white,filter_linear,repeat_enable;

uniform float u_EffectTime = 0.0;
uniform float u_ObjectTime = 0.0;

uniform vec3 u_Emission;

uniform int u_ColorBlendMode;

uniform bool u_SoftParticles;
uniform float u_SoftParticleTransition;
uniform bool u_DistanceFade;
uniform float u_DistanceFadeTransition;

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
	vec4 color = texture(u_MainTexture, UV);
	color = blend(color, COLOR, u_ColorBlendMode);
	color.rgb += u_Emission;

	COLOR = color;
}
)!"