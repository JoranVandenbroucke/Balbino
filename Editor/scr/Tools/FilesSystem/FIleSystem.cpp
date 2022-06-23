
#include "FileSystem.h"
#include "FileParcer.h"
#include "Inporter/Importer.h"

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
		character = static_cast< char >( std::toupper( character ) );
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

SFile BalEditor::GetData( const std::filesystem::path& path )
{
	SFile file{};
	std::string extenstion = path.extension().string();
	file.path = std::filesystem::relative( path );
	file.fileName = path.filename().string();
	for ( int i{}; i < static_cast< int >( std::size( extenstion ) ); ++i )
	{
		extenstion[i] = static_cast< char >( std::toupper( extenstion[i] ) );
	}
	if ( extenstion.empty() )
	{
		file.type = EFileTypes::Folder;
		file.isFolder = true;
	}
	else if ( extenstion == ".BalEditor" )
	{
		file.type = EFileTypes::Scene;
	}
	else if ( std::ranges::find( supportedScriptFormat, extenstion ) != supportedScriptFormat.end() )
	{
		file.type = EFileTypes::Code;
	}
	else if ( extenstion == ".BASSET" )
	{
		std::ifstream fileStream( path, std::ios::in | std::ios::binary );
		if ( fileStream.is_open() )
		{
			uint8_t value;
			BinaryReadWrite::Read( fileStream, file.uuid );
			BinaryReadWrite::Read( fileStream, value );

			file.type = static_cast< EFileTypes >( value );
			
			fileStream.close();
		}
	}
	return file;
}
