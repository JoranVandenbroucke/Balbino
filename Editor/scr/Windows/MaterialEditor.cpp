#include "pch.h"
#include "MaterialEditor.h"

#include "IManager.h"
#include "imgui.h"
#include "IResourceManager.h"
#include "Material.h"

BalEditor::CMaterialEditor::CMaterialEditor()
	: m_isVisible{}
	, m_currentMaterial{}
	, m_pSystem{ nullptr }
{
}

void BalEditor::CMaterialEditor::Initialize( const ISystem* pSystem )
{
	m_pSystem = pSystem;
}

void BalEditor::CMaterialEditor::Draw()
{
	if ( m_isVisible && ImGui::Begin( "Material Editor", &m_isVisible ) )
	{
		ImGui::Columns( 2 );
		for ( const auto& snd : m_shaderResources )
		{
			ImGui::Text( snd.name.c_str() );
			ImGui::NextColumn();
			switch ( snd.type )
			{
				case BalVulkan::EShaderResourceType::None:
					break;
				case BalVulkan::EShaderResourceType::Input:
					ImGui::Text( "Input" );
					break;
				case BalVulkan::EShaderResourceType::InputAttachment:
					ImGui::Text( "Input Attachment" );
					break;
				case BalVulkan::EShaderResourceType::Output:
					ImGui::Text( "Output" );
					break;
				case BalVulkan::EShaderResourceType::Image:
					ImGui::Text( "Image" );
					break;
				case BalVulkan::EShaderResourceType::ImageSampler:
					ImGui::Text( "Image Sampler" );
					break;
				case BalVulkan::EShaderResourceType::ImageStorage:
					ImGui::Text( "Image Storage" );
					break;
				case BalVulkan::EShaderResourceType::Sampler:
					ImGui::Text( "Sampler" );
					break;
				case BalVulkan::EShaderResourceType::BufferUniform:
					ImGui::Text( "Uniform Buffer" );
					break;
				case BalVulkan::EShaderResourceType::BufferStorage:
					ImGui::Text( "Storage Buffer" );
					break;
				case BalVulkan::EShaderResourceType::PushConstant:
					ImGui::Text( "Push Constant" );
					break;
				case BalVulkan::EShaderResourceType::SpecializationConstant:
					ImGui::Text( "Specialization Constant" );
					break;
				default:;
			}
			ImGui::NextColumn();
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

void BalEditor::CMaterialEditor::SetMaterial( const SFile& currentMaterial )
{
	Balbino::CMaterial* pMaterial = m_pSystem->GetResourceManager()->LoadMaterial( currentMaterial.path.string() );
	if ( pMaterial )
	{
		m_shaderResources = pMaterial->GetShaderResourcesVector();
		m_currentMaterial = currentMaterial;
	}

	//std::ifstream file( currentMaterial.path, std::ios::in | std::ios::binary );
	//if(!file.is_open() )
	//	return;
	//uint64_t uuid;
	//uint64_t shaderUuid;
	//uint8_t type;
	//BinaryReadWrite::Read( file, uuid);
	//BinaryReadWrite::Read( file, type );
	//bool isAtEnd;
	//BinaryReadWrite::Read( file, shaderUuid );
	//BinaryReadWrite::Read( file, m_shaderResources );
	//BinaryReadWrite::IsAtEnd( file, isAtEnd );
	//if( isAtEnd )
	//{
	//	file.close();
	//	m_currentMaterial = currentMaterial;
	//	return;
	//}
}
