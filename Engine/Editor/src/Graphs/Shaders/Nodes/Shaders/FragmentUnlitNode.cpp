//
// Created by joran on 27/05/2023.
//


#include "FragmentUnlitNode.h"
#include "../Attribute.h"

CFragmentUnlitNode::CFragmentUnlitNode( int& id )
        : CShaderNode{
        id,
        { SSocketType{ .type=SSocketType::var_type_color, .name="colour", .uiName="Colour" },},
        { SSocketType{ .type=SSocketType::var_type_shader, .name="shader", .uiName="Fragment Shader" }}}
        {
        }
void CFragmentUnlitNode::Draw()
{
    StartNode( "Fragment Output", GetId());
    CShaderNode::Draw();
    EndNode();
}
std::string CFragmentUnlitNode::GetCode() const
{
    return "fragment_specular(colour, outFragcolor);";
}
void CFragmentUnlitNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.shaderTypes = shader_stage_fragment;
    shaderInfo.AddInclude( "node_fragment.glsl" );
    shaderInfo.AddBinding(
            { SShaderBinding::layout_output, SShaderBinding::value_type_vec_4, 0, 0, 0, "outFragcolor" },
            -99
    );
}
