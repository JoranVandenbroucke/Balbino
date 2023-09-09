#include "Displacement.h"

#include "../../../../Renderer/EditorGui.h"
#include "../Attribute.h"

CDisplacementNode::CDisplacementNode( int& id )
    : CShaderNode { id,
                    { SSocketType { .type = SSocketType::var_type::float_type, .name = "height", .uiName = "Height" }, SSocketType { .type = SSocketType::var_type::float_type, .name = "mid_level", .uiName = "Mid Level" },
                      SSocketType { .type = SSocketType::var_type::float_type, .name = "strength", .uiName = "Strength" }, SSocketType { .hasEditorValues = false, .type = SSocketType::var_type::vector3, .name = "vector", .uiName = "Vector" } },
                    { SSocketType { .type = SSocketType::var_type::vector3, .name = "normal", .uiName = "Normal" } } }
{
}

void CDisplacementNode::Draw() noexcept
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
    shaderInfo.AddInclude( "node_displacement.glsl" );
}
