//
// Created by Joran Vandenbroucke on 27/05/2023.
//

#include "CombineXYZ.h"
#include "../Attribute.h"

CCombineXYZ::CCombineXYZ( int& id )
    : CShaderNode { id,
                    { SSocketType { .type = SSocketType::var_type::float_type, .name = "x", .uiName = "X" }, SSocketType { .type = SSocketType::var_type::float_type, .name = "y", .uiName = "Y" },
                      SSocketType { .type = SSocketType::var_type::float_type, .name = "z", .uiName = "Z" } },
                    { SSocketType { .type = SSocketType::var_type::vector3, .name = "outVector", .uiName = "Vector" } } }
{
}
void CCombineXYZ::Draw() noexcept
{
    StartNode( "Combine XYZ", GetId() );
    CShaderNode::Draw();
    EndNode();
}
std::string CCombineXYZ::GetCode() const
{
    return "combine_xyz( x, y, z, outVector);";
}
void CCombineXYZ::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_separate_combine.glsl" );
}
