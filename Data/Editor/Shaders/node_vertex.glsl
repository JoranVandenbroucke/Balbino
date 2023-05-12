#ifndef VERTEX_GLSL
#define VERTEX_GLSL

void vertex(
vec3 inPos,
mat4 proj,

#if defined(USED_MODEL_VIEW)
mat4 view,
mat4 model,
#endif

#if defined(USES_WORD_POSITION)
out vec4 worldPos,
#endif

#if defined(USES_NORMAL)
vec3 inNormal,
out vec3 normal,
#endif

#if defined(USES_TANGENT)
vec4 inTangent,
out vec4 tangent,
#endif

#if defined(USES_COLOUR)
vec4 inColor,
out vec4 color,
#endif

#if defined(USES_UV)
vec2 inUV,
out vec2 uv,
#endif

#if defined(USES_POSITION_OFFSET)
vec3 offset,
#endif

out vec4 position
)
{
    #if defined(USED_MODEL_VIEW)
    position = proj * view * model * vec4(inPos+offset, 1.0);
    #else
    position = proj * vec4(inPos, 1.0);
    #endif
    
    #if defined(USES_WORD_POSITION)
    worldPos = model * vec4(inPos, 1.0f);
    #endif
    
    #if defined(USES_NORMAL)
    normal = normalize(mat3(model) * inNormal);
    #if defined(USES_TANGENT)
    tangent = vec4(normalize(mat3(model) * inTangent.xyz), inTangent.w);
    #endif
    #endif
    
    #if defined(USES_COLOUR)
    color=inColor;
    #endif
    
    #if defined(USES_UV)
    //todo: support multiple uv layers
    uv=inUV;
    #endif
}

#endif//   VERTEX_GLSL
