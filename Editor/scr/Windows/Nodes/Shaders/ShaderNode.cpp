//
// Created by joran on 25/06/2022.
//

#include "ShaderNode.h"
#include "FileParcer.h"
#include "../../../EditorGUI/EditorGui.h"
#include "../Attribute.h"

CShaderNode::CShaderNode( int id, int& attributeStartId )
        : INode( id, attributeStartId )
        , m_type{ EMode::Graphics }
        , m_connections{}
        , m_currentlyEvaluating{}
{
    attributeStartId += 6;
}

void CShaderNode::Draw()
{
    uint64_t                 current{ (uint64_t) m_type };
    std::vector<std::string> options;
    
    for ( int n = 0; n < (int) EMode::MaxIndex; n++ )
    {
        options.emplace_back( ToString( EMode( n )));
    }
    StartNode( "Shader Output", m_id );
    BalEditor::GUI::DrawComboBox( "Mode", current, options );
    
    BalEditor::GUI::Spacing();
    switch ( m_type )
    {
        case EMode::Graphics:
            DrawInputShaderAttribute( "Vertex Shader", m_attributeStartId );
            DrawInputShaderAttribute( "Tessellation Control Shader", m_attributeStartId + 1 );
            DrawInputShaderAttribute( "Tessellation Evaluation Shader", m_attributeStartId + 2 );
            DrawInputShaderAttribute( "Geometry Shader", m_attributeStartId + 3 );
            DrawInputShaderAttribute( "Fragment Shader", m_attributeStartId + 4 );
            break;
        case EMode::Mesh:
            DrawInputShaderAttribute( "Mesh", m_attributeStartId );
            DrawInputShaderAttribute( "Task", m_attributeStartId + 1 );
            break;
        case EMode::RTX:
            DrawInputShaderAttribute( "Raygen", m_attributeStartId );
            DrawInputShaderAttribute( "Any Hit", m_attributeStartId + 1 );
            DrawInputShaderAttribute( "Closest Hit", m_attributeStartId + 2 );
            DrawInputShaderAttribute( "Miss", m_attributeStartId + 3 );
            DrawInputShaderAttribute( "Intersection", m_attributeStartId + 4 );
            DrawInputShaderAttribute( "Callable", m_attributeStartId + 5 );
            break;
        case EMode::MaxIndex:
            break;
    }
    EndNode();
    
    
    m_type = (EMode) current;
}

void CShaderNode::Attach( int endAttr )
{
    (void) endAttr;
    m_connections[endAttr] = true;
}

void CShaderNode::Detach( int endAttr )
{
    (void) endAttr;
    m_connections[endAttr] = false;
}

std::string CShaderNode::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void) begin;
    (void) bindings;
    (void) includes;
    (void) attributeType;
    std::string shader;
    shader = ( *begin++ )->Evaluate( begin, bindings, includes, attributeType );
    
    /*
    0: "Vertex"
    1: "Tessellation Control"
    2: "Tessellation"
    3: "Geometry"
    4: "Fragment"
    5: "Mesh"
    6: "Task"
    8: "Raygen"
    9: "Any"
    a: "Closest"
    b: "Miss"
    c: "Intersection"
    d: "Callable"
    */
    while ( !m_connections[m_currentlyEvaluating] )
    {
        if ( m_type == EMode::Graphics )
        {
            m_currentlyEvaluating = ++m_currentlyEvaluating % 5;
        }
        else if ( m_type == EMode::Mesh )
        {
            m_currentlyEvaluating = ++m_currentlyEvaluating % 2;
        }
        else
        {
            m_currentlyEvaluating = ++m_currentlyEvaluating % 6;
        }
    }
    std::stringstream ss;
    if ( m_type == EMode::Graphics )
    {
        ss << std::hex << m_currentlyEvaluating++;
    }
    else if ( m_type == EMode::Mesh )
    {
        ss << std::hex << m_currentlyEvaluating++ + 5;
    }
    else
    {
        ss << std::hex << m_currentlyEvaluating++ + 7;
    }
    shader += ss.str();
    return shader;
}

bool CShaderNode::HasFreeAttachment( int endAttr ) const
{
    return !m_connections[endAttr];
}

int CShaderNode::GetId() const
{
    return m_id;
}

std::vector<int> CShaderNode::GetInputs() const
{
    std::vector<int> value( 7 );
    for ( int        i{ 1 }; i < 7; ++i )
    {
        value[i] = value[i - 1] + 1;
    }
    return value;
}

const char* CShaderNode::ToString( EMode type )
{
    switch ( type )
    {
        case EMode::Graphics:
            return "Graphics";
        case EMode::Mesh:
            return "Mesh";
        case EMode::RTX:
            return "Ray Tracing";
        case EMode::MaxIndex:
            break;
    }
    return nullptr;
}
uint8_t CShaderNode::GetVertexFlags() const
{
    return m_vertexFlags;
}
