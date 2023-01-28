#ifndef DEFAULT_GLSL
#define DEFAULT_GLSL

#define PI          3.14159265359f
#define PI2         6.28318530718f
#define PI4         12.56637061436f
#define PI_INV      0.31830988618f
#define PI_DIV2     1.57079632679f
#define PI_DIV4     0.78539816339f

// 2D
vec4 GetTexture2D(in sampler2D tex, vec2 uv){ return texture(tex, uv); }
vec4 GetTexture2DGrad(in sampler2D tex, vec2 uv, vec2 ddxUV, vec2 ddyUV){ return textureGrad(tex, uv, ddxUV, ddyUV); }
vec4 GetTexture2DLod(in sampler2D tex, vec2 uv, float lod){ return textureLod(tex, uv.xy, lod); }
vec4 GetTexture2DProject(in sampler2D tex, vec3 uv){ return textureProj(tex, uv); }
vec4 GetTexture2DProject(in sampler2D tex, vec4 uv){ return textureProj(tex, uv); }

// 3D
vec4 GetTexture3D(in sampler3D tex, vec3 uv){ return texture(tex, uv); }
vec4 GetTexture3DGrad(in sampler3D tex, vec3 uv, vec3 ddxUV, vec3 ddyUV){ return textureGrad(tex, uv, ddxUV, ddyUV); }
vec4 GetTexture3DLod(in sampler3D tex, vec3 uv, float lod){ return textureLod(tex, uv.xy, lod); }
vec4 GetTexture3DProject(in sampler3D tex, vec4 uv){ return textureProj(tex, uv); }

// Cube
vec4 GetTextureCube(in samplerCube tex, vec3 uv){ return texture(tex, uv); }
vec4 GetTextureCubeGrad(in samplerCube tex, vec3 uv, vec3 ddxUV, vec3 ddyUV){ return textureGrad(tex, uv, ddxUV, ddyUV); }
vec4 GetTextureCubeLod(in samplerCube tex, vec3 uv, float lod){ return textureLod(tex, uv.xy, lod); }

// 2DArray
vec4 GetTexture2DArray(in sampler2DArray tex, vec3 uv){ return texture(tex, uv); }
vec4 GetTexture2DArrayGrad(in sampler2DArray tex, vec3 uv, vec2 ddxUV, vec2 ddyUV){ return textureGrad(tex, uv, ddxUV, ddyUV); }
vec4 GetTexture2DArrayLod(in sampler2DArray tex, vec3 uv, float lod){ return textureLod(tex, uv.xy, lod); }

// Normal map
vec2 GetXYNormalMap(in sampler2D tex, vec2 uv){ return GetTexture2D(tex, uv).yx; }
vec2 GetXYNormalMapLod(in sampler2D tex, vec4 uv){ return GetTexture2DLod(tex, uv).yx; }
vec2 GetXYNormalMapGrad(in sampler2D tex, vec2 uv, vec2 ddxUV, vec2 ddyUV){ return GetTexture2DGrad(tex, uv, ddxUV, ddyUV).yx; }

vec3 GetNormalMap(sampler2D bumpTex, vec2 bumpTC){
    vec3 bumpNormal;
    bumpNormal.xy = GetXYNormalMap(bumpTex, bumpTC) * 2.0f - 1.0f;
    bumpNormal.z = sqrt(clamp(1.0f + dot(bumpNormal.xy, -bumpNormal.xy), 0.0f, 1.0f));
    return normalize(bumpNormal);
}
vec3 GetNormalMapLod(sampler2D bumpTex, vec4 bumpTC){
    vec3 bumpNormal;
    bumpNormal.xy = GetXYNormalMapLod(bumpTex, bumpTC) * 2.0f - 1.0f;
    bumpNormal.z = sqrt(clamp(1.0f + dot(bumpNormal.xy, -bumpNormal.xy), 0.0f, 1.0f));
    return normalize(bumpNormal);
}
vec3 GetNormalMapGrad(sampler2D bumpTex, vec2 bumpTC, vec2 ddxUV, vec2 ddyUV){
    vec3 bumpNormal;
    bumpNormal.xy = GetXYNormalMapGrad(bumpTex, bumpTC, ddxUV, ddyUV) * 2.0f - 1.0f;
    bumpNormal.z = sqrt(clamp(1.0f + dot(bumpNormal.xy, -bumpNormal.xy), 0.0f, 1.0f));
    return normalize(bumpNormal);
}

vec3 ProccesNormalMap(vec3 normal, vec3 tangent, vec3 texNormal){
    vec3 bitangent = tangent.w * cross(normal, tangent.xyz);
    return normalize(texNormal.x * tangent.xyz + texNormal.y * bitangent + texnormal.z * normal);
}

// Size
ivec2 GetTextureSize(sampler2D tex, int lod){ return textureSize(tex, lod); }
ivec3 GetTextureSize(sampler3D tex, int lod){ return textureSize(tex, lod); }
ivec2 GetTextureSize(samplerCube tex, int lod){ return textureSize(tex, lod); }

// Mip
int GetTextureMipCount(sampler2D tex){ return textureQueryLevels(tex); }
int GetTextureMipCount(sampler3D tex){ return textureQueryLevels(tex); }
int GetTextureMipCount(samplerCube tex){ return textureQueryLevels(tex); }

// Extra Sample
float GetTextureAlpha(in sampler2D tex, vec2 uv){ return GetTexture2D(tex, uv).a; }

// Helper Functions
float sqr(float x) { return x * x; }

#endif  // DEFAULT_GLSL