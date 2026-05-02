//type vertex
#version 450 core

out vec2 v_TexCoord;

void main()
{
	const vec2 Positions[3] = vec2[](
		vec2(-1.0, -1.0),
		vec2( 3.0, -1.0),
		vec2(-1.0,  3.0)
	);
	const vec2 TexCoords[3] = vec2[](
		vec2(0.0, 0.0),
		vec2(2.0, 0.0),
		vec2(0.0, 2.0)
	);

	v_TexCoord = TexCoords[gl_VertexID];
	gl_Position = vec4(Positions[gl_VertexID], 0.0, 1.0);
}


//type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

uniform sampler2D u_Normal;
uniform sampler2D u_Albedo;
uniform sampler2D u_Light;
uniform sampler2D u_VolumetricLight;

uniform vec3 u_AmbientLight;

void main()
{
	vec2 texCoord = v_TexCoord;

	vec4 fragColor = texture(u_Albedo, texCoord);
	vec3 light = texture(u_Light, texCoord).rgb;
	vec3 volumetricLight = texture(u_VolumetricLight, texCoord).rgb;
	float lightInfluence = texture(u_Normal, texCoord).b;

	light += u_AmbientLight;

	o_Color = vec4(mix(fragColor.rgb, fragColor.rgb * light, lightInfluence), fragColor.a);
	o_Color.rgb += volumetricLight;
	// volumetric is not working with transparent sprites!

	//o_Color = fragColor;
	//o_Color = vec4(texture(u_Normal, texCoord));
}