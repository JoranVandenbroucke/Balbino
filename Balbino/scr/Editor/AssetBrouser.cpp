#include "BalbinoPCH.h"
#include "AssetBrouser.h"

#include "../ResourceManager.h"

Balbino::AssetBrouser::AssetBrouser()
	: m_BalbinoTexture{ (unsigned int) ResourceManager::LoadTexture( "../Balbino/AssetBrousetIcons/Balbino.png" ) }
	, m_AudioTexture{ (unsigned int) ResourceManager::LoadTexture( "../Balbino/AssetBrousetIcons/AudioFile.png" ) }
	, m_CppChTexture{ ResourceManager::LoadTexture( "../Balbino/AssetBrousetIcons/CppChFile.png" ) }
	, m_UnknownTexture{ ResourceManager::LoadTexture( "../Balbino/AssetBrousetIcons/UnknownFile.png" ) }
	, m_FolderTexture{ ResourceManager::LoadTexture( "../Balbino/AssetBrousetIcons/Folder.png" ) }
{
}

std::vector<Balbino::file> Balbino::AssetBrouser::GetFilesInPath( const std::filesystem::path& path )
{
	std::vector<file> files;

	for( const auto& dirEntry : std::filesystem::directory_iterator( path ) )
	{
		const std::filesystem::path& dirPath = dirEntry.path();
		std::string extention = dirPath.extension().string();
		fileTypes type{};
		for( char& character : extention )
		{
			character = std::toupper( character );
		}

		if( extention == "" )
		{
			type = fileTypes::folder;
		}
		else if( extention == ".BALBINO" )
		{
			type = fileTypes::scene;
		}
		else if( std::find( m_SuportedImageFormat.begin(), m_SuportedImageFormat.end(), extention ) != m_SuportedImageFormat.end() )
		{
			type = fileTypes::image;
		}
		else if( std::find( m_SuportedAudioFormat.begin(), m_SuportedAudioFormat.end(), extention ) != m_SuportedAudioFormat.end() )
		{
			type = fileTypes::audio;
		}
		else if( std::find( m_SuportedFontFormat.begin(), m_SuportedFontFormat.end(), extention ) != m_SuportedFontFormat.end() )
		{
			type = fileTypes::font;
		}
		else if( extention == ".CPP" || extention == ".H" )
		{
			type = fileTypes::cppch;
		}
		else
		{
			type = fileTypes::unknown;
		}

		files.push_back( {
			extention == "",
			type,
			dirPath.filename().string(),
			std::filesystem::relative( dirPath )
			} );
	}
	return files;
}

void Balbino::AssetBrouser::MoveIn( const std::filesystem::path& path, std::vector<file>& selectedPath )
{
	static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

	std::vector<file> files = GetFilesInPath( path );
	for( const auto& file : files )
	{
		ImGuiTreeNodeFlags nodeFlags = baseFlags;
		nodeFlags |= ( file.isFolder == false ? ImGuiTreeNodeFlags_Leaf : 0 );

		if( file.isFolder )
		{
			bool open = ImGui::TreeNodeEx( file.alias.c_str(), nodeFlags );
			if( ImGui::IsItemClicked() )
			{
				selectedPath = GetFilesInPath( file.path );
			}
			if( open )
			{
				MoveIn( file.path, selectedPath );
				ImGui::TreePop();
			}
		}
	}
}

