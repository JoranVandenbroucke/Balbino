#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 4) in vec4 aColor;

layout(push_constant) uniform uPushConstant {
    vec2 uScale;
    vec2 uTranslate;
} pc;


out gl_PerVertex { vec4 gl_Position; };
layout (location = 0) out vec2 outUV;
layout (location = 1) out vec4 outColor;


void main()
{
    outUV = aUV;
    outColor = aColor;
    gl_Position = vec4(aPos * pc.uScale + pc.uTranslate, 0, 1);
}