//
// Created by Joran Vandenbroucke on 27/05/2022.
//

#include "ImageTextureNode.h"
#include "../../../../Renderer/EditorGui.h"
#include "../Attribute.h"

CImageTextureNode::CImageTextureNode( int& id )
    : CShaderNode { id,
                    { SSocketType { false, SSocketType::var_type::vector2, SSocketType::compiler_define( 0 ), "uv", "UV" } },
                    { SSocketType { false, SSocketType::var_type::color, SSocketType::compiler_define( 0 ), "color", "Colour" }, SSocketType { false, SSocketType::var_type::float_type, SSocketType::compiler_define( 0 ), "alpha", "Alpha" } } }
{
    m_allModeNames.reserve( texture_max );

    for ( int n {}; n < texture_max; ++n )
    {
        m_allModeNames.emplace_back( ToString( texture( n ) ) );
    }
    m_binding.layoutType = SShaderBinding::layout::image_sampler;
    m_binding.valueType  = SShaderBinding::value_type::sampler_2d;
    strcpy_s( m_binding.name, m_bindingName.data() );
}
void CImageTextureNode::Draw() noexcept
{
    uint64_t type { (uint64_t)m_type };
    const int size { 64 };
    char name[ size ] {};
    for ( int i {}; i < std::min( (int)m_bindingName.size(), size ); ++i )
    {
        name[ i ] = m_bindingName[ i ];
    }
    StartNode( "Texture", GetId() );
    if ( FawnForge::Gui::ComboBox( "texture Type", type, m_allModeNames, {}, 100.f, true ) )
    {
        m_type = (texture)type;
        switch ( m_type )
        {
            case texture_1d:
                m_binding.valueType = m_isShadow ? SShaderBinding::value_type::sampler_1d_shadow : SShaderBinding::value_type::sampler_1d;
                GetInput( 0 )->SetSocketType( { false, SSocketType::var_type::float_type, SSocketType::compiler_define( 0 ), "uv", "UV" } );
                break;
            case texture_2d:
                m_binding.valueType = m_isShadow ? SShaderBinding::value_type::sampler_2d_shadow : SShaderBinding::value_type::sampler_2d;
                GetInput( 0 )->SetSocketType( { false, SSocketType::var_type::vector2, SSocketType::compiler_define( 0 ), "uv", "UV" } );
                break;
            case texture_3d:
                m_binding.valueType = SShaderBinding::value_type::sampler_3d;
                GetInput( 0 )->SetSocketType( { false, SSocketType::var_type::vector3, SSocketType::compiler_define( 0 ), "uv", "UV" } );
                break;
            case texture_cube:
                m_binding.valueType = m_isShadow ? SShaderBinding::value_type::sampler_cube_shadow : SShaderBinding::value_type::sampler_cube;
                GetInput( 0 )->SetSocketType( { false, SSocketType::var_type::vector3, SSocketType::compiler_define( 0 ), "uv", "UV" } );
                break;
            case texture_2d_rect:
                m_binding.valueType = m_isShadow ? SShaderBinding::value_type::sampler_2d_rect_shadow : SShaderBinding::value_type::sampler_2d_rect;
                GetInput( 0 )->SetSocketType( { false, SSocketType::var_type::vector2, SSocketType::compiler_define( 0 ), "uv", "UV" } );
                break;
            case texture_1d_array:
                m_binding.valueType = m_isShadow ? SShaderBinding::value_type::sampler_1d_array_shadow : SShaderBinding::value_type::sampler_1d_array;
                GetInput( 0 )->SetSocketType( { false, SSocketType::var_type::vector2, SSocketType::compiler_define( 0 ), "uv", "UV" } );
                break;
            case texture_2d_array:
                m_binding.valueType = m_isShadow ? SShaderBinding::value_type::sampler_2d_array_shadow : SShaderBinding::value_type::sampler_2d_array;
                GetInput( 0 )->SetSocketType( { false, SSocketType::var_type::vector3, SSocketType::compiler_define( 0 ), "uv", "UV" } );
                break;
            case texture_cube_array:
                m_binding.valueType = m_isShadow ? SShaderBinding::value_type::sampler_cube_array_shadow : SShaderBinding::value_type::sampler_cube_array;
                GetInput( 0 )->SetSocketType( { false, SSocketType::var_type::vector4, SSocketType::compiler_define( 0 ), "uv", "UV" } );
                break;
            case texture_max: break;
        }
    }

    if ( FawnForge::Gui::InputText( "Binding Name", name, 64, ImGuiInputTextFlags_EnterReturnsTrue ) )
    {
        m_bindingName = "";
        for ( int i {}; i < 64; ++i )
        {
            if ( name[ i ] == '\0' )
            {
                break;
            }
            m_bindingName += name[ i ];
        }
    }
    FawnForge::Gui::Toggle( "Is Shadow", m_isShadow );
    CShaderNode::Draw();
    EndNode();
}
std::string CImageTextureNode::GetCode() const
{
    const std::string prefix { "texture_sample_" };
    const std::string postfix { ( m_isShadow ? "_shadow(" : "(" ) + m_bindingName + ",uv,color,alpha);" };
    switch ( m_type )
    {
        case texture_1d: return prefix + "1d" + postfix;
        case texture_2d: return prefix + "2d" + postfix;
        case texture_3d: return "Texture 3D";
        case texture_cube:
        case texture_2d_rect: return prefix + "cube" + postfix;
        case texture_1d_array: return prefix + "1d_array" + postfix;
        case texture_2d_array: return prefix + "2d_array" + postfix;
        case texture_cube_array: return prefix + "cube_array" + postfix;
        case texture_max: break;
    }
    return "";
}
void CImageTextureNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_texture_image.glsl" );
    shaderInfo.AddBinding( m_binding );
    if ( !GetInput( 0 )->IsConnected() )
    {
        shaderInfo.AddDefine( ToDefine( SSocketType::compiler_define_uses_uv ), "", true );
    }
}

const char* CImageTextureNode::ToString( texture mode )
{
    switch ( mode )
    {
        case texture_1d: return "Texture 1D";
        case texture_2d: return "Texture 2D";
        case texture_3d: return "Texture 3D";
        case texture_cube: return "Texture Cube";
        case texture_2d_rect: return "Texture 2D Rectangle";
        case texture_1d_array: return "Texture 1D Array";
        case texture_2d_array: return "Texture 2D Array";
        case texture_cube_array: return "Texture Cube Array";
        case texture_max: break;
    }
    return nullptr;
}