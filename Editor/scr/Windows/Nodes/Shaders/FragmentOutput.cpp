#include "FragmentOutput.h"

#include <string>
#include <format>

#include "../../../EditorGUI/EditorGui.h"
#include "../Attribute.h"

CFragmentOutputNode::CFragmentOutputNode( int id, int& attributeStartId )
        : INode{ id, attributeStartId }
        , m_connections{}
        , m_ndfs{}
        , m_shadows{}
        , m_fresnels{}
        , m_diffuses{}
        , m_ndf{ ENormalDistributionFunction::GGX }
        , m_shadow{ EShadow::GGXCorrelated }
        , m_fresnel{ EFresnel::Schlick }
        , m_diffuse{ EDiffuse::Burley }
        , m_specularString{"GGXNDF(NdotH, roughness)"}
        , m_shadowString{"GGXCorrelatedshadow(NdotV, NdotL, roughness)"}
        , m_fresnelString{"SchlickFresnel(LdotH, f0)"}
        , m_diffuseString{"Burley(NdotV, NdotL, LdotH, roughness)"}
{
    m_vertexFlags = 0b00101000;
    attributeStartId += m_connectionSize + 1;
    for ( uint8_t i{}; i <= (uint8_t) ENormalDistributionFunction::GGX; ++i )
    {
        m_ndfs.emplace_back( ToString( ENormalDistributionFunction( i )));
    }
    for ( uint8_t i{}; i <= (uint8_t) EShadow::GGXCorrelatedFast; ++i )
    {
        m_shadows.emplace_back( ToString( EShadow( i )));
    }
    for ( uint8_t i{}; i <= (uint8_t) EFresnel::Schlick; ++i )
    {
        m_fresnels.emplace_back( ToString( EFresnel( i )));
    }
    for ( uint8_t i{}; i <= (uint8_t) EDiffuse::OrenNayar; ++i )
    {
        m_diffuses.emplace_back( ToString( EDiffuse( i )));
    }
}

void CFragmentOutputNode::Draw()
{
    uint64_t di{ uint64_t( m_diffuse ) };
    uint64_t sh{ uint64_t( m_shadow ) };
    uint64_t sp{ uint64_t( m_ndf ) };
    uint64_t fr{ uint64_t( m_fresnel ) };
    
    ImNodes::BeginNode( m_id );
    
    ImNodes::BeginNodeTitleBar();
    BalEditor::GUI::DrawText( "Fragment Output" );
    ImNodes::EndNodeTitleBar();
    
    if(BalEditor::GUI::DrawComboBox( "Diffuse", di, m_diffuses ))
    {
        m_diffuse = EDiffuse( di );
        switch ( m_diffuse )
        {
            case EDiffuse::Lambert:
                m_diffuseString = "Lambert()";
                break;
            case EDiffuse::Burley:
                m_diffuseString = "Burley(NdotV, NdotL, LdotH, roughness)";
                break;
            case EDiffuse::OrenNayar:
                m_diffuseString = "OrenNayar(NdotV, NdotL, roughness, )";
                break;
        }
    }
    if(BalEditor::GUI::DrawComboBox( "Shadow", sh, m_shadows )){
    
        m_shadow = EShadow( sh );
        switch ( m_shadow )
        {
            case EShadow::Implicit:
                m_shadowString = "Implicitshadow(NdotL, NdotV )";
                break;
            case EShadow::Neumann:
                m_shadowString = "Neumannshadow(NdotL, NdotV )";
                break;
            case EShadow::CookTorrance:
                m_shadowString = "CookTorranceshadow(LdotH, NdotV, NdotH, max(dot(v, h), 0.0) )";
                break;
            case EShadow::Kelemen:
                m_shadowString = "Kelemenshadow(LdotH, NdotV, NdotH)";
                break;
            case EShadow::Neubelt:
                m_shadowString = "Neumannshadow(NdotL, NdotV)";
                break;
            case EShadow::Beckmann:
                m_shadowString = "Beckmann(NdotV, roughness)";
                break;
            case EShadow::GGXCorrelated:
                m_shadowString = "GGXCorrelatedshadow(roughness, NdotV, NdotL)";
                break;
            case EShadow::GGXCorrelatedFast:
                m_shadowString = "GGXCorrelatedFastshadow(roughness, NdotV, NdotL)";
                break;
        }
    }
    if(BalEditor::GUI::DrawComboBox( "Specular", sp, m_ndfs )){
    
        m_ndf = ENormalDistributionFunction( sp );
        switch ( m_ndf )
        {
            case ENormalDistributionFunction::BlinnPhong:
                m_specularString = "BlinnPhongNDF(roughness, NdotH)";
                break;
            case ENormalDistributionFunction::Beckmann:
                m_specularString = "BeckmannNDF(roughness, NdotH, n, h)";
                break;
            case ENormalDistributionFunction::GGX:
                m_specularString = "GGXNDF(NdotH, roughness)";
                break;
        }
    }
    if(BalEditor::GUI::DrawComboBox( "Fresnel", fr, m_fresnels )){
    
        m_fresnel = EFresnel( fr );
        switch ( m_fresnel )
        {
            case EFresnel::CookTorrance:
                m_fresnelString = "CookTorranceFresnel(LdotH, f0)";
                break;
            case EFresnel::Schlick:
                m_fresnelString = "SchlickFresnel(LdotH, f0, VdotH)";
                break;
        }
    }
    
    DrawInputColorAttribute( m_color, m_attributeStartId, m_connections[0], "Diffuse Color" );
    DrawInputFloatAttribute( m_metallic, m_attributeStartId + 1, m_connections[1], "Metallic", 0, 1 );
    DrawInputFloatAttribute( m_roughness, m_attributeStartId + 2, m_connections[2], "Roughness", 0, 1 );
    DrawInputFloatAttribute( m_alpha, m_attributeStartId + 3, m_connections[3], "Alpha", 0, 1 );
    DrawInputVectorAttribute( m_normal, m_attributeStartId + 4, true, "Normal" );
    
    DrawOutputShaderAttribute( "Output:", m_attributeStartId + m_connectionSize );
    ImNodes::EndNode();
    
    m_shadow = EShadow( sh );
    m_ndf = ENormalDistributionFunction( sp );
    m_fresnel = EFresnel( fr );
    
}

