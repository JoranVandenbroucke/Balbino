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
                SSocketType{ true, SSocketType::var_type_color, SSocketType::compiler_define( 0 ), "colour", "Colour" },
                SSocketType{ true, SSocketType::var_type_float, SSocketType::compiler_define( 0 ), "gamma", "Gama" }}, {
                SSocketType{ true, SSocketType::var_type_color, SSocketType::compiler_define( 0 ), "out_colour", "Out Colour" }}}
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
