//
// Created by Joran Vandenbroucke on 27/05/2023.
//

#include "SeparateRGB.h"
#include "../Attribute.h"

CSeparateRgb::CSeparateRgb( int& id )
    : CShaderNode { id,
                    { SSocketType { .type = SSocketType::var_type::float_type, .name = "color", .uiName = "Colour" } },
                    { SSocketType { .type = SSocketType::var_type::float_type, .name = "outR", .uiName = "R" }, SSocketType { .type = SSocketType::var_type::float_type, .name = "outG", .uiName = "G" },
                      SSocketType { .type = SSocketType::var_type::vector4, .name = "outB", .uiName = "B" } } }
{
}
void CSeparateRgb::Draw() noexcept
{
    StartNode( "Separate RGB", GetId() );
    CShaderNode::Draw();
    EndNode();
}
std::string CSeparateRgb::GetCode() const
{
    return "separate_rgb(color, outR, outG, outB);";
}
void CSeparateRgb::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_separate_combine.glsl" );
}
