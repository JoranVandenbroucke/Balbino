#ifndef MAPPING_GLSL
#define MAPPING_GLSL

#include "mathUtil.glsl"

void mapping_point(vec3 vector, vec3 location, vec3 rotation, vec3 scale, out vec3 outVec)
{
  outVec = (EulerToMat3(rotation) * (vector * scale)) + location;
}

void mapping_texture(vec3 vector, vec3 location, vec3 rotation, vec3 scale, out vec3 outVec)
{
  outVec = SafeDivide(transpose(EulerToMat3(rotation)) * (vector - location), scale);
}

void mapping_vector(vec3 vector, vec3 location, vec3 rotation, vec3 scale, out vec3 outVec)
{
  outVec = EulerToMat3(rotation) * (vector * scale);
}

void mapping_normal(vec3 vector, vec3 location, vec3 rotation, vec3 scale, out vec3 outVec)
{
  outVec = normalize(EulerToMat3(rotation) * SafeDivide(vector, scale));
}

#endif  // MAPPING_GLSL