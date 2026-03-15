#include "MappingNode.h"

#include "../../../../Renderer/EditorGui.h"
#include "../Attribute.h"

CMappingNode::CMappingNode( int& id )
    : CShaderNode { id,
                    { SSocketType { .hasEditorValues = false, .type = SSocketType::var_type::vector3, .name = "vector", .uiName = "Vector" }, SSocketType { .type = SSocketType::var_type::vector3, .name = "location", .uiName = "Location" },
                      SSocketType { .type = SSocketType::var_type::vector3, .name = "rotation", .uiName = "Rotation" }, SSocketType { .type = SSocketType::var_type::vector3, .name = "scale", .uiName = "Scale" } },
                    { SSocketType { .type = SSocketType::var_type::vector3, .name = "outVector", .uiName = "Vector" } } }
{
    m_allNames.reserve( mapping_type::mapping_type_max );
    for ( int n = 0; n < mapping_type::mapping_type_max; ++n )
    {
        m_allNames.emplace_back( ToString( mapping_type( n ) ) );
    }
}

void CMappingNode::Draw() noexcept
{
    uint64_t type { (uint64_t)m_type };
    StartNode( m_allNames[ type ].c_str(), GetId() );
    if ( FawnForge::Gui::ComboBox( "mode", type, m_allNames, {}, 100.f, true ) )
    {
        m_type = (mapping_type)type;
    }
    CShaderNode::Draw();
    EndNode();
}
std::string CMappingNode::GetCode() const
{
    std::string prefix { "mapping_" };
    std::string postfix { "(vector, location, rotation, scale, outVector);" };
    switch ( m_type )
    {
        case mapping_type_point: return prefix + "point" + postfix;
        case mapping_type_texture: return prefix + "texture" + postfix;
        case mapping_type_vector: return prefix + "vector" + postfix;
        case mapping_type_normal: return prefix + "normal" + postfix;
        case mapping_type_max: break;
    }
    return "";
}
void CMappingNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_mapping.glsl" );
}
const char* CMappingNode::ToString( mapping_type type )
{
    switch ( type )
    {
        case mapping_type_point: return "Point";
        case mapping_type_texture: return "texture";
        case mapping_type_vector: return "Vector";
        case mapping_type_normal: return "Normal";
        case mapping_type_max: break;
    }
    return "";
}
