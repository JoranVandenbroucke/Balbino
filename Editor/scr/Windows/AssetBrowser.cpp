#include "pch.h"
#include "AssetBrowser.h"

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "Importer.h"

BalEditor::CAssetBrowser::CAssetBrowser()
	: m_pUnknownIcon{ nullptr }
	, m_pFolderIcon{ nullptr }
	, m_pBalbinoIcon{ nullptr }
	, m_pImageIcon{ nullptr }
	, m_pAudioIcon{ nullptr }
	, m_pModelIcon{ nullptr }
	, m_pPresetIcon{ nullptr }
	, m_pCodeIcon{ nullptr }
	, m_pFontIcon{ nullptr }
	, m_pVkDescriptorSetUnknownIcon{ VK_NULL_HANDLE }
	, m_pVkDescriptorSetFolderIcon{ VK_NULL_HANDLE }
	, m_pVkDescriptorSetBalbinoIcon{ VK_NULL_HANDLE }
	, m_pVkDescriptorSetImageIcon{ VK_NULL_HANDLE }
	, m_pVkDescriptorSetAudioIcon{ VK_NULL_HANDLE }
	, m_pVkDescriptorSetModelIcon{ VK_NULL_HANDLE }
	, m_pVkDescriptorSetPresetIcon{ VK_NULL_HANDLE }
	, m_pVkDescriptorSetCodeIcon{ VK_NULL_HANDLE }
	, m_pVkDescriptorSetFontIcon{ VK_NULL_HANDLE }
	, m_isVisible{ true }
	, m_size{ 32 }
{
}

BalEditor::CAssetBrowser::~CAssetBrowser()
{
}

