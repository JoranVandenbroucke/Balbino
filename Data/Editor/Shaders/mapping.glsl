#include "mathUtil.glsl"

vec3 MappingPoint(vec3 vector, vec3 location, vec3 rotation, vec3 scale)
{
  return (EulerToMat3(rotation) * (vector * scale)) + location;
}

vec3 MappingTexture(vec3 vector, vec3 location, vec3 rotation, vec3 scale)
{
  return SafeDivide(transpose(EulerToMat3(rotation)) * (vector - location), scale);
}

vec3 MappingVector(vec3 vector, vec3 location, vec3 rotation, vec3 scale)
{
  return EulerToMat3(rotation) * (vector * scale);
}

vec3 MappingNormal(vec3 vector, vec3 location, vec3 rotation, vec3 scale)
{
  return normalize(EulerToMat3(rotation) * SafeDivide(vector, scale));
}
