//type vertex
#version 450 core
// TODO: maybe boil this down...
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in vec2 a_Normal;
layout(location = 6) in float a_LightInfluence;

uniform mat4 u_VP;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TilingFactor;

out vec2 v_Normal;
out float v_LightInfluence;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	v_Normal = a_Normal;
	v_LightInfluence = a_LightInfluence;
	gl_Position = u_VP * vec4(a_Position, 1.0);
}

//type fragment
#version 450 core

layout(location = 0) out vec4 o_Normal;
layout(location = 1) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;

in vec2 v_Normal;
in float v_LightInfluence;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 finalColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;

	// cull transparent pixels
	// for alpha masking textures should use GL_NEAREST
	// this threshold is a result of using GL_LINEAR
	if (finalColor.a < 0.99)
		discard;

	o_Color = finalColor;
	o_Normal = vec4((v_Normal + 1.0) / 2.0 , v_LightInfluence, 1.0);
}