#include "FragmentOutput.h"

#include <string>
#include <format>
#include <sstream>

#include "Attribute.h"
#include "imgui.h"
#include "imnodes.h"

CFragmentOutputNode::CFragmentOutputNode(int id, int& attributeStartId)
        : INode{ id, attributeStartId }
          , m_connections{}
{
    attributeStartId += m_connectionSize;
}

void CFragmentOutputNode::Draw()
{
    ImGui::PushItemWidth( 200 );
    ImNodes::BeginNode( m_id );

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted( "Fragment Output" );
    ImNodes::EndNodeTitleBar();

    /*
     *  1.  color baseColor .82 .67 .16
     *  2.  float metallic 0 1 0
     *  3.  float subsurface 0 1 0
     *  4.  float specular 0 1 .5
     *  5.  float roughness 0 1 .5
     *  6.  float specularTint 0 1 0
     *  7.  float anisotropic 0 1 0
     *  8.  float sheen 0 1 0
     *  9.  float sheenTint 0 1 .5
     *  10. float clearCoat 0 1 0
     *  11. float clearCoatGloss 0 1 1
     *  12. float alpha 0 1 1
     *  13. vector normal 0 1 0
     */

    DrawInputColorAttribute( m_color, m_attributeStartId, m_connections[0], "Diffuse Color" );
    DrawInputFloatAttribute( m_metallic, m_attributeStartId + 1, m_connections[1], "Metallic", 0, 1 );
    DrawInputFloatAttribute( m_subsurface, m_attributeStartId + 2, m_connections[2], "Subsurface", 0, 1 );
    DrawInputFloatAttribute( m_specular, m_attributeStartId + 3, m_connections[3], "Specular", 0, 1 );
    DrawInputFloatAttribute( m_roughness, m_attributeStartId + 4, m_connections[4], "Roughness", 0, 1 );
    DrawInputFloatAttribute( m_specularTint, m_attributeStartId + 5, m_connections[5], "Specular tint", 0, 1 );
    DrawInputFloatAttribute( m_anisotropic, m_attributeStartId + 6, m_connections[6], "Anisotropic", 0, 1 );
    DrawInputFloatAttribute( m_sheen, m_attributeStartId + 7, m_connections[7], "Sheen", 0, 1 );
    DrawInputFloatAttribute( m_sheenTint, m_attributeStartId + 8, m_connections[8], "Sheen tint", 0, 1 );
    DrawInputFloatAttribute( m_clearCoat, m_attributeStartId + 9, m_connections[9], "clearCoat", 0, 1 );
    DrawInputFloatAttribute( m_clearCoatGloss, m_attributeStartId + 10, m_connections[10], "clearCoat tint", 0, 1 );
    DrawInputFloatAttribute( m_alpha, m_attributeStartId + 11, m_connections[11], "Alpha", 0, 1 );
    DrawInputVectorAttribute( m_normal, m_attributeStartId + 12, true, "Normal" );

    ImNodes::EndNode();
}

void CFragmentOutputNode::Attach(int endAttr)
{
    m_connections[endAttr - m_attributeStartId] = true;
}

void CFragmentOutputNode::Detach(int endAttr)
{
    m_connections[endAttr - m_attributeStartId] = false;
}

