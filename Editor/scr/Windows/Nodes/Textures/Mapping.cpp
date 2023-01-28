#include "Mapping.h"

#include <string>
#include <format>

#include "../../../EditorGUI/EditorGui.h"
#include "../Attribute.h"

CMapping::CMapping( const int id, int& attributeStartId )
        : INode{ id, attributeStartId }
        , m_connected{}
        , m_vector{}
        , m_position{}
        , m_rotation{}
        , m_scale{}
        , m_type{ EMode::Point }
{
    attributeStartId += 5;
}

CMapping::~CMapping() = default;

void CMapping::Draw()
{
    uint64_t                 current{ (uint64_t) m_type };
    std::vector<std::string> options;
    
    for ( int n = 0; n < (int) EMode::MaxIndex; n++ )
    {
        options.emplace_back( ToString( EMode( n )));
    }
    ImNodes::BeginNode( m_id );
    
    ImNodes::BeginNodeTitleBar();
    FawnForge::GUI::DrawText( ToString( m_type ));
    ImNodes::EndNodeTitleBar();
    FawnForge::GUI::DrawComboBox( "Mode", current, options, {}, 42 );
    DrawInputVectorAttribute( m_vector, m_attributeStartId, false, "Vector" );
    DrawInputVectorAttribute( m_position, m_attributeStartId + 1, m_connected[1], "Position" );
    DrawInputVectorAttribute( m_rotation, m_attributeStartId + 2, m_connected[2], "Rotation" );
    DrawInputVectorAttribute( m_scale, m_attributeStartId + 3, m_connected[3], "Scale" );
    
    FawnForge::GUI::Spacing();
    DrawOutputVectorAttribute( m_attributeStartId + 4 );
    
    ImNodes::EndNode();
    m_type = (EMode) current;
}

void CMapping::Attach( int endAttr )
{
    if ( m_attributeStartId == endAttr )
    {
        m_connected[0] = true;
    }
    if ( m_attributeStartId + 1 == endAttr )
    {
        m_connected[1] = true;
    }
    if ( m_attributeStartId + 2 == endAttr )
    {
        m_connected[2] = true;
    }
    if ( m_attributeStartId + 3 == endAttr )
    {
        m_connected[3] = true;
    }
}

void CMapping::Detach( int endAttr )
{
    if ( m_attributeStartId == endAttr )
    {
        m_connected[0] = false;
    }
    if ( m_attributeStartId + 1 == endAttr )
    {
        m_connected[1] = false;
    }
    if ( m_attributeStartId + 2 == endAttr )
    {
        m_connected[2] = false;
    }
    if ( m_attributeStartId + 3 == endAttr )
    {
        m_connected[3] = false;
    }
}

std::string CMapping::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void) begin;
    (void) bindings;
    (void) attributeType;
    includes.insert( "mapping.glsl" );
    std::string shader;
    switch ( m_type )
    {
        case EMode::Point:
            shader = std::format(
                    "MappingPoint({},{},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_vector[0], m_vector[1], m_vector[2] ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_position[0], m_position[1], m_position[2] ),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_rotation[0], m_rotation[1], m_rotation[2] ),
                    m_connected[3]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_scale[0], m_scale[1], m_scale[2] ));
            break;
        case EMode::Texture:
            shader = std::format(
                    "MappingTexture({},{},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_vector[0], m_vector[1], m_vector[2] ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_position[0], m_position[1], m_position[2] ),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_rotation[0], m_rotation[1], m_rotation[2] ),
                    m_connected[3]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_scale[0], m_scale[1], m_scale[2] ));
            break;
        case EMode::Vector:
            shader = std::format(
                    "MappingVector({},{},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_vector[0], m_vector[1], m_vector[2] ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_position[0], m_position[1], m_position[2] ),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_rotation[0], m_rotation[1], m_rotation[2] ),
                    m_connected[3]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_scale[0], m_scale[1], m_scale[2] ));
            
            break;
        case EMode::Normal:
            shader = std::format(
                    "MappingNormal({},{},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_vector[0], m_vector[1], m_vector[2] ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_position[0], m_position[1], m_position[2] ),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_rotation[0], m_rotation[1], m_rotation[2] ),
                    m_connected[3]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : std::format( "vec3({},{},{})", m_scale[0], m_scale[1], m_scale[2] ));
            break;
        case EMode::MaxIndex:
            break;
    }
    
    if ( attributeType == EAttributeType::Float )
    {
        shader += ".x";
    }
    return shader;
}

bool CMapping::HasFreeAttachment( int endAttr ) const
{
    if ( m_attributeStartId == endAttr )
    {
        return !m_connected[0];
    }
    if ( m_attributeStartId + 1 == endAttr )
    {
        return !m_connected[1];
    }
    if ( m_attributeStartId + 2 == endAttr )
    {
        return !m_connected[2];
    }
    if ( m_attributeStartId + 3 == endAttr )
    {
        return !m_connected[3];
    }
    return false;
}

int CMapping::GetId() const
{
    return m_id;
}

std::vector<int> CMapping::GetInputs() const
{
    return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2, m_attributeStartId + 3 };
}

const char* CMapping::ToString( EMode type )
{
    switch ( type )
    {
        case EMode::Point:
            return "Point";
        case EMode::Texture:
            return "texture";
        case EMode::Vector:
            return "Vector";
        case EMode::Normal:
            return "Normal";
        case EMode::MaxIndex:
            break;
    }
    return "";
}
uint8_t CMapping::GetVertexFlags() const
{
    return m_vertexFlags;
}
