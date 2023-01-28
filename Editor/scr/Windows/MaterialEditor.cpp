#include "MaterialEditor.h"

#include <unordered_map>

#include "ISystem.h"
#include "IResourceManager.h"
#include "Material.h"
#include "Texture.h"

#include "../EditorGUI/EditorGui.h"
#include "../Tools/FilesSystem/Exporter.h"
#include "PropertyPanel.h"

BalEditor::CMaterialEditor::CMaterialEditor()
        : m_isVisible{}
          , m_currentMaterial{}
          , m_shaderID{}
          , m_shaderPresetNr{}
          , m_pSystem{ nullptr }
          , m_pPropertyPanel{ nullptr }
          , m_defaultShaderNames{ "Default", "Transparent", "Cloth", "Skin", "Hair", "Debug", "Skybox", "Custom" }
{
}

void BalEditor::CMaterialEditor::Initialize( const ISystem* pSystem, const CPropertyPanel* pPropertyPanel )
{
    m_pSystem        = pSystem;
    m_pPropertyPanel = pPropertyPanel;
}

void BalEditor::CMaterialEditor::Draw()
{
    if ( m_isVisible )
    {
        if ( GUI::Begin( "Material Editor", m_isVisible, 1 << 10 ))
        {
            if ( GUI::BeginMenuBar())
            {
                if ( GUI::DrawButton( "Save/Reload" ))
                {
                    SaveMaterial();
                    ReloadMaterial();
                }
                if ( GUI::DrawComboBox( "Preset Shader", m_shaderPresetNr, m_defaultShaderNames, {}, 100, true ))
                {
                    switch ( m_shaderPresetNr )
                    {
                        case 0:
                            m_shaderID = m_pPropertyPanel->GetDefaultShader();
                            break;
                        case 1:
                            m_shaderID = m_pPropertyPanel->GetDefaultTransparentShader();
                            break;
                        case 2:
                            m_shaderID = m_pPropertyPanel->GetDefaultClothShader();
                            break;
                        case 3:
                            m_shaderID = m_pPropertyPanel->GetDefaultSkinShader();
                            break;
                        case 4:
                            m_shaderID = m_pPropertyPanel->GetDefaultHairShader();
                            break;
                        case 5:
                            m_shaderID = m_pPropertyPanel->GetDefaultDebugShader();
                            break;
                        case 6:
                            m_shaderID = m_pPropertyPanel->GetDefaultSkyboxShader();
                            break;
                        case 7:
                        default:
                            break;
                    }
                    LoadShader();
                }
                GUI::DrawText( "Current Shader: " );
                GUI::DrawText( std::to_string( m_shaderID ).c_str());
                if ( void* pData = GUI::ReceiveDragDrop( ToString( EFileTypes::Shader )))
                {
                    m_shaderID = ((SFile*) ( pData ))->uuid;
                    LoadShader();
                }
                GUI::EndMenuBar();
            }
            GUI::Columns( 2 );
            for ( auto& snd : m_shaderResources )
            {
                if ( snd.type != BalVulkan::EShaderResourceType::Input || snd.type != BalVulkan::EShaderResourceType::Output )
                {
                    switch ( snd.type )
                    {
                        case BalVulkan::EShaderResourceType::Image:
                        case BalVulkan::EShaderResourceType::ImageSampler:
                        case BalVulkan::EShaderResourceType::ImageStorage:
                        case BalVulkan::EShaderResourceType::Sampler:
                            GUI::DrawText( snd.name.c_str());
                            GUI::NextColumn();
                            GUI::DrawText( "Image / Image Sampler" );
                            if ( void* pData = GUI::ReceiveDragDrop( ToString( EFileTypes::Image )))
                            {
                                const Balbino::CTexture* pModel = m_pSystem->GetResourceManager()->GetTexture(
                                        static_cast<SFile*>( pData )->uuid,
                                        true
                                );
                                snd.resourceID = pModel->GetID();
                            }
                            break;
                        default:
                            break;
                    }
                    GUI::NextColumn();
                }
            }
            GUI::Columns( 1 );
        }
        GUI::End();
    }
}

void BalEditor::CMaterialEditor::Cleanup()
{
}

void BalEditor::CMaterialEditor::ShowWindow( const SFile& currentMaterial )
{
    GUI::SetWindowFocus( "Material Editor" );
    m_isVisible       = true;
    m_currentMaterial = currentMaterial;
    LoadEditorFromData();
}

void BalEditor::CMaterialEditor::LoadShader()
{
    m_shaderResources.clear();
    m_shaderResources = m_pSystem->GetResourceManager()->GetShader( m_shaderID, true )->GetShaderResources();
}
void BalEditor::CMaterialEditor::LoadEditorFromData()
{
    std::ifstream file( m_currentMaterial.path, std::ios::in | std::ios::binary );
    if ( !file.is_open())
    {
        file.close();
        m_isVisible = false;
        return;
    }
    
    uint64_t id;
    uint64_t size;
    uint8_t  type;
    BinaryReadWrite::Read( file, id );
    BinaryReadWrite::Read( file, type );
    m_currentMaterial.uuid = id;
    if ( type != (uint8_t) EFileTypes::Material )
    {
        m_isVisible = false;
        return;
    }
    m_currentMaterial.type = (EFileTypes) type;
    
    m_shaderResources.clear();
    BinaryReadWrite::Read( file, m_shaderID );
    BinaryReadWrite::Read( file, size );
    void* pData = malloc( size * sizeof( BalVulkan::SShaderResource ));
    BinaryReadWrite::Read( file, pData, size * sizeof( BalVulkan::SShaderResource ));
    m_shaderResources.assign((BalVulkan::SShaderResource*) pData, (BalVulkan::SShaderResource*) pData + size );
    free( pData );
}
void BalEditor::CMaterialEditor::ReloadMaterial() const
{
    m_pSystem->GetResourceManager()->UnloadMaterial( m_currentMaterial.uuid );
    m_pSystem->GetResourceManager()->LoadMaterial( m_currentMaterial.path );
    
}
void BalEditor::CMaterialEditor::SaveMaterial() const
{
    Exporter::ExportMaterial(
            m_currentMaterial.fileName,
            m_currentMaterial.path,
            m_shaderID,
            m_shaderResources,
            m_currentMaterial.uuid
    );
}
