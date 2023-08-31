#include "ClampNode.h"
#include "../Attribute.h"
#include <unordered_set>

CClampNode::CClampNode( int& id )
    : CShaderNode { id,
                    { SSocketType { .type = SSocketType::var_type::float_type, .name = "val", .uiName = "mix_mode_value" }, SSocketType { .type = SSocketType::var_type::float_type, .name = "min", .uiName = "Min" },
                      SSocketType { .type = SSocketType::var_type::float_type, .name = "mix_mode_max", .uiName = "Max" } },
                    { SSocketType { .type = SSocketType::var_type::color, .name = "result", .uiName = "Result" } } }
{
}
void CClampNode::Draw() noexcept
{
    StartNode( "Clamp", GetId() );
    CShaderNode::Draw();
    EndNode();
}
std::string CClampNode::GetCode() const
{
    return "math_clamp(val, min, mix_mode_max, result)";
}
void CClampNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_math.glsl" );
}