void CFragmentOutputNode::Attach( int endAttr )
{
    m_connections[endAttr - m_attributeStartId] = true;
}

void CFragmentOutputNode::Detach( int endAttr )
{
    m_connections[endAttr - m_attributeStartId] = false;
}

std::string CFragmentOutputNode::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void) bindings;
    (void) attributeType;
    
    includes.emplace( "default.glsl" );
    includes.emplace( "light.glsl" );
    includes.emplace( "brdf.glsl" );
    std::string shader{
            R"(#version 450

{0}
layout (location = 0) out vec4 outFragColor;

layout(set=0, binding=0) uniform ModelData {
    mat4 view;
    mat4 proj;
    vec4 viewPos;
    int displayDebugTarget;
} modelBufferObject;

layout(set=0, binding=1) uniform LightData
{
    Light lights[LIGHT_COUNT];
};

void main()
{
    if (modelBufferObject.displayDebugTarget >= 3.0)
    {
        outFragColor = vec4((fragNormal + vec3(1)) * vec3(0.5), 1);
        return;
    }
    else if (modelBufferObject.displayDebugTarget >= 2.0)
    {
        vec3 view = normalize(modelBufferObject.viewPos.xyz - fragWorldPos.xyz);
        outFragColor = vec4((view + vec3(1)) * vec3(0.5), 1);
        return;
    }
    else if (modelBufferObject.displayDebugTarget >= 1.0)
    {
        Light light = lights[0];
        vec3 l = normalize(modelBufferObject.viewPos.xyz - fragWorldPos.xyz);
        outFragColor = vec4((l + vec3(1)) * vec3(0.5), 1);
        return;
    }
    float metallic = {1};
    float perceptualRoughness = {2};
    float ambientOcclusion = 1;
    vec3 baseColor = (1.0 - metallic) * {3};

    vec3 n = {4};
    vec3 v = normalize(modelBufferObject.viewPos.xyz - fragWorldPos.xyz);
    float NdotV = abs(dot(n, v)) + 1e-5;
    
    vec3 directColor = vec3(0.0);
    // float viewScatter=SchlickFresnel(NdotV, vec3(1.0, 1.0, 1.0), f90).x;
    for (int i = 0; i < LIGHT_COUNT; ++i)
    {
        Light light = lights[i];
        if(light.strength < 0)
            break;

        vec3 Fd = vec3(0, 0, 0);
        vec3 Fr = vec3(0.0);

        vec3 lightColor = light.color * light.strength;
        vec3 lightVector = light.position.xyz - fragWorldPos.xyz;
        vec3 l = normalize(lightVector);
        if(light.type != 0)
        {
            lightColor *= GetSquareFalloffAttenuation(lightVector,1/light.size.x);
            if(light.type == 2)
                lightColor *= GetSpotAngleAttenuation(l, light.direction,light.size.y, light.size.z);
        }
        else
        {
            lightVector = light.direction;
            l = normalize(light.direction);
        }

        vec3 h = normalize(n + l);
        float NdotL = max(dot(n, l), 0.0);
        float NdotH = max(dot(n, h), 0.0);
        float LdotH = max(dot(l, h), 0.0);

        // perceptually linear roughness to roughness (see parameterization)
        float roughness = perceptualRoughness * perceptualRoughness;
        if (NdotL > 0.0)
        {
            vec3 f0 = 0.16 * sqr(roughness) * (1.0 - metallic) + baseColor * metallic;
            float D = {5};
            vec3  F = {6};
            float V = {7};

            // specular BRDF
            Fr = (D * V) * F;
        }

        // diffuse BRDF
        Fd = baseColor * {8};
        directColor+=(Fd + Fr) * (NdotL * ambientOcclusion) * light.color * light.strength;
    }
    outFragColor = vec4(directColor, {9});
})"
    };
    shader.replace(
            shader.find( "{0}" ), 3, m_inputString );
    shader.replace(
            shader.find( "{1}" ),
            3,
            m_connections[1]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_metallic ));
    shader.replace(
            shader.find( "{2}" ),
            3,
            m_connections[2]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_roughness ));
    shader.replace(
            shader.find( "{3}" ),
            3,
            m_connections[0]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
            : ( "vec3(" + std::to_string( m_color[0] ) + "," + std::to_string( m_color[1] ) + "," + std::to_string(
                    m_color[2]
            ) + ")" ));
    shader.replace(
            shader.find( "{4}" ),
            3,
            m_connections[4]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
            : "fragNormal"
    );
    
    shader.replace(
            shader.find( "{5}" ),
            3,m_specularString);
    shader.replace(
            shader.find( "{6}" ),
            3, m_fresnelString);
    shader.replace(
            shader.find( "{7}" ),
            3,m_shadowString);
    shader.replace(
            shader.find( "{8}" ),
            3,m_diffuseString);
    
    shader.replace(
            shader.find( "{9}" ),
            3,
            m_connections[2]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_alpha ));
    
    return shader;
}

