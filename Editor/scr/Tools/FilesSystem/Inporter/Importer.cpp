#include "Importer.h"

#include "CalcTangents.h"
#include "hdrloader.h"

#include <Common.h>
#include <FileParcer.h>
#include <MeshMetadata.h>
#include <UUID.h>

#include <fstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#pragma warning (push)
#pragma warning (disable : 4100)
#pragma warning (disable : 4201)
#pragma warning (disable : 4244)
#pragma warning (disable : 4458)
#pragma warning (disable : 5054)
#include <gli/gli.hpp>

#pragma warning (pop)

bool BalEditor::ImportImage( const std::filesystem::path& path, const char* pDestinationDirection )
{
    std::filesystem::path p{ std::string( pDestinationDirection ) + path.filename().string() };
    p.replace_extension( ".basset" );
    std::ofstream file( p.string().c_str(), std::ios::out | std::ios::binary );
    if ( !file.is_open())
    {
        return false;
    }
    
    std::string extension = path.extension().string();
    std::ranges::for_each( extension, []( char& c )
    {
        c = static_cast< char >( toupper( c ));
    } );
    
    uint8_t imageType;
    uint32_t imageFormat;
    uint8_t imageMips;
    uint8_t imageLayers;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint8_t pitch;
    void* pData;
    if ( extension == ".KTX" || extension == ".DDS" )
    {
        gli::texture_cube texture{ gli::load( path.string().c_str()) };
        if ( texture.empty())
        {
            return false;
        }
        
        imageType = ( uint8_t ) texture.target();
        imageFormat = ( uint32_t ) texture.format();
        imageMips = ( uint8_t ) ( texture.max_level());
        imageLayers = ( uint8_t ) ( texture.max_face());
        width = ( uint32_t ) texture.extent().x;
        height = ( uint32_t ) texture.extent().y;
        depth = ( uint32_t ) texture.max_layer();
        pitch = (uint8_t)(( width * height * depth ) / texture.size());
        pData = malloc( texture.size());
        memcpy( pData, texture.data(), texture.size());
        texture.clear();
    }
    else if ( extension == ".hdr" )
    {
        HDRLoaderResult hdrLoaderResult{};
        if ( !HDRLoader::load( path.string().c_str(), hdrLoaderResult ))
        {
            return false;
        }
        imageType = ( uint8_t ) BalVulkan::EImageViewType::Cube;
        imageFormat = ( uint32_t ) static_cast< uint32_t >( BalVulkan::EFormat::R16G16B16Sfloat);
        imageMips = ( uint8_t ) 1;
        imageLayers = ( uint8_t ) 6;
        width = hdrLoaderResult.width;
        height = hdrLoaderResult.height;
        depth = 1;
        pitch = 3;
        pData = malloc( pitch );
        memcpy( pData, ( uint8_t* ) ( hdrLoaderResult.cols ), hdrLoaderResult.width * hdrLoaderResult.height * 3 );
        delete hdrLoaderResult.cols;
    }
    else
    {
        SDL_Surface* pSurface{ IMG_Load( path.string().c_str()) };
        if ( pSurface == nullptr )
        {
            std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
            return false;
        }
        imageType = ( uint8_t ) 2;
        switch ( SDL_PixelFormatEnum( pSurface->format->format ))
        {
            
            case SDL_PIXELFORMAT_UNKNOWN:
                imageFormat = ( uint32_t ) VK_FORMAT_UNDEFINED;
                break;
            case SDL_PIXELFORMAT_INDEX1LSB:
                imageFormat = ( uint32_t ) VK_FORMAT_UNDEFINED;
                break;
            case SDL_PIXELFORMAT_INDEX1MSB:
                imageFormat = ( uint32_t ) VK_FORMAT_UNDEFINED;
                break;
            case SDL_PIXELFORMAT_INDEX4LSB:
                imageFormat = ( uint32_t ) VK_FORMAT_UNDEFINED;
                break;
            case SDL_PIXELFORMAT_INDEX4MSB:
                imageFormat = ( uint32_t ) VK_FORMAT_UNDEFINED;
                break;
            case SDL_PIXELFORMAT_INDEX8:
                imageFormat = ( uint32_t ) VK_FORMAT_R8_SRGB;
                break;
            case SDL_PIXELFORMAT_RGB332:
                imageFormat = ( uint32_t ) VK_FORMAT_R32G32B32_SFLOAT;
                break;
            case SDL_PIXELFORMAT_XRGB4444:
                imageFormat = ( uint32_t ) VK_FORMAT_A4R4G4B4_UNORM_PACK16;
                break;
            case SDL_PIXELFORMAT_XBGR4444:
                imageFormat = ( uint32_t ) VK_FORMAT_A4B4G4R4_UNORM_PACK16;
                break;
            case SDL_PIXELFORMAT_XRGB1555:
                imageFormat = ( uint32_t ) VK_FORMAT_A1R5G5B5_UNORM_PACK16;
                break;
            case SDL_PIXELFORMAT_XBGR1555:
                imageFormat = ( uint32_t ) VK_FORMAT_A1R5G5B5_UNORM_PACK16;  //TODO SWAP RGB TO BGR
                break;
            case SDL_PIXELFORMAT_ARGB4444:
                imageFormat = ( uint32_t ) VK_FORMAT_A4R4G4B4_UNORM_PACK16;
                break;
            case SDL_PIXELFORMAT_RGBA4444:
                imageFormat = ( uint32_t ) VK_FORMAT_R4G4B4A4_UNORM_PACK16;
                break;
            case SDL_PIXELFORMAT_ABGR4444:
                imageFormat = ( uint32_t ) VK_FORMAT_A4B4G4R4_UNORM_PACK16;
                break;
            case SDL_PIXELFORMAT_BGRA4444:
                imageFormat = ( uint32_t ) VK_FORMAT_B4G4R4A4_UNORM_PACK16;
                break;
            case SDL_PIXELFORMAT_ARGB1555:
                imageFormat = ( uint32_t ) VK_FORMAT_A1R5G5B5_UNORM_PACK16;
                break;
            case SDL_PIXELFORMAT_RGBA5551:
                imageFormat = ( uint32_t ) VK_FORMAT_R5G5B5A1_UNORM_PACK16;
                break;
            case SDL_PIXELFORMAT_ABGR1555:
                imageFormat = ( uint32_t ) VK_FORMAT_B5G6R5_UNORM_PACK16;    //TODO SWAP RGB TO BGR
                break;
            case SDL_PIXELFORMAT_BGRA5551:
                imageFormat = ( uint32_t ) VK_FORMAT_B5G5R5A1_UNORM_PACK16;
                break;
            case SDL_PIXELFORMAT_RGB565:
                imageFormat = ( uint32_t ) VK_FORMAT_B5G6R5_UNORM_PACK16;    //TODO SWAP BGR TO RGB
                break;
            case SDL_PIXELFORMAT_BGR565:
                imageFormat = ( uint32_t ) VK_FORMAT_B5G6R5_UNORM_PACK16;
                break;
            case SDL_PIXELFORMAT_RGB24:
                imageFormat = ( uint32_t ) VK_FORMAT_R8G8B8_SRGB;
                break;
            case SDL_PIXELFORMAT_BGR24:
                imageFormat = ( uint32_t ) VK_FORMAT_B8G8R8_SRGB;
                break;
            case SDL_PIXELFORMAT_XRGB8888:
                imageFormat = ( uint32_t ) VK_FORMAT_A8B8G8R8_SRGB_PACK32;   //TODO SWAR BGR TO RGB
                break;
            case SDL_PIXELFORMAT_RGBX8888:
                imageFormat = ( uint32_t ) VK_FORMAT_R8G8B8A8_SRGB;
                break;
            case SDL_PIXELFORMAT_XBGR8888:
                imageFormat = ( uint32_t ) VK_FORMAT_A8B8G8R8_SRGB_PACK32;
                break;
            case SDL_PIXELFORMAT_BGRX8888:
                imageFormat = ( uint32_t ) VK_FORMAT_B8G8R8A8_SRGB;
                break;
            case SDL_PIXELFORMAT_ARGB8888:
                imageFormat = ( uint32_t ) VK_FORMAT_A8B8G8R8_SRGB_PACK32;   //TODO SWAR BGR TO RGB
                break;
            case SDL_PIXELFORMAT_RGBA8888:
                imageFormat = ( uint32_t ) VK_FORMAT_R8G8B8A8_SRGB;
                break;
            case SDL_PIXELFORMAT_ABGR8888:
                imageFormat = ( uint32_t ) VK_FORMAT_A8B8G8R8_SRGB_PACK32;
                break;
            case SDL_PIXELFORMAT_BGRA8888:
                imageFormat = ( uint32_t ) VK_FORMAT_B8G8R8A8_SRGB;
                break;
            case SDL_PIXELFORMAT_ARGB2101010:
                imageFormat = ( uint32_t ) VK_FORMAT_A2R10G10B10_UNORM_PACK32;
                break;
            case SDL_PIXELFORMAT_YV12:
                imageFormat = ( uint32_t ) VK_FORMAT_UNDEFINED;
                break;
            case SDL_PIXELFORMAT_IYUV:
                imageFormat = ( uint32_t ) VK_FORMAT_UNDEFINED;
                break;
            case SDL_PIXELFORMAT_YUY2:
                imageFormat = ( uint32_t ) VK_FORMAT_UNDEFINED;
                break;
            case SDL_PIXELFORMAT_UYVY:
                imageFormat = ( uint32_t ) VK_FORMAT_UNDEFINED;
                break;
            case SDL_PIXELFORMAT_YVYU:
                imageFormat = ( uint32_t ) VK_FORMAT_UNDEFINED;
                break;
            case SDL_PIXELFORMAT_NV12:
                imageFormat = ( uint32_t ) VK_FORMAT_UNDEFINED;
                break;
            case SDL_PIXELFORMAT_NV21:
                imageFormat = ( uint32_t ) VK_FORMAT_UNDEFINED;
                break;
            case SDL_PIXELFORMAT_EXTERNAL_OES:
                imageFormat = ( uint32_t ) VK_FORMAT_UNDEFINED;
                break;
        }
        
        imageMips = ( uint8_t ) 1;
        imageLayers = ( uint8_t ) 1;
        width = pSurface->w;
        height = pSurface->h;
        depth = 1;
        pitch = (uint8_t)pSurface->pitch;
        pData = malloc( width * height * pitch );
        memcpy( pData, pSurface->pixels, width * height * pitch );
        SDL_FreeSurface( pSurface );
    }
    
    BinaryReadWrite::Write( file, ( uint64_t ) CUuid());
    BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Image ));
    BinaryReadWrite::Write( file, imageType );
    BinaryReadWrite::Write( file, imageFormat );
    BinaryReadWrite::Write( file, imageMips );
    BinaryReadWrite::Write( file, imageLayers );
    BinaryReadWrite::Write( file, width );
    BinaryReadWrite::Write( file, height );
    BinaryReadWrite::Write( file, depth );
    BinaryReadWrite::Write( file, pitch );
    free( pData );
    file.close();
    return true;
}

