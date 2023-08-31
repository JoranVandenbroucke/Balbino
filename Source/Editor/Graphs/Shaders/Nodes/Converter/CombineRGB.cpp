//
// Created by Joran Vandenbroucke on 27/05/2023.
//

#include "CombineRGB.h"
#include "../Attribute.h"

CCombineRGB::CCombineRGB( int& id )
    : CShaderNode { id,
                    { SSocketType { .type = SSocketType::var_type::float_type, .name = "r", .uiName = "R" }, SSocketType { .type = SSocketType::var_type::float_type, .name = "g", .uiName = "G" },
                      SSocketType { .type = SSocketType::var_type::float_type, .name = "b", .uiName = "B" } },
                    { SSocketType { .type = SSocketType::var_type::vector4, .name = "outColor", .uiName = "Colour" } } }
{
}
void CCombineRGB::Draw() noexcept
{
    StartNode( "Combine RGB", GetId() );
    CShaderNode::Draw();
    EndNode();
}
std::string CCombineRGB::GetCode() const
{
    return "combine_rgb(r, g, b, outColor)";
}
void CCombineRGB::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_separate_combine.glsl" );
}
