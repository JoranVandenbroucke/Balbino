//
// Created by joran on 12/03/2023.
//


#include <unordered_set>
#include <set>
#include "GammaNode.h"
#include "../Attribute.h"

CGammaNode::CGammaNode( int& id )
        : CShaderNode{
        id, {
                SSocketType{ .type=SSocketType::var_type_color, .name="colour", .uiName="Colour" },
                SSocketType{ .type=SSocketType::var_type_float, .name="gamma", .uiName="Gama" }}, {
                SSocketType{ .type=SSocketType::var_type_color, .name="out_colour", .uiName="Out Colour" }}}
{
}
void CGammaNode::Draw()
{
    StartNode( "Gamma", GetId());
    CShaderNode::Draw();
    EndNode();
}
std::string CGammaNode::GetCode() const
{
    return "color_gamma(colour, gamma, out_colour);";
}
void CGammaNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude("node_color.glsl");
}