bool BalEditor::ImportFont( const std::filesystem::path& path, const char* pDestinationDirection )
{
    TTF_Font* pFont = TTF_OpenFont( path.string().c_str(), 12 );
    if ( pFont == nullptr )
    {
        return false;
    }
    
    std::filesystem::path p{ std::string( pDestinationDirection ) + path.filename().string() };
    p.replace_extension( ".basset" );
    std::ofstream file( p.filename(), std::ios::out | std::ios::binary );
    
    if ( !file.is_open())
    {
        return false;
    }
    TTF_SetFontKerning( pFont, 0 );
    BinaryReadWrite::Write( file, CUuid());
    BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Font ));
    BinaryReadWrite::Write( file, TTF_FontLineSkip( pFont ));
    BinaryReadWrite::Write( file, TTF_GetFontHinting( pFont ));
    BinaryReadWrite::Write( file, TTF_FontHeight( pFont ));
    BinaryReadWrite::Write( file, TTF_FontAscent( pFont ));
    BinaryReadWrite::Write( file, TTF_FontDescent( pFont ));
    BinaryReadWrite::Write( file, TTF_FontLineSkip( pFont ));
    
    for ( uint16_t i{}; i < 256u; ++i )
    {
        const char c[1]{ static_cast< char >( i ) };
        SDL_Surface* pSurface = TTF_RenderText_Solid( pFont, c, { 255, 255, 255, 255 } );
        if ( pSurface == nullptr )
        {
            return false;
        }
        BinaryReadWrite::Write( file, pSurface->w );
        BinaryReadWrite::Write( file, pSurface->h );
        BinaryReadWrite::Write( file, pSurface->h * pSurface->pitch );
        BinaryReadWrite::Write( file, static_cast< uint8_t* >( pSurface->pixels ), static_cast< uint64_t >( pSurface->h ) * pSurface->pitch );
        TTF_CloseFont( pFont );
    }
    file.close();
    return true;
}

bool BalEditor::ImportAudio( const std::filesystem::path& path, const char* pDestinationDirection )
{
    ( void ) pDestinationDirection;
    Mix_Music* pMusic = Mix_LoadMUS( path.string().c_str());
    if ( pMusic == nullptr )
    {
        return false;
    }
    Mix_FreeMusic( pMusic );
    return true;
}
