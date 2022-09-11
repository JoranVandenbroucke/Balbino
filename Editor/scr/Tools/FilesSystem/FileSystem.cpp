#include "FileSystem.h"
#include "FileParcer.h"
#include "Inporter/Importer.h"
#include "Inporter/MeshFileImporter.h"
#include "Inporter/TextureFileImporter.h"

#include <fstream>

bool BalEditor::ImportFile( const char* pPath, const char* pDestinationDirection, BalEditor::CMeshFileImporter* pMeshImporter, BalEditor::CTextureFileImporter* pTextureImporter )
{
    std::ifstream fileChecker{ pPath };
    if ( !fileChecker.is_open())
    {
        return false;
    }
    fileChecker.close();

    const std::filesystem::path dirPath{ pPath };
    std::string                 extension = dirPath.extension().string();
    for ( char& character : extension )
    {
        character = static_cast< char >( std::toupper( character ));
    }
    if ( extension == ".BALBINO" || extension == ".BASSET" || extension == ".CPP" || extension == ".H" )
    {
        copy( dirPath, pDestinationDirection );
        return true;
    }
    if ( std::ranges::find( supportedImageFormat, extension ) != supportedImageFormat.end())
    {
        pTextureImporter->SetVisible( dirPath, pDestinationDirection );
        return true;
    }
    if ( std::ranges::find( supportedAudioFormat, extension ) != supportedAudioFormat.end())
    {
        return ImportAudio( dirPath, pDestinationDirection );
    }
    if ( std::ranges::find( supportedFontFormat, extension ) != supportedFontFormat.end())
    {
        return ImportFont( dirPath, pDestinationDirection );
    }
    if ( std::ranges::find( supportedMeshFormat, extension ) != supportedMeshFormat.end())
    {
        pMeshImporter->SetVisible( dirPath, pDestinationDirection );
        return true;
    }
    return false;
}

SFile BalEditor::GetData( const std::filesystem::path& path )
{
    SFile       file{};
    std::string extension = path.extension().string();
    file.path     = std::filesystem::relative( path ).string();
    file.fileName = path.filename().string();
    for ( int i{}; i < static_cast< int >( std::size( extension )); ++i )
    {
        extension[i] = static_cast< char >( std::toupper( extension[i] ));
    }
    if ( extension.empty())
    {
        file.type     = EFileTypes::Folder;
        file.isFolder = true;
    }
    else if ( extension == ".BalEditor" )
    {
        file.type = EFileTypes::Scene;
    }
    else if ( std::ranges::find( supportedScriptFormat, extension ) != supportedScriptFormat.end())
    {
        file.type = EFileTypes::Code;
    }
    else if ( extension == ".BASSET" )
    {
        std::ifstream fileStream( path, std::ios::in | std::ios::binary );
        if ( fileStream.is_open())
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
