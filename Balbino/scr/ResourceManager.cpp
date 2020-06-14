#include "BalbinoPCH.h"
#include "ResourceManager.h"
#include "Renderer/Renderer.h"
#include "Font.h"
#include "Editor/Debug.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fstream>
#include <regex>

void Balbino::ResourceManager::Init( const std::string& dataPath )
{
	m_DataPath = dataPath;

	/********************************************************************************
								setup images suport
	*********************************************************************************/

	if( ( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG ) != IMG_INIT_PNG )
	{
		throw std::runtime_error( std::string( "Failed to load support for png's: " ) + SDL_GetError() );
	}

	if( ( IMG_Init( IMG_INIT_JPG ) & IMG_INIT_JPG ) != IMG_INIT_JPG )
	{
		throw std::runtime_error( std::string( "Failed to load support for jpg's: " ) + SDL_GetError() );
	}

	if( ( IMG_Init( IMG_INIT_TIF ) & IMG_INIT_TIF ) != IMG_INIT_TIF )
	{
		throw std::runtime_error( std::string( "Failed to load support for tif's: " ) + SDL_GetError() );
	}

	if( ( IMG_Init( IMG_INIT_WEBP ) & IMG_INIT_WEBP ) != IMG_INIT_WEBP )
	{
		throw std::runtime_error( std::string( "Failed to load support for webp's: " ) + SDL_GetError() );
	}

	/********************************************************************************
								setup TTF suport
	*********************************************************************************/
	if( TTF_Init() != 0 )
	{
		throw std::runtime_error( std::string( "Failed to load support for fonts: " ) + SDL_GetError() );
	}

	/********************************************************************************
								setup Audio suport
	*********************************************************************************/
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		std::cerr << "Core::Initialize( ), error when calling Mix_OpenAudio: " << Mix_GetError() << std::endl;
		return;
	}

	if( ( Mix_Init( MIX_INIT_FLAC ) & MIX_INIT_FLAC ) != MIX_INIT_FLAC )
	{
		throw std::runtime_error( std::string( "Failed to load support for flac's: " ) + SDL_GetError() );
	}

	if( ( Mix_Init( MIX_INIT_MID ) & MIX_INIT_MID ) != MIX_INIT_MID )
	{
		throw std::runtime_error( std::string( "Failed to load support for mid's: " ) + SDL_GetError() );
	}

	if( ( Mix_Init( MIX_INIT_MOD ) & MIX_INIT_MOD ) != MIX_INIT_MOD )
	{
		throw std::runtime_error( std::string( "Failed to load support for mod's: " ) + SDL_GetError() );
	}

	if( ( Mix_Init( MIX_INIT_MP3 ) & MIX_INIT_MP3 ) != MIX_INIT_MP3 )
	{
		throw std::runtime_error( std::string( "Failed to load support for mp3's: " ) + SDL_GetError() );
	}

	if( ( Mix_Init( MIX_INIT_OGG ) & MIX_INIT_OGG ) != MIX_INIT_OGG )
	{
		throw std::runtime_error( std::string( "Failed to load support for ogg's: " ) + SDL_GetError() );
	}

	//if( ( Mix_Init( MIX_INIT_OPUS ) & MIX_INIT_OPUS ) != MIX_INIT_OPUS )
	//{
	//	throw std::runtime_error( std::string( "Failed to load support for opus's: " ) + SDL_GetError() );
	//}
}

GLuint Balbino::ResourceManager::LoadTexture( const std::string& file, vertex* vert )
{
	return Get().ILoadTexture( file, vert );
}

GLuint Balbino::ResourceManager::LoadTexture( const Font& font, const std::string& text, vertex* vert )
{
	return Get().ILoadTexture( font, text, vert );
}

GLuint Balbino::ResourceManager::CreateFromSurface( SDL_Surface* pSurface )
{
	return Get().ICreateFromSurface( pSurface );
}

Balbino::Font* Balbino::ResourceManager::LoadFont( const std::string& file, unsigned int size )
{
	return Get().ILoadFont( file, size );
}

Mix_Chunk* Balbino::ResourceManager::LoadAudio( const std::string& file )
{
	return Get().ILoadAudio( file );
}

void Balbino::ResourceManager::Fini()
{
	Get().ICleanup();
}

Balbino::ResourceManager::~ResourceManager()
{
}

