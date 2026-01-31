#include "BumpNode.h"

#include "../../../../Renderer/EditorGui.h"
#include "../Attribute.h"

CBumpNode::CBumpNode( int& id )
    : CShaderNode { id,
                    { SSocketType { .type = SSocketType::var_type::float_type, .name = "strength", .uiName = "Strength" }, SSocketType { .type = SSocketType::var_type::float_type, .name = "distance", .uiName = "Distance" },
                      SSocketType { .hasEditorValues = false, .type = SSocketType::var_type::float_type, .name = "height", .uiName = "Height" },
                      SSocketType { .hasEditorValues = false, .type = SSocketType::var_type::vector3, .name = "vector", .uiName = "Vector" } },
                    { SSocketType { .type = SSocketType::var_type::vector3, .name = "normal", .uiName = "Normal" } } }
{
}

void CBumpNode::Draw() noexcept
{
    StartNode( "Bump Node", GetId() );
    FawnForge::Gui::Toggle( "Invert", m_isInverted );
    CShaderNode::Draw();
    ImNodes::EndNode();
}
std::string CBumpNode::GetCode() const
{
    return std::string( "bump(" ) + ( m_isInverted ? "true" : "false" ) + ", strength, distance, height, vector, gl_position.xyz*gl_position.w, out vec3 outNormal);";
}
void CBumpNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_bimp.glsl" );
}
