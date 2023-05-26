//
// Created by joran on 02/03/2023.
//

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
    m_flags = SSocketType::compiler_define_uses_world_position | SSocketType::compiler_define_uses_normal;
    m_flags |= ToCompilerDefine( m_diffuse ) | ToCompilerDefine( m_distribution ) | ToCompilerDefine( m_specular ) | ToCompilerDefine( m_fresnel );
    
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
           "fragNormal,\n"
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
//    shaderInfo.AddDefine( "BRDF_DIFFUSE",    ToDefine( ToCompilerDefine(m_diffuse) ));
//    shaderInfo.AddDefine( "BRDF_SPECULAR_D", ToDefine( ToCompilerDefine(m_distribution) ));
//    shaderInfo.AddDefine( "BRDF_SPECULAR_V", ToDefine( ToCompilerDefine(m_specular) ));
//    shaderInfo.AddDefine( "BRDF_SPECULAR_F", ToDefine( ToCompilerDefine(m_fresnel) ));
    
    if ( m_flags & SSocketType::compiler_define_uses_colour )
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_input, SShaderBinding::value_type_vec_4, 0, 0, 1, "fragColor" }, -99
        );
    }
    if ( m_flags & SSocketType::compiler_define_uses_uv )
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_input, SShaderBinding::value_type_vec_4, 0, 1, 1, "fragTexCoord" }, -99
        );
    }
    if ( m_flags & SSocketType::compiler_define_uses_normal )
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_input, SShaderBinding::value_type_vec_3, 0, 2, 1, "fragNormal" }, -99
        );
        shaderInfo.AddDefine( ToDefine( SSocketType::compiler_define_uses_normal ), "", true );
    }
    if ( m_flags & SSocketType::compiler_define_uses_tangent )
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_input, SShaderBinding::value_type_vec_4, 0, 3, 1, "fragTangent" }, -99
        );
    }
    if ( m_flags & SSocketType::compiler_define_uses_world_position )
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_input, SShaderBinding::value_type_vec_4, 0, 4, 1, "fragWorldPos" }, -99
        );
        shaderInfo.AddDefine( ToDefine( SSocketType::compiler_define_uses_world_position ), "", true );
    }
    
    shaderInfo.AddBinding(
            { SShaderBinding::layout_output, SShaderBinding::value_type_vec_4, 0, 0, 0, "outFragcolor" },
            -99
    );
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
    m_flags |= SSocketType::compiler_define_uses_world_position | SSocketType::compiler_define_uses_normal;
    m_flags |= ToCompilerDefine( m_diffuse ) | ToCompilerDefine( m_distribution ) | ToCompilerDefine( m_specular ) | ToCompilerDefine( m_fresnel );
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
SSocketType::compiler_define CFragmentOutputNode::ToCompilerDefine( CFragmentOutputNode::diffuse type )
{
    switch ( type )
    {
        case diffuse_lambert:
            return SSocketType::compiler_define_diffuse_lambert;
        case diffuse_burley:
            return SSocketType::compiler_define_diffuse_burley;
        case diffuse_oren_nayar:
            return SSocketType::compiler_define_diffuse_oren_nayar;
        default:
            break;
    }
    return SSocketType::compiler_define_diffuse_lambert;
}
SSocketType::compiler_define CFragmentOutputNode::ToCompilerDefine( CFragmentOutputNode::distribution type )
{
    switch ( type )
    {
        case distribution_blin_phong:
            return SSocketType::compiler_define_specular_blin_phong;
        case distribution_beckmann:
            return SSocketType::compiler_define_specular_beckmann;
        case distribution_ggx:
            return SSocketType::compiler_define_specular_ggx;
        default:
            break;
    }
    return SSocketType::compiler_define_specular_ggx;
}
SSocketType::compiler_define CFragmentOutputNode::ToCompilerDefine( CFragmentOutputNode::specular type )
{
    switch ( type )
    {
        case specular_implicit:
            return SSocketType::compiler_define_specular_implicit;
        case specular_neumann:
            return SSocketType::compiler_define_specular_neumann;
        case specular_cooktorrance:
            return SSocketType::compiler_define_specular_cook_torrance;
        case specular_kelemen:
            return SSocketType::compiler_define_specular_kelemen;
        case specular_ggx_correlated:
            return SSocketType::compiler_define_specular_ggx_correlated;
        case specular_ggx_correlated_fast:
            return SSocketType::compiler_define_specular_ggx_correlated_fast;
        case specular_kelemen_two:
            return SSocketType::compiler_define_specular_kelemen_two;
        case specular_neubelt:
            return SSocketType::compiler_define_specular_neubelt;
        default:
            break;
    }
    return SSocketType::compiler_define_specular_beckmann;
}
SSocketType::compiler_define CFragmentOutputNode::ToCompilerDefine( CFragmentOutputNode::fresnel type )
{
    switch ( type )
    {
        case fresnel_cook_torrance:
            return SSocketType::compiler_define_fresnel_cook_torrance;
        case fresnel_schlick:
            return SSocketType::compiler_define_fresnel_schlick;
        default:
            break;
    }
    return SSocketType::compiler_define_fresnel_cook_torrance;
}
