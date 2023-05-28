//
// Created by joran on 27/05/2023.
//


#include "SeparateRGB.h"
#include "../Attribute.h"

CSeparateRGB::CSeparateRGB( int& id )
        : CShaderNode{
        id, {
                SSocketType{ .type=SSocketType::var_type_float, .name="color", .uiName="Colour" }
                }, {
                SSocketType{ .type=SSocketType::var_type_float, .name="outR", .uiName="R" },
                SSocketType{ .type=SSocketType::var_type_float, .name="outG", .uiName="G" },
                SSocketType{ .type=SSocketType::var_type_vector_4, .name="outB", .uiName="B" }
        }}
{

}
void CSeparateRGB::Draw()
{
    StartNode( "Separate RGB", GetId());
    CShaderNode::Draw();
    EndNode();
}
std::string CSeparateRGB::GetCode() const
{
    return "separate_rgb(color, outR, outG, outB);";
}
void CSeparateRGB::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_separate_combine.glsl" );
}
