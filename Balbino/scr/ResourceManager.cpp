#include "BalbinoPCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fstream>
#include "Renderer.h"
#include "Font.h"

void Balbino::ResourceManager::Init( const std::string& dataPath )
{
	m_DataPath = dataPath;

	// load support for png and jpg, this takes a while!

	if( ( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG ) != IMG_INIT_PNG )
	{
		throw std::runtime_error( std::string( "Failed to load support for png's: " ) + SDL_GetError() );
	}

	if( ( IMG_Init( IMG_INIT_JPG ) & IMG_INIT_JPG ) != IMG_INIT_JPG )
	{
		throw std::runtime_error( std::string( "Failed to load support for jpg's: " ) + SDL_GetError() );
	}

	if( TTF_Init() != 0 )
	{
		throw std::runtime_error( std::string( "Failed to load support for fonts: " ) + SDL_GetError() );
	}
}

SDL_Texture* Balbino::ResourceManager::LoadTexture( const std::string& file )
{
	return Get().ILoadTexture( file );
}

std::shared_ptr<Balbino::Font> Balbino::ResourceManager::LoadFont( const std::string& file, unsigned int size )
{
	return Get().ILoadFont( file, size );
}

std::shared_ptr<Mix_Chunk> Balbino::ResourceManager::LoadAudio( const std::string& file )
{
	return Get().ILoadAudio(file);
}

SDL_Texture* Balbino::ResourceManager::ILoadTexture( const std::string& file )
{
	if( m_Textures.find( file ) != m_Textures.cend() )
	{
		return m_Textures[file];
	}
	const auto fullPath = m_DataPath + file;
	std::ifstream fileChecker{ fullPath };
	if( !fileChecker.is_open() )
	{
		throw std::runtime_error( "file does not exist" );
	}

	auto texture = IMG_LoadTexture( Renderer::Get().GetSDLRenderer(), fullPath.c_str() );
	if( texture == nullptr )
	{
		throw std::runtime_error( std::string( "Failed to load texture: " ) + SDL_GetError() );
	}
	return m_Textures[file] = texture;
}

std::shared_ptr<Balbino::Font> Balbino::ResourceManager::ILoadFont( const std::string& file, unsigned int size )
{
	if( m_Fonts.find( std::make_pair( file, size ) ) != m_Fonts.end() )
	{
		return m_Fonts[std::make_pair( file, size )];
	}
	return m_Fonts[std::make_pair( file, size )] = std::make_shared<Font>( m_DataPath + file, size );
}

std::shared_ptr<Mix_Chunk> Balbino::ResourceManager::ILoadAudio( const std::string& file )
{
	if( m_Audio.find(file) != m_Audio.end() )
	{
		return m_Audio[file];
	}
	const auto fullPath = m_DataPath + file;

	std::shared_ptr<Mix_Chunk> audio = std::shared_ptr<Mix_Chunk>( Mix_LoadWAV( fullPath.c_str() ) );

	if( !audio )
	{
		std::string errorMsg = "SoundEffect: Failed to load ";
		errorMsg += fullPath;
		errorMsg += ",\nSDL_mixer Error: ";
		errorMsg += Mix_GetError();
		std::cerr << errorMsg;
		throw std::runtime_error{ errorMsg };
	}
	return m_Audio[file] = audio;
}
