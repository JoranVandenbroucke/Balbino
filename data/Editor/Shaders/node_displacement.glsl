#ifndef DISPLACEMENT_GLSL
#define DISPLACEMENT_GLSL

void displacement( float height, float midlevel, float scale, vec3 normal, out vec3 outVector)
{
    outVector = (height - midlevel) * scale * normalize(normal);
}
#endif  // DISPLACEMENT_GLSL