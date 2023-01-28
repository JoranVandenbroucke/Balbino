//
// Created by joran on 23/10/2022.
//

#include "PropertyPanel.h"
#include "../EditorGUI/EditorGui.h"
#include <mini/ini.h>

namespace FawnForge
{
    CPropertyPanel::CPropertyPanel()
            : m_currentPropertyType{ EPropertyTypes::Audio }
              , m_isVisible{}
              , m_defaultShaderID{}
              , m_defaultTransparentShaderID{}
              , m_defaultClothShaderID{}
              , m_defaultSkinShaderID{}
              , m_defaultHairShaderID{}
              , m_defaultDebugShaderID{}
              , m_defaultSkyboxShaderID{}
              , m_defaultMaterialID{}
              , m_defaultTransparentMaterialID{}
              , m_defaultClothMaterialID{}
              , m_defaultSkinMaterialID{}
              , m_defaultHairMaterialID{}
              , m_defaultDebugMaterialID{}
              , m_defaultSkyboxMaterialID{}
              , m_defaultTextureID{}
    {
        LoadFromIni();
    }
    void CPropertyPanel::Draw()
    {
        if ( GUI::Begin( "Property Panel", m_isVisible, 0 ))
        {
            if ( GUI::BeginChild( "Category", { 128, 0 }, true, 1 << 11 ))
            {
                for ( int i{}; i < (int) EPropertyTypes::MaxItems; ++i )
                {
                    if ( GUI::DrawButton( ToString( EPropertyTypes::Enum( i )), { -1, 32 } ))
                    {
                        m_currentPropertyType = EPropertyTypes::Enum( i );
                    }
                }
            }
            GUI::EndChild();
            GUI::SameLine();
            if ( GUI::BeginChild( "Properties", { 0, 0 }, true, 1 << 11 ))
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
            GUI::EndChild();
            if ( GUI::DrawButton( "Save" ))
            {
                SaveToIni();
            }
            GUI::DrawText( "Some changes only apply after restarting the editor" );
            GUI::End();
        }
    }
    void CPropertyPanel::ShowWindow()
    {
        m_isVisible = true;
        GUI::SetWindowFocus( "Property Panel" );
        LoadFromIni();
    }
    void CPropertyPanel::DrawAudioProperties()
    {
        GUI::DrawText( "Waw such empty" );
    }
    void CPropertyPanel::DrawGraphicsProperties()
    {
        void* pData;
        GUI::DrawIntValue( "Default Shader", m_defaultShaderID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Shader" )) != nullptr )
        {
            m_defaultShaderID = ((SFile*) ( pData ))->uuid;
        }
        GUI::DrawIntValue( "Default Transparent Shader", m_defaultTransparentShaderID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Shader" )) != nullptr )
        {
            m_defaultTransparentShaderID = ((SFile*) ( pData ))->uuid;
        }
        GUI::DrawIntValue( "Default Cloth Shader", m_defaultClothShaderID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Shader" )) != nullptr )
        {
            m_defaultClothShaderID = ((SFile*) ( pData ))->uuid;
        }
        GUI::DrawIntValue( "Default Skin Shader", m_defaultSkinShaderID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Shader" )) != nullptr )
        {
            m_defaultSkinShaderID = ((SFile*) ( pData ))->uuid;
        }
        GUI::DrawIntValue( "Default Hair Shader", m_defaultHairShaderID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Shader" )) != nullptr )
        {
            m_defaultHairShaderID = ((SFile*) ( pData ))->uuid;
        }
        GUI::DrawIntValue( "Default Debug Shader", m_defaultDebugShaderID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Shader" )) != nullptr )
        {
            m_defaultDebugShaderID = ((SFile*) ( pData ))->uuid;
        }
        GUI::DrawIntValue( "Default Skybox Shader", m_defaultSkyboxShaderID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Shader" )) != nullptr )
        {
            m_defaultSkyboxShaderID = ((SFile*) ( pData ))->uuid;
        }
        
        GUI::DrawIntValue( "Default Material", m_defaultMaterialID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Material" )) != nullptr )
        {
            m_defaultMaterialID = ((SFile*) ( pData ))->uuid;
        }
        GUI::DrawIntValue( "Default Transparent Material", m_defaultTransparentMaterialID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Material" )) != nullptr )
        {
            m_defaultTransparentMaterialID = ((SFile*) ( pData ))->uuid;
        }
        GUI::DrawIntValue( "Default Cloth Material", m_defaultClothMaterialID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Material" )) != nullptr )
        {
            m_defaultClothMaterialID = ((SFile*) ( pData ))->uuid;
        }
        GUI::DrawIntValue( "Default Skin Material", m_defaultSkinMaterialID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Material" )) != nullptr )
        {
            m_defaultSkinMaterialID = ((SFile*) ( pData ))->uuid;
        }
        GUI::DrawIntValue( "Default Hair Material", m_defaultHairMaterialID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Material" )) != nullptr )
        {
            m_defaultHairMaterialID = ((SFile*) ( pData ))->uuid;
        }
        GUI::DrawIntValue( "Default Debug Material", m_defaultDebugMaterialID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Material" )) != nullptr )
        {
            m_defaultDebugMaterialID = ((SFile*) ( pData ))->uuid;
        }
        GUI::DrawIntValue( "Default Skybox Material", m_defaultSkyboxMaterialID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Material" )) != nullptr )
        {
            m_defaultSkyboxMaterialID = ((SFile*) ( pData ))->uuid;
        }
        
        GUI::DrawIntValue( "Default Texture", m_defaultTextureID, 256 );
        if (( pData = GUI::ReceiveDragDrop( "Image" )) != nullptr )
        {
            m_defaultTextureID = ((SFile*) ( pData ))->uuid;
        }
    }
    void CPropertyPanel::DrawInputManagerProperties()
    {
        GUI::DrawText( "Waw such empty" );
    }
    void CPropertyPanel::DrawPhysicsProperties()
    {
        GUI::DrawText( "Waw such empty" );
    }
    void CPropertyPanel::DrawPlayerProperties()
    {
        GUI::DrawText( "Waw such empty" );
    }
    void CPropertyPanel::DrawTimeProperties()
    {
        GUI::DrawText( "Waw such empty" );
    }
    void CPropertyPanel::SaveToIni() const
    {
        mINI::INIFile      file( "Properties.ini" );
        mINI::INIStructure ini;
        
        ini["Graphics"]["Default_Shader"]             = std::to_string( m_defaultShaderID );
        ini["Graphics"]["Default_Transparent_Shader"] = std::to_string( m_defaultTransparentShaderID );
        ini["Graphics"]["Default_Skin_Shader"]        = std::to_string( m_defaultSkinShaderID );
        ini["Graphics"]["Default_Hair_Shader"]        = std::to_string( m_defaultHairShaderID );
        ini["Graphics"]["Default_Cloth_Shader"]       = std::to_string( m_defaultClothShaderID );
        ini["Graphics"]["Default_Debug_Shader"]       = std::to_string( m_defaultDebugShaderID );
        ini["Graphics"]["Default_Skybox_Shader"]      = std::to_string( m_defaultSkyboxShaderID );
        
        ini["Graphics"]["Default_Material"]             = std::to_string( m_defaultMaterialID );
        ini["Graphics"]["Default_Transparent_Material"] = std::to_string( m_defaultTransparentMaterialID );
        ini["Graphics"]["Default_Skin_Material"]        = std::to_string( m_defaultSkinMaterialID );
        ini["Graphics"]["Default_Hair_Material"]        = std::to_string( m_defaultHairMaterialID );
        ini["Graphics"]["Default_Cloth_Material"]       = std::to_string( m_defaultClothMaterialID );
        ini["Graphics"]["Default_Debug_Material"]       = std::to_string( m_defaultDebugMaterialID );
        ini["Graphics"]["Default_Skybox_Material"]      = std::to_string( m_defaultSkyboxMaterialID );
        
        ini["Graphics"]["Default_Texture"] = std::to_string( m_defaultTextureID );
        (void) file.generate( ini, true );
    }
    void CPropertyPanel::LoadFromIni()
    {
        mINI::INIFile      file( "Properties.ini" );
        mINI::INIStructure ini;
        if ( file.read( ini ))
        {
            std::string defaultShaderID              = ini["Graphics"]["Default_Shader"];
            std::string defaultTransparentShaderID   = ini["Graphics"]["Default_Transparent_Shader"];
            std::string defaultSkinShaderID          = ini["Graphics"]["Default_Skin_Shader"];
            std::string defaultHairShaderID          = ini["Graphics"]["Default_Hair_Shader"];
            std::string defaultClothShaderID         = ini["Graphics"]["Default_Cloth_Shader"];
            std::string defaultDebugShaderID         = ini["Graphics"]["Default_Debug_Shader"];
            std::string defaultSkyboxShaderID        = ini["Graphics"]["Default_Skybox_Shader"];
            std::string defaultMaterialID            = ini["Graphics"]["Default_Material"];
            std::string defaultTransparentMaterialID = ini["Graphics"]["Default_Transparent_Material"];
            std::string defaultSkinMaterialID        = ini["Graphics"]["Default_Skin_Material"];
            std::string defaultHairMaterialID        = ini["Graphics"]["Default_Hair_Material"];
            std::string defaultClothMaterialID       = ini["Graphics"]["Default_Cloth_Material"];
            std::string defaultDebugMaterialID       = ini["Graphics"]["Default_Debug_Material"];
            std::string defaultSkyboxMaterialID      = ini["Graphics"]["Default_Skybox_Material"];
            std::string defaultTextureID             = ini["Graphics"]["Default_Texture"];
            
            m_defaultShaderID            = std::stoull( defaultShaderID.empty() ? "0" : defaultShaderID );
            m_defaultTransparentShaderID = std::stoull(
                    defaultTransparentShaderID.empty()
                    ? "0"
                    : defaultTransparentShaderID
            );
            m_defaultSkinShaderID        = std::stoull( defaultSkinShaderID.empty() ? "0" : defaultSkinShaderID );
            m_defaultHairShaderID        = std::stoull( defaultHairShaderID.empty() ? "0" : defaultHairShaderID );
            m_defaultClothShaderID       = std::stoull( defaultClothShaderID.empty() ? "0" : defaultClothShaderID );
            m_defaultDebugShaderID       = std::stoull( defaultDebugShaderID.empty() ? "0" : defaultDebugShaderID );
            m_defaultSkyboxShaderID      = std::stoull( defaultSkyboxShaderID.empty() ? "0" : defaultSkyboxShaderID );
            
            m_defaultMaterialID            = std::stoull( defaultMaterialID.empty() ? "0" : defaultMaterialID );
            m_defaultTransparentMaterialID = std::stoull(
                    defaultTransparentMaterialID.empty()
                    ? "0"
                    : defaultTransparentMaterialID
            );
            m_defaultSkinMaterialID        = std::stoull( defaultSkinMaterialID.empty() ? "0" : defaultSkinMaterialID );
            m_defaultHairMaterialID        = std::stoull( defaultHairMaterialID.empty() ? "0" : defaultHairMaterialID );
            m_defaultClothMaterialID       = std::stoull(
                    defaultClothMaterialID.empty()
                    ? "0"
                    : defaultClothMaterialID
            );
            m_defaultDebugMaterialID       = std::stoull(
                    defaultDebugMaterialID.empty()
                    ? "0"
                    : defaultDebugMaterialID
            );
            m_defaultSkyboxMaterialID      = std::stoull(
                    defaultSkyboxMaterialID.empty()
                    ? "0"
                    : defaultSkyboxMaterialID
            );
            
            m_defaultTextureID = std::stoull( defaultTextureID.empty() ? "0" : defaultTextureID );
        }
        else
        {
            SaveToIni();
        }
    }
    const char* CPropertyPanel::ToString( CPropertyPanel::EPropertyTypes::Enum types )
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
    uint64_t CPropertyPanel::GetDefaultShader() const
    {
        return m_defaultShaderID;
    }
    uint64_t CPropertyPanel::GetDefaultTransparentShader() const
    {
        return m_defaultTransparentShaderID;
    }
    uint64_t CPropertyPanel::GetDefaultClothShader() const
    {
        return m_defaultClothShaderID;
    }
    uint64_t CPropertyPanel::GetDefaultSkinShader() const
    {
        return m_defaultSkinShaderID;
    }
    uint64_t CPropertyPanel::GetDefaultHairShader() const
    {
        return m_defaultHairShaderID;
    }
    uint64_t CPropertyPanel::GetDefaultDebugShader() const
    {
        return m_defaultDebugShaderID;
    }
    uint64_t CPropertyPanel::GetDefaultSkyboxShader() const
    {
        return m_defaultSkyboxShaderID;
    }
    uint64_t CPropertyPanel::GetDefaultMaterial() const
    {
        return m_defaultMaterialID;
    }
    uint64_t CPropertyPanel::GetDefaultTransparentMaterial() const
    {
        return m_defaultTransparentMaterialID;
    }
    uint64_t CPropertyPanel::GetDefaultClothMaterial() const
    {
        return m_defaultClothMaterialID;
    }
    uint64_t CPropertyPanel::GetDefaultSkinMaterial() const
    {
        return m_defaultSkinMaterialID;
    }
    uint64_t CPropertyPanel::GetDefaultHairMaterial() const
    {
        return m_defaultHairMaterialID;
    }
    uint64_t CPropertyPanel::GetDefaultDebugMaterial() const
    {
        return m_defaultDebugMaterialID;
    }
    uint64_t CPropertyPanel::GetDefaultSkyboxMaterial() const
    {
        return m_defaultSkyboxMaterialID;
    }
    uint64_t CPropertyPanel::GetDefaultTexture() const
    {
        return m_defaultTextureID;
    }
} // FawnForge