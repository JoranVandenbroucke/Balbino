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
        StartNode( "Texture", m_id );
        char      name[64]{};
        for ( int i{}; i < m_textureVariableName.size(); ++i )
        {
            name[i] = m_textureVariableName[i];
        }
        if ( BalEditor::EditorGUI::DrawInputText( "Texture Variable Name", name, 64 ))
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
            std::string currentValue{ ToString( m_textureType ) };
            if ( BalEditor::EditorGUI::DrawComboBox( "##Mode", currentValue, { ToString( ETextureType::Texture2D ) } ))
            {
                if ( currentValue == ToString( ETextureType::Texture2D ))
                {
                    m_textureType = ETextureType::Texture2D;
                }
            }
        }
        BalEditor::EditorGUI::Spacing();
        DrawOutputTextureAttribute( m_attributeStartId );

        EndNode();
    }

    void CImageTexture::Attach( int endAttr )
    {
        (void) endAttr;
    }

    void CImageTexture::Detach( int endAttr )
    {
        (void) endAttr;
    }

    std::string CImageTexture::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType attributeType )
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