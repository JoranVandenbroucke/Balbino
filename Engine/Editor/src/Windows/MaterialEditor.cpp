#include "MaterialEditor.h"

#include <unordered_map>

#include "ISystem.h"
#include "IResourceManager.h"
#include "Material.h"
#include "Texture.h"

#include "../Renderer/EditorGui.h"
#include "../Tools/FilesSystem/Exporter.h"
#include "PropertyPanel.h"

FawnForge::CMaterialEditor::CMaterialEditor()
        : m_isVisible{}
          , m_currentMaterial{}
          , m_shaderId{}
          , m_shaderPresetNr{}
          , m_pSystem{ nullptr }
          , m_pPropertyPanel{ nullptr }
          , m_defaultShaderNames{ "Default", "Transparent", "Cloth", "Skin", "Hair", "Debug", "Skybox", "Custom" }
{
}

void FawnForge::CMaterialEditor::Initialize( const ISystem* pSystem, const CPropertyPanel* pPropertyPanel )
{
    m_pSystem        = pSystem;
    m_pPropertyPanel = pPropertyPanel;
}

void FawnForge::CMaterialEditor::Draw()
{
    if ( m_isVisible )
    {
        if ( Gui::Begin( "Material Editor", m_isVisible, 1 << 10 ))
        {
            if ( Gui::BeginMenuBar())
            {
                if ( Gui::DrawButton( "Save/Reload" ))
                {
                    SaveMaterial();
                    ReloadMaterial();
                }
                if ( Gui::DrawComboBox( "Preset Shader", m_shaderPresetNr, m_defaultShaderNames, {}, 100, true ))
                {
                    switch ( m_shaderPresetNr )
                    {
                        case 0:
                            m_shaderId = m_pPropertyPanel->GetDefaultShader();
                            break;
                        case 1:
                            m_shaderId = m_pPropertyPanel->GetDefaultTransparentShader();
                            break;
                        case 2:
                            m_shaderId = m_pPropertyPanel->GetDefaultClothShader();
                            break;
                        case 3:
                            m_shaderId = m_pPropertyPanel->GetDefaultSkinShader();
                            break;
                        case 4:
                            m_shaderId = m_pPropertyPanel->GetDefaultHairShader();
                            break;
                        case 5:
                            m_shaderId = m_pPropertyPanel->GetDefaultDebugShader();
                            break;
                        case 6:
                            m_shaderId = m_pPropertyPanel->GetDefaultSkyboxShader();
                            break;
                        case 7:
                        default:
                            break;
                    }
                    LoadShader();
                }
                Gui::DrawText( "Current Shader: " );
                Gui::DrawText( std::to_string( m_shaderId ).c_str());
                if ( void* pData = Gui::ReceiveDragDrop( ToString( file_type::file_type_shader )))
                {
                    m_shaderId = ((SFile*) ( pData ))->uuid;
                    LoadShader();
                }
                Gui::EndMenuBar();
            }
            Gui::Columns( 2 );
            for ( auto& snd : m_shaderResources )
            {
                if ( snd.type != FawnVision::EShaderResourceType::Input || snd.type != FawnVision::EShaderResourceType::Output )
                {
                    switch ( snd.type )
                    {
                        case FawnVision::EShaderResourceType::Image:
                        case FawnVision::EShaderResourceType::ImageSampler:
                        case FawnVision::EShaderResourceType::ImageStorage:
                        case FawnVision::EShaderResourceType::Sampler:
                            Gui::DrawText( snd.name.c_str());
                            Gui::NextColumn();
                            Gui::DrawText( "Image / Image Sampler" );
                            if ( void* pData = Gui::ReceiveDragDrop( ToString( file_type::file_type_image )))
                            {
                                const CTexture* pModel = m_pSystem->GetResourceManager()->GetTexture(
                                        static_cast<SFile*>( pData )->uuid,
                                        true
                                );
                                snd.resourceID = pModel->GetID();
                            }
                            break;
                        default:
                            break;
                    }
                    Gui::NextColumn();
                }
            }
            Gui::Columns( 1 );
        }
        Gui::End();
    }
}

void FawnForge::CMaterialEditor::Cleanup()
{
}

void FawnForge::CMaterialEditor::ShowWindow( const SFile& currentMaterial )
{
    Gui::SetWindowFocus( "Material Editor" );
    m_isVisible       = true;
    m_currentMaterial = currentMaterial;
    LoadEditorFromData();
}

void FawnForge::CMaterialEditor::LoadShader()
{
    m_shaderResources.clear();
    m_shaderResources = m_pSystem->GetResourceManager()->GetShader( m_shaderId, true )->GetShaderResources();
}
void FawnForge::CMaterialEditor::LoadEditorFromData()
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
    if ( type != (uint8_t) file_type::file_type_material )
    {
        m_isVisible = false;
        return;
    }
    m_currentMaterial.type = (file_type) type;
    
    m_shaderResources.clear();
    BinaryReadWrite::Read( file, m_shaderId );
    BinaryReadWrite::Read( file, size );
    void* pData = malloc( size * sizeof( FawnVision::SShaderResource ));
    BinaryReadWrite::Read( file, pData, size * sizeof( FawnVision::SShaderResource ));
    m_shaderResources.assign((FawnVision::SShaderResource*) pData, (FawnVision::SShaderResource*) pData + size );
    free( pData );
}
void FawnForge::CMaterialEditor::ReloadMaterial() const
{
    m_pSystem->GetResourceManager()->UnloadMaterial( m_currentMaterial.uuid );
    m_pSystem->GetResourceManager()->LoadMaterial( m_currentMaterial.path );
    
}
void FawnForge::CMaterialEditor::SaveMaterial() const
{
    Exporter::ExportMaterial(
            m_currentMaterial.fileName,
            m_currentMaterial.path,
            m_shaderId,
            m_shaderResources,
            m_currentMaterial.uuid
    );
}
