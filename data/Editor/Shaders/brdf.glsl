#ifndef BRDF_GLSL
#define BRDF_GLSL

// Implementation of brdfs found here:
// http://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
// Some implementations came directly from:
// https://google.github.io/filament/Filament.md.html

// Diffuse BRDFs
#define DIFFUSE_LAMBERT                     0
#define DIFFUSE_BURLEY                      1
#define DIFFUSE_OREN_NAYAR                  2

// Specular BRDF
// Normal distribution functions
#define SPECULAR_D_BLIN_PHONG               0
#define SPECULAR_D_BECKMANN                 1
#define SPECULAR_D_GGX                      2

// Visibility functions
#define SPECULAR_S_IMPLICIT                 0
#define SPECULAR_S_NEUMANN                  1
#define SPECULAR_S_COOKTORRANCE             2
#define SPECULAR_S_KELEMEN                  3
#define SPECULAR_S_GGX_CORRELATED           4
#define SPECULAR_S_GGX_CORRELATED_FAST      5
#define SPECULAR_S_KELEMEN_TWO              6
#define SPECULAR_S_NEUBELT                  7

// Fresnel functions
#define SPECULAR_F_COOK_TORRANCE_FRESNEL    0
#define SPECULAR_F_SCHLICK                  1

#define BRDF_DIFFUSE                DIFFUSE_LAMBERT
#define BRDF_SPECULAR_D             SPECULAR_D_GGX
#define BRDF_SPECULAR_V             SPECULAR_S_GGX_CORRELATED_FAST
#define BRDF_SPECULAR_F             SPECULAR_F_SCHLICK

#define MEDIUM_MAX 65504.0
//////////////////////////////////////////////////////
//////////Normal Distribution Function (NDF)//////////
//////////////////////////////////////////////////////
float d_blinn_phong(float a, float NdotH)//a: roughness^2    n: normal   h: half
{
    // Blinn 1977, "Models of light reflection for computer synthesized pictures"
    float rought2 = a * a;
    float p = (1.f/(PI * rought2));
    return p * pow(NdotH, 2.f/(rought2)-2);
}
float d_beckmann(float a, float NdotH)
{
    // Beckmann 1963, "The scattering of electromagnetic waves from rough surfaces"
    float rough2 = a * a;
    float ndh2 = NdotH*NdotH;
    float ndh4 = ndh2*ndh2;
    float p = (1.f/(PI * rough2 * ndh4));
    return exp((ndh2-1)/(rough2*ndh2))*p;
}
float d_ggx(float roughness, float NdotH )
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
float s_implicit(float NdotL, float NdotV)
{
    // Hoffman 2013, "Background: Physics and Math of Shading"
    return NdotL*NdotV;
}
float s_neumann(float NdotL, float NdotV)
{
    // Neumann et al. 1999, "Compact metallic reflectance models"
    return s_implicit(NdotL, NdotV)/max(NdotL, NdotV);
}
float s_cook_torrance(float NdotL, float NdotV, float NdotH, float vdotH)
{
    //Cook and Torrance 1982, "A Reflectance Model for Computer Graphics"
    float d = (2*NdotH)/vdotH;
    return min(min(d*NdotV, d*NdotL), 1);
}
float s_kelemen(float NdotL, float NdotV, float NdotH)
{
    return s_implicit(NdotL,NdotV)/(NdotH*NdotH);
}

/////////////////////////////////////////////
//////////Smith Geometric Shadowing//////////
/////////////////////////////////////////////
float s_beckmann(float NdotV, float roughness)
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
float s_ggx_correlated(float NdotL, float NdotV, float roughness)
{
    // Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"
    float a2 = roughness * roughness;
    // TODO: lambdaV can be pre-computed for all the lights, it should be moved out of this function
    float lambdaV = NdotL * sqrt((NdotV - a2 * NdotV) * NdotV + a2);
    float lambdaL = NdotV * sqrt((NdotL - a2 * NdotL) * NdotL + a2);
    float v = 0.5 / (lambdaV + lambdaL);
    // a2=0 => v = 1 / 4*NdotL*NdotV   => min=1/4, max=+inf
    // a2=1 => v = 1 / 2*(NdotL+NdotV) => min=1/4, max=+inf
    // clamp to the maximum value representable in mediump
    return min(v, MEDIUM_MAX);
}
float s_ggx_correlated_fast(float NdotL, float NdotV, float roughness)
{
    // Hammon 2017, "PBR Diffuse Lighting for GGX+Smith Microsurfaces"
    float v = 0.5 / mix(2.0 * NdotL * NdotV, NdotL + NdotV, roughness);
    return min(v, MEDIUM_MAX);
}
float s_kelemen(float LdotH)
{
    // Kelemen 2001, "A Microfacet Based Coupled Specular-Matte BRDF Model with Importance Sampling"
    return min(0.25 / (LdotH * LdotH), MEDIUM_MAX);
}
float s_neubelt(float NdotL, float NdotV)
{
    // Neubelt and Pettineo 2013, "Crafting a Next-gen Material Pipeline for The Order: 1886"
    return min(1.0 / (4.0 * (NdotL + NdotV - NdotL * NdotV)), MEDIUM_MAX);
}

