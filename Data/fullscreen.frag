#version 450
layout (location = 0) in vec2 inUv;
layout (location = 0) out vec4 outColor;
void main()
{
    outColor = vec4(inUv.x, inUv.y, 0, 1);
}