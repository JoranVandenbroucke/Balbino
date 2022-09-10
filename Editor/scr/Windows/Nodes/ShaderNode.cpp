//
// Created by joran on 25/06/2022.
//

#include "ShaderNode.h"
#include <imnodes.h>
#include <imgui.h>
#include <FileParcer.h>
#include "../../EditorGUI/EditorGui.h"
#include "Attribute.h"

namespace BalEditor
{
    CShaderNode::CShaderNode( int id, int& attributeStartId )
            : INode( id, attributeStartId ),
              m_type{ ShaderCombos::Fragment },
              m_connections{},
              m_currentlyEvaluating{ 1 }
    {
        attributeStartId += 8;
    }

    void CShaderNode::Draw()
    {
        std::string currentValue{ ToString( m_type ) };
        StartNode( "Shader Output", m_id );
        BalEditor::EditorGUI::DrawComboBox( "##Mode", currentValue, {
                "Vertex", "Geometry", "Fragment", "Ray Tracing"
        } );
        BalEditor::EditorGUI::Spacing();
        switch ( m_type )
        {
            case ShaderCombos::Vertex:
                DrawInputShaderAttribute( "Vertex Shader", m_attributeStartId );
                break;
            case ShaderCombos::Geometry:
                DrawInputShaderAttribute( "Vertex Shader", m_attributeStartId );
                DrawInputShaderAttribute( "Geometry Shader", m_attributeStartId + 1 );
                DrawInputShaderAttribute( "Fragment Shader", m_attributeStartId + 2 );
                break;
            case ShaderCombos::Fragment:
                DrawInputShaderAttribute( "Vertex Shader", m_attributeStartId );
                DrawInputShaderAttribute( "Fragment Shader", m_attributeStartId + 2 );
                break;
            case ShaderCombos::RTX:
                DrawInputShaderAttribute( "Task", m_attributeStartId );
                DrawInputShaderAttribute( "Mesh", m_attributeStartId + 1 );
                DrawInputShaderAttribute( "Raygen", m_attributeStartId + 2 );
                DrawInputShaderAttribute( "Any Hit", m_attributeStartId + 3 );
                DrawInputShaderAttribute( "Closest Hit", m_attributeStartId + 2 );
                DrawInputShaderAttribute( "Miss", m_attributeStartId + 5 );
                DrawInputShaderAttribute( "Intersection", m_attributeStartId + 6 );
                DrawInputShaderAttribute( "Callable", m_attributeStartId + 7 );
                break;
        }
        EndNode();

        if ( currentValue != ToString( m_type ))
        {
            /*
             * "Vertex";
             * "Geometry"
             * "Fragment"
             * "Ray Tracing";
             */
            if ( currentValue == "Vertex" )
            {
                m_type                = ShaderCombos::Vertex;
                m_currentlyEvaluating = 1;
            }
            else if ( currentValue == "Geometry" )
            {
                m_type                = ShaderCombos::Geometry;
                m_currentlyEvaluating = 1;
            }
            else if ( currentValue == "Fragment" )
            {
                m_type                = ShaderCombos::Fragment;
                m_currentlyEvaluating = 1;
            }
            else if ( currentValue == "Ray Tracing" )
            {
                m_type                = ShaderCombos::RTX;
                m_currentlyEvaluating = 4;
            }
        }
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

    std::string CShaderNode::Evaluate( std::_Vector_iterator<std::_Vector_val<std::_Simple_types<INode*>>>& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType attributeType )
    {
        (void) begin;
        (void) bindings;
        (void) includes;
        (void) attributeType;
        std::string shader;
        shader = ( *begin++ )->Evaluate( begin, bindings, includes, attributeType );
        switch ( m_type )
        {
            case ShaderCombos::Vertex:
                m_currentlyEvaluating = 1;
                break;
            case ShaderCombos::Geometry:
                if ( m_currentlyEvaluating >= 4 )
                {
                    m_currentlyEvaluating = 1;
                }
                break;
            case ShaderCombos::Fragment:
                if ( m_currentlyEvaluating == 2 )
                {
                    m_currentlyEvaluating = 3;
                }
                else if ( m_currentlyEvaluating >= 4 )
                {
                    m_currentlyEvaluating = 1;
                }
                break;
            case ShaderCombos::RTX:

                if ( m_currentlyEvaluating >= 9 )
                {
                    m_currentlyEvaluating = 1;
                }
                break;
        }
//        shader += std::to_string(( uint16_t ) m_type );
//        shader += std::to_string( m_currentlyEvaluating++ );
        std::stringstream ss;
        ss << std::setw( 3 ) << std::setfill( '0' ) << (uint32_t) m_type << m_currentlyEvaluating++;
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
        std::vector<int> value( 8 );
        for ( int        i{ 1 }; i < 8; ++i )
        {
            value[i] = value[i - 1] + 1;
        }
        return value;
    }

    const char* CShaderNode::ToString( ShaderCombos type )
    {
        switch ( type )
        {
            case ShaderCombos::Vertex:
                return "Vertex";
            case ShaderCombos::Geometry:
                return "Geometry";
            case ShaderCombos::Fragment:
                return "Fragment";
            case ShaderCombos::RTX:
                return "Ray Tracing";
        }
        return nullptr;
    }
} // BalEditor