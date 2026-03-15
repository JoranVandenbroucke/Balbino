//
// Created by Joran Vandenbroucke on 27/05/2023.
//

#include "FragmentUnlitNode.h"
#include "../Attribute.h"

CFragmentUnlitNode::CFragmentUnlitNode( int& id )
    : CShaderNode { id,
                    {
                        SSocketType { .type = SSocketType::var_type::color, .name = "colour", .uiName = "Colour" },
                        SSocketType { .type = SSocketType::var_type::float_type, .name = "alpha", .uiName = "Alpha" },
                    },
                    { SSocketType { .type = SSocketType::var_type::shader, .name = "shader", .uiName = "Fragment Shader" } } }
{
}
void CFragmentUnlitNode::Draw() noexcept
{
    StartNode( "Fragment output", GetId() );
    CShaderNode::Draw();
    EndNode();
}
std::string CFragmentUnlitNode::GetCode() const
{
    return "fragment_unlit(colour, alpha, outFragcolor);";
}
void CFragmentUnlitNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.shaderTypes = shader_stage_fragment;
    shaderInfo.AddInclude( "node_fragment.glsl" );
    shaderInfo.AddDefine( "FRAGMENT_UNLINT", "" );
    shaderInfo.AddBinding( { SShaderBinding::layout::output, SShaderBinding::value_type::vec4, 0, 0, 0, "outFragcolor" }, -99 );
}