bool CFragmentOutputNode::HasFreeAttachment( int endAttr ) const
{
    if ( endAttr >= m_attributeStartId && endAttr < m_attributeStartId + m_connectionSize && !m_connections[endAttr - m_attributeStartId] )
    {
        return true;
    }
    return false;
}

int CFragmentOutputNode::GetId() const
{
    return m_id;
}

std::vector<int> CFragmentOutputNode::GetInputs() const
{
    std::vector<int> inputs( m_connectionSize, m_attributeStartId );
    for ( int        i = 0; i < m_connectionSize; ++i )
    {
        inputs[i] += i;
    }
    return inputs;
}

uint8_t CFragmentOutputNode::GetVertexFlags() const
{
    return m_vertexFlags;
}

const char* CFragmentOutputNode::ToString( CFragmentOutputNode::ENormalDistributionFunction type )
{
    switch ( type )
    {
        case ENormalDistributionFunction::BlinnPhong:
            return "Blinn-Phong";
        case ENormalDistributionFunction::Beckmann:
            return "Beckmann";
        case ENormalDistributionFunction::GGX:
            return "GGX";
    }
    return nullptr;
}
const char* CFragmentOutputNode::ToString( CFragmentOutputNode::EShadow type )
{
    switch ( type )
    {
        case EShadow::Implicit:
            return "Implicit";
        case EShadow::Neumann:
            return "Neumann";
        case EShadow::CookTorrance:
            return "Cook-Torrance";
        case EShadow::Kelemen:
            return "Kelemen";
        case EShadow::Neubelt:
            return "Neubelt";
        case EShadow::Beckmann:
            return "Beckmann";
        case EShadow::GGXCorrelated:
            return "GGX Correlated";
        case EShadow::GGXCorrelatedFast:
            return "GGX Correlated Fast";
    }
    return nullptr;
}
const char* CFragmentOutputNode::ToString( CFragmentOutputNode::EFresnel type )
{
    switch ( type )
    {
        case EFresnel::CookTorrance:
            return "Cook-Torrance";
        case EFresnel::Schlick:
            return "Schlick";
    }
    return nullptr;
}
const char* CFragmentOutputNode::ToString( CFragmentOutputNode::EDiffuse type )
{
    switch ( type )
    {
        case EDiffuse::Lambert:
            return "Lambert";
        case EDiffuse::Burley:
            return "Burley";
        case EDiffuse::OrenNayar:
            return "Oren-Nayar";
    }
    return nullptr;
}
void CFragmentOutputNode::SetVertexFlags( uint8_t flags )
{
    m_inputString = "";
    
    if(0b00000001&flags)
        m_inputString += "layout(location = 0) in vec4 fragColor;\n";
    if(0b00000010&flags)
        m_inputString += "layout(location = 1) in vec4 fragTexCoord;\n";
    m_inputString += "layout(location = 2) in vec3 fragNormal;\n";
    if(0b00000100&flags)
        m_inputString += "layout(location = 3) in vec4 fragTangent;\n";
    
    m_inputString += "layout(location = 4) in vec4 fragWorldPos;\n";
}
