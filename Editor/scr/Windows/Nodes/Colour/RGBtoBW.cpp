#include "RGBtoBW.h"

#include <string>
#include <format>

#include "../Attribute.h"
#include "../../../EditorGUI/EditorGui.h"

CRGBtoBW::CRGBtoBW( const int id, int& attributeStartId )
        : INode{ id, attributeStartId }
          , m_connections{ false }
          , m_color{}
{
    attributeStartId += 2;
}

CRGBtoBW::~CRGBtoBW()
= default;

void CRGBtoBW::Draw()
{
    ImNodes::BeginNode( m_id );
    ImNodes::BeginNodeTitleBar();
    FawnForge::GUI::DrawText( "RGB To BW" );
    ImNodes::EndNodeTitleBar();
    
    DrawInputColorAttribute( m_color, m_attributeStartId, m_connections );
    
    FawnForge::GUI::Spacing();
    DrawOutputFloatAttribute( m_attributeStartId + 1 );
    
    ImNodes::EndNode();
}

void CRGBtoBW::Attach( int endAttr )
{
    if ( m_attributeStartId == endAttr )
    {
        m_connections = true;
    }
}

void CRGBtoBW::Detach( int endAttr )
{
    if ( m_attributeStartId == endAttr )
    {
        m_connections = false;
    }
}

std::string CRGBtoBW::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void) begin;
    (void) bindings;
    (void) attributeType;
    includes.insert( "RGBtoBW.glsl" );
    std::string shader;
    shader = std::format( "RGBtoBW({})",
                          m_connections
                          ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                          : std::format( "vec3({})", m_color[0], m_color[1], m_color[2] ));
    
    if ( attributeType != EAttributeType::Float )
    {
        shader += std::format( "vec3({})", shader );
    }
    return shader;
}

bool CRGBtoBW::HasFreeAttachment( int endAttr ) const
{
    if ( m_attributeStartId == endAttr )
    {
        return !m_connections;
    }
    return false;
}

int CRGBtoBW::GetId() const
{
    return m_id;
}

std::vector<int> CRGBtoBW::GetInputs() const
{
    return { m_attributeStartId };
}
uint8_t  CRGBtoBW::GetVertexFlags() const
{
    return m_vertexFlags;
}
