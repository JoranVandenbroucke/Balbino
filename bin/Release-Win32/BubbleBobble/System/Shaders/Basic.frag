#version 330 core

layout( location = 0 ) out vec4 f_color;

in vec4 v_Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
	vec4 texColor = texture( u_Texture, v_TexCoord ) * u_Color;
	f_color = texColor;
}