//
// Created by joran on 02/03/2023.
//


#include <format>
#include "FragmentOutput.h"
#include "../Attribute.h"

CFragmentOutputNode::CFragmentOutputNode( int& id )
        : CShaderNode{
        id,
        {
                SSocketType{ .type=SSocketType::var_type_color, .name="colour", .uiName="Colour" },
                SSocketType{ .type=SSocketType::var_type_float, .name="metallic", .uiName="Metallic" },
                SSocketType{ .type=SSocketType::var_type_float, .name="roughness", .uiName="Roughness" },
                SSocketType{ .type=SSocketType::var_type_float, .name="occlusion", .uiName="Ambient Occlusion" },
                SSocketType{ .hasEditorValues=false, .type=SSocketType::var_type_vector_3, .name="normal", .uiName="Normal" }},
        { SSocketType{ .type=SSocketType::var_type_shader, .name="shader", .uiName="Fragment Shader" }}}
{
    m_flags = SSocketType::compiler_define_uses_normal | SSocketType::compiler_define_uses_word_position;
    m_flags |= 1u << ( m_diffuse + 6u );
    m_flags |= 1u << ( m_distribution + (uint8_t) diffuse_max + 6u );
    m_flags |= 1u << ( m_specular + (uint8_t) distribution_max + diffuse_max + 6u );
    m_flags |= 1u << ( m_fresnel + (uint8_t) specular_max + distribution_max + diffuse_max + 6u );
    
    for ( int i{}; i < specular_max; ++i )
    {
        if ( i < fresnel_max )
        {
            m_fresnelNames[i] = ToString( fresnel( i ));
        }
        if ( i < diffuse_max )
        {
            m_diffuseNames[i] = ToString( diffuse( i ));
        }
        if ( i < distribution_max )
        {
            m_distributionNames[i] = ToString( distribution( i ));
        }
        m_specularNames[i] = ToString( specular( i ));
    }
}
void CFragmentOutputNode::Draw()
{
    StartNode( "Fragment Output", GetId());
    // todo, add dropdowns for diffuse, fesnel, distribution and shadow
    CShaderNode::Draw();
    EndNode();
}
std::string CFragmentOutputNode::GetCode() const
{
    return "fragment_specular(\n"
           "colour,\n"
           "normal,\n"
           "metallic,\n"
           "roughness,\n"
           "occlusion,\n"
           "outFragcolor\n"
           ");";
}
void CFragmentOutputNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.shaderTypes = shader_stage_fragment;
    shaderInfo.AddInclude( "node_fragment.glsl" );
    shaderInfo.AddInclude( "light.glsl", -100 );
    shaderInfo.AddInclude( "brdf.glsl", -100 );
    shaderInfo.AddDefine( "BRDF_DIFFUSE", ToDefine( m_diffuse ));
    shaderInfo.AddDefine( "BRDF_SPECULAR_D", ToDefine( m_distribution ));
    shaderInfo.AddDefine( "BRDF_SPECULAR_V", ToDefine( m_specular ));
    shaderInfo.AddDefine( "BRDF_SPECULAR_F", ToDefine( m_fresnel ));
    
    if ( m_flags & SSocketType::compiler_define_uses_colour )
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_input, SShaderBinding::value_type_vec_4, 0, 0, 1, "fragColor" }
        );
    }
    if ( m_flags & SSocketType::compiler_define_uses_uv )
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_input, SShaderBinding::value_type_vec_4, 0, 1, 1, "fragTexCoord" }
        );
    }
    if ( m_flags & SSocketType::compiler_define_uses_normal )
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_input, SShaderBinding::value_type_vec_3, 0, 2, 1, "fragNormal" }
        );
    }
    if ( m_flags & SSocketType::compiler_define_uses_tangent )
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_input, SShaderBinding::value_type_vec_4, 0, 3, 1, "fragTangent" }
        );
    }
    if ( m_flags & SSocketType::compiler_define_uses_word_position )
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_input, SShaderBinding::value_type_vec_4, 0, 4, 1, "fragWorldPos" }
        );
    }
    
    shaderInfo.AddBinding( { SShaderBinding::layout_output, SShaderBinding::value_type_vec_4, 0, 0, 0, "outFragcolor" } );
    SShaderBinding binding{ SShaderBinding::layout_uniform, SShaderBinding::value_type_struct, 0, 1, 1, "LightData" };
    binding.members.emplace_back(
            SShaderBinding::layout_max,
            SShaderBinding::value_type_struct,
            SShaderBinding::autoConfig,
            SShaderBinding::autoConfig,
            "LIGHT_COUNT",
            "lights",
            "Light"
    );
    shaderInfo.AddBinding( binding, -99 );
}
void CFragmentOutputNode::SetFlag( SSocketType::compiler_define flags )
{
    m_flags = flags;
    m_flags |= 1u << ( m_diffuse + 6u );
    m_flags |= 1u << ( m_distribution + (uint8_t) diffuse_max + 6u );
    m_flags |= 1u << ( m_specular + (uint8_t) distribution_max + diffuse_max + 6u );
    m_flags |= 1u << ( m_fresnel + (uint8_t) specular_max + distribution_max + diffuse_max + 6u );
}

