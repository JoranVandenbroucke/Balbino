#ifndef NORMAL_GLSL
#define NORMAL_GLSL

vec3 Normal( float strength, vec3 normal, vec4 tangent, vec3 texnormal )
{
    texnormal = 2.0 * vec3(texnormal.r - 0.5, texnormal.g - 0.5, texnormal.b - 0.5);

    vec3 outnormal;
    if (all(equal(tangent, vec4(0.0, 0.0, 0.0, 1.0)))) {
      return normal;
    }
    tangent *= (gl_FrontFacing ? 1.0 : -1.0);
    vec3 B = tangent.w * cross(normal, tangent.xyz);
    
    outnormal = texnormal.x * tangent.xyz + texnormal.y * B + texnormal.z * normal;
    return normalize(outnormal);
}
#endif  // NORMAL_GLSL
