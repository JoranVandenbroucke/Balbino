#ifndef BUMP_GLSL
#define BUMP_GLSL
//Bump Mapping Unparametrized Surfaces on the GPU
void bump(bool invert, float strength, float dist, float height, vec3 normal, vec3 position, out vec3 outNormal)
{
    dist *= gl_FrontFacing ? invert? 1 : -1 : invert? -1 : 1;
    vec3 ddx = dFdx(position);
    vec3 ddy = dFdy(position);
    
    vec3 Rx = cross(ddy, normal);
    vec3 Ry = cross(normal, ddx);
    
    float det = dot(ddx, Rx);
    
    float dBs = dFdxFine  (height);
    float dBt = dFdyFine  (height);
    vec3 vSurfGrad = sign(det) * (dBs * Rx + dBt * Ry) * dist;
    outNormal = normalize(mix(normal, (abs(det) * normal - vSurfGrad), strength));
}

#endif// BUMP_GLSL