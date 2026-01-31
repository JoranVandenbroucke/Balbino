#include "FileSystem.h"
#include "Inporter/Importer.h"
#include "Inporter/MeshFileImporter.h"
#include "Inporter/TextureFileImporter.h"

#include <fstream>

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
