//I created this shader by reading
//https://google.github.io/filament/Filament.md.html

#version 450
#include "default.glsl"

#define LIGHT_COUNT 1024
#define MEDIUM_MAX 65504.0
#define SATUREATE_MADIUM(x) min(x, MEDIUM_MAX)

layout(set=1, binding=4) uniform samplerCube gRadEnvironment;
layout(set=1, binding=5) uniform samplerCube gIrradEnvironment;
layout(set=1, binding=6) uniform sampler2D gBRDFLUP;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragTangent;
layout(location = 4) in vec4 fragWorldPos;

layout (location = 0) out vec4 outFragcolor;

struct Light {
    int type;               //type: Directional, Point, Spot, Area
    float strength;         //strength
    vec3 position;          //position
    vec3 direction;         //direction
    vec3 color;             //color
    vec3 size;              //Point: size,0,0; Spot: size, front, back; area: width, height, 0
};


layout(set=0, binding=0) uniform ModelData {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 viewPos;
    int displayDebugTarget;
} modelBufferObject;

layout(set=0, binding=1) uniform LightData
{
    Light lights[LIGHT_COUNT];
};

float sqr(float x) { return x * x; }

float GGX(float NdotH, float roughness, const vec3 n, const vec3 h)
{
    vec3 NxH = cross(n, h);
    float a = NdotH * roughness;
    float k = roughness / (dot(NxH, NxH) + a * a);
    float d = k * k * (1.0 / BALBINO_PI);
    return SATUREATE_MADIUM(d);
}

float SmithGGXCorrelated(float NdotV, float NdotL, float roughness)
{
    float a = roughness;
    float GGXV = NdotL * (NdotV * (1.0 - a) + a);
    float GGXL = NdotV * (NdotL * (1.0 - a) + a);
    return 0.5 / (GGXV + GGXL);
}

vec3 FresnelSchlick(float cosTheta, vec3 f0)
{
    float f = pow(1.0 - cosTheta, 5.0);
    return f + f0 * (1.0 - f);
}

vec3 FresnelSchlick(float cosTheta, vec3 f0, float f90)
{
    return f0 + (f90 - f0) * pow(1.0 - cosTheta, 5.0);
}

float Lambert() {
    return BALBINO_PI_INV;
}

float Burley(float NdotV, float NdotL, float LdotH, float roughness) {
    float f90 = 0.5 + 2.0 * roughness * sqr(LdotH);
    float lightScatter = FresnelSchlick(NdotL, vec3(1.0, 1.0, 1.0), f90).x;
    float viewScatter = FresnelSchlick(NdotV, vec3(1.0, 1.0, 1.0), f90).x;
    return lightScatter * viewScatter * Lambert();
}

void main()
{
    if (modelBufferObject.displayDebugTarget >= 1.0)
    {
        outFragcolor = vec4((fragNormal + vec3(1)) * vec3(0.5), 1);
        return;
    }
    else if (modelBufferObject.displayDebugTarget >= 2.0)
    {
        vec3 view = normalize(modelBufferObject.viewPos.xyz - fragWorldPos.xyz);
        outFragcolor = vec4((view + vec3(1)) * vec3(0.5), 1);
        return;
    }
    else if (modelBufferObject.displayDebugTarget >= 1.0)
    {
        Light light = lights[0];
        vec3 l = normalize(modelBufferObject.viewPos.xyz - fragWorldPos.xyz);
        outFragcolor = vec4((l + vec3(1)) * vec3(0.5), 1);
        return;
    }
    float metallic = 0;
    float perceptualRoughness = 0.5;
    float ambientOcclusion = 1;
    vec3 baseColor = (1.0 - metallic) * fragColor.rgb;

    vec3 n = normalize(fragNormal);
    vec3 v = normalize(modelBufferObject.viewPos.xyz - fragWorldPos.xyz);
    float NdotV = abs(dot(n, v)) + 1e-5;

    //TODO: implement this when I have a dfg texture
    //vec3 energyCompensation = 1.0 + f0 * (1.0 / dfg.y - 1.0);
    vec3 directColor = vec3(0.0);
    for (int i = 0; i < LIGHT_COUNT; ++i)
    {
        Light light = lights[i];
        if(light.strength < 0)
            break;

        vec3 Fd = vec3(0, 0, 0);
        vec3 Fr = vec3(0.0);

        vec3 l = normalize(light.position.xyz - fragWorldPos.xyz);
        vec3 h = normalize(n + l);

        float NdotL = max(dot(n, l), 0.0);
        float NdotH = max(dot(n, h), 0.0);
        float LdotH = max(dot(l, h), 0.0);

        // perceptually linear roughness to roughness (see parameterization)
        float roughness = perceptualRoughness * perceptualRoughness;
        if (NdotL > 0.0)
        {
            vec3 f0 = 0.16 * sqr(roughness) * (1.0 - metallic) + baseColor * metallic;
            float D = GGX(NdotH, roughness, n, h);
            vec3  F = FresnelSchlick(LdotH, f0);
            float V = SmithGGXCorrelated(NdotV, NdotL, roughness);

            // specular BRDF
            Fr = (D * V) * F;

            //TODO: implement this when I have a dfg texture
            //vec3 energyCompensation = 1.0 + f0 * (1.0 / dfg.y - 1.0);
            //// Scale the specular lobe to account for multiscattering
            //Fr *= pixel.energyCompensation;
        }

        // diffuse BRDF
        Fd = baseColor * Burley(NdotV, NdotL, LdotH, roughness);
        directColor+=(Fd + Fr) * (NdotL * ambientOcclusion) * light.color * light.strength;
    }
    //todo add clearcoat brdf
    //todo add Anisotropic brdf
    outFragcolor = vec4(directColor, fragColor.a);
}