///////////////////////////
//////////Fresnel//////////
///////////////////////////
float f_cook_torrance (float cosTheta, float f0, float VdotH)
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
vec3 f_cook_torrance (float cosTheta, vec3 f0, float VdotH)
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
float f_schlick(float cosTheta, float f0, float f90)
{
    return f0 + (f90 - f0) * pow(1.0 - cosTheta, 5.0);
}
vec3 f_schlick(float cosTheta, vec3 f0, float f90)
{
    return f0 + (f90 - f0) * pow(1.0 - cosTheta, 5.0);
}
float f_schlick(float cosTheta, float f0)
{
    float f = pow(1.0 - cosTheta, 5.0);
    return f + f0 * (1.0 - f);
}
vec3 f_schlick(float cosTheta, vec3 f0)
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
    float lightScatter = f_schlick(NdotL, vec3(1.0, 1.0, 1.0), f90).x;
    float viewScatter = f_schlick(NdotV, vec3(1.0, 1.0, 1.0), f90).x;//todo: can calculate outside function
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

///////////////////////////
///////////////////////////
float difuse(float NdotV, float NdotL, float LoH, float roughness)
{
    #if BRDF_DIFFUSE == DIFFUSE_LAMBERT
    return Lambert();
    #elif BRDF_DIFFUSE == DIFFUSE_BURLEY
    return Burley(NdotV, NdotL, LoH, roughness);
    #elif BRDF_DIFFUSE == DIFFUSE_OREN_NAYAR
    return OrenNayar(NdotV, NdotL,roughness );
    #endif
}
float distribution(float roughness, float NdotH)
{
    #if BRDF_SPECULAR_D == SPECULAR_D_BLIN_PHONG
    return d_blinn_phong(roughness, NdotH);
    #elif BRDF_SPECULAR_D == SPECULAR_D_BECKMANN
    return d_beckmann(roughness, NdotH);
    #elif BRDF_SPECULAR_D == SPECULAR_D_GGX
    return d_ggx(roughness, NdotH);
    #endif
}
float shadow(float NdotL, float NdotV, float NdotH, float VdotH, float roughness)
{
    #if BRDF_SPECULAR_V == SPECULAR_S_IMPLICIT
    return s_implicit(NdotL, NdotV);
    #elif BRDF_SPECULAR_V == SPECULAR_S_NEUMANN
    return s_neumann(NdotL, NdotV);
    #elif BRDF_SPECULAR_V == SPECULAR_S_COOKTORRANCE
    return s_cook_torrance(NdotL, NdotV, NdotH, VdotH);
    #elif BRDF_SPECULAR_V == SPECULAR_S_KELEMEN
    return s_beckmann(NdotL,roughness)*s_beckmann(NdotV, roughness);
    #elif BRDF_SPECULAR_V == SPECULAR_S_GGX_CORRELATED
    return s_ggx_correlated(NdotL, NdotV, roughness);
    #elif BRDF_SPECULAR_V == SPECULAR_S_GGX_CORRELATED_FAST
    return s_ggx_correlated_fast(NdotL, NdotV, roughness);
    #elif BRDF_SPECULAR_V == SPECULAR_S_KELEMEN_TWO
    return s_kelemen(NdotL)* s_kelemen(NdotV);
    #elif BRDF_SPECULAR_V == SPECULAR_S_NEUBELT
    return s_neubelt(NdotL, NdotV);
    #endif
}
vec3 fresnel(float cosTheta, vec3 f0, float VdotH)
{
    #if BRDF_SPECULAR_F == SPECULAR_F_COOK_TORRANCE_FRESNEL
    return f_cook_torrance(cosTheta, f0, VdotH);
    #elif BRDF_SPECULAR_F == SPECULAR_F_SCHLICK
    return f_schlick(cosTheta, f0);
    #endif
}
#endif  // BRDF_GLSL