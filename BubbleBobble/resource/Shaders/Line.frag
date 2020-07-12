#version 330 core

layout( location = 0 ) out vec4 f_color;

uniform vec4 u_Color;

void main()
{
    f_color = u_Color;
}