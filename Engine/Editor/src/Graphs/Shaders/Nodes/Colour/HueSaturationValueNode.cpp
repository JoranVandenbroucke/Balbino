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
                SSocketType{ .type=SSocketType::var_type_color, .name="colour", .uiName="Colour" },
                SSocketType{ .type=SSocketType::var_type_float, .name="hue", .uiName="Hue" },
                SSocketType{ .type=SSocketType::var_type_float, .name="saturation", .uiName="Saturation" },
                SSocketType{ .type=SSocketType::var_type_float, .name="value", .uiName="Value" },
                SSocketType{ .hasEditorValues=false, .type=SSocketType::var_type_float, .name="fac", .uiName="Factor" }}, {
                SSocketType{ .type=SSocketType::var_type_color, .name="out_colour", .uiName="Out Colour" }}}
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
