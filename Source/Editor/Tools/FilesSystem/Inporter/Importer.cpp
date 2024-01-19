#include "Importer.h"
#include "FawnFiles.h"

#include <UUID.h>

#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <fstream>

bool FawnForge::ImportFont( const std::filesystem::path& path, const char* pDestinationDirection )
{
    TTF_Font* pFont = TTF_OpenFont( path.string().c_str(), 12 );
    if ( pFont == nullptr )
    {
        return false;
    }

    std::filesystem::path p { std::string( pDestinationDirection ) + path.filename().string() };
    p.replace_extension( ".basset" );
    std::ofstream file( p.filename(), std::ios::out | std::ios::binary );

    if ( !file.is_open() )
    {
        return false;
    }
    TTF_SetFontKerning( pFont, 0 );
    FawnFiles::BinaryStream binaryStream;
    binaryStream << CUuid {} << static_cast<uint16_t>( FawnFiles::file_type::font ) << TTF_FontLineSkip( pFont ) << TTF_GetFontHinting( pFont ) << TTF_FontHeight( pFont ) << TTF_FontAscent( pFont ) << TTF_FontDescent( pFont )
                 << TTF_FontLineSkip( pFont );

    for ( uint16_t i {}; i < 256u; ++i )
    {
        const char c[ 1 ] { static_cast<char>( i ) };
        const SDL_Surface* pSurface = TTF_RenderText_Solid( pFont, c, { 255, 255, 255, 255 } );
        if ( pSurface == nullptr )
        {
            return false;
        }
        binaryStream
        << pSurface->w
        << pSurface->h
        << pSurface->h * pSurface->pitch;
        binaryStream.Write( static_cast<const char*>( pSurface->pixels ), static_cast<uint64_t>( pSurface->h ) * pSurface->pitch );
        TTF_CloseFont( pFont );
    }
    file.close();
    return true;
}

bool FawnForge::ImportAudio( const std::filesystem::path& path, const char* pDestinationDirection )
{
    (void) pDestinationDirection;
    Mix_Music* pMusic = Mix_LoadMUS( path.string().c_str() );
    if ( pMusic == nullptr )
    {
        return false;
    }
    Mix_FreeMusic( pMusic );
    return true;
}
