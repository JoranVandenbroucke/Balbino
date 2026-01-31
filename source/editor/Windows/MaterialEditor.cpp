#include "MaterialEditor.h"

#include <ErrorHandling.h>
#include <IResourceManager.h>
#include <ISystem.h>

#include "../Renderer/EditorGui.h"
#include "../Tools/FilesSystem/Exporter.h"
#include "PropertyPanel.h"

#include <string>
using namespace std::string_literals;

void FawnForge::CMaterialEditor::Initialize( ISystem* pSystem, const CPropertyPanel* pPropertyPanel )
{
    m_pSystem        = pSystem;
    m_pPropertyPanel = pPropertyPanel;
}

void FawnForge::CMaterialEditor::Draw() noexcept
{
    if ( m_isVisible )
    {
        if ( Gui::Begin( "Material Editor", m_isVisible, 1 << 10 ) )
        {
            if ( Gui::BeginMenuBar() )
            {
                if ( Gui::Button( "Save/Reload" ) )
                {
                    SaveMaterial();
                    ReloadMaterial();
                }
                if ( Gui::ComboBox( "Preset Shader", m_shaderPresetNr, m_defaultShaderNames, {}, 100, true ) )
                {
                    switch ( m_shaderPresetNr )
                    {
                        case 0: m_shaderId = m_pPropertyPanel->GetDefaultShader(); break;
                        case 1: m_shaderId = m_pPropertyPanel->GetDefaultTransparentShader(); break;
                        case 2: m_shaderId = m_pPropertyPanel->GetDefaultClothShader(); break;
                        case 3: m_shaderId = m_pPropertyPanel->GetDefaultSkinShader(); break;
                        case 4: m_shaderId = m_pPropertyPanel->GetDefaultHairShader(); break;
                        case 5: m_shaderId = m_pPropertyPanel->GetDefaultDebugShader(); break;
                        case 6: m_shaderId = m_pPropertyPanel->GetDefaultSkyboxShader(); break;
                        default: break;
                    }
                    LoadShader();
                }
                Gui::Text( "Current Shader: " );
                Gui::Text( m_shaderId.ToString() );
                if ( void* pData = Gui::ReceiveDragDrop( ToString( file_type::shader ) ) )
                {
                    m_shaderId = ( (SFile*)( pData ) )->uuid;
                    LoadShader();
                }
                Gui::EndMenuBar();
            }
            Gui::Columns( 2 );
            for ( auto& snd : m_shaderResources )
            {
                FawnVision::shader_resource_type type { (FawnVision::shader_resource_type)snd.type };
                if ( type != FawnVision::shader_resource_type::input && type != FawnVision::shader_resource_type::output )
                {
                    switch ( type )
                    {
                        case FawnVision::shader_resource_type::image:
                        case FawnVision::shader_resource_type::image_sampler:
                        case FawnVision::shader_resource_type::image_storage:
                        case FawnVision::shader_resource_type::sampler:
                            Gui::Text( snd.name );
                            Gui::NextColumn();
                            Gui::Text( "image / image sampler" );
                            if ( void* pData = Gui::ReceiveDragDrop( ToString( file_type::image ) ) )
                            {
                                try
                                {
                                    const FawnVision::CTexture* pModel = m_pSystem->GetResourceManager()->GetTexture( static_cast<SFile*>( pData )->uuid );
                                    snd.resourceId                     = pModel->GetId();
                                }
                                catch ( const FawnTrace::Exception& e )
                                {
                                    m_pSystem->Log.Error( e.what() );
                                }
                            }
                            break;
                        default: break;
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
    // todo
}

void FawnForge::CMaterialEditor::ShowWindow( const SFile& currentMaterial )
{
    Gui::SetWindowFocus( "Material Editor" );
    m_isVisible       = true;
    m_currentMaterial = currentMaterial;
    LoadEditorFromData();
}

void FawnForge::CMaterialEditor::LoadShader() noexcept
{
    try
    {
        auto tempResources = m_pSystem->GetResourceManager()->GetShader( m_shaderId )->GetShaderResources();
        m_shaderResources.clear();
        m_shaderResources.insert(m_shaderResources.end(), std::make_move_iterator(tempResources.begin()), std::make_move_iterator(tempResources.end()));
    }
    catch ( const FawnTrace::Exception& e )
    {
        m_pSystem->Log.Error( ( "Could not load material resources: "s + e.what() ).c_str() );
    }
}
void FawnForge::CMaterialEditor::LoadEditorFromData() noexcept
{
    std::ifstream file( m_currentMaterial.path, std::ios::in | std::ios::binary );
    if ( !file.is_open() )
    {
        file.close();
        m_isVisible = false;
        return;
    }

    CUuid id;
    uint64_t size;
    uint8_t type;
    Serialization::Read( file, id );
    Serialization::Read( file, type );
    m_currentMaterial.uuid = id;
    if ( type != (uint8_t)file_type::material )
    {
        m_isVisible = false;
        return;
    }
    m_currentMaterial.type = (file_type)type;

    m_shaderResources.clear();
    Serialization::Read( file, m_shaderId );
    Serialization::Read( file, size );
    void* pData = malloc( size * sizeof( FawnVision::SShaderResource ) );
    Serialization::Read( file, pData, size * sizeof( FawnVision::SShaderResource ) );
    m_shaderResources.assign( (FawnVision::SShaderResource*)pData, (FawnVision::SShaderResource*)pData + size );
    free( pData );
}
void FawnForge::CMaterialEditor::ReloadMaterial() const
{
#pragma todo("reload material, so that the data can be updated")    // todo: reload material, so that the data can be updated
}
void FawnForge::CMaterialEditor::SaveMaterial() const
{
#pragma todo("save and reload material, so that the data can be updated")    // todo: save and reload material, so that the data can be updated
}
