#include "NormalMapNode.h"
#include "../Attribute.h"

CNormalMapNode::CNormalMapNode( int& id )
    : CShaderNode { id,
                    { SSocketType { .type = SSocketType::var_type::float_type, .name = "strength", .uiName = "strength" }, SSocketType { .type = SSocketType::var_type::vector3, .name = "normal", .uiName = "Normal" } },
                    { SSocketType { .type = SSocketType::var_type::vector3, .name = "outVector", .uiName = "Normal" } } }
{
}
void CNormalMapNode::Draw() noexcept
{
    StartNode( "Normal Map", GetId() );
    CShaderNode::Draw();
    EndNode();
}
std::string CNormalMapNode::GetCode() const
{
    return "normal_map( imageNormal, fragNormal, fragTangent, strength, outNormal);";
}
void CNormalMapNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_normal.glsl" );
}
