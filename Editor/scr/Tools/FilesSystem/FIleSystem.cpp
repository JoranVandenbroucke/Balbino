#include "pch.h"
#include "FIleSystem.h"
#include <fstream>

#include "Inporter/ImageImporter.h"

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

std::vector<BalEditor::SFile> BalEditor::GetFilesInPath( const std::filesystem::path& path )
{
	std::vector<SFile> files;

	for ( const auto& dirEntry : std::filesystem::directory_iterator( path ) )
	{
		const std::filesystem::path& dirPath = dirEntry.path();
		std::string extenstion = dirPath.extension().string();
		EFileTypes type{};
		for ( int i{}; i < std::size( extenstion ); ++i )
		{
			extenstion[i] = static_cast<char>( std::toupper( extenstion[i] ) );
		}

		if ( extenstion == "" )
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
			if ( !file.is_open() )
			{
				uint8_t value;
				BinaryReadWrite::Read( file, value );
				type = static_cast<EFileTypes>( value );
				file.close();
			}
		}
		files.push_back( {
			extenstion == "",
			type,
			dirPath.filename().string(),
			relative( dirPath )
		} );
	}
	return files;
}

std::ostream& BalEditor::BinaryReadWrite::Write( std::ostream& file, const std::string& value )
{
	const char* pText = value.c_str();
	const int size = static_cast<int>( value.size() );

	Write( file, size );
	return file.write( pText, size );
}

std::istream& BalEditor::BinaryReadWrite::Read( std::istream& file, std::string& value )
{
	value.clear();
	int size{};
	Read( file, size );
	if ( size == 0 )
		return file;
	auto pBuffer{ DBG_NEW char[size] };
	file.read( pBuffer, size );
	value.append( pBuffer, size );
	delete[] pBuffer;
	return file;
}
