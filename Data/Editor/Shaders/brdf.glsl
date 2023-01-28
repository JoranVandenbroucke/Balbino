#ifndef BRDF_GLSL
#define BRDF_GLSL
#include "default.glsl"

// Implementation of brdfs found here:
// http://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
// Some implementations came directly from:
// https://google.github.io/filament/Filament.md.html

//////////////////////////////////////////////////////
//////////Normal Distribution Function (NDF)//////////
//////////////////////////////////////////////////////
#define MEDIUM_MAX 65504.0

float BlinnPhongNDF(float a, float NdotH)//a: roughness^2    n: normal   h: half
{
    // Blinn 1977, "Models of light reflection for computer synthesized pictures"
    float rought2 = a * a;
    float p = (1.f/(PI * rought2));
    return p * pow(NdotH, 2.f/(rought2)-2);
}
float BeckmannNDF(float a, float NdotH)
{
    // Beckmann 1963, "The scattering of electromagnetic waves from rough surfaces"
    float rough2 = a * a;
    float ndh2 = NdotH*NdotH;
    float ndh4 = ndh2*ndh2;
    float p = (1.f/(PI * rough2 * ndh4));
    return exp((ndh2-1)/(rough2*ndh2))*p;
}
float GGXNDF(float NdotH, float roughness )
{
    // Walter et al. 2007, "Microfacet models for refraction through rough surfaces"
    float oneMinusNoHSquared = 1.0 - NdotH * NdotH;
    float a = NdotH * roughness;
    float k = roughness / (oneMinusNoHSquared + a * a);
    return min(k * k * (1.0 / PI), MEDIUM_MAX);
}

///////////////////////////////////////
//////////Geometric Shadowing//////////
///////////////////////////////////////

float Implicitshadow(float NdotL, float NdotV)
{
    // Hoffman 2013, "Background: Physics and Math of Shading"
    return NdotL*NdotV;
}
float Neumannshadow(float NdotL, float NdotV)
{
    // Neumann et al. 1999, "Compact metallic reflectance models"
    return Implicitshadow(NdotL, NdotV)/max(NdotL, NdotV);
}
float CookTorranceshadow(float NdotL, float NdotV, float NdotH, float vdotH)
{
    //Cook and Torrance 1982, "A Reflectance Model for Computer Graphics"
    float d = (2*NdotH)/vdotH;
    return min(min(d*NdotV, d*NdotL), 1);
}
float Kelemenshadow(float NdotL, float NdotV, float NdotH)
{
    return Implicitshadow(NdotL,NdotV)/(NdotH*NdotH);
}

/////////////////////////////////////////////
//////////Smith Geometric Shadowing//////////
/////////////////////////////////////////////

float Beckmannshadow(float NdotV, float roughness)
{
    // Walter et al. 2007, "Microfacet models for refraction through rough surfaces"
    //c=(n⋅v)/α√(1−(n⋅v)^2)
    float c = NdotV/(roughness*sqrt(1-NdotV*NdotV));
    if (c < 1.6f)
    {
        float c2 = c*c;
        return (3.535*c+2.181*c2)/(1+2.276*c+2.577*c2);
    }
    return 1;
}
float GGXCorrelatedshadow(float roughness, float NoV, float NoL)
{
    // Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"
    float a2 = roughness * roughness;
    // TODO: lambdaV can be pre-computed for all the lights, it should be moved out of this function
    float lambdaV = NoL * sqrt((NoV - a2 * NoV) * NoV + a2);
    float lambdaL = NoV * sqrt((NoL - a2 * NoL) * NoL + a2);
    float v = 0.5 / (lambdaV + lambdaL);
    // a2=0 => v = 1 / 4*NoL*NoV   => min=1/4, max=+inf
    // a2=1 => v = 1 / 2*(NoL+NoV) => min=1/4, max=+inf
    // clamp to the maximum value representable in mediump
    return min(v, MEDIUM_MAX);
}
float GGXCorrelatedFastshadow(float roughness, float NoV, float NoL)
{
    // Hammon 2017, "PBR Diffuse Lighting for GGX+Smith Microsurfaces"
    float v = 0.5 / mix(2.0 * NoL * NoV, NoL + NoV, roughness);
    return min(v, MEDIUM_MAX);
}
float Kelemenshadow(float LdotH)
{
    // Kelemen 2001, "A Microfacet Based Coupled Specular-Matte BRDF Model with Importance Sampling"
    return min(0.25 / (LdotH * LdotH), MEDIUM_MAX);
}
float Neubeltshadow(float NoV, float NoL)
{
    // Neubelt and Pettineo 2013, "Crafting a Next-gen Material Pipeline for The Order: 1886"
    return min(1.0 / (4.0 * (NoL + NoV - NoL * NoV)), MEDIUM_MAX);
}

