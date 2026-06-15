//type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in float a_Thickness;
layout(location = 3) in float a_Fade;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

layout(location = 0) out vec2 v_LocalPosition;
layout(location = 1) out vec4 v_Color;
layout(location = 2) out float v_Thickness;
layout(location = 3) out float v_Fade;

void main()
{
	const vec2 LocalPositions[4] = vec2[](
		vec2(-1.0, -1.0),
		vec2( 1.0, -1.0),
		vec2( 1.0,  1.0),
		vec2(-1.0,  1.0)
	);

	v_LocalPosition = LocalPositions[gl_VertexIndex % 4];
	v_Color = a_Color;
	v_Thickness = a_Thickness;
	v_Fade = a_Fade;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}


//type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

layout(location = 0) in vec2 v_LocalPosition;
layout(location = 1) in vec4 v_Color;
layout(location = 2) in float v_Thickness;
layout(location = 3) in float v_Fade;

void main()
{
	float distance = 1.0 - length(v_LocalPosition);
	float circleAlpha = smoothstep(0.0, v_Fade, distance);
	circleAlpha *= smoothstep(v_Thickness + v_Fade, v_Thickness, distance);

	if (circleAlpha == 0.0)
		discard;

	o_Color = v_Color;
	o_Color.a *= circleAlpha;
}