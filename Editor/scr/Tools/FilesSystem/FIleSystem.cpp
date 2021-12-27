#include "pch.h"
#include "FileSystem.h"
#include "FileParcer.h"
#include "Inporter/ImageImporter.h"

#include <fstream>

bool BalEditor::ImportFile( const char* pPath )
{
	std::ifstream fileChecker{ pPath };
	if ( !fileChecker.is_open() )
	{
		return false;
	}
	fileChecker.close();

	const std::filesystem::path dirPath{ pPath };
	std::string extenstion = dirPath.extension().string();
	for ( char& character : extenstion )
	{
		character = static_cast<char>( std::toupper( character ) );
	}
	if ( extenstion == ".BALBINO" || extenstion == ".BASSET" || extenstion == ".CPP" || extenstion == ".H" )
	{
		copy( dirPath, "../Data" );
		return true;
	}
	if ( std::ranges::find( supportedImageFormat, extenstion ) != supportedImageFormat.end() )
		return ImportImage( dirPath );
	if ( std::ranges::find( supportedAudioFormat, extenstion ) != supportedAudioFormat.end() )
		return ImportAudio( dirPath );
	if ( std::ranges::find( supportedFontFormat, extenstion ) != supportedFontFormat.end() )
		return ImportFont( dirPath );
	if ( std::ranges::find( supportedMeshFormat, extenstion ) != supportedMeshFormat.end() )
		return ImportMesh( dirPath );
	return false;
}

std::vector<SFile> BalEditor::GetFilesInPath( const std::filesystem::path& path )
{
	std::vector<SFile> files;

	for ( const auto& dirEntry : std::filesystem::directory_iterator( path ) )
	{
		const std::filesystem::path& dirPath = dirEntry.path();
		std::string extenstion = dirPath.extension().string();
		EFileTypes type{};
		for ( int i{}; i < (int)std::size( extenstion ); ++i )
		{
			extenstion[i] = static_cast<char>( std::toupper( extenstion[i] ) );
		}

		if ( extenstion.empty() )
		{
			type = EFileTypes::Folder;
		}
		else if ( extenstion == ".BalEditor" )
		{
			type = EFileTypes::Scene;
		}
		else if ( std::ranges::find( supportedScriptFormat, extenstion ) != supportedScriptFormat.end() )
		{
			type = EFileTypes::Code;
		}
		else if ( extenstion == ".BASSET" )
		{
			std::ifstream file( dirPath, std::ios::in | std::ios::binary );
			if ( file.is_open() )
			{
				uint8_t value;
				BinaryReadWrite::Read( file, value );
				type = static_cast<EFileTypes>( value );
				file.close();
			}
		}
		files.push_back( {
			extenstion.empty(),
			type,
			dirPath.filename().string(),
			relative( dirPath )
		} );
	}
	return files;
}
