#include "MaterialEditor.h"

#include "ISystem.h"
#include "IResourceManager.h"
#include "Material.h"
#include "Texture.h"

#include "../EditorGUI/EditorGui.h"

BalEditor::CMaterialEditor::CMaterialEditor()
        : m_isVisible{},
          m_currentMaterial{},
          m_pSystem{ nullptr }
{
}

void BalEditor::CMaterialEditor::Initialize( const ISystem* pSystem )
{
    m_pSystem = pSystem;
}

void BalEditor::CMaterialEditor::Draw()
{
    if ( m_isVisible )
    {
        if ( BalEditor::EditorGUI::Begin( "Material Editor", m_isVisible, 1 << 10 ))
        {
            if ( BalEditor::EditorGUI::BeginMenuBar())
            {
                if ( BalEditor::EditorGUI::DrawButton( "Reload" ))
                {
                    uint64_t shaderID{
                            (uint64_t) m_pSystem->GetResourceManager()->GetMaterial(
                                    m_currentMaterial.uuid )->GetShaderID()
                    };
                    m_pSystem->GetResourceManager()->UnloadMaterial( m_currentMaterial.uuid );
                    //Todo change this
                    std::ofstream materialFile( m_currentMaterial.path, std::ios::out | std::ios::binary );
                    if ( !materialFile.is_open())
                    {
                        return;
                    }
                    BinaryReadWrite::Write( materialFile, m_currentMaterial.uuid );
                    BinaryReadWrite::Write( materialFile, (uint8_t) EFileTypes::Material );
                    BinaryReadWrite::Write( materialFile, shaderID );
                    BinaryReadWrite::Write( materialFile, m_shaderResources );
                    materialFile.close();
                    m_pSystem->GetResourceManager()->LoadMaterial( m_currentMaterial.path );
                }
                BalEditor::EditorGUI::EndMenuBar();
            }
            BalEditor::EditorGUI::Columns( 2 );
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
                            BalEditor::EditorGUI::DrawText( snd.name.c_str());
                            BalEditor::EditorGUI::NextColumn();
                            BalEditor::EditorGUI::DrawText( "Image / Image Sampler" );
                            if ( void* pData = BalEditor::EditorGUI::ReceiveDragDrop( ToString( EFileTypes::Image )))
                            {
                                const Balbino::CTexture* pModel = m_pSystem->GetResourceManager()->GetTexture(
                                        static_cast<SFile*>( pData )->uuid, true );
                                snd.resourceID = pModel->GetID();
                            }
                            break;
                        default:
                            break;
                    }
                    BalEditor::EditorGUI::NextColumn();
                }
            }
            BalEditor::EditorGUI::Columns( 1 );
        }
        BalEditor::EditorGUI::End();
    }
}

void BalEditor::CMaterialEditor::Cleanup()
{
}

void BalEditor::CMaterialEditor::ShowWindow()
{
    BalEditor::EditorGUI::SetWindowFocus( "Material Editor" );
    m_isVisible = true;
}

void BalEditor::CMaterialEditor::SetMaterial( const SFile& currentMaterial )
{
    Balbino::CMaterial* pMaterial = m_pSystem->GetResourceManager()->GetMaterial( currentMaterial.uuid, true );
    if ( pMaterial )
    {
        m_shaderResources = pMaterial->GetShaderResourcesVector();
        m_currentMaterial = currentMaterial;
    }
}