GLuint Balbino::ResourceManager::ILoadTexture( const std::string& file, vertex* vert )
{
	if( file == "" ) return GLuint(-1);
	if( m_Textures.find( file ) != m_Textures.cend() )
	{
		auto fullPath = m_DataPath + file;
		SDL_Surface* pTexture = IMG_Load( fullPath.c_str() );
		if( vert )
		{
			vert[0].x = -pTexture->w / 2.f;	vert[0].y = -pTexture->h / 2.f;
			vert[1].x = -pTexture->w / 2.f;	vert[1].y = pTexture->h / 2.f;
			vert[2].x = pTexture->w / 2.f;	vert[2].y = -pTexture->h / 2.f;
			vert[3].x = pTexture->w / 2.f;	vert[3].y = pTexture->h / 2.f;
		}
		SDL_FreeSurface( pTexture );
		return m_Textures[file];
	}
	auto fullPath = m_DataPath + file;
	std::regex completePathChecker{ "^[A-Z]:.*$|^\\.\\.\\/.*$" };
	if( std::regex_match(file, completePathChecker) )
	{
		fullPath = file;
	}
	std::ifstream fileChecker{ fullPath };
	if( !fileChecker.is_open() )
	{
		throw std::runtime_error( file + "does not exist" );
	}
	fileChecker.close();
	SDL_Surface* pTexture = IMG_Load( fullPath.c_str() );
	if( pTexture == nullptr )
	{
		//throw std::runtime_error( std::string( "Failed to load texture: " ) + SDL_GetError() );
		Debug::LogError( std::string( "Failed to load texture: " ) + SDL_GetError() );
		return 0;
	}
	if( vert )
	{
		//float relativeW = pTexture->w / 640.f;
		//float relativeH = pTexture->h / 480.f;

		vert[0].x = -pTexture->w / 2.f;	vert[0].y = -pTexture->h / 2.f;
		vert[1].x = -pTexture->w / 2.f;	vert[1].y = pTexture->h / 2.f;
		vert[2].x = pTexture->w / 2.f;	vert[2].y = -pTexture->h / 2.f;
		vert[3].x = pTexture->w / 2.f;	vert[3].y = pTexture->h / 2.f;
	}
	return m_Textures[file] = CreateFromSurface( pTexture );
}

GLuint Balbino::ResourceManager::ILoadTexture( const Font& font, const std::string& text, vertex* vert )
{
	SDL_Surface* pTexture = TTF_RenderText_Blended( font.GetFont(), ( text != "" ) ? text.c_str() : " ", { 255,255,255 } );
	if( pTexture == nullptr )
	{
		throw std::runtime_error( std::string( "Failed to load texture: " ) + SDL_GetError() );
	}

	if( vert )
	{
		//float relativeW = pTexture->w / 640.f;
		//float relativeH = pTexture->h / 480.f;

		vert[0].x = -pTexture->w / 2.f;	vert[0].y = -pTexture->h / 2.f;
		vert[1].x = -pTexture->w / 2.f;	vert[1].y = pTexture->h / 2.f;
		vert[2].x = pTexture->w / 2.f;	vert[2].y = -pTexture->h / 2.f;
		vert[3].x = pTexture->w / 2.f;	vert[3].y = pTexture->h / 2.f;
	}
	return ICreateFromSurface( pTexture );
}

Balbino::Font* Balbino::ResourceManager::ILoadFont( const std::string& file, unsigned int size )
{
	if( m_Fonts.find( std::make_pair( file, size ) ) != m_Fonts.end() )
	{
		return m_Fonts[std::make_pair( file, size )];
	}
	return m_Fonts[std::make_pair( file, size )] = DBG_NEW Font{ m_DataPath + file, size };
}

Mix_Chunk* Balbino::ResourceManager::ILoadAudio( const std::string& file )
{
	if( m_Audio.find( file ) != m_Audio.end() )
	{
		return m_Audio[file];
	}
	const auto fullPath = m_DataPath + file;

	Mix_Chunk* audio = Mix_LoadWAV( fullPath.c_str() );

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

GLuint Balbino::ResourceManager::ICreateFromSurface( SDL_Surface* pSurface )
{
	GLenum pixelFormat{ GL_RGB };
	switch( pSurface->format->BytesPerPixel )
	{
	case 3:
		if( pSurface->format->Rmask == 0x000000ff )
		{
			pixelFormat = GL_RGB;
		}
		else
		{
			pixelFormat = GL_BGR;
		}
		break;
	case 4:
		if( pSurface->format->Rmask == 0x000000ff )
		{
			pixelFormat = GL_RGBA;
		}
		else
		{
			pixelFormat = GL_BGRA;
		}
		break;
	default:
		std::string error( "Texture::CreateFromSurface, unknow pixel format, BytesPerPixel: " + pSurface->format->BytesPerPixel );
		error += "\nUse 32 bit or 24 bit images.\n";
		throw error;
	}
	GLuint id{};
	glGenTextures( 1, &id );
	glBindTexture( GL_TEXTURE_2D, id );
	GLenum e = glGetError();
	if( e != GL_NO_ERROR )
	{
		std::cerr << "Texture::CreateFromSurface, error binding textures, Error id = " << e << '\n';
		std::cerr << "Can happen if a texture is created before performing the initialization code (e.g. a static Texture object).\n";
		std::cerr << "There might be a white rectangle instead of the image.\n";
	}

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, pSurface->w, pSurface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, pSurface->pixels );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	SDL_FreeSurface( pSurface );
	return id;
}

void Balbino::ResourceManager::ICleanup()
{
	for( auto& mix : m_Audio )
	{
		Mix_FreeChunk( mix.second );
	}
	for( auto& font : m_Fonts )
	{
		delete font.second;
	}
	for( auto& texture : m_Textures )
	{
		glDeleteTextures( 1, &texture.second );
	}
	m_Audio.clear();
	m_Fonts.clear();
	m_Textures.clear();
}
