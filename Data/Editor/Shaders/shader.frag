#version 450
#include "default.glsl"

layout(set=1, binding=4) uniform samplerCube gRadEnvironment;
layout(set=1, binding=5) uniform samplerCube gIrradEnvironment;
layout(set=1, binding=6) uniform sampler2D gBRDFLUP;

layout( location = 0) in vec4 fragColor;
layout( location = 1 ) in vec2 fragTexCoord;
layout( location = 2 ) in vec3 fragNormal;
layout( location = 3 ) in vec3 fragTangent;
layout( location = 4 ) in vec3 fragBinormal;
layout( location = 5 ) in vec4 fragWorldPosition;

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

float sqr(float x) { return x*x; }

float SchlickFresnel(float u)
{
    float m = clamp(1-u, 0, 1);
    float m2 = m * m;
    return m2 * m2 * m; // pow(m,5)
}
vec3 SchlickFresnelRough(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
float GTR1(float NdotH, float a)
{
    if (a >= 1) return 1 / BALBINO_PI;
    float a2 = a * a;
    float t = 1 + (a2 - 1) * NdotH * NdotH;
    return (a2 - 1) / (BALBINO_PI * log(a2) * t);
}

float GTR2(float NdotH, float a)
{
    float a2 = a * a;
    float t = 1 + (a2 - 1) * NdotH * NdotH;
    return a2 / (BALBINO_PI * t * t);
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)
{
    return 1 / (BALBINO_PI * ax * ay * sqr( sqr(HdotX / ax) + sqr(HdotY / ay) + NdotH * NdotH ));
}

float smithG_GGX(float NdotV, float alphaG)
{
    float a = alphaG * alphaG;
    float b = NdotV * NdotV;
    return 1 / (NdotV + sqrt(a + b - a * b));
}

float smithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay)
{
    return 1 / (NdotV + sqrt( sqr(VdotX * ax) + sqr(VdotY * ay) + sqr(NdotV) ));
}

vec3 mon2lin(vec3 x)
{
    return vec3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
}

void main(void)
{
	vec3 baseColor = vec3(0.820000, 0.670000, 0.160000 ); 
	float metallic = 0.000000;
	float subsurface = 0.000000;
	float specular = 0.500000;
	float roughness = 0.500000;
	float specularTint = 0.000000;
	float anisotropic = 0.000000;
	float sheen = 0.000000;
float sheenTint = 0.500000;
	float clearcoat = 0.000000;
	float clearcoatGloss = 0.100000;
float alpha = 1.000000;
vec3 normal = normalize(fragNormal);
	vec3 tangent = normalize( fragTangent );
	vec3 bitangent = normalize( fragTangent );
	vec3 view = normalize(ubo.viewPos.xyz - fragWorldPosition.xyz);
	vec3 directLight = vec3(0, 0, 0);
	float NdotV = dot(normal,view);
	float FV = SchlickFresnel(NdotV);
	
	vec3 Cdlin = mon2lin(baseColor);
	float Cdlum = .3 * Cdlin[0] + .6 * Cdlin[1]  + .1 * Cdlin[2]; // luminance approx.
	vec3 Ctint = Cdlum > 0 ? Cdlin/Cdlum : vec3(1); // normalize lum. to isolate hue+sat
	vec3 Cspec0 = mix(specular * .08 * mix(vec3(1), Ctint, specularTint), Cdlin, metallic);
	vec3 Csheen = mix(vec3(1), Ctint, sheenTint);
	
	// specular
	float aspect = sqrt(1 - anisotropic * .9);
	float ax = max(.001, sqr(roughness )/ aspect);
	float ay = max(.001, sqr(roughness) * aspect);
	
	for(int i = 0; i < LIGHT_COUNT; ++i)
	{
		vec3 lightDir = -normalize(ubo.lights[i].position.xyz - fragWorldPosition.xyz);
		float NdotL = dot(normal,lightDir);
		if (NdotL < 0 || NdotV < 0) 
		{
			continue;
		}

		vec3 H = normalize(lightDir + view);
		float NdotH = dot(normal, H);
		float LdotH = dot(lightDir, H);

		// Diffuse fresnel - go from 1 at normal incidence to .5 at grazing
		// and mix in diffuse retro-reflection based on roughness
		float FL = SchlickFresnel(NdotL);
		float Fd90 = 0.5 + 2 * LdotH * LdotH * roughness;
		float Fd = mix(1.0, Fd90, FL) * mix(1.0, Fd90, FV);

		// Based on Hanrahan-Krueger brdf approximation of isotropic bssrdf
		// 1.25 scale is used to (roughly) preserve albedo
		// Fss90 used to "flatten" retroreflection based on roughness
		float Fss90 = LdotH * LdotH * roughness;
		float Fss = mix(1.0, Fss90, FL) * mix(1.0, Fss90, FV);
		float ss = 1.25 * (Fss * (1 / (NdotL + NdotV) - .5) + .5);

		// specular
		float Ds = GTR2_aniso(NdotH, dot(H, tangent), dot(H, bitangent), ax, ay);
		float FH = SchlickFresnel(LdotH);
		vec3 Fs = mix(Cspec0, vec3(1), FH);
		float Gs;
		Gs  = smithG_GGX_aniso(NdotL, dot(lightDir, tangent), dot(lightDir, bitangent), ax, ay);
		Gs *= smithG_GGX_aniso(NdotV, dot(view, tangent), dot(view, bitangent), ax, ay);

		// sheen
		vec3 Fsheen = FH * sheen * Csheen;

		// clearcoat (ior = 1.5 -> F0 = 0.04)
		float Dr = GTR1(NdotH, mix(.1,.001,clearcoatGloss));
		float Fr = mix(.04, 1.0, FH);
		float Gr = smithG_GGX(NdotL, .25) * smithG_GGX(NdotV, .25);
		float lightStrength = dot(ubo.lights[i].position.xyz - fragWorldPosition.xyz, ubo.lights[i].position.xyz - fragWorldPosition.xyz);
		directLight += (((1 / BALBINO_PI) * mix(Fd, ss, subsurface) * Cdlin + Fsheen)
		    * (1-metallic)
		    + Gs * Fs * Ds + .25 * clearcoat * Gr * Fr * Dr) * ubo.lights[i].color * lightStrength;
	}

	vec3 ambientLighting = vec3(0, 0, 0);
	{
		vec3 F0 = mix(vec3(0.04f), baseColor, metallic);

		//ambient lighting (we now use IBL as the ambient term)
		vec3 kS = SchlickFresnelRough(NdotV, F0, roughness); 
		vec3 kD = 1.0 - kS;
		kD *= 1.0 - metallic;
	
		vec3 irradiance = GetTextureCUBE(gIrradEnvironment, normal).xyz;
		vec3 diffuse = irradiance * baseColor * kD;

		// sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
		vec3 reflectVec = reflect(-view, normal);
		vec3 prefilteredColor = GetTextureCUBELevel(gRadEnvironment, reflectVec, roughness * 4).xyz;
		vec2 brdf = texture(gBRDFLUP, vec2(NdotV, roughness)).rg;
		vec3 specularColor = prefilteredColor * (kS * brdf.x + brdf.y);
		vec3 ambient = diffuse + specularColor;
	
		ambientLighting = ambient;
	}
    outFragcolor = vec4(directLight /*+ ambientLighting*/, alpha);
}