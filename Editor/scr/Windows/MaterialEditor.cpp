#include "MaterialEditor.h"

#include "IManager.h"
#include "imgui.h"
#include "IResourceManager.h"
#include "Material.h"
#include "Texture.h"

BalEditor::CMaterialEditor::CMaterialEditor()
        : m_isVisible{}
          , m_currentMaterial{}
          , m_pSystem{ nullptr }
{
}

void BalEditor::CMaterialEditor::Initialize(const ISystem* pSystem)
{
    m_pSystem = pSystem;
}

void BalEditor::CMaterialEditor::Draw()
{
    if( m_isVisible )
    {
        if( ImGui::Begin( "Material Editor", &m_isVisible, ImGuiWindowFlags_MenuBar ))
        {
            if( ImGui::BeginMenuBar())
            {
                if( ImGui::Button( "Reload" ))
                {
                    uint64_t shaderID {(uint64_t)m_pSystem->GetResourceManager()->GetMaterial(m_currentMaterial.uuid)->GetShaderID()};
                    m_pSystem->GetResourceManager()->UnloadMaterial( m_currentMaterial.uuid );
                    std::ofstream materialFile( m_currentMaterial.path, std::ios::out | std::ios::binary );
                    if( !materialFile.is_open() )
                    {
                        return;
                    }
                    BinaryReadWrite::Write( materialFile,  m_currentMaterial.uuid);
                    BinaryReadWrite::Write( materialFile, (uint8_t) EFileTypes::Material );
                    BinaryReadWrite::Write( materialFile, shaderID );
                    BinaryReadWrite::Write( materialFile, m_shaderResources );
                    materialFile.close();
                    m_pSystem->GetResourceManager()->LoadMaterial( m_currentMaterial.path.string());
                }
                ImGui::EndMenuBar();
            }
            ImGui::Columns( 2 );
            for( auto& snd : m_shaderResources )
            {
                if(snd.type != BalVulkan::EShaderResourceType::Input ||snd.type != BalVulkan::EShaderResourceType::Output )
                {
                    ImGui::Text( "%s", snd.name.c_str());
                    ImGui::NextColumn();
                    switch ( snd.type )
                    {
                        case BalVulkan::EShaderResourceType::None:break;
                        case BalVulkan::EShaderResourceType::InputAttachment:ImGui::Text( "Input Attachment" );
                            break;
                        case BalVulkan::EShaderResourceType::Image:ImGui::Text( "Image" );
                            if ( ImGui::BeginDragDropTarget())
                            {
                                if ( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( ToString( EFileTypes::Image )))
                                {
                                    std::cout << "Image bound: ";
                                    std::cout << payload->DataSize;
                                    std::cout << "\n";
                                    const Balbino::CTexture* pModel = m_pSystem->GetResourceManager()->GetTexture( static_cast<SFile*>( payload->Data )->uuid, true );
                                    snd.resourceID = pModel->GetID();
                                }
                                ImGui::EndDragDropTarget();
                            }
                            break;
                        case BalVulkan::EShaderResourceType::ImageSampler:ImGui::Text( "Image Sampler" );
                            if ( ImGui::BeginDragDropTarget())
                            {
                                if ( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( ToString( EFileTypes::Image )))
                                {
                                    std::cout << "Image bound: ";
                                    std::cout << payload->DataSize;
                                    std::cout << "\n";
                                    const Balbino::CTexture* pModel = m_pSystem->GetResourceManager()->GetTexture( static_cast<SFile*>( payload->Data )->uuid, true );
                                    snd.resourceID = pModel->GetID();
                                }
                                ImGui::EndDragDropTarget();
                            }
                            break;
                        case BalVulkan::EShaderResourceType::ImageStorage:ImGui::Text( "Image Storage" );
                            break;
                        case BalVulkan::EShaderResourceType::Sampler:ImGui::Text( "Sampler" );
                            break;
                        case BalVulkan::EShaderResourceType::BufferUniform:ImGui::Text( "Uniform Buffer" );
                            break;
                        case BalVulkan::EShaderResourceType::BufferStorage:ImGui::Text( "Storage Buffer" );
                            break;
                        case BalVulkan::EShaderResourceType::PushConstant:ImGui::Text( "Push Constant" );
                            break;
                        case BalVulkan::EShaderResourceType::SpecializationConstant:ImGui::Text( "Specialization Constant" );
                            break;
                        default:;
                    }
                    ImGui::NextColumn();
                }
            }
        }
        ImGui::End();
    }
}

void BalEditor::CMaterialEditor::Cleanup()
{
}

void BalEditor::CMaterialEditor::ShowWindow()
{
    m_isVisible = true;
    ImGui::SetWindowFocus( "Material Editor" );
}

void BalEditor::CMaterialEditor::SetMaterial(const SFile& currentMaterial)
{
    Balbino::CMaterial* pMaterial = m_pSystem->GetResourceManager()->GetMaterial( currentMaterial.uuid, true );
    if( pMaterial )
    {
        m_shaderResources = pMaterial->GetShaderResourcesVector();
        m_currentMaterial = currentMaterial;
    }
}
