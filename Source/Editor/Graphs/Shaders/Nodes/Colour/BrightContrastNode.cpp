//
// Created by Joran Vandenbroucke on 11/03/2023.
//

#include "BrightContrastNode.h"
#include "../Attribute.h"

CBrightContrastNode::CBrightContrastNode( int& id )
    : CShaderNode { id,
                    { SSocketType { .type = SSocketType::var_type::color, .name = "colour", .uiName = "Colour" }, SSocketType { .type = SSocketType::var_type::float_type, .name = "brightness", .uiName = "Brightness" },
                      SSocketType { .type = SSocketType::var_type::float_type, .name = "contrast", .uiName = "Contrast" } },
                    { SSocketType { .type = SSocketType::var_type::color, .name = "out_colour", .uiName = "Out Colour" } } }
{
}
void CBrightContrastNode::Draw() noexcept
{
    StartNode( "Brightness/Contrast", GetId() );
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