//
// Created by joran on 23/10/2022.
//

#include "PropertyPanel.h"
#include "../Renderer/EditorGui.h"
#include <Serializer.h>
#include <mini/ini.h>

namespace FawnForge
{
    CPropertyPanel::CPropertyPanel()
    {
        LoadFromIni();
    }
    void CPropertyPanel::Draw() noexcept
    {
        if ( Gui::Begin( "Property Panel", m_isVisible, 0 ) )
        {
            if ( Gui::BeginChild( "Category", { 128, 0 }, true, 1 << 11 ) )
            {
                for ( int i {}; i < (int)property_type::max; ++i )
                {
                    if ( Gui::Button( ToString( property_type( i ) ), { -1, 32 } ) )
                    {
                        m_currentPropertyType = property_type( i );
                    }
                }
            }
            Gui::EndChild();
            Gui::SameLine();
            if ( Gui::BeginChild( "Properties", { 0, 0 }, true, 1 << 11 ) )
            {
                switch ( m_currentPropertyType )
                {
                    case property_type::audio: DrawAudioProperties(); break;
                    case property_type::graphics: DrawGraphicsProperties(); break;
                    case property_type::input_manager: DrawInputManagerProperties(); break;
                    case property_type::physics: DrawPhysicsProperties(); break;
                    case property_type::player: DrawPlayerProperties(); break;
                    case property_type::time: DrawTimeProperties(); break;
                    default: break;
                }
            }
            Gui::EndChild();
            if ( Gui::Button( "Save" ) )
            {
                SaveToIni();
            }
            Gui::Text( "Some changes only apply after restarting the editor" );
            Gui::End();
        }
    }
    void CPropertyPanel::ShowWindow() noexcept
    {
        m_isVisible = true;
        Gui::SetWindowFocus( "Property Panel" );
        LoadFromIni();
    }
    void CPropertyPanel::DrawAudioProperties() const noexcept
    {
        Gui::Text( "Waw such empty" );
    }
    void CPropertyPanel::DrawGraphicsProperties() noexcept
    {
        for ( int i {}; i < shader_types_max; ++i )
        {
            void* pData {};
            Gui::Text( shaderNames[ i ], 250.0f );
            Gui::SameLine( 50.0f );
            Gui::Text( m_shaderIds[ i ].ToString() );
            if ( ( pData = Gui::ReceiveDragDrop( "Shader" ) ) != nullptr )
            {
                m_shaderIds[ i ] = ( (SFile*)( pData ) )->uuid;
            }
        }

        for ( int i {}; i < material_types_max; ++i )
        {
            void* pData {};
            Gui::Text( materialNames[ i ], 250.0f );
            Gui::SameLine( 50.0f );
            Gui::Text( m_shaderIds[ i ].ToString() );
            if ( ( pData = Gui::ReceiveDragDrop( "Material" ) ) != nullptr )
            {
                m_materialIds[ i ] = ( (SFile*)( pData ) )->uuid;
            }
        }
    }
    void CPropertyPanel::DrawInputManagerProperties() const noexcept
    {
        Gui::Text( "Waw such empty" );
    }
    void CPropertyPanel::DrawPhysicsProperties() const noexcept
    {
        Gui::Text( "Waw such empty" );
    }
    void CPropertyPanel::DrawPlayerProperties() const noexcept
    {
        Gui::Text( "Waw such empty" );
    }
    void CPropertyPanel::DrawTimeProperties() const noexcept
    {
        Gui::Text( "Waw such empty" );
    }
    void CPropertyPanel::SaveToIni() const
    {
        mINI::INIFile file( "Properties.ini" );
        mINI::INIStructure ini;

        for ( int i {}; i < shader_types_max; ++i )
            ini[ "Graphics" ][ shaderNames[ i ] ] = m_shaderIds[ i ].Data();

        for ( int i {}; i < shader_types_max; ++i )
            ini[ "Graphics" ][ materialNames[ i ] ] = m_shaderIds[ i ].Data();

        ini[ "Graphics" ][ "Default_Texture" ] = m_defaultTextureID.Data();
        (void)file.generate( ini, true );
    }
    void CPropertyPanel::LoadFromIni() noexcept
    {
        mINI::INIFile file( "Properties.ini" );
        mINI::INIStructure ini;
        if ( file.read( ini ) )
        {

            for ( int i {}; i < material_types_max; ++i )
            {
                m_shaderIds[ i ] = ini[ "Graphics" ][ shaderNames[ i ] ];
            }

            for ( int i {}; i < material_types_max; ++i )
            {
                m_shaderIds[ i ] = ini[ "Graphics" ][ materialNames[ i ] ];
            }

            m_defaultTextureID = ini[ "Graphics" ][ "Default_Texture" ];
        }
        else
        {
            SaveToIni();
        }
    }
    const char* CPropertyPanel::ToString( CPropertyPanel::property_type types )
    {
        switch ( types )
        {
            case property_type::audio: return "Audio";
            case property_type::graphics: return "Graphics";
            case property_type::input_manager: return "Input Manager";
            case property_type::physics: return "Physics";
            case property_type::player: return "Player";
            case property_type::time: return "Time";
            default: break;
        }
        return nullptr;
    }
    CUuid CPropertyPanel::GetDefaultShader() const
    {
        return m_shaderIds[ shader_types_default ];
    }
    CUuid CPropertyPanel::GetDefaultTransparentShader() const
    {
        return m_shaderIds[ shader_types_transparent ];
    }
    CUuid CPropertyPanel::GetDefaultClothShader() const
    {
        return m_shaderIds[ shader_types_cloth ];
    }
    CUuid CPropertyPanel::GetDefaultSkinShader() const
    {
        return m_shaderIds[ shader_types_skin ];
    }
    CUuid CPropertyPanel::GetDefaultHairShader() const
    {
        return m_shaderIds[ shader_types_hair ];
    }
    CUuid CPropertyPanel::GetDefaultDebugShader() const
    {
        return m_shaderIds[ shader_types_debug ];
    }
    CUuid CPropertyPanel::GetDefaultSkyboxShader() const
    {
        return m_shaderIds[ shader_types_skybox ];
    }
    CUuid CPropertyPanel::GetDefaultMaterial() const
    {
        return m_materialIds[material_types_default];
    }
    CUuid CPropertyPanel::GetDefaultTransparentMaterial() const
    {
        return m_materialIds[material_types_transparent];
    }
    CUuid CPropertyPanel::GetDefaultClothMaterial() const
    {
        return m_materialIds[material_types_cloth];
    }
    CUuid CPropertyPanel::GetDefaultSkinMaterial() const
    {
        return m_materialIds[material_types_skin];
    }
    CUuid CPropertyPanel::GetDefaultHairMaterial() const
    {
        return m_materialIds[material_types_hair];
    }
    CUuid CPropertyPanel::GetDefaultDebugMaterial() const
    {
        return m_materialIds[material_types_debug];
    }
    CUuid CPropertyPanel::GetDefaultSkyboxMaterial() const
    {
        return m_materialIds[ material_types_skybox ];
    }
    CUuid CPropertyPanel::GetDefaultTexture() const
    {
        return m_materialIds[material_types_default];
    }
}// namespace FawnForge