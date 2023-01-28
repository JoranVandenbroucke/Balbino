#ifndef DISPLACEMENT_GLSL
#define DISPLACEMENT_GLSL

vec3 Displacement( float height, float midlevel, float scale, vec3 normal )
{
    return (height - midlevel) * scale * normalize(normal);
}
#endif  // DISPLACEMENT_GLSL