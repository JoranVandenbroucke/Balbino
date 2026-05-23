#version 450 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec2 iScale;
layout (location = 3) in vec2 iTranslate;
layout (location = 4) in vec4 iColor;
layout (location = 5) in vec4 iUVRect;

out gl_PerVertex { vec4 gl_Position; };
layout (location = 0) out vec2 outUV;
layout (location = 1) out vec4 outColor;

void main()
{
    outUV = mix(iUVRect.xy, iUVRect.zw, aUV);
    outColor = iColor;
    gl_Position = vec4(aPos * iScale + iTranslate, 0.0, 1.0);
}
