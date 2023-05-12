#include "ColorMixNode.h"
#include "../Attribute.h"
#include "../../../../Renderer/EditorGui.h"

CColorMixNode::CColorMixNode( int& id )
        : CShaderNode{
        id, {
                SSocketType{ true, SSocketType::var_type_color, SSocketType::compiler_define( 0 ), "col1", "color_colour" },
                SSocketType{ true, SSocketType::var_type_color, SSocketType::compiler_define( 0 ), "col2", "color_colour" },
                SSocketType{ true, SSocketType::var_type_float, SSocketType::compiler_define( 0 ), "fac", "color_factor" }}, {
                SSocketType{
                        true, SSocketType::var_type_color, SSocketType::compiler_define( 0 ), "out_colour", "color_out Colour"
                }}}
{
    m_allModeNames.reserve( MixMode::max );
    for ( int n = 0; n < MixMode::max; ++n )
    {
        m_allModeNames.emplace_back( ToString( MixMode( n )));
    }
}
void CColorMixNode::Draw()
{
    uint64_t type{ (uint64_t) m_type };
    StartNode( m_allModeNames[type].c_str(), GetId());
    if ( FawnForge::Gui::DrawComboBox( "mode", type, m_allModeNames, {}, 100.f, true ))
    {
        m_type = (MixMode) type;
    }
    CShaderNode::Draw();
    EndNode();
}
std::string CColorMixNode::GetCode() const
{
    const std::string parameters{ "(col1,col2,fac,out_colour);" };
    switch ( m_type )
    {
        case MixMode::add:
            return "color_add" + parameters;
        case MixMode::subtract:
            return "color_subtract" + parameters;
        case MixMode::multiply:
            return "color_multiply" + parameters;
        case MixMode::screen:
            return "color_screen" + parameters;
        case MixMode::divide:
            return "color_divide" + parameters;
        case MixMode::difference:
            return "color_difference" + parameters;
        case MixMode::darken:
            return "color_darken" + parameters;
        case MixMode::lighten:
            return "color_lighten" + parameters;
        case MixMode::overlay:
            return "color_overlay" + parameters;
        case MixMode::color_dodge:
            return "color_color_dodge" + parameters;
        case MixMode::color_burn:
            return "color_color_burn" + parameters;
        case MixMode::hue:
            return "color_hue" + parameters;
        case MixMode::saturation:
            return "color_saturation" + parameters;
        case MixMode::value:
            return "color_value" + parameters;
        case MixMode::color_mix:
            return "color_color_mix" + parameters;
        case MixMode::soft_light:
            return "color_soft_light" + parameters;
        case MixMode::linear_light:
            return "color_linear_light" + parameters;
        default:
            return "";
    }
}
void CColorMixNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude("node_color.glsl");
}

const char* CColorMixNode::ToString( CColorMixNode::MixMode type )
{
    switch ( type )
    {
        case MixMode::add:
            return "Add";
        case MixMode::subtract:
            return "Subtract";
        case MixMode::multiply:
            return "Multiply";
        case MixMode::screen:
            return "Screen";
        case MixMode::divide:
            return "Divide";
        case MixMode::difference:
            return "Difference";
        case MixMode::darken:
            return "Darken";
        case MixMode::lighten:
            return "Lighten";
        case MixMode::overlay:
            return "Overlay";
        case MixMode::color_dodge:
            return "Color Dodge";
        case MixMode::color_burn:
            return "Color Burn";
        case MixMode::hue:
            return "Hue";
        case MixMode::saturation:
            return "Saturation";
        case MixMode::value:
            return "Value";
        case MixMode::color_mix:
            return "Color Mix";
        case MixMode::soft_light:
            return "Soft Light";
        case MixMode::linear_light:
            return "Linear Light";
        default:
            break;
    }
    return nullptr;
}
