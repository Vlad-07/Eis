#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_VP;
uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_VP * u_Transform * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform float u_Fade;
uniform float u_Thickness;
uniform sampler2D u_Texture;

void main()
{
    float distance = 1.0 - length(v_TexCoord - vec2(0.5, 0.5)) * 2;
    vec4 colorr = vec4(smoothstep(0.0, u_Fade, distance));
    colorr *= vec4(smoothstep(u_Thickness + u_Fade, u_Thickness, distance));
    
    color = colorr;
    color *= texture(u_Texture, v_TexCoord) * u_Color;
}