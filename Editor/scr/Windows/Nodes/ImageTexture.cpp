//
// Created by Joran Vandenbroucke on 27/05/2022.
//

#include <format>
#include "ImageTexture.h"

#include "Attribute.h"
#include "../../EditorGUI/EditorGui.h"


namespace BalEditor
{
    CImageTexture::CImageTexture( int id, int& attributeStartId )
            : INode( id, attributeStartId ),
              m_textureVariableName{ "defaultTexture" },
              m_textureType{ ETextureType::Texture2D }
    {
        attributeStartId += 1;
    }

    void CImageTexture::Draw()
    {
        ImGui::PushItemWidth( 200 );
        ImNodes::BeginNode( m_id );

        ImNodes::BeginNodeTitleBar();
        ImGui::Text( "Texture" );
        ImNodes::EndNodeTitleBar();
        char      name[64]{};
        for ( int i{}; i < m_textureVariableName.size(); ++i )
        {
            name[i] = m_textureVariableName[i];
        }
        if ( ImGui::InputText( "Texture Variable Name", name, 64, ImGuiInputTextFlags_EnterReturnsTrue ))
        {
            m_textureVariableName = "";
            for ( int i{}; i < 64; ++i )
            {
                if ( name[i] == '\0' )
                {
                    break;
                }
                m_textureVariableName += name[i];
            }
        }
        {
            if ( ImGui::BeginCombo( "##Mode", ToString( m_textureType )))
            {
                for ( int n = 0; n < static_cast<int>( ETextureType::MaxIndex ); n++ )
                {
                    const bool isSelected = ( static_cast<int>( m_textureType ) == n );
                    if ( ImGui::Selectable( ToString( static_cast<ETextureType>( n )), isSelected ))
                    {
                        m_textureType = static_cast<ETextureType>( n );
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if ( isSelected )
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }
        ImGui::Spacing();
        DrawOutputTextureAttribute( m_attributeStartId );

        ImNodes::EndNode();
    }

    void CImageTexture::Attach( int endAttr )
    {
        (void) endAttr;
    }

    void CImageTexture::Detach( int endAttr )
    {
        (void) endAttr;
    }

    std::string CImageTexture::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
    {
        (void) begin;
        (void) bindings;
        (void) includes;
        (void) attributeType;
        std::string shader;
        //todo: allow for custom UVs and Lod/Grad sampling
        switch ( m_textureType )
        {
            case ETextureType::Texture2D:
                shader = std::format( "GetTextureDiffused({},fragTexCoord).xyz", m_textureVariableName );
                bindings.insert( "sampler2D " + m_textureVariableName );
                break;
            case ETextureType::MaxIndex:
                break;
        }
        return shader;
    }

    bool CImageTexture::HasFreeAttachment( int endAttr ) const
    {
        (void) endAttr;
        return false;
    }

    int CImageTexture::GetId() const
    {
        return m_id;
    }

    std::vector<int> CImageTexture::GetInputs() const
    {
        return { 0 };
    }

    const char* CImageTexture::ToString( ETextureType mode )
    {
        switch ( mode )
        {
            case ETextureType::Texture2D:
                return "Texture 2D";
            case ETextureType::MaxIndex:
                break;
        }
        return nullptr;
    }
} // BalEditor