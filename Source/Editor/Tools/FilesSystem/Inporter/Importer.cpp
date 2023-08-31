#include "Importer.h"
#include "Serializer.h"

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
    Serialization::Write( file, CUuid() );
    Serialization::Write( file, static_cast<uint8_t>( file_type::font ) );
    Serialization::Write( file, TTF_FontLineSkip( pFont ) );
    Serialization::Write( file, TTF_GetFontHinting( pFont ) );
    Serialization::Write( file, TTF_FontHeight( pFont ) );
    Serialization::Write( file, TTF_FontAscent( pFont ) );
    Serialization::Write( file, TTF_FontDescent( pFont ) );
    Serialization::Write( file, TTF_FontLineSkip( pFont ) );

    for ( uint16_t i {}; i < 256u; ++i )
    {
        const char c[ 1 ] { static_cast<char>( i ) };
        SDL_Surface* pSurface = TTF_RenderText_Solid( pFont, c, { 255, 255, 255, 255 } );
        if ( pSurface == nullptr )
        {
            return false;
        }
        Serialization::Write( file, pSurface->w );
        Serialization::Write( file, pSurface->h );
        Serialization::Write( file, pSurface->h * pSurface->pitch );
        Serialization::Write( file, static_cast<uint8_t*>( pSurface->pixels ), static_cast<uint64_t>( pSurface->h ) * pSurface->pitch );
        TTF_CloseFont( pFont );
    }
    file.close();
    return true;
}

bool FawnForge::ImportAudio( const std::filesystem::path& path, const char* pDestinationDirection )
{
    (void)pDestinationDirection;
    Mix_Music* pMusic = Mix_LoadMUS( path.string().c_str() );
    if ( pMusic == nullptr )
    {
        return false;
    }
    Mix_FreeMusic( pMusic );
    return true;
}
