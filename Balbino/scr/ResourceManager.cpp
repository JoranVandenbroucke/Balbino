#include "BalbinoPCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fstream>
#include "Renderer.h"
#include "Font.h"

void Balbino::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	// load support for png and jpg, this takes a while!

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) 
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG) 
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	if (TTF_Init() != 0) 
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

SDL_Texture* Balbino::ResourceManager::LoadTexture(const std::string& file)
{
	return Get().ILoadTexture(file);
}

std::shared_ptr<Balbino::Font> Balbino::ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	return Get().ILoadFont( file, size );
}

SDL_Texture* Balbino::ResourceManager::ILoadTexture( const std::string& file ) const
{
	const auto fullPath = m_DataPath + file;
	std::ifstream fileChecker{ fullPath };
	if( !fileChecker.is_open() )
	{
		throw std::runtime_error("file does not exist");
	}

	auto texture = IMG_LoadTexture(Renderer::Get().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	return texture;
}

std::shared_ptr<Balbino::Font> Balbino::ResourceManager::ILoadFont( const std::string& file, unsigned int size ) const
{
	return std::make_shared<Font>( m_DataPath + file, size );
}