void BalEditor::CAssetBrowser::Initialize()
{
	ImportTexture( "../Data/Editor/Icons/UnknownFile.basset", m_pUnknownIcon );
	ImportTexture( "../Data/Editor/Icons/Folder.basset", m_pFolderIcon );
	ImportTexture( "../Data/Editor/Icons/Balbino.basset", m_pBalbinoIcon );
	ImportTexture( "../Data/Editor/Icons/ImageFile.basset", m_pImageIcon );
	ImportTexture( "../Data/Editor/Icons/AudioFile.basset", m_pAudioIcon );
	ImportTexture( "../Data/Editor/Icons/ModelFile.basset", m_pModelIcon );
	ImportTexture( "../Data/Editor/Icons/PresetFile.basset", m_pPresetIcon );
	ImportTexture( "../Data/Editor/Icons/CodeFile.basset", m_pCodeIcon );
	ImportTexture( "../Data/Editor/Icons/FontFile.basset", m_pFontIcon );
	m_pVkDescriptorSetUnknownIcon = ( VkDescriptorSet ) ImGui_ImplVulkan_AddTexture( m_pUnknownIcon->GetSampler(), m_pUnknownIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
	m_pVkDescriptorSetFolderIcon = ( VkDescriptorSet ) ImGui_ImplVulkan_AddTexture( m_pFolderIcon->GetSampler(), m_pFolderIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
	m_pVkDescriptorSetBalbinoIcon = ( VkDescriptorSet ) ImGui_ImplVulkan_AddTexture( m_pBalbinoIcon->GetSampler(), m_pBalbinoIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
	m_pVkDescriptorSetImageIcon = ( VkDescriptorSet ) ImGui_ImplVulkan_AddTexture( m_pImageIcon->GetSampler(), m_pImageIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
	m_pVkDescriptorSetAudioIcon = ( VkDescriptorSet ) ImGui_ImplVulkan_AddTexture( m_pAudioIcon->GetSampler(), m_pAudioIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
	m_pVkDescriptorSetModelIcon = ( VkDescriptorSet ) ImGui_ImplVulkan_AddTexture( m_pModelIcon->GetSampler(), m_pModelIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
	m_pVkDescriptorSetPresetIcon = ( VkDescriptorSet ) ImGui_ImplVulkan_AddTexture( m_pPresetIcon->GetSampler(), m_pPresetIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
	m_pVkDescriptorSetCodeIcon = ( VkDescriptorSet ) ImGui_ImplVulkan_AddTexture( m_pCodeIcon->GetSampler(), m_pCodeIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
	m_pVkDescriptorSetFontIcon = ( VkDescriptorSet ) ImGui_ImplVulkan_AddTexture( m_pFontIcon->GetSampler(), m_pFontIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

}

void BalEditor::CAssetBrowser::Draw()
{
	const std::filesystem::path path( "../Data" );

	//ImGui::SetNextWindowSizeConstraints( ImVec2( 100.0f, -1.0f ), ImVec2( -1.0f, -1.0f ), ImGuiCond_FirstUseEver );
	if( m_isVisible && ImGui::Begin( "Asset Browser", &m_isVisible ))
	{
		ImGui::BeginChild( "Asset Tree", ImVec2{ 128, -1 }, true, ImGuiWindowFlags_HorizontalScrollbar );
		ImGui::SetNextItemOpen( true );
		const bool open{ ImGui::TreeNode( "Data" ) };
		if ( ImGui::IsItemClicked() )
		{
			m_selected = GetFilesInPath( path );
		}
		if ( open )
		{
			MoveIn( path, m_selected );
			ImGui::TreePop();
		}
		ImGui::EndChild();

		std::ranges::sort( m_selected, []( const SFile& left, const SFile& right )
		{
			return left.alias < right.alias;
		} );
		std::ranges::sort( m_selected, []( const SFile& left, const SFile& right )
		{
			return left.isFolder > right.isFolder;
		} );

		ImGui::SameLine();
		ImGui::BeginChild( "Asset File", ImVec2{ -1, -1 }, true, ImGuiWindowFlags_AlwaysAutoResize );
		int id{};
		for ( const auto& currentFile : m_selected )
		{
			ImGui::PushID( id++ );
			switch ( currentFile.type )
			{
				case EFileTypes::Folder:
					{
						ImGui::Image( m_pVkDescriptorSetFolderIcon, { m_size, m_size } );
						break;
					}
				case EFileTypes::Scene:
					{
						ImGui::Image( m_pVkDescriptorSetBalbinoIcon, { m_size, m_size } );
						break;
					}
				case EFileTypes::Image:
					{
						//todo:: get image data
						//GLuint Icon = ResourceManager::LoadIcon( currentFile.path.generic_u8string() );
						//int w{}, h{};
						//int miplevel{};
						//glBindIcon( GL_Icon_2D, Icon );
						//glGetTexLevelParameteriv( GL_Icon_2D, miplevel, GL_Icon_WIDTH, &w );
						//glGetTexLevelParameteriv( GL_Icon_2D, miplevel, GL_Icon_HEIGHT, &h );
						//glBindIcon( GL_Icon_2D, 0 );
						//( Icon, miplevel, GL_Icon_HEIGHT, &h );
						//const int biggestSide{ ( w > h ) ? w : h };
						ImGui::Image( m_pVkDescriptorSetImageIcon, { m_size, m_size } );
						break;
					}
				case EFileTypes::Audio:
					{
						ImGui::Image( m_pVkDescriptorSetAudioIcon, { m_size, m_size } );
						break;
					}
				case EFileTypes::Code:
					{
						ImGui::Image( m_pVkDescriptorSetCodeIcon, { m_size, m_size } );
						break;
					}
				case EFileTypes::Unknown:
					{
						ImGui::Image( m_pVkDescriptorSetUnknownIcon, { m_size, m_size } );
						break;
					}
				case EFileTypes::Font:	//todo add font icon
					{
						ImGui::Image( m_pVkDescriptorSetUnknownIcon, { m_size, m_size } );
						break;
					}
				case EFileTypes::Model:
					{
						ImGui::Image( m_pVkDescriptorSetModelIcon, { m_size, m_size } );
						break;
					}
				case EFileTypes::Preset:
					{
						ImGui::Image( m_pVkDescriptorSetPresetIcon, { m_size, m_size } );
						break;
					}
			}
			ImGui::SameLine();
			bool isSelected{ false };
			ImGui::Selectable( currentFile.alias.c_str(), &isSelected, ImGuiSelectableFlags_AllowDoubleClick );
			if ( isSelected )
			{
				if ( currentFile.isFolder && ImGui::IsMouseDoubleClicked( 0 ) )
				{
					m_selected = GetFilesInPath( currentFile.path );
					ImGui::PopID();
					break;
				}
			}
			if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
			{
				ImGui::SetDragDropPayload( "asset", &currentFile, sizeof( SFile ) );
				ImGui::Text( currentFile.alias.c_str() );
				ImGui::EndDragDropSource();
			}
			ImGui::NewLine();
			ImGui::PopID();
		}
		ImGui::SliderFloat( "Icon Size", &m_size, 8.0f, 128.f, "%.0f", ImGuiSliderFlags_NoInput);
		ImGui::EndChild();
		ImGui::End();
	}
}

void BalEditor::CAssetBrowser::Cleanup()
{
	ImGui_ImplVulkan_DestroyTexture( m_pVkDescriptorSetUnknownIcon );
	ImGui_ImplVulkan_DestroyTexture( m_pVkDescriptorSetFolderIcon );
	ImGui_ImplVulkan_DestroyTexture( m_pVkDescriptorSetBalbinoIcon );
	ImGui_ImplVulkan_DestroyTexture( m_pVkDescriptorSetImageIcon );
	ImGui_ImplVulkan_DestroyTexture( m_pVkDescriptorSetAudioIcon );
	ImGui_ImplVulkan_DestroyTexture( m_pVkDescriptorSetModelIcon );
	ImGui_ImplVulkan_DestroyTexture( m_pVkDescriptorSetPresetIcon );
	ImGui_ImplVulkan_DestroyTexture( m_pVkDescriptorSetCodeIcon );
	ImGui_ImplVulkan_DestroyTexture( m_pVkDescriptorSetFontIcon );
	delete m_pUnknownIcon;
	delete m_pFolderIcon;
	delete m_pBalbinoIcon;
	delete m_pImageIcon;
	delete m_pAudioIcon;
	delete m_pModelIcon;
	delete m_pPresetIcon;
	delete m_pCodeIcon;
	delete m_pFontIcon;
	m_selected.clear();
}

void BalEditor::CAssetBrowser::ShowWindow()
{
	m_isVisible = true;
	ImGui::SetWindowFocus( "Asset Browser" );
}

void BalEditor::CAssetBrowser::MoveIn( const std::filesystem::path& path, std::vector<SFile>& selectedPath )
{
	constexpr ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

	const std::vector<SFile> files = GetFilesInPath( path );
	for ( const auto& file : files )
	{
		ImGuiTreeNodeFlags nodeFlags = baseFlags;
		nodeFlags |= ( file.isFolder == false ? ImGuiTreeNodeFlags_Leaf : 0 );

		if ( file.isFolder )
		{
			const bool open = ImGui::TreeNodeEx( file.alias.c_str(), nodeFlags );
			if ( ImGui::IsItemClicked() )
			{
				selectedPath = GetFilesInPath( file.path );
			}
			if ( open )
			{
				MoveIn( file.path, selectedPath );
				ImGui::TreePop();
			}
		}
	}
}
