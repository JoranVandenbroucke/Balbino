#include <unordered_set>
#include "ClampNode.h"
#include "../Attribute.h"

CClampNode::CClampNode( int& id )
        : CShaderNode{
        id, {
                SSocketType{ true, SSocketType::var_type_float, SSocketType::compiler_define( 0 ), "val", "value" },
                SSocketType{ true, SSocketType::var_type_float, SSocketType::compiler_define( 0 ), "min", "Min" },
                SSocketType{ false, SSocketType::var_type_float, SSocketType::compiler_define( 0 ), "max", "Max" }}, {
                SSocketType{ true, SSocketType::var_type_color, SSocketType::compiler_define( 0 ), "result", "Result" }}}
{
}
void CClampNode::Draw()
{
    StartNode( "Clamp", GetId());
    CShaderNode::Draw();
    EndNode();
}
std::string CClampNode::GetCode() const
{
    return "math_clamp(val, min, max, result)";
}
void CClampNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude("node_math.glsl");
}