///////////////////////////
//////////Fresnel//////////
///////////////////////////
float CookTorranceFresnel (float cosTheta, float f0, float VdotH)
{
    // Cook and Torrance 1982, "A Reflectance Model for Computer Graphics"
    float n = (1+sqrt(f0))/(1-sqrt(f0));
    float g = sqrt(n*n+VdotH*VdotH-1);
    
    float gmc = g-VdotH;
    float gpc = g+VdotH;
    float gcdiv = gmc/gpc;
    float k = ((gpc)*VdotH-1)/((gmc)*VdotH+1);
    return 1/2*gcdiv*gcdiv*(1+k*k);
}
vec3 CookTorranceFresnel (float cosTheta, vec3 f0, float VdotH)
{
    // Cook and Torrance 1982, "A Reflectance Model for Computer Graphics"
    vec3 n = (1+sqrt(f0))/(1-sqrt(f0));
    vec3 g = sqrt(n*n+VdotH*VdotH-1);
    
    vec3 gmc = g-VdotH;
    vec3 gpc = g+VdotH;
    vec3 gcdiv = gmc/gpc;
    vec3 k = ((gpc)*VdotH-1)/((gmc)*VdotH+1);
    return 1/2*gcdiv*gcdiv*(1+k*k);
}
float SchlickFresnel(float cosTheta, float f0, float f90)
{
    return f0 + (f90 - f0) * pow(1.0 - cosTheta, 5.0);
}
vec3 SchlickFresnel(float cosTheta, vec3 f0, float f90)
{
    return f0 + (f90 - f0) * pow(1.0 - cosTheta, 5.0);
}
float SchlickFresnel(float cosTheta, float f0)
{
    float f = pow(1.0 - cosTheta, 5.0);
    return f + f0 * (1.0 - f);
}
vec3 SchlickFresnel(float cosTheta, vec3 f0)
{
    float f = pow(1.0 - cosTheta, 5.0);
    return f + f0 * (1.0 - f);
}

///////////////////////////
//////////Diffuse//////////
///////////////////////////

float Lambert()
{
    return PI_INV;
}
float Burley(float NdotV, float NdotL, float LdotH, float roughness)
{
    float f90 = 0.5 + 2.0 * roughness * sqr(LdotH);
    float lightScatter = SchlickFresnel(NdotL, vec3(1.0, 1.0, 1.0), f90).x;
    float viewScatter = SchlickFresnel(NdotV, vec3(1.0, 1.0, 1.0), f90).x;//todo: can calculate outside function
    return lightScatter * viewScatter * Lambert();
}
float OrenNayar(float NdotV, float NdotL, float roughness )
{
    if(roughness == 0)
    return Lambert() * NdotL;
    
    float alpha = max(NdotV, NdotL);
    float beta = min(NdotV, NdotL);
    float gamma = cos(NdotV - NdotL);
    
    float roughness2 = roughness * roughness;
    
    float A = 1.0 - 0.5 * (roughness2 / (roughness2 + 0.57));
    float B = 0.45 * (roughness2 / (roughness2 + 0.09));
    float C = sin(alpha) * tan(beta);
    
    return Lambert() * NdotL * (A + (B * max(0.0, gamma) * C));
}

#endif  // BRDF_GLSL