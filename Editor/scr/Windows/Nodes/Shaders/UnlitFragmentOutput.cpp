//
// Created by joran on 28/01/2023.
//

#include "UnlitFragmentOutput.h"

#include "../../../EditorGUI/EditorGui.h"
#include "../Attribute.h"

CUnlitFragmentOutput::CUnlitFragmentOutput( int id, int& attributeStartId )
        : INode( id, attributeStartId )
        , m_connected{}
{
    attributeStartId += 2;
}
void CUnlitFragmentOutput::Draw()
{
    ImNodes::BeginNode( m_id );
    
    ImNodes::BeginNodeTitleBar();
    FawnForge::GUI::DrawText( "Unlit Fragment Output" );
    ImNodes::EndNodeTitleBar();
    
    DrawInputColorAttribute( m_color, m_attributeStartId, m_connected, "Diffuse Color" );
    
    DrawOutputShaderAttribute( "Output:", m_attributeStartId + 1 );
    ImNodes::EndNode();
}
void CUnlitFragmentOutput::Attach( int endAttr )
{
    if (endAttr == m_attributeStartId)
        m_connected = true;
}
void CUnlitFragmentOutput::Detach( int endAttr )
{
    if (endAttr == m_attributeStartId)
        m_connected = false;
}
std::string CUnlitFragmentOutput::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void)bindings;
    (void)includes;
    (void)attributeType;
    return m_connected
           ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
           : ( "vec3(" + std::to_string( m_color[0] ) + "," + std::to_string( m_color[1] ) + "," + std::to_string(
                    m_color[2]
            ) + ")" );
}
bool CUnlitFragmentOutput::HasFreeAttachment( int endAttr ) const
{if (endAttr == m_attributeStartId)
        return m_connected;
    return false;
}
int CUnlitFragmentOutput::GetId() const
{
    return m_id;
}
std::vector<int> CUnlitFragmentOutput::GetInputs() const
{
    std::vector<int> inputs( 2, m_attributeStartId );
    ++inputs[1];
    return inputs;
}
uint8_t CUnlitFragmentOutput::GetVertexFlags() const
{
    return 0;
}
