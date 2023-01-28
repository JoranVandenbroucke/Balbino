#include "Clamp.h"

#include <string>
#include <format>

#include "../../../EditorGUI/EditorGui.h"
#include "../Attribute.h"

CClamp::CClamp( int id, int& firstAttributeId )
        : INode{ id, firstAttributeId }
          , m_connected{ false }
          , m_value{ 0 }
{
    firstAttributeId += 4;
}

void CClamp::Draw()
{
    ImNodes::BeginNode( m_id );
    
    ImNodes::BeginNodeTitleBar();
    BalEditor::GUI::DrawText( "Clamp" );
    ImNodes::EndNodeTitleBar();
    
    DrawInputFloatAttribute( m_value[0], m_attributeStartId, m_connected[0] );
    DrawInputFloatAttribute( m_value[1], m_attributeStartId, m_connected[1], ":Min" );
    DrawInputFloatAttribute( m_value[2], m_attributeStartId, m_connected[2], ":Max" );
    
    BalEditor::GUI::Spacing();
    DrawOutputFloatAttribute( m_attributeStartId + 3 );
    
    ImNodes::EndNode();
}

void CClamp::Attach( int endAttr )
{
    if ( m_attributeStartId <= endAttr && m_attributeStartId + 4 > endAttr )
    {
        m_connected[endAttr - m_attributeStartId] = true;
    }
}

void CClamp::Detach( int endAttr )
{
    if ( m_attributeStartId <= endAttr && m_attributeStartId + 4 > endAttr )
    {
        m_connected[endAttr - m_attributeStartId] = false;
    }
}

std::string CClamp::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void) begin;
    (void) bindings;
    (void) attributeType;
    std::string shader;
    shader = std::format(
            "clamp({},{},{})",
            m_connected[0]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_value[0] ),
            m_connected[0]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_value[1] ),
            m_connected[0]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_value[2] ));
    if ( attributeType != EAttributeType::Float )
    {
        shader = std::format( "vec3({})", shader );
    }
    return shader;
}

bool CClamp::HasFreeAttachment( int endAttr ) const
{
    if ( m_attributeStartId <= endAttr && m_attributeStartId + 4 > endAttr )
    {
        return !m_connected[endAttr - m_attributeStartId];
    }
    return false;
}

int CClamp::GetId() const
{
    return m_id;
}

std::vector<int> CClamp::GetInputs() const
{
    return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2 };
}
uint8_t CClamp::GetVertexFlags() const
{
    return m_vertexFlags;
}