void Balbino::AssetBrouser::Draw()
{
	static std::vector<file> selected{};
	std::filesystem::current_path( "../Data" );

	//ImGui::SetNextWindowSizeConstraints( ImVec2( 100.0f, -1.0f ), ImVec2( -1.0f, -1.0f ), ImGuiCond_FirstUseEver );
	ImGui::Begin( "Asset Brouser", nullptr, 0, ImVec4{ 0.1f, 0.3f, 0.f, 1.f } );
	ImGui::BeginChild( "Asset Tree", ImVec2{ 100,-1 }, true, ImGuiWindowFlags_HorizontalScrollbar );
	ImGui::SetNextItemOpen( true );
	bool open = ImGui::TreeNode( "Data" );
	if( ImGui::IsItemClicked() )
	{
		selected = GetFilesInPath( std::filesystem::current_path() );
	}
	if( open )
	{
		MoveIn( std::filesystem::current_path(), selected );
		ImGui::TreePop();
	}
	ImGui::EndChild();

	std::sort( selected.begin(), selected.end(), []( file& left, file& right )
	{
		return left.alias < right.alias;
	} );
	std::sort( selected.begin(), selected.end(), []( file& left, file& right )
	{
		return left.isFolder > right.isFolder;
	} );

	ImGui::SameLine();
	ImGui::BeginChild( "Asset File", ImVec2{ -1,-1 }, true/*, ImGuiWindowFlags_AlwaysAutoResize */ );

	for( const auto& currentFile : selected )
	{
		switch( currentFile.type )
		{
		case fileTypes::folder:
		{
			ImGui::Image( (void*) (intptr_t) ( m_FolderTexture ), { 16,16 } );
			break;
		}
		case fileTypes::scene:
		{
			ImGui::Image( (void*) (intptr_t) ( m_BalbinoTexture ), { 16,16 } );
			break;
		}
		case fileTypes::image:
		{
			GLuint texture = ResourceManager::LoadTexture( currentFile.path.generic_u8string() );
			int w{}, h{};
			int miplevel{};
			glBindTexture( GL_TEXTURE_2D, texture );
			glGetTexLevelParameteriv( GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w );
			glGetTexLevelParameteriv( GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h );
			glBindTexture( GL_TEXTURE_2D, 0 );
			( texture, miplevel, GL_TEXTURE_HEIGHT, &h );
			int biggestSide{ ( w > h ) ? w : h };
			ImGui::Image( (void*) (intptr_t) ( texture ), { float( w ) / biggestSide * 16, float( h ) / biggestSide * 16 } );
			break;
		}
		case fileTypes::audio:
		{
			ImGui::Image( (void*) (intptr_t) ( m_AudioTexture ), { 16,16 } );
			break;
		}
		case fileTypes::cppch:
		{
			ImGui::Image( (void*) (intptr_t) ( m_CppChTexture ), { 16,16 } );
			break;
		}
		case fileTypes::unknown:
		default:
		{
			ImGui::Image( (void*) (intptr_t) ( m_UnknownTexture ), { 16,16 } );
			break;
		}
		}
		//if( currentFile.type )
		//{
		//	ImGui::Image( (void*) (intptr_t) ( m_FolderTexture ), { 16,16 } );
		//}
		//else if( currentFile.type == ".BALBINO" )
		//{
		//	ImGui::Image( (void*) (intptr_t) ( m_BalbinoTexture ), { 16,16 } );
		//}
		//else if( currentFile.type )
		//{
		//	GLuint texture = ResourceManager::LoadTexture( currentFile.path.generic_u8string() );
		//	int w{}, h{};
		//	int miplevel = 0;
		//	glBindTexture( GL_TEXTURE_2D, texture );
		//	glGetTexLevelParameteriv( GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w );
		//	glGetTexLevelParameteriv( GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h );
		//	glBindTexture( GL_TEXTURE_2D, 0 );
		//	( texture, miplevel, GL_TEXTURE_HEIGHT, &h );
		//	int biggestSide{ ( w > h ) ? w : h };
		//	ImGui::Image( (void*) (intptr_t) ( texture ), { float( w ) / biggestSide * 16, float( h ) / biggestSide * 16 } );
		//}
		//else if( currentFile.type )
		//{
		//	ImGui::Image( (void*) (intptr_t) ( m_AudioTexture ), { 16,16 } );
		//}
		//else if( currentFile.type == ".CPP" || currentFile.type == ".H" )
		//{
		//	ImGui::Image( (void*) (intptr_t) ( m_CppChTexture ), { 16,16 } );
		//}
		//else
		//{
		//	ImGui::Image( (void*) (intptr_t) ( m_UnknownTexture ), { 16,16 } );
		//}
		ImGui::SameLine();
		//ImGui::TextColored( ImVec4{ .3f, .6f, 0.f, 1.f }, file.alias.c_str() );
		bool isSelected{ false };
		ImGui::Selectable( currentFile.alias.c_str(), &isSelected, ImGuiSelectableFlags_AllowDoubleClick );
		if( isSelected )
		{
			if( currentFile.isFolder && ImGui::IsMouseDoubleClicked( 0 ) )
			{
				selected = GetFilesInPath( currentFile.path );
				break;
			}
		}
		if( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
		{
			ImGui::SetDragDropPayload( "asset", &currentFile, sizeof( file ) );
			ImGui::Text( currentFile.alias.c_str() );
			ImGui::EndDragDropSource();
		}
	}
	ImGui::EndChild();
	ImGui::End();
	std::filesystem::current_path( m_GameStart );
}