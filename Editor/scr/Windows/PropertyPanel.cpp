//
// Created by joran on 23/10/2022.
//

#include "PropertyPanel.h"
#include "../EditorGUI/EditorGui.h"
#include <mini/ini.h>

namespace BalEditor
{
    void CPropertyPanel::Draw()
    {
        if ( BalEditor::EditorGUI::Begin( "Property Panel", m_isVisible, 0 ))
        {
            if ( BalEditor::EditorGUI::BeginChild( "Category", { 128, 0 }, true, 1 << 11 ))
            {
                for ( int i{}; i < (int) EPropertyTypes::MaxItems; ++i )
                {
                    if ( BalEditor::EditorGUI::DrawButton( ToString( EPropertyTypes( i )), { -1, 32 } ))
                    {
                        m_currentPropertyType = EPropertyTypes( i );
                    }
                }
            }
            BalEditor::EditorGUI::EndChild();
            BalEditor::EditorGUI::SameLine();
            if ( BalEditor::EditorGUI::BeginChild( "Properties", { 0, 0 }, true, 1 << 11 ))
            {
                switch ( m_currentPropertyType )
                {
                    case EPropertyTypes::Audio:
                        DrawAudioProperties();
                        break;
                    case EPropertyTypes::Graphics:
                        DrawGraphicsProperties();
                        break;
                    case EPropertyTypes::InputManager:
                        DrawInputManagerProperties();
                        break;
                    case EPropertyTypes::Physics:
                        DrawPhysicsProperties();
                        break;
                    case EPropertyTypes::Player:
                        DrawPlayerProperties();
                        break;
                    case EPropertyTypes::Time:
                        DrawTimeProperties();
                        break;
                    case EPropertyTypes::MaxItems:
                        break;
                }
            }
            BalEditor::EditorGUI::EndChild();
            if ( BalEditor::EditorGUI::DrawButton( "Save" ))
            {
                SaveToIni();
            }
            BalEditor::EditorGUI::DrawText( "Some changes only apply after restarting the editor" );
            BalEditor::EditorGUI::End();
        }
    }
    void CPropertyPanel::ShowWindow()
    {
        m_isVisible = true;
        BalEditor::EditorGUI::SetWindowFocus( "Property Panel" );
        LoadFromIni();
    }
    void CPropertyPanel::DrawAudioProperties()
    {
        BalEditor::EditorGUI::DrawText( "Waw such empty" );
    }
    void CPropertyPanel::DrawGraphicsProperties()
    {
        void* pData;
        BalEditor::EditorGUI::DrawIntValue( "Default Shader", m_defaultShaderID, 256 );
        if (( pData = BalEditor::EditorGUI::ReceiveDragDrop( "Shader" )) != nullptr )
        {
            m_defaultShaderID = ((SFile*) ( pData ))->uuid;
        }
        BalEditor::EditorGUI::DrawIntValue( "Default Transparent Shader", m_defaultTransparentShaderID, 256 );
        if (( pData = BalEditor::EditorGUI::ReceiveDragDrop( "Shader" )) != nullptr )
        {
            m_defaultTransparentShaderID = ((SFile*) ( pData ))->uuid;
        }
        BalEditor::EditorGUI::DrawIntValue( "Default Debug Shader", m_defaultDebugShaderID, 256 );
        if (( pData = BalEditor::EditorGUI::ReceiveDragDrop( "Shader" )) != nullptr )
        {
            m_defaultDebugShaderID = ((SFile*) ( pData ))->uuid;
        }
        BalEditor::EditorGUI::DrawIntValue( "Default Skybox Shader", m_defaultSkyboxShaderID, 256 );
        if (( pData = BalEditor::EditorGUI::ReceiveDragDrop( "Shader" )) != nullptr )
        {
            m_defaultSkyboxShaderID = ((SFile*) ( pData ))->uuid;
        }
        
        BalEditor::EditorGUI::DrawIntValue( "Default Material", m_defaultMaterialID, 256 );
        if (( pData = BalEditor::EditorGUI::ReceiveDragDrop( "Material" )) != nullptr )
        {
            m_defaultMaterialID = ((SFile*) ( pData ))->uuid;
        }
        BalEditor::EditorGUI::DrawIntValue( "Default Transparent Material", m_defaultTransparentMaterialID, 256 );
        if (( pData = BalEditor::EditorGUI::ReceiveDragDrop( "Material" )) != nullptr )
        {
            m_defaultTransparentMaterialID = ((SFile*) ( pData ))->uuid;
        }
        BalEditor::EditorGUI::DrawIntValue( "Default Debug Material", m_defaultDebugMaterialID, 256 );
        if (( pData = BalEditor::EditorGUI::ReceiveDragDrop( "Material" )) != nullptr )
        {
            m_defaultDebugMaterialID = ((SFile*) ( pData ))->uuid;
        }
        BalEditor::EditorGUI::DrawIntValue( "Default Skybox Material", m_defaultSkyboxMaterialID, 256 );
        if (( pData = BalEditor::EditorGUI::ReceiveDragDrop( "Material" )) != nullptr )
        {
            m_defaultSkyboxMaterialID = ((SFile*) ( pData ))->uuid;
        }
        
        BalEditor::EditorGUI::DrawIntValue( "Default Texture", m_defaultTextureID, 256 );
        if (( pData = BalEditor::EditorGUI::ReceiveDragDrop( "Image" )) != nullptr )
        {
            m_defaultShaderID = ((SFile*) ( pData ))->uuid;
        }
    }
    void CPropertyPanel::DrawInputManagerProperties()
    {
        BalEditor::EditorGUI::DrawText( "Waw such empty" );
    }
    void CPropertyPanel::DrawPhysicsProperties()
    {
        BalEditor::EditorGUI::DrawText( "Waw such empty" );
    }
    void CPropertyPanel::DrawPlayerProperties()
    {
        BalEditor::EditorGUI::DrawText( "Waw such empty" );
    }
    void CPropertyPanel::DrawTimeProperties()
    {
        BalEditor::EditorGUI::DrawText( "Waw such empty" );
    }
    void CPropertyPanel::SaveToIni() const
    {
        mINI::INIFile      file( "Properties.ini" );
        mINI::INIStructure ini;
        file.read( ini );
        
        ini["Graphics"]["Default_Shader"]               = std::to_string( m_defaultShaderID );
        ini["Graphics"]["Default_Transparent_Shader"]   = std::to_string( m_defaultTransparentShaderID );
        ini["Graphics"]["Default_Debug_Shader"]         = std::to_string( m_defaultDebugShaderID );
        ini["Graphics"]["Default_Skybox_Shader"]        = std::to_string( m_defaultSkyboxShaderID );
        ini["Graphics"]["Default_Material"]             = std::to_string( m_defaultMaterialID );
        ini["Graphics"]["Default_Transparent_Material"] = std::to_string( m_defaultTransparentMaterialID );
        ini["Graphics"]["Default_Debug_Material"]       = std::to_string( m_defaultDebugMaterialID );
        ini["Graphics"]["Default_Skybox_Material"]      = std::to_string( m_defaultSkyboxMaterialID );
        ini["Graphics"]["Default_Texture"]              = std::to_string( m_defaultTextureID );
        
        file.write( ini );
    }
    void CPropertyPanel::LoadFromIni()
    {
        std::ifstream test( "Properties.ini" );
        bool          fileExists{ test.good() };
        test.close();
        if ( !fileExists )
        {
            SaveToIni();
        }
        
        mINI::INIFile      file( "Properties.ini" );
        mINI::INIStructure ini;
        file.read( ini );
        
        m_defaultShaderID              = std::stoull( ini.get( "Graphics" ).get( "Default_Shader" ));
        m_defaultTransparentShaderID   = std::stoull( ini.get( "Graphics" ).get( "Default_Transparent_Shader" ));
        m_defaultDebugShaderID         = std::stoull( ini.get( "Graphics" ).get( "Default_Debug_Shader" ));
        m_defaultSkyboxShaderID        = std::stoull( ini.get( "Graphics" ).get( "Default_Skybox_Shader" ));
        m_defaultMaterialID            = std::stoull( ini.get( "Graphics" ).get( "Default_Material" ));
        m_defaultTransparentMaterialID = std::stoull( ini.get( "Graphics" ).get( "Default_Transparent_Material" ));
        m_defaultDebugMaterialID       = std::stoull( ini.get( "Graphics" ).get( "Default_Debug_Material" ));
        m_defaultSkyboxMaterialID      = std::stoull( ini.get( "Graphics" ).get( "Default_Skybox_Material" ));
        m_defaultTextureID             = std::stoull( ini.get( "Graphics" ).get( "Default_Texture" ));
    }
    const char* CPropertyPanel::ToString( CPropertyPanel::EPropertyTypes types )
    {
        switch ( types )
        {
            case EPropertyTypes::Audio:
                return "Audio";
            case EPropertyTypes::Graphics:
                return "Graphics";
            case EPropertyTypes::InputManager:
                return "Input Manager";
            case EPropertyTypes::Physics:
                return "Physics";
            case EPropertyTypes::Player:
                return "Player";
            case EPropertyTypes::Time:
                return "Time";
            case EPropertyTypes::MaxItems:
                break;
        }
        return nullptr;
    }
} // BalEditor