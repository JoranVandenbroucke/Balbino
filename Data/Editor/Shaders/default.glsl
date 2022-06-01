#define BALBINO_PI		3.14159265359f
#define BALBINO_2PI		6.28318530718f
#define BALBINO_4PI		12.56637061436f
#define BALBINO_PI_INV	0.31830988618f
#define BALBINO_PI_DIV2	1.57079632679f
#define BALBINO_PI_DIV4	0.78539816339f

//start from crytec common.cfi
vec4 GetTexture2DGrad(in sampler2D tex, vec2 uv, vec2 ddxUV, vec2 ddyUV){return textureGrad(tex, uv, ddxUV, ddyUV);}
vec4 GetTexture2DLod(in sampler2D tex, vec4 uv){return textureLod(tex, uv.xy, uv.w);}
vec4 GetTexture2DLod(in sampler2D tex, vec2 uv, float lod){return textureLod(tex, uv.xy, lod);}
vec4 GetTexture2DProject(in sampler2D tex, vec4 uv){return textureProj(tex, uv,1.0/uv.w);}
vec4 GetTexture2D(in sampler2D tex, vec2 uv){return texture(tex, uv);}
vec4 GetTextureCube(in samplerCube tex, vec3 uv){return texture(tex, uv);}
vec4 GetTextureCubelod(in samplerCube tex, vec4 uv){return textureLod(tex, uv.xyz, uv.w);}

vec2 GetXYNormalMap(in sampler2D tex, vec2 uv ){return GetTexture2D(tex, uv).yx;}
vec2 GetXYNormalMapLod(in sampler2D tex, vec4 uv ){return GetTexture2DLod(tex, uv).yx;}
vec2 GetXYNormalMapGrad(in sampler2D tex, vec2 uv, vec2 ddxUV, vec2 ddyUV ){return GetTexture2DGrad(tex, uv, ddxUV, ddyUV).yx;}

vec3 GetNormalMap(sampler2D bumpTex, vec2 bumpTC)
{
	vec3 bumpNormal;

	bumpNormal.xy = GetXYNormalMap(bumpTex, bumpTC) * 2.0f - 1.0f;
	bumpNormal.xy *= .5f;
	bumpNormal.z = sqrt(clamp(1.0f + dot(bumpNormal.xy, -bumpNormal.xy), 0.0f, 1.0f));
	return normalize(bumpNormal);
}
vec3 GetNormalMapLod(sampler2D bumpTex, vec4 bumpTC)
{
	vec3 bumpNormal;

	bumpNormal.xy = GetXYNormalMapLod(bumpTex, bumpTC) * 2.0f - 1.0f;
	bumpNormal.xy *= .5f;
	bumpNormal.z = sqrt(clamp(1.0f + dot(bumpNormal.xy, -bumpNormal.xy), 0.0f, 1.0f));
	return normalize(bumpNormal);
}
vec3 GetNormalMapGrad(sampler2D bumpTex, vec2 bumpTC, vec2 ddxUV, vec2 ddyUV)
{
	vec3 bumpNormal;

	bumpNormal.xy = GetXYNormalMapGrad(bumpTex, bumpTC, ddxUV, ddyUV) * 2.0f - 1.0f;
	bumpNormal.xy *= .5f;
	bumpNormal.z = sqrt(clamp(1.0f + dot(bumpNormal.xy, -bumpNormal.xy), 0.0f, 1.0f));
	return normalize(bumpNormal);
}

vec3 ProccesNormalMap(vec3 normal, vec3 tangent, vec3 binormal, vec2 uv, sampler2D normalSampler)
{
	mat3 tangentSpaceAxis = transpose(mat3(tangent, binormal, normal));
	vec3 normalMap = GetNormalMap(normalSampler, uv);
	return normalize(tangentSpaceAxis * normalMap);
}
//end from crytec common.cfi

ivec2 GetTextureSize(sampler2D tex, int lod){return textureSize(tex, lod);}
ivec3 GetTextureSize(sampler3D tex, int lod){return textureSize(tex, lod);}
ivec2 GetTextureSize(samplerCube tex, int lod){return textureSize(tex, lod);}
int GetTextureMipCount(sampler2D tex){return textureQueryLevels(tex);}
int GetTextureMipCount(sampler3D tex){return textureQueryLevels(tex);}
int GetTextureMipCount(samplerCube tex){return textureQueryLevels(tex);}
vec3 GetTextureDiffused(in sampler2D tex, vec2 uv){return GetTexture2D(tex, uv).xyz;}
float GetTextureAlpha(in sampler2D tex, vec2 uv){return GetTexture2D(tex, uv).a;}
