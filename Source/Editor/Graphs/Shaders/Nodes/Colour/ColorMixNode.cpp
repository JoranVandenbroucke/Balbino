#include "ColorMixNode.h"
#include "../../../../Renderer/EditorGui.h"
#include "../Attribute.h"

CColorMixNode::CColorMixNode( int& id )
    : CShaderNode { id,
                    { SSocketType { .type = SSocketType::var_type::color, .name = "col1", .uiName = "color_colour" }, SSocketType { .type = SSocketType::var_type::color, .name = "col2", .uiName = "color_colour" },
                      SSocketType { .type = SSocketType::var_type::float_type, .name = "fac", .uiName = "color_factor" } },
                    { SSocketType { .type = SSocketType::var_type::color, .name = "out_colour", .uiName = "color_out Colour" } } }
{
    m_allModeNames.reserve( mix_mode::mix_mode_max );
    for ( int n = 0; n < mix_mode::mix_mode_max; ++n )
    {
        m_allModeNames.emplace_back( ToString( mix_mode( n ) ) );
    }
}
void CColorMixNode::Draw() noexcept
{
    uint64_t type { (uint64_t)m_type };
    StartNode( m_allModeNames[ type ].c_str(), GetId() );
    if ( FawnForge::Gui::ComboBox( "mode", type, m_allModeNames, {}, 100.f, true ) )
    {
        m_type = (mix_mode)type;
    }
    CShaderNode::Draw();
    EndNode();
}
std::string CColorMixNode::GetCode() const
{
    const std::string parameters { "(col1,col2,fac,out_colour);" };
    switch ( m_type )
    {
        case mix_mode::mix_mode_add: return "color_add" + parameters;
        case mix_mode::mix_mode_subtract: return "color_subtract" + parameters;
        case mix_mode::mix_mode_multiply: return "color_multiply" + parameters;
        case mix_mode::mix_mode_screen: return "color_screen" + parameters;
        case mix_mode::mix_mode_divide: return "color_divide" + parameters;
        case mix_mode::mix_mode_difference: return "color_difference" + parameters;
        case mix_mode::mix_mode_darken: return "color_darken" + parameters;
        case mix_mode::mix_mode_lighten: return "color_lighten" + parameters;
        case mix_mode::mix_mode_overlay: return "color_overlay" + parameters;
        case mix_mode::mix_mode_color_dodge: return "color_color_dodge" + parameters;
        case mix_mode::mix_mode_color_burn: return "color_color_burn" + parameters;
        case mix_mode::mix_mode_hue: return "color_hue" + parameters;
        case mix_mode::mix_mode_saturation: return "color_saturation" + parameters;
        case mix_mode::mix_mode_value: return "color_value" + parameters;
        case mix_mode::mix_mode_color_mix: return "color_color_mix" + parameters;
        case mix_mode::mix_mode_soft_light: return "color_soft_light" + parameters;
        case mix_mode::mix_mode_linear_light: return "color_linear_light" + parameters;
        default: return "";
    }
}
void CColorMixNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_color.glsl" );
}

const char* CColorMixNode::ToString( CColorMixNode::mix_mode type ) const
{
    switch ( type )
    {
        case mix_mode::mix_mode_add: return "Add";
        case mix_mode::mix_mode_subtract: return "Subtract";
        case mix_mode::mix_mode_multiply: return "Multiply";
        case mix_mode::mix_mode_screen: return "Screen";
        case mix_mode::mix_mode_divide: return "Divide";
        case mix_mode::mix_mode_difference: return "Difference";
        case mix_mode::mix_mode_darken: return "Darken";
        case mix_mode::mix_mode_lighten: return "Lighten";
        case mix_mode::mix_mode_overlay: return "Overlay";
        case mix_mode::mix_mode_color_dodge: return "Color Dodge";
        case mix_mode::mix_mode_color_burn: return "Color Burn";
        case mix_mode::mix_mode_hue: return "Hue";
        case mix_mode::mix_mode_saturation: return "Saturation";
        case mix_mode::mix_mode_value: return "Value";
        case mix_mode::mix_mode_color_mix: return "Color Mix";
        case mix_mode::mix_mode_soft_light: return "Soft Light";
        case mix_mode::mix_mode_linear_light: return "Linear Light";
        default: break;
    }
    return nullptr;
}
