//type vertex
#version 450 core

layout(location = 0) in vec2 a_Position;

out vec2 v_FragWorldPos;

uniform mat4 u_VP;

void main()
{
	v_FragWorldPos = a_Position;
	gl_Position = u_VP * vec4(a_Position, 0.0, 1.0);
}

//type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out vec4 o_Volume;

in vec2 v_FragWorldPos;

uniform sampler2D u_Normal;

uniform vec2 LightPos;
uniform vec3 LightColor;
uniform float LightRange;
uniform float LightVolume;

uniform vec2 u_ScreenSize;

void main()
{
	vec2 texCoord = gl_FragCoord.xy / u_ScreenSize;
	vec2 fragNormal = texture(u_Normal, texCoord).xy * 2.0 - 1.0;

	float dist = length(LightPos - v_FragWorldPos) / LightRange;
	float falloff = clamp(1.0 - dist, 0.0, 1.0);
	falloff *= falloff;

	vec2 lightDir = normalize(LightPos - v_FragWorldPos);
	float normalResponse = clamp(dot(lightDir, fragNormal), 0.0, 1.0);



	o_Color = vec4(LightColor * falloff * normalResponse, 1.0);

	o_Volume = vec4(vec3(LightColor * falloff * LightVolume), 1.0);
}