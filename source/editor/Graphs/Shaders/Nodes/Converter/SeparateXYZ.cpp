//
// Created by Joran Vandenbroucke on 27/05/2023.
//

#include "SeparateXYZ.h"
#include "../Attribute.h"

CSeparateXyz::CSeparateXyz( int& id )
    : CShaderNode { id,
                    { SSocketType { .type = SSocketType::var_type::vector3, .name = "vector", .uiName = "Vector" } },
                    { SSocketType { .type = SSocketType::var_type::float_type, .name = "outX", .uiName = "X" }, SSocketType { .type = SSocketType::var_type::float_type, .name = "outY", .uiName = "Y" },
                      SSocketType { .type = SSocketType::var_type::float_type, .name = "outZ", .uiName = "Z" } } }
{
}
void CSeparateXyz::Draw() noexcept
{
    StartNode( "Separate XYZ", GetId() );
    CShaderNode::Draw();
    EndNode();
}
std::string CSeparateXyz::GetCode() const
{
    return "separate_xyz(vector, outX, outY, outZ);";
}
void CSeparateXyz::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_separate_combine.glsl" );
}