const char* CFragmentOutputNode::ToString( CFragmentOutputNode::diffuse type )
{
    switch ( type )
    {
        case diffuse_lambert:
            return "Lambert";
        case diffuse_burley:
            return "Burley";
        case diffuse_oren_nayar:
            return "Oren Nayar";
        default:
            break;
    }
    return nullptr;
}
const char* CFragmentOutputNode::ToString( CFragmentOutputNode::distribution type )
{
    switch ( type )
    {
        case distribution_blin_phong:
            return "Blin Phong";
        case distribution_beckmann:
            return "Beckmann";
        case distribution_ggx:
            return "Ggx";
        default:
            break;
    }
    return nullptr;
}
const char* CFragmentOutputNode::ToString( CFragmentOutputNode::specular type )
{
    switch ( type )
    {
        case specular_implicit:
            return "Implicit";
        case specular_neumann:
            return "Neumann";
        case specular_cooktorrance:
            return "Cooktorrance";
        case specular_kelemen:
            return "Kelemen";
        case specular_ggx_correlated:
            return "Ggx Correlated";
        case specular_ggx_correlated_fast:
            return "Ggx Correlated Fast";
        case specular_kelemen_two:
            return "Kelemen Two";
        case specular_neubelt:
            return "Neubelt";
        default:
            break;
    }
    return nullptr;
}
const char* CFragmentOutputNode::ToString( CFragmentOutputNode::fresnel type )
{
    switch ( type )
    {
        case fresnel_cook_torrance:
            return "Cook Torrance";
        case fresnel_schlick:
            return "Schlick";
        default:
            break;
    }
    return nullptr;
}
const char* CFragmentOutputNode::ToDefine( CFragmentOutputNode::diffuse type )
{
    switch ( type )
    {
        case diffuse_lambert:
            return "DIFFUSE_LAMBERT";
        case diffuse_burley:
            return "DIFFUSE_BURLEY";
        case diffuse_oren_nayar:
            return "DIFFUSE_OREN_NAYAR";
        default:
            break;
    }
    return nullptr;
}
const char* CFragmentOutputNode::ToDefine( CFragmentOutputNode::distribution type )
{
    switch ( type )
    {
        case distribution_blin_phong:
            return "SPECULAR_D_BLIN_PHONG";
        case distribution_beckmann:
            return "SPECULAR_D_BECKMANN";
        case distribution_ggx:
            return "SPECULAR_D_GGX";
        default:
            break;
    }
    return nullptr;
}
const char* CFragmentOutputNode::ToDefine( CFragmentOutputNode::specular type )
{
    switch ( type )
    {
        case specular_implicit:
            return "SPECULAR_S_IMPLICIT";
        case specular_neumann:
            return "SPECULAR_S_NEUMANN";
        case specular_cooktorrance:
            return "SPECULAR_S_COOKTORRANCE";
        case specular_kelemen:
            return "SPECULAR_S_KELEMEN";
        case specular_ggx_correlated:
            return "SPECULAR_S_GGX_CORRELATED";
        case specular_ggx_correlated_fast:
            return "SPECULAR_S_GGX_CORRELATED_FAST";
        case specular_kelemen_two:
            return "SPECULAR_S_KELEMEN_TWO";
        case specular_neubelt:
            return "SPECULAR_S_NEUBELT";
        default:
            break;
    }
    return nullptr;
}
const char* CFragmentOutputNode::ToDefine( CFragmentOutputNode::fresnel type )
{
    switch ( type )
    {
        case fresnel_cook_torrance:
            return "SPECULAR_F_COOK_TORRANCE_FRESNEL";
        case fresnel_schlick:
            return "SPECULAR_F_SCHLICK";
        default:
            break;
    }
    return nullptr;
}
