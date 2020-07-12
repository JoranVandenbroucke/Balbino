//https://learnopengl.com/Advanced-OpenGL/Framebuffers
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0); 
    TexCoords = a_TexCoord;
}