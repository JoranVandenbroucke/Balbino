//
// Created by Joran Vandenbroucke on 06/06/2023.
//

#include "ConverterNode.h"

CConverterNode::CConverterNode( int& id, SSocketType::var_type left, SSocketType::var_type right )
    : CShaderNode { id, { SSocketType { .type = left, .name = "from", .uiName = "In" } }, { SSocketType { .type = right, .name = "to", .uiName = "Out" } } }
{
}
std::string CConverterNode::GetCode() const
{
    switch ( GetOutput( 0 )->GetType() )
    {
        case SSocketType::var_type::float_type: return "to_float(from, to);";
        case SSocketType::var_type::int_type: return "to_int(from, to);";
        case SSocketType::var_type::color: return "to_col(from, to);";
        case SSocketType::var_type::vector2: return "to_vec2(from, to);";
        case SSocketType::var_type::vector3: return "to_vec3(from, to);";
        case SSocketType::var_type::vector4: return "to_vec4(from, to);";
        case SSocketType::var_type::none:
        case SSocketType::var_type::shader: break;
    }
    return "";
}
void CConverterNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_value_type_converter.glsl" );
}
void CConverterNode::Draw() noexcept
{
    // Disable drawing for this node
}
