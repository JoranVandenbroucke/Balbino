#version 450
#include "default.glsl"

layout (binding = 1) uniform sampler2D samplerDifuse;
layout (binding = 2) uniform sampler2D samplerSRM;
layout (binding = 5) uniform sampler2D samplerNormal;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragTangent;
layout(location = 4) in vec3 fragBinormal;
layout(location = 5) in vec4 fragWorldPosition;

layout (location = 0) out vec4 outFragcolor;

struct Light {
	vec4 position;
	vec3 color;
	float radius;
};

#define LIGHT_COUNT 6
layout (set = 0, binding = 1) uniform UBO 
{
	Light lights[LIGHT_COUNT];
	vec4 viewPos;
	int displayDebugTarget;
} ubo;

vec3 LambertDifuse(vec3 color, vec3 reflectance)
{
	return (color * reflectance) / BALBINO_PI;
}

float DistributionGGX(float cosTheta, float alpha)
{
	alpha *= alpha;
	float ct2 = cosTheta * cosTheta;
	float pow2 = (ct2 * (alpha - 1.f) + 1.f);
	pow2 *= pow2;
	return (alpha) / (float(BALBINO_PI) * pow2);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
//	float pow5 = 1 - cosTheta;
//	pow5 *= pow5 * pow5 * pow5 * pow5;
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);;
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
//	float pow5 = 1 - cosTheta;
//	pow5 *= pow5 * pow5 * pow5 * pow5;
//	float oneMinR = 1.0 - roughness;
//	return F0 + (max(vec3(oneMinR, oneMinR, oneMinR), F0) - F0) * pow5;
	return F0 + ((1.0f-roughness) - F0) * pow(1.0 - cosTheta, 5.0);
}

float GeometrySchlickGGX(float cosTheta, float roughness)
{
	return cosTheta / (cosTheta * (1.f - roughness) + roughness);
}

float GeometrySmith(float cosView, float cosLight, float roughness)
{
	float ggx2 = GeometrySchlickGGX(cosView, roughness);
	float ggx1 = GeometrySchlickGGX(cosLight, roughness);

	return ggx1 * ggx2;
}

void main()
{
	vec4 albedo = GetDifuse(samplerDifuse, fragTexCoord);
	vec3 srm = GetTexture2D(samplerSRM, fragTexCoord).rgb;
	vec3 normal = ProccesNormalMap(fragNormal, fragTangent, fragBinormal, fragTexCoord, samplerNormal);

	float specular = srm.r;
	float roughness = srm.g;
	float metalic = srm.b;

	if (ubo.displayDebugTarget > 0) {
		switch (ubo.displayDebugTarget) {
			case 1: 
				outFragcolor.rgb = albedo.rgb;
				break;
			case 2: 
				outFragcolor.rgb = vec3(specular);
				break;
			case 3: 
				outFragcolor.rgb = vec3(roughness);
				break;
				outFragcolor.rgb = vec3(metalic);
			case 4: 
				outFragcolor.rgb = vec3(normal);
				break;
			case 5: 
				outFragcolor.rgb = vec3(fragWorldPosition);
				break;
		}		
		outFragcolor.a = 1.0;
		return;
	}

	vec3 view = normalize(ubo.viewPos.xyz - fragWorldPosition.xyz);
	vec3 reflectVec = reflect(view, normal);

	//schlick ggx rouchnes remaped (directlicht)
	float k = roughness * roughness + 1;
	k *= k;
	
	//Snell’s Law
	vec3 F0 = mix(vec3(specular, specular, specular), albedo.xyz, metalic);
	
	float VdotN = max(dot(view, normalize(normal)), 0);
	
	vec3 directLight = vec3(0, 0, 0);
	for(int i = 0; i < LIGHT_COUNT; ++i)
	{
		vec3 lightDir = -normalize(ubo.lights[i].position.xyz - fragWorldPosition.xyz);
		vec3 halfVector = normalize(lightDir + view);
		float LdotN = max(dot(lightDir, normal), 0);
		float HdotN = max(dot(halfVector, normal), 0);
	
		vec3 fresnel = FresnelSchlick(HdotN, F0);
		float normalDistributution = DistributionGGX(HdotN, roughness * roughness);
		float geometry = GeometrySmith(VdotN, LdotN, k);
		vec3 cookTorrance = (fresnel * geometry * normalDistributution) / (4 * VdotN * LdotN + 0.001f); // 0.001 to prevent divide by zero.
	
		vec3 Kd = 1 - fresnel;
		Kd *= 1 - metalic;
		//float shadowValue = ShadowContribution(fragLPos);
		//float shadowValue = Shadow(fragLPos, LdotN);
		float shadowValue = 1.0f;
		directLight += (LambertDifuse(albedo.xyz, Kd) + cookTorrance) * LdotN * shadowValue;
	}

	vec3 ambientLighting = vec3(0, 0, 0);
	{
		// ambient lighting (we now use IBL as the ambient term)
//		vec3 kS = FresnelSchlickRoughness(VdotN, F0, roughness);
//		vec3 kD = 1.0 - kS;
//		kD *= 1.0 - metallic;
	
//		vec3 irradiance = GetTextureCUBE(gIrradEnvironment, normal).xyz;
//		vec3 diffuse = irradiance * albedo.rgb * kD;

		// sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
//		vec3 prefilteredColor = GetTextureCUBELevel(gRadEnvironment, reflectVec, roughness * 4).xyz;
//		vec2 brdf = texture(gBRDFLUP, vec2(VdotN, roughness)).rg;
//		vec3 specularColor = prefilteredColor * (kS * brdf.x + brdf.y);
//		vec3 ambient = diffuse + specularColor;
	
//		ambientLighting = ambient;

		// ambient lighting (we now use IBL as the ambient term)
		vec3 kS = FresnelSchlickRoughness(VdotN, F0, roughness);
		vec3 kD = 1.0 - kS;
		kD *= 1.0 - metalic;
		ambientLighting = vec3(228, 241, 246)/255.f * albedo.rgb * kD + vec3(248, 251, 255)/255.f * kS;
	}
    outFragcolor = vec4(directLight + ambientLighting, albedo.a);
}