//
// Created by joran on 12/03/2023.
//


#include <unordered_set>
#include <set>
#include "HueSaturationValueNode.h"
#include "../Attribute.h"

CHueSaturationValueNode::CHueSaturationValueNode( int& id )
        : CShaderNode{
        id, {
                SSocketType{ true, SSocketType::var_type_color, SSocketType::compiler_define( 0 ), "colour", "Colour" },
                SSocketType{ true, SSocketType::var_type_float, SSocketType::compiler_define( 0 ), "hue", "Hue" },
                SSocketType{ true, SSocketType::var_type_float, SSocketType::compiler_define( 0 ), "saturation", "Saturation" },
                SSocketType{ true, SSocketType::var_type_float, SSocketType::compiler_define( 0 ), "value", "Value" },
                SSocketType{ false, SSocketType::var_type_float, SSocketType::compiler_define( 0 ), "fac", "Factor" }}, {
                SSocketType{ true, SSocketType::var_type_color, SSocketType::compiler_define( 0 ), "out_colour", "Out Colour" }}}
{
}
void CHueSaturationValueNode::Draw()
{
    StartNode( "Hue Saturation Value", GetId());
    CShaderNode::Draw();
    EndNode();
}
std::string CHueSaturationValueNode::GetCode() const
{
    return "color_hue_sat_val(colour, hue, saturation, value, fac, out_colour);";
}
void CHueSaturationValueNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude("node_color.glsl");
}
