//
// Created by Joran Vandenbroucke on 27/05/2022.
//

#include "ImageTextureNode.h"
#include "../Attribute.h"
#include "../../../../Renderer/EditorGui.h"


CImageTextureNode::CImageTextureNode( int& id )
        : CShaderNode{
        id, {
                SSocketType{ false, SSocketType::var_type_vector_3, SSocketType::compiler_define( 0 ), "uv", "UV" }}, {
                SSocketType{ false, SSocketType::var_type_color, SSocketType::compiler_define( 0 ), "color", "Colour" },
                SSocketType{ false, SSocketType::var_type_float, SSocketType::compiler_define( 0 ), "alpha", "Alpha" }}}
{
    m_allModeNames.reserve( texture_max );
    for ( int                  n = 0; n < texture_max; ++n )
    {
        m_allModeNames.emplace_back( ToString( texture( n )));
    }
    char                       name[64]{ "" };
    SShaderBinding::value_type valueType{};
    strcpy_s( name, m_bindingName.data());
    switch ( m_type )
    {
        case texture_1d:
            valueType = m_isShadow
                        ? SShaderBinding::value_type_sampler_1_d_shadow
                        : SShaderBinding::value_type_sampler_1_d;
            break;
        case texture_2d:
            valueType = m_isShadow
                        ? SShaderBinding::value_type_sampler_2_d_shadow
                        : SShaderBinding::value_type_sampler_2_d;
            break;
        case texture_3d:
            valueType = SShaderBinding::value_type_sampler_3_d;
            break;
        case texture_cube:
            valueType = m_isShadow
                        ? SShaderBinding::value_type_sampler_cube_shadow
                        : SShaderBinding::value_type_sampler_cube;
            break;
        case texture_2d_rect:
            valueType = m_isShadow
                        ? SShaderBinding::value_type_sampler_2_d_rect_shadow
                        : SShaderBinding::value_type_sampler_2_d_rect;
            break;
        case texture_1d_array:
            valueType = m_isShadow
                        ? SShaderBinding::value_type_sampler_1_d_array_shadow
                        : SShaderBinding::value_type_sampler_1_d_array;
            break;
        case texture_2d_array:
            valueType = m_isShadow
                        ? SShaderBinding::value_type_sampler_2_d_array_shadow
                        : SShaderBinding::value_type_sampler_2_d_array;
            break;
        case texture_cube_array:
            valueType = m_isShadow
                        ? SShaderBinding::value_type_sampler_cube_array_shadow
                        : SShaderBinding::value_type_sampler_cube_array;
            break;
        case texture_max:
            break;
    }
    m_binding.layoutType = SShaderBinding::layout_image_sampler;
    m_binding.valueType = valueType;
    strcpy_s( m_binding.name, m_bindingName.data());
}
void CImageTextureNode::Draw()
{
    uint64_t  type{ (uint64_t) m_type };
    const int size{ 64 };
    char      name[size]{};
    for ( int i{}; i < std::min((int) m_bindingName.size(), size ); ++i )
    {
        name[i] = m_bindingName[i];
    }
    StartNode( "Texture", GetId());
    if ( FawnForge::Gui::DrawComboBox( "texture Type", type, m_allModeNames, {}, 100.f, true ))
    {
        m_type = (texture) type;
    }
    
    if ( FawnForge::Gui::DrawInputText( "Binding Name", name, 64, ImGuiInputTextFlags_EnterReturnsTrue ))
    {
        m_bindingName = "";
        for ( int i{}; i < 64; ++i )
        {
            if ( name[i] == '\0' )
            {
                break;
            }
            m_bindingName += name[i];
        }
    }
    FawnForge::Gui::DrawToggle( "Is Shadow", m_isShadow );
    CShaderNode::Draw();
    EndNode();
}
std::string CImageTextureNode::GetCode() const
{
    const std::string prefix{ "texture_sample_" };
    const std::string postfix{ ( m_isShadow ? "_shadow(" : "(" ) + m_bindingName + ",uv,color,alpha);" };
    switch ( m_type )
    {
        case texture_1d:
            return prefix + "1d" + postfix;
        case texture_2d:
            return prefix + "2d" + postfix;
        case texture_3d:
            return "Texture 3D";
        case texture_cube:
        case texture_2d_rect:
            return prefix + "cube" + postfix;
        case texture_1d_array:
            return prefix + "1d_array" + postfix;
        case texture_2d_array:
            return prefix + "2d_array" + postfix;
        case texture_cube_array:
            return prefix + "cube_array" + postfix;
        case texture_max:
            break;
    }
    return "";
}
void CImageTextureNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_texture_image.glsl" );
    shaderInfo.AddBinding( m_binding );
    if( !GetInput(0)->IsConnected() )
    {
        shaderInfo.AddDefine( ToDefine(SSocketType::compiler_define_uses_uv), "", true );
    }
}

const char* CImageTextureNode::ToString( texture mode )
{
    switch ( mode )
    {
        case texture_1d:
            return "Texture 1D";
        case texture_2d:
            return "Texture 2D";
        case texture_3d:
            return "Texture 3D";
        case texture_cube:
            return "Texture Cube";
        case texture_2d_rect:
            return "Texture 2D Rectangle";
        case texture_1d_array:
            return "Texture 1D Array";
        case texture_2d_array:
            return "Texture 2D Array";
        case texture_cube_array:
            return "Texture Cube Array";
        case texture_max:
            break;
    }
    return nullptr;
}