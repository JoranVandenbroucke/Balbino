#ifndef NODE_NORMAL_MAPING
#define NODE_NORMAL_MAPING
void normal_map(vec3 imageNormal, vec3 normal, vec4 tangent, float strength, out vec3 outNormal)
{
    vec3 B = cross(normal, tangent.xyz) * tangent.w;
    vec3 newNormal = normalize(tangent.xyz * imageNormal.x + B * imageNormal.y + normal * imageNormal.z);
    outNormal = mix(normal, newNormal, strength);
}
#endif //NODE_NORMAL_MAPING