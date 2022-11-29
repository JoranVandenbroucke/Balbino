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
              m_type{ EShaderCombos::Fragment },
              m_connections{},
              m_currentlyEvaluating{ 1 }
    {
        attributeStartId += 8;
    }

    void CShaderNode::Draw()
    {
        StartNode( "Shader Output", m_id );
        if ( ImGui::BeginCombo( "##Mode", ToString( m_type )))
        {
            for ( int n = 0; n < static_cast<int>( EShaderCombos::MaxIndex ); n++ )
            {
                const bool isSelected = ( static_cast<int>( m_type ) == n );
                if ( ImGui::Selectable( ToString( static_cast<EShaderCombos::Enum>( n )), isSelected ))
                {
                    m_type = static_cast<EShaderCombos::Enum>( n );
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if ( isSelected )
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        GUI::Spacing();
        switch ( m_type )
        {
            case EShaderCombos::Vertex:
                DrawInputShaderAttribute( "Vertex Shader", m_attributeStartId );
                break;
            case EShaderCombos::Geometry:
                DrawInputShaderAttribute( "Vertex Shader", m_attributeStartId );
                DrawInputShaderAttribute( "Geometry Shader", m_attributeStartId + 1 );
                DrawInputShaderAttribute( "Fragment Shader", m_attributeStartId + 2 );
                break;
            case EShaderCombos::Fragment:
                DrawInputShaderAttribute( "Vertex Shader", m_attributeStartId );
                DrawInputShaderAttribute( "Fragment Shader", m_attributeStartId + 2 );
                break;
            case EShaderCombos::RTX:
                DrawInputShaderAttribute( "Task", m_attributeStartId );
                DrawInputShaderAttribute( "Mesh", m_attributeStartId + 1 );
                DrawInputShaderAttribute( "Raygen", m_attributeStartId + 2 );
                DrawInputShaderAttribute( "Any Hit", m_attributeStartId + 3 );
                DrawInputShaderAttribute( "Closest Hit", m_attributeStartId + 2 );
                DrawInputShaderAttribute( "Miss", m_attributeStartId + 5 );
                DrawInputShaderAttribute( "Intersection", m_attributeStartId + 6 );
                DrawInputShaderAttribute( "Callable", m_attributeStartId + 7 );
                break;
            case EShaderCombos::MaxIndex:
                break;
        }
        EndNode();

        /*
         * "Vertex";
         * "Geometry"
         * "Fragment"
         * "Ray Tracing";
         */
        if ( m_type == EShaderCombos::RTX )
        {
            m_currentlyEvaluating = 4;
        }
        else
        {
            m_currentlyEvaluating = 1;
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

    std::string CShaderNode::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
    {
        (void) begin;
        (void) bindings;
        (void) includes;
        (void) attributeType;
        std::string shader;
        shader = ( *begin++ )->Evaluate( begin, bindings, includes, attributeType );
        switch ( m_type )
        {
            case EShaderCombos::Vertex:
                m_currentlyEvaluating = 1;
                break;
            case EShaderCombos::Geometry:
                if ( m_currentlyEvaluating >= 4 )
                {
                    m_currentlyEvaluating = 1;
                }
                break;
            case EShaderCombos::Fragment:
                if ( m_currentlyEvaluating == 2 )
                {
                    m_currentlyEvaluating = 3;
                }
                else if ( m_currentlyEvaluating >= 4 )
                {
                    m_currentlyEvaluating = 1;
                }
                break;
            case EShaderCombos::RTX:

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

    const char* CShaderNode::ToString( EShaderCombos::Enum type )
    {
        switch ( type )
        {
            case EShaderCombos::Vertex:
                return "Vertex";
            case EShaderCombos::Geometry:
                return "Geometry";
            case EShaderCombos::Fragment:
                return "Fragment";
            case EShaderCombos::RTX:
                return "Ray Tracing";
        }
        return nullptr;
    }
} // BalEditor