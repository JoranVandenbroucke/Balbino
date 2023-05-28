//
// Created by joran on 27/05/2023.
//


#include "CombineRGB.h"
#include "../Attribute.h"

CCombineRGB::CCombineRGB( int& id )
        : CShaderNode{
        id, {
                SSocketType{ .type=SSocketType::var_type_float, .name="r", .uiName="R" },
                SSocketType{ .type=SSocketType::var_type_float, .name="g", .uiName="G" },
                SSocketType{ .type=SSocketType::var_type_float, .name="b", .uiName="B" }}, {
                SSocketType{ .type=SSocketType::var_type_vector_4, .name="outColor", .uiName="Colour" }}}
{

}
void CCombineRGB::Draw()
{
    StartNode( "Combine RGB", GetId());
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