std::string CFragmentOutputNode::Evaluate(std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set< std::string >& includes, EAttributeType attributeType)
{
    (void) begin;
    (void) bindings;
    (void) includes;
    (void) attributeType;
    std::string shader{ R"(//I created this shader by reading
//https://google.github.io/filament/Filament.md.html

#version 450
#include "default.glsl"

#define LIGHT_COUNT 6
#define MEDIUM_MAX 65504.0
#define SATUREATE_MADIUM(x) min(x, MEDIUM_MAX)

layout(set=0, binding=2) uniform samplerCube gRadEnvironment;
layout(set=0, binding=3) uniform samplerCube gIrradEnvironment;
layout(set=0, binding=4) uniform sampler2D gBRDFLUP;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragTangent;
layout(location = 4) in vec4 fragWorldPos;

layout (location = 0) out vec4 outFragcolor;

struct Light {
    vec4 position;
    vec4 color;
    float radius;
};

layout(set=0, binding=1) uniform LightData
{
    Light lights[LIGHT_COUNT];
    vec4 viewPos;
    int displayDebugTarget;
} ubo;

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
    if (ubo.displayDebugTarget >= 1.0)
    {
        outFragcolor = vec4((fragNormal + vec3(1)) * vec3(0.5), 1);
        return;
    }
    else if (ubo.displayDebugTarget >= 2.0)
    {
        vec3 view = normalize(ubo.viewPos.xyz - fragWorldPos.xyz);
        outFragcolor = vec4((view + vec3(1)) * vec3(0.5), 1);
        return;
    }
    else if (ubo.displayDebugTarget >= 1.0)
    {
        Light light = ubo.lights[0];
        vec3 l = normalize(ubo.viewPos.xyz - fragWorldPos.xyz);
        outFragcolor = vec4((l + vec3(1)) * vec3(0.5), 1);
        return;
    }

    float metallic = {1};
    float perceptualRoughness = {2};
    float ambientOcclusion = 1;
    vec3 baseColor = (1.0 - metallic) * fragColor.rgb * {0};

    vec3 n = {3};
    vec3 v = normalize(ubo.viewPos.xyz - fragWorldPos.xyz);
    float NdotV = abs(dot(n, v)) + 1e-5;

    vec3 directColor = vec3(0.0);
    for (int i = 0; i < LIGHT_COUNT; ++i)
    {
        vec3 Fd = vec3(0, 0, 0);
        vec3 Fr = vec3(0.0);

        Light light = ubo.lights[i];
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
        vec3 distToLight = light.position.xyz - fragWorldPos.xyz;
        directColor+=(Fd + Fr) * (NdotL * ambientOcclusion) * light.color.rgb * light.color.a/(BALBINO_4PI * dot(distToLight,distToLight) );
        //directColor+=(Fd + Fr) * (NdotL * ambientOcclusion) * light.color.rgb;
    }
    //todo add clearcoat brdf
    //todo add Anisotropic brdf
    outFragcolor = vec4(directColor, fragColor.a);
})" };
    /*
    *  1.  color baseColor .82 .67 .16
    *  2.  float metallic 0 1 0
    *  3.  float subsurface 0 1 0
    *  4.  float specular 0 1 .5
    *  5.  float roughness 0 1 .5
    *  6.  float specularTint 0 1 0
    *  7.  float anisotropic 0 1 0
    *  8.  float sheen 0 1 0
    *  9.  float sheenTint 0 1 .5
    *  10. float clearCoat 0 1 0
    *  11. float clearCoatGloss 0 1 1
    *  12. float alpha 0 1 1
    *  13. vector normal 0 1 0
    */
    //Can't do this because C++ 23 formatting is not as strong as expected
    //return std::format(shader
    //	, m_connections[0] ? (*(begin++))->Evaluate(begin, bindings, includes, EAttributeType::Color).c_str() : ("vec3(" + std::to_string(m_color[0]) + "," + std::to_string(m_color[0]) + "," + std::to_string(m_color[0]) + ")").c_str()
    //	, m_connections[1] ? (*(begin++))->Evaluate(begin, bindings, includes, EAttributeType::Float).c_str() : std::to_string(m_metallic).c_str()
    //	, m_connections[4] ? (*(begin++))->Evaluate(begin, bindings, includes, EAttributeType::Float).c_str() : std::to_string(m_roughness).c_str()
    //	, m_connections[12] ? (*(begin++))->Evaluate(begin, bindings, includes, EAttributeType::Vector).c_str() : "normalize(fragNormal)");
    // So therefore I have the "find/replace mechanic
    shader.replace( shader.find( "{0}" ), 3, m_connections[0] ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color ) : ( "vec3(" + std::to_string( m_color[0] ) + "," + std::to_string( m_color[1] ) + "," + std::to_string( m_color[2] ) + ")" ));
    shader.replace( shader.find( "{1}" ), 3, m_connections[1] ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color ) : std::to_string( m_metallic ));
    shader.replace( shader.find( "{2}" ), 3, m_connections[4] ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color ) : std::to_string( m_roughness ));
    shader.replace( shader.find( "{3}" ), 3, m_connections[12] ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector ).c_str() : "normalize(fragNormal)" );
    return shader;
}

bool CFragmentOutputNode::HasFreeAttachment(int endAttr) const
{
    if( endAttr >= m_attributeStartId && endAttr < m_attributeStartId + m_connectionSize && !m_connections[endAttr - m_attributeStartId] )
    {
        return true;
    }
    return false;
}

int CFragmentOutputNode::GetId() const
{
    return m_id;
}

std::vector< int > CFragmentOutputNode::GetInputs() const
{
    std::vector< int > inputs( m_connectionSize, m_attributeStartId );
    for( int i = 0; i < m_connectionSize; ++i )
    {
        inputs[i] += i;
    }
    return inputs;
}
