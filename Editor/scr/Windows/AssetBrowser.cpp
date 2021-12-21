#include "pch.h"
#include "AssetBrowser.h"

#include "ImageResource.h"
#include "imgui.h"

BalEditor::CAssetBrowser::CAssetBrowser()
	: m_pUnknownIcon{ nullptr }
	, m_pFolderIcon{ nullptr }
	, m_pBalbinoIcon{ nullptr }
	, m_pImageIcon{ nullptr }
	, m_pAudioIcon{ nullptr }
	, m_pModelIcon{ nullptr }
	, m_pPresetIcon{ nullptr }
	, m_pCodeIcon{ nullptr }
{
}

BalEditor::CAssetBrowser::~CAssetBrowser()
{
}

void BalEditor::CAssetBrowser::Initialize( BalVulkan::CDevice* pDevice)
{
	m_pUnknownIcon = BalVulkan::CImageResource::CreateNew( pDevice );
	m_pFolderIcon = BalVulkan::CImageResource::CreateNew( pDevice );
	m_pBalbinoIcon = BalVulkan::CImageResource::CreateNew( pDevice );
	m_pImageIcon = BalVulkan::CImageResource::CreateNew( pDevice );
	m_pAudioIcon = BalVulkan::CImageResource::CreateNew( pDevice );
	m_pModelIcon = BalVulkan::CImageResource::CreateNew( pDevice );
	m_pPresetIcon = BalVulkan::CImageResource::CreateNew( pDevice );
	m_pCodeIcon = BalVulkan::CImageResource::CreateNew( pDevice );
}

void BalEditor::CAssetBrowser::Draw()
{
	const std::filesystem::path path( "../Data" );

	//ImGui::SetNextWindowSizeConstraints( ImVec2( 100.0f, -1.0f ), ImVec2( -1.0f, -1.0f ), ImGuiCond_FirstUseEver );
	ImGui::Begin( "Asset Browser", nullptr, 0 );
	ImGui::BeginChild( "Asset Tree", ImVec2{ 100, -1 }, true, ImGuiWindowFlags_HorizontalScrollbar );
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

	for ( const auto& currentFile : m_selected )
	{
		switch ( currentFile.type )
		{
			case EFileTypes::Folder:
				{
					ImGui::Image( m_pFolderIcon->GetImage(), { 16, 16 } );
					break;
				}
			case EFileTypes::Scene:
				{
					ImGui::Image( m_pBalbinoIcon->GetImage(), { 16, 16 } );
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
					//ImGui::Image( ( void* ) ( intptr_t ) ( Icon ), { float( w ) / biggestSide * 16, float( h ) / biggestSide * 16 } );
					break;
				}
			case EFileTypes::Audio:
				{
					ImGui::Image( m_pAudioIcon->GetImage(), { 16, 16 } );
					break;
				}
			case EFileTypes::Code:
				{
					ImGui::Image( m_pCodeIcon->GetImage(), { 16, 16 } );
					break;
				}
			case EFileTypes::Unknown:
				{
					ImGui::Image( m_pUnknownIcon->GetImage(), { 16, 16 } );
					break;
				}
			case EFileTypes::Font:
				{
					ImGui::Image( m_pFolderIcon->GetImage(), { 16, 16 } );
					break;
				}
			case EFileTypes::Model:
				{
					ImGui::Image( m_pModelIcon->GetImage(), { 16, 16 } );
					break;
				}
			case EFileTypes::Preset:
				{
					ImGui::Image( m_pPresetIcon->GetImage(), { 16, 16 } );
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
				break;
			}
		}
		if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
		{
			ImGui::SetDragDropPayload( "asset", &currentFile, sizeof( SFile ) );
			ImGui::Text( currentFile.alias.c_str() );
			ImGui::EndDragDropSource();
		}
	}
	ImGui::EndChild();
	ImGui::End();
}

void BalEditor::CAssetBrowser::Cleanup()
{
	m_pUnknownIcon->Release();
	m_pFolderIcon->Release();
	m_pBalbinoIcon->Release();
	m_pImageIcon->Release();
	m_pAudioIcon->Release();
	m_pModelIcon->Release();
	m_pPresetIcon->Release();
	m_pCodeIcon->Release();
}

void BalEditor::CAssetBrowser::MoveIn( const std::filesystem::path& path, std::vector<SFile>& selectedPath )
{
	static ImGuiTreeNodeFlags s_baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

	const std::vector<SFile> files = GetFilesInPath( path );
	for ( const auto& file : files )
	{
		ImGuiTreeNodeFlags nodeFlags = s_baseFlags;
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
