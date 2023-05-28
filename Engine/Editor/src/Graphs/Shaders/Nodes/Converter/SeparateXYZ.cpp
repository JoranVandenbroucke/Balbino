//
// Created by joran on 27/05/2023.
//


#include "SeparateXYZ.h"
#include "../Attribute.h"

CSeparateXYZ::CSeparateXYZ( int& id )
        : CShaderNode{
        id, {
                SSocketType{ .type=SSocketType::var_type_vector_3, .name="vector", .uiName="Vector" }
        }, {
                SSocketType{ .type=SSocketType::var_type_float, .name="outX", .uiName="X" },
                SSocketType{ .type=SSocketType::var_type_float, .name="outY", .uiName="Y" },
                SSocketType{ .type=SSocketType::var_type_float, .name="outZ", .uiName="Z" }
        }}
{

}
void CSeparateXYZ::Draw()
{
    StartNode( "Separate XYZ", GetId());
    CShaderNode::Draw();
    EndNode();
}
std::string CSeparateXYZ::GetCode() const
{
    return "separate_xyz(vector, outX, outY, outZ);";
}
void CSeparateXYZ::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_separate_combine.glsl" );
}
