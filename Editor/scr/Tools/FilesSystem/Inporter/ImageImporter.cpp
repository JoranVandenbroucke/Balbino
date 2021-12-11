#include "pch.h"
#include "ImageImporter.h"

#include <fstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <gli/gli.hpp>

#include "Common.h"
#include "../FIleSystem.h"

bool BalEditor::ImportImage( const std::filesystem::path& path )
{
	std::filesystem::path p{ std::string("../Data/") + path.filename().string()};
	p.replace_extension( ".basset" );
	std::ofstream file( p.string().c_str(), std::ios::out | std::ios::binary );
	if ( !file.is_open() )
		return false;

	std::string extenstion = path.extension().string();
	std::ranges::for_each( extenstion, []( char& c )
	{
		c = static_cast<char>( toupper( c ) );
	} );

	if ( extenstion == ".KTX" || extenstion == ".DDS" )
	{
		gli::texture_cube texture{ gli::load( path.string().c_str() ) };
		if ( texture.empty() )
			return false;

		BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Image ) );
		BinaryReadWrite::Write( file, texture.extent().x );
		BinaryReadWrite::Write( file, texture.extent().y );
		BinaryReadWrite::Write( file, texture.base_face() );
		BinaryReadWrite::Write( file, texture.base_layer() );
		BinaryReadWrite::Write( file, texture.base_level() );
		BinaryReadWrite::Write( file, texture.faces() );
		BinaryReadWrite::Write( file, texture.layers() );
		BinaryReadWrite::Write( file, texture.levels() );
		BinaryReadWrite::Write( file, texture.format() );
		BinaryReadWrite::Write( file, texture.size() );
		BinaryReadWrite::Write( file, static_cast<uint8_t*>( texture.data() ), texture.size() );
	}
	else
	{
		SDL_Surface* pSurface{ IMG_Load( path.string().c_str() ) };
		if ( pSurface == nullptr )
		{
			return false;
		}

		if ( pSurface->pitch / pSurface->w != 4 )
			pSurface = SDL_ConvertSurfaceFormat( pSurface, SDL_PIXELFORMAT_RGBA8888, 0 );

		BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Image ) );
		BinaryReadWrite::Write( file, pSurface->w );
		BinaryReadWrite::Write( file, pSurface->h );
		BinaryReadWrite::Write( file, 0 );
		BinaryReadWrite::Write( file, 0 );
		BinaryReadWrite::Write( file, 0 );
		BinaryReadWrite::Write( file, 0 );
		BinaryReadWrite::Write( file, 0 );
		BinaryReadWrite::Write( file, 0 );
		BinaryReadWrite::Write( file, static_cast<uint32_t>( BalVulkan::EFormat::R8G8B8A8Srgb ) );
		BinaryReadWrite::Write( file, pSurface->h * pSurface->pitch );
		BinaryReadWrite::Write( file, static_cast<uint8_t*>( pSurface->pixels ), static_cast<uint64_t>(pSurface->h) * pSurface->pitch );
		SDL_FreeSurface( pSurface );
	}
	file.close();
	return true;
}

bool BalEditor::ImportFont( const std::filesystem::path& path )
{
	TTF_Font* pFont = TTF_OpenFont( path.string().c_str(), 12 );
	if ( pFont == nullptr )
		return false;

	std::filesystem::path p{ std::string( "../Data/" ) + path.filename().string() };
	p.replace_extension( ".basset" );
	std::ofstream file( p.filename(), std::ios::out | std::ios::binary );

	if ( !file.is_open() )
		return false;
	TTF_SetFontKerning( pFont, 0 );
	BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Font ) );
	BinaryReadWrite::Write( file, TTF_FontLineSkip( pFont ) );
	BinaryReadWrite::Write( file, TTF_GetFontHinting( pFont ) );
	BinaryReadWrite::Write( file, TTF_FontHeight( pFont ) );
	BinaryReadWrite::Write( file, TTF_FontAscent( pFont ) );
	BinaryReadWrite::Write( file, TTF_FontDescent( pFont ) );
	BinaryReadWrite::Write( file, TTF_FontLineSkip( pFont ) );

	for ( uint16_t i{}; i < 256u; ++i )
	{
		const char c[1]{ static_cast<char>( i ) };
		SDL_Surface* pSurface = TTF_RenderText_Solid( pFont, c, { 255,255,255,255 } );
		if ( pSurface == nullptr )
			return false;
		BinaryReadWrite::Write( file, pSurface->w );
		BinaryReadWrite::Write( file, pSurface->h );
		BinaryReadWrite::Write( file, pSurface->h * pSurface->pitch );
		BinaryReadWrite::Write( file,static_cast<uint8_t*>( pSurface->pixels ), static_cast<uint64_t>(pSurface->h) * pSurface->pitch );
		TTF_CloseFont( pFont );
	}

	return true;
}

bool BalEditor::ImportAudio( const std::filesystem::path& path )
{
	Mix_Music* pMusic = Mix_LoadMUS( path.string().c_str() );
	if ( pMusic == nullptr )
		return false;
	Mix_FreeMusic( pMusic );
	return true;
}

bool BalEditor::ImportMesh( const std::filesystem::path& path )
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( path.string(), static_cast<unsigned>( aiProcess_Triangulate | aiProcess_RemoveRedundantMaterials | aiProcess_FlipUVs | aiProcess_GenBoundingBoxes | aiProcess_ValidateDataStructure ) );
	
	// If the import failed, report it
	if ( !scene )
	{
		return false;
	}
	return true;
}
