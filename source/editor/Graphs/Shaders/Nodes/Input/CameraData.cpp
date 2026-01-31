//
// Created by Joran Vandenbroucke on 27/05/2023.
//

#include "CameraData.h"
#include "../Attribute.h"

CCameraData::CCameraData( int& id )
    : CShaderNode { id,
                    {},
                    { SSocketType { .type = SSocketType::var_type::vector3, .name = "direction", .uiName = "Direction" }, SSocketType { .type = SSocketType::var_type::float_type, .name = "depth", .uiName = "Depth" },
                      SSocketType { .type = SSocketType::var_type::float_type, .name = "z_value", .uiName = "Z-Value" } } }
{
}
void CCameraData::Draw() noexcept
{
    StartNode( "Camera Data", GetId() );
    CShaderNode::Draw();
    EndNode();
}
std::string CCameraData::GetCode() const
{
    return "camera_data(direction, depth, z_value);";
}
void CCameraData::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_camera_data.glsl" );

    shaderInfo.AddBinding( { SShaderBinding::layout::input, SShaderBinding::value_type::vec4, 0, 4, 1, "fragWorldPos" }, -99 );
    shaderInfo.AddDefine( ToDefine( SSocketType::compiler_define_uses_world_position ), "", true );
}
