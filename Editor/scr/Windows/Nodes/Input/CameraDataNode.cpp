//
// Created by joran on 05/12/2022.
//

#include "CameraDataNode.h"

#include "../../../EditorGUI/EditorGui.h"
#include "../Attribute.h"

CCameraView::CCameraView( int id, int& attributeStartId )
        : INode( id, attributeStartId )
{
    m_vertexFlags = 0b00100000;
    attributeStartId++;
}
void CCameraView::Draw()
{
    ImNodes::BeginNode( m_id );
    
    ImNodes::BeginNodeTitleBar();
    BalEditor::GUI::DrawText( "View Direction" );
    ImNodes::EndNodeTitleBar();
    
    DrawOutputShaderAttribute( "Output:", m_attributeStartId );
    ImNodes::EndNode();
}
void CCameraView::Attach( int endAttr )
{
    (void)endAttr;
}
void CCameraView::Detach( int endAttr )
{
    (void)endAttr;
}
std::string CCameraView::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void)begin;
    (void)bindings;
    (void)includes;
    (void)attributeType;
    return "normalize(modelBufferObject.viewPos.xyz - fragWorldPos.xyz);";
}
bool CCameraView::HasFreeAttachment( int endAttr ) const
{
    (void)endAttr;
    return false;
}
int CCameraView::GetId() const
{
    return m_id;
}
std::vector<int> CCameraView::GetInputs() const
{
    return {};
}
uint8_t CCameraView::GetVertexFlags() const
{
    return m_vertexFlags;
}

CCameraZ::CCameraZ( int id, int& attributeStartId )
        : INode( id, attributeStartId )
{
    m_vertexFlags = 0b00100000;
    attributeStartId++;
}
void CCameraZ::Draw()
{
    ImNodes::BeginNode( m_id );
    
    ImNodes::BeginNodeTitleBar();
    BalEditor::GUI::DrawText( "View Z" );
    ImNodes::EndNodeTitleBar();
    
    DrawOutputShaderAttribute( "Output:", m_attributeStartId );
    ImNodes::EndNode();
}
void CCameraZ::Attach( int endAttr )
{
    (void)endAttr;
}
void CCameraZ::Detach( int endAttr )
{
    (void)endAttr;
}
std::string CCameraZ::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void)begin;
    (void)bindings;
    (void)includes;
    (void)attributeType;
    return "modelBufferObject.viewPos.z - fragWorldPos.z;";
}
bool CCameraZ::HasFreeAttachment( int endAttr ) const
{
    (void)endAttr;
    return false;
}
int CCameraZ::GetId() const
{
    return m_id;
}
std::vector<int> CCameraZ::GetInputs() const
{
    return {};
}
uint8_t CCameraZ::GetVertexFlags() const
{
    return m_vertexFlags;
}

CCameraDepth::CCameraDepth( int id, int& attributeStartId )
        : INode( id, attributeStartId )
{
    m_vertexFlags = 0b00100000;
    attributeStartId++;
}
void CCameraDepth::Draw()
{
    ImNodes::BeginNode( m_id );
    
    ImNodes::BeginNodeTitleBar();
    BalEditor::GUI::DrawText( "View Z" );
    ImNodes::EndNodeTitleBar();
    
    DrawOutputShaderAttribute( "Output:", m_attributeStartId );
    ImNodes::EndNode();
}
void CCameraDepth::Attach( int endAttr )
{
    (void)endAttr;
}
void CCameraDepth::Detach( int endAttr )
{
    (void)endAttr;
}
std::string CCameraDepth::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void)begin;
    (void)bindings;
    (void)includes;
    (void)attributeType;
    return "length(modelBufferObject.viewPos.xyz - fragWorldPos.xyz);";
}
bool CCameraDepth::HasFreeAttachment( int endAttr ) const
{
    (void)endAttr;
    return false;
}
int CCameraDepth::GetId() const
{
    return m_id;
}
std::vector<int> CCameraDepth::GetInputs() const
{
    return {};
}
uint8_t CCameraDepth::GetVertexFlags() const
{
    return m_vertexFlags;
}

