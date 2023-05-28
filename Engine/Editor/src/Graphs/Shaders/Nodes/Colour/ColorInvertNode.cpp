//
// Created by joran on 12/03/2023.
//


#include "ColorInvertNode.h"
#include "../Attribute.h"

CColorInvertNode::CColorInvertNode( int& id )
        : CShaderNode{
        id, {
                SSocketType{ .type=SSocketType::var_type_color, .name="colour", .uiName="Colour" },
                SSocketType{ .hasEditorValues=false, .type=SSocketType::var_type_float, .name="fac", .uiName="Factor" }}, {
                SSocketType{ .type=SSocketType::var_type_color, .name="out_colour", .uiName="Out Colour" }}}
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
