#include "Importer.h"

#include <FileParcer.h>
#include <UUID.h>

#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <fstream>

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
        BinaryReadWrite::Write(
                file,
                static_cast< uint8_t* >( pSurface->pixels ),
                static_cast< uint64_t >( pSurface->h ) * pSurface->pitch
        );
        TTF_CloseFont( pFont );
    }
    file.close();
    return true;
}

bool BalEditor::ImportAudio( const std::filesystem::path& path, const char* pDestinationDirection )
{
    (void) pDestinationDirection;
    Mix_Music* pMusic = Mix_LoadMUS( path.string().c_str());
    if ( pMusic == nullptr )
    {
        return false;
    }
    Mix_FreeMusic( pMusic );
    return true;
}
