//
// Created by Joran Vandenbroucke on 12/03/2023.
//

#include "HueSaturationValueNode.h"
#include "../Attribute.h"
#include <set>
#include <unordered_set>

CHueSaturationValueNode::CHueSaturationValueNode( int& id )
    : CShaderNode { id,
                    { SSocketType { .type = SSocketType::var_type::color, .name = "colour", .uiName = "Colour" }, SSocketType { .type = SSocketType::var_type::float_type, .name = "mix_mode_hue", .uiName = "Hue" },
                      SSocketType { .type = SSocketType::var_type::float_type, .name = "mix_mode_saturation", .uiName = "Saturation" }, SSocketType { .type = SSocketType::var_type::float_type, .name = "mix_mode_value", .uiName = "Value" },
                      SSocketType { .hasEditorValues = false, .type = SSocketType::var_type::float_type, .name = "fac", .uiName = "Factor" } },
                    { SSocketType { .type = SSocketType::var_type::color, .name = "out_colour", .uiName = "Out Colour" } } }
{
}
void CHueSaturationValueNode::Draw() noexcept
{
    StartNode( "Hue Saturation Value", GetId() );
    CShaderNode::Draw();
    EndNode();
}
std::string CHueSaturationValueNode::GetCode() const
{
    return "color_hue_sat_val(colour, mix_mode_hue, mix_mode_saturation, mix_mode_value, fac, out_colour);";
}
void CHueSaturationValueNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_color.glsl" );
}
