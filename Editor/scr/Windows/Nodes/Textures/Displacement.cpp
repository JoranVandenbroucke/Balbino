#include "Displacement.h"

#include <string>
#include <format>

#include "../../../EditorGUI/EditorGui.h"
#include "../Attribute.h"

CDisplacement::CDisplacement( const int id, int& attributeStartId )
        : INode{ id, attributeStartId }
          , m_connections{}
          , m_height{ 0 }
          , m_midLevel{ 0 }
          , m_strength{ 0 }
          , m_normal{ 0, 1, 0 }
{
    attributeStartId += 5;
}

CDisplacement::~CDisplacement()
{
}

void CDisplacement::Draw()
{
    ImNodes::BeginNode( m_id );
    ImNodes::BeginNodeTitleBar();
    BalEditor::GUI::DrawText( "Displacement Map" );
    ImNodes::EndNodeTitleBar();
    
    DrawInputFloatAttribute( m_strength, m_attributeStartId, m_connections[0], "Height", 0, 1 );
    DrawInputFloatAttribute( m_strength, m_attributeStartId + 1, m_connections[1], "Mid Level", 0, 1 );
    DrawInputFloatAttribute( m_strength, m_attributeStartId + 2, m_connections[2], "Strength" );
    DrawInputVectorAttribute( m_normal, m_attributeStartId + 3, false, "Normal" );
    
    BalEditor::GUI::Spacing();
    DrawOutputVectorAttribute( m_attributeStartId + 4, "Normal" );
    ImNodes::EndNode();
}

void CDisplacement::Attach( int endAttr )
{
    if ( m_attributeStartId == endAttr )
    {
        m_connections[0] = true;
    }
    else if ( m_attributeStartId + 1 == endAttr )
    {
        m_connections[1] = true;
    }
    else if ( m_attributeStartId + 2 == endAttr )
    {
        m_connections[2] = true;
    }
    else if ( m_attributeStartId + 3 == endAttr )
    {
        m_connections[3] = true;
    }
}

void CDisplacement::Detach( int endAttr )
{
    if ( m_attributeStartId == endAttr )
    {
        m_connections[0] = false;
    }
    else if ( m_attributeStartId + 1 == endAttr )
    {
        m_connections[1] = false;
    }
    else if ( m_attributeStartId + 2 == endAttr )
    {
        m_connections[2] = false;
    }
    else if ( m_attributeStartId + 3 == endAttr )
    {
        m_connections[3] = false;
    }
}

std::string CDisplacement::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void) begin;
    (void) bindings;
    (void) attributeType;
    //vec3 Displacement( float height, float midlevel, float scale, vec3 normal )
    includes.insert( "displacement.glsl" );
    std::string shader;
    shader = std::format(
            "Displacement({},{},{},{})",
            m_connections[0]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_height ),
            m_connections[1]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_midLevel ),
            m_connections[2]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_strength ),
            m_connections[3] ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector ) : "normal"
    );
    if ( attributeType == EAttributeType::Float )
    {
        shader += ".x";
    }
    return shader;
}

bool CDisplacement::HasFreeAttachment( int endAttr ) const
{
    if ( m_attributeStartId == endAttr )
    {
        return !m_connections[0];
    }
    if ( m_attributeStartId + 1 == endAttr )
    {
        return !m_connections[1];
    }
    if ( m_attributeStartId + 2 == endAttr )
    {
        return !m_connections[2];
    }
    if ( m_attributeStartId + 3 == endAttr )
    {
        return !m_connections[3];
    }
    return false;
}

int CDisplacement::GetId() const
{
    return m_id;
}

std::vector<int> CDisplacement::GetInputs() const
{
    return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2, m_attributeStartId + 3 };
}
uint8_t CDisplacement::GetVertexFlags() const
{
    return m_vertexFlags;
}
