#include "Displacement.h"

#include "../Attribute.h"
#include "../../../../Renderer/EditorGui.h"

CDisplacementNode::CDisplacementNode( int& id )
	: CShaderNode{
        id,
        {
                SSocketType{ .type=SSocketType::var_type_float, .name="height", .uiName="Height" },
                SSocketType{ .type=SSocketType::var_type_float, .name="mid_level", .uiName="Mid Level" },
                SSocketType{ .type=SSocketType::var_type_float, .name="strength", .uiName="Strength" },
                SSocketType{ .hasEditorValues=false, .type=SSocketType::var_type_vector_3, .name="vector", .uiName="Vector" }},
        { SSocketType{ .type=SSocketType::var_type_vector_3, .name="normal", .uiName="Normal" }}}
{
}

void CDisplacementNode::Draw()
{
    StartNode( "Displacement Map", GetId() );
    CShaderNode::Draw();
    ImNodes::EndNode();
}
std::string CDisplacementNode::GetCode() const
{
    return "displacement( height, mid_level, strength, vector, normal);";
}
void CDisplacementNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude("node_displacement.glsl");
}

