//
// Created by Joran Vandenbroucke on 12/03/2023.
//

#include "GammaNode.h"
#include "../Attribute.h"
#include <unordered_set>

CGammaNode::CGammaNode( int& id )
    : CShaderNode { id,
                    { SSocketType { .type = SSocketType::var_type::color, .name = "colour", .uiName = "Colour" }, SSocketType { .type = SSocketType::var_type::float_type, .name = "gamma", .uiName = "Gama" } },
                    { SSocketType { .type = SSocketType::var_type::color, .name = "out_colour", .uiName = "Out Colour" } } }
{
}
void CGammaNode::Draw() noexcept
{
    StartNode( "Gamma", GetId() );
    CShaderNode::Draw();
    EndNode();
}
std::string CGammaNode::GetCode() const
{
    return "color_gamma(colour, gamma, out_colour);";
}
void CGammaNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_color.glsl" );
}
