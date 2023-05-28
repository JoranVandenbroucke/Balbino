#include <unordered_set>
#include "ClampNode.h"
#include "../Attribute.h"

CClampNode::CClampNode( int& id )
        : CShaderNode{
        id, {
                SSocketType{ .type=SSocketType::var_type_float, .name="val", .uiName="value" },
                SSocketType{ .type=SSocketType::var_type_float, .name="min", .uiName="Min" },
                SSocketType{ .type=SSocketType::var_type_float, .name="max", .uiName="Max" }}, {
                SSocketType{ .type=SSocketType::var_type_color, .name="result", .uiName="Result" }}}
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
