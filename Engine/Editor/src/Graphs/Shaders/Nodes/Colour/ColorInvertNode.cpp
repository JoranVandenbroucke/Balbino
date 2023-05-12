//
// Created by joran on 12/03/2023.
//


#include "ColorInvertNode.h"
#include "../Attribute.h"

CColorInvertNode::CColorInvertNode( int& id )
        : CShaderNode{
        id, {
                SSocketType{ true, SSocketType::var_type_color, SSocketType::compiler_define( 0 ), "colour", "Colour" },
                SSocketType{ false, SSocketType::var_type_float, SSocketType::compiler_define( 0 ), "fac", "Factor" }}, {
                SSocketType{ true, SSocketType::var_type_color, SSocketType::compiler_define( 0 ), "out_colour", "Out Colour" }}}
{
}
void CColorInvertNode::Draw()
{
    StartNode( "Colour Invert", GetId());
    CShaderNode::Draw();
    EndNode();
}
std::string CColorInvertNode::GetCode() const
{
    return "color_invert(colour,fac,out_colour);";
}
void CColorInvertNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude("node_color.glsl");
}
