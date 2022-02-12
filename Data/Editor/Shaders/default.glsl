#define BALBINO_PI		3.14159265359f
#define BALBINO_2PI		6.28318530718f
#define BALBINO_4PI		12.56637061436f
#define BALBINO_PI_DIV2	1.57079632679f
#define BALBINO_PI_DIV4	0.78539816339f

vec4 GetTextureCUBELevel(in samplerCube tex, vec3 uv, float lod)
{
	return texture(tex, uv, lod);
}
vec4 GetTextureCUBE(in samplerCube tex, vec3 uv)
{
	return texture(tex, uv);
}
vec4 GetDifuse(in sampler2D tex, vec2 uv)
{
	return texture(tex, uv);
}
vec4 GetTexture2D(in sampler2D tex, vec2 uv)
{
	return texture(tex, uv);
}
vec2 GetXYNormalMap(in sampler2D bumpTex, vec2 bumpTC)
{
	return texture(bumpTex, bumpTC).yx;
}

vec3 GetNormalMap(sampler2D bumpTex, vec2 bumpTC)
{
	vec3 bumpNormal;

	bumpNormal.xy = GetXYNormalMap(bumpTex, bumpTC) * 2.0f - 1.0f;
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