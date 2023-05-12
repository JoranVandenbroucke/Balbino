//
// Created by joran on 11/03/2023.
//


#include "BrightContrastNode.h"
#include "../Attribute.h"

CBrightContrastNode::CBrightContrastNode( int& id )
        : CShaderNode{
        id, {
                SSocketType{ true, SSocketType::var_type_color, SSocketType::compiler_define( 0 ), "colour", "Colour" },
                SSocketType{
                        true,
                        SSocketType::var_type_float,
                        SSocketType::compiler_define( 0 ),
                        "brightness",
                        "Brightness"
                },
                SSocketType{
                        true,
                        SSocketType::var_type_float,
                        SSocketType::compiler_define( 0 ),
                        "contrast",
                        "Contrast"
                }}, {
                SSocketType{
                        true,
                        SSocketType::var_type_color,
                        SSocketType::compiler_define( 0 ),
                        "out_colour",
                        "Out Colour"
                }}}
{
}
void CBrightContrastNode::Draw()
{
    StartNode( "Brightness/Contrast", GetId());
    CShaderNode::Draw();
    EndNode();
}
std::string CBrightContrastNode::GetCode() const
{
    return "color_brightness_contrast( colour, brightness, contrast, out_colour);";
}
void CBrightContrastNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_color.glsl" );
}