#include "FileSystem.h"
#include "Inporter/Importer.h"
#include "Inporter/MeshFileImporter.h"
#include "Inporter/TextureFileImporter.h"

#include <fstream>
#include <ranges>

bool FawnForge::ImportFile( const char* pPath, const char* pDestinationDirection, FawnForge::CMeshFileImporter* pMeshImporter, FawnForge::CTextureFileImporter* pTextureImporter )
{
    std::ifstream fileChecker { pPath };
    if ( !fileChecker.is_open() )
    {
        return false;
    }
    fileChecker.close();

    const std::filesystem::path dirPath { pPath };
    std::string extension = dirPath.extension().string();
    for ( char& character : extension )
    {
        character = static_cast<char>( std::toupper( character ) );
    }
    if ( extension == ".BALBINO" || extension == ".BASSET" || extension == ".CPP" || extension == ".H" )
    {
        copy( dirPath, pDestinationDirection );
        return true;
    }
    if ( std::ranges::find( g_supportedImageFormat, extension ) != std::end(g_supportedImageFormat) )
    {
        pTextureImporter->SetVisible( dirPath, pDestinationDirection );
        return true;
    }
    if ( std::ranges::find( g_supportedAudioFormat, extension ) != std::end(g_supportedAudioFormat) )
    {
        return ImportAudio( dirPath, pDestinationDirection );   // todo: add importer
    }
    if ( std::ranges::find( g_supportedFontFormat, extension ) != std::end(g_supportedFontFormat) )
    {
        return ImportFont( dirPath, pDestinationDirection );    // todo: add importer
    }
    if ( std::ranges::find( g_supportedMeshFormat, extension ) != std::end(g_supportedMeshFormat) )
    {
        pMeshImporter->SetVisible( dirPath, pDestinationDirection );
        return true;
    }
    return false;
}

SFile FawnForge::GetData( const std::filesystem::path& path )
{
    SFile file {};
    std::string extension { path.extension().string() };
    std::ranges::transform( extension.begin(), extension.end(), extension.begin(), []( unsigned char c ) {
        return (unsigned char)std::tolower( c );
    } );
    file.path     = std::filesystem::relative( path ).string();
    file.fileName = path.filename().string();
    if ( extension.empty() )
    {
        file.type     = file_type::folder;
        file.isFolder = true;
    }
    else if ( extension == ".fawnforge" )
    {
        file.type = file_type::scene;
    }
    else if ( std::ranges::find( g_supportedScriptFormat, extension ) != std::end(g_supportedScriptFormat) )
    {
        file.type = file_type::code;
    }
    else if ( extension == ".basset" )
    {
        std::ifstream fileStream( path, std::ios::in | std::ios::binary );
        if ( fileStream.is_open() )
        {
            uint8_t value;
            Serialization::Read( fileStream, file.uuid );
            Serialization::Read( fileStream, value );

            file.type = static_cast<file_type>( value );

            fileStream.close();
        }
    }
    return file;
}
