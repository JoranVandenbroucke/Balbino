#include "BalbinoPCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fstream>
#include "Renderer/Renderer.h"
#include "Font.h"
#include "Debug.h"

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

	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		std::cerr << "Core::Initialize( ), error when calling Mix_OpenAudio: " << Mix_GetError() << std::endl;
		return;
	}
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

std::shared_ptr<Balbino::Font> Balbino::ResourceManager::LoadFont( const std::string& file, unsigned int size )
{
	return Get().ILoadFont( file, size );
}

std::shared_ptr<Mix_Chunk> Balbino::ResourceManager::LoadAudio( const std::string& file )
{
	return Get().ILoadAudio( file );
}

void Balbino::ResourceManager::Cleanup()
{
	Get().ICleanup();
}

Balbino::ResourceManager::~ResourceManager()
{
}

GLuint Balbino::ResourceManager::ILoadTexture( const std::string& file, vertex* vert )
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
	fileChecker.close();
	SDL_Surface* pTexture = IMG_Load( fullPath.c_str() );
	if( pTexture == nullptr )
	{
		//throw std::runtime_error( std::string( "Failed to load texture: " ) + SDL_GetError() );
		Debug::LogError( std::string( "Failed to load texture: " ) + SDL_GetError() );
		return 0;
	}
	float relativeW = pTexture->w / 640.f;
	float relativeH = pTexture->h / 480.f;

	vert[0].x = -relativeW;	vert[0].y = -relativeH;
	vert[1].x = -relativeW;	vert[1].y = relativeH;
	vert[2].x = relativeW;	vert[2].y = -relativeH;
	vert[3].x = relativeW;	vert[3].y = relativeH;
	return m_Textures[file] = CreateFromSurface( pTexture );
}

GLuint Balbino::ResourceManager::ILoadTexture( const Font& font, const std::string& text, vertex* vert )
{
	SDL_Surface* pTexture = TTF_RenderText_Blended( font.GetFont(), ( text != "" ) ? text.c_str() : " ", { 255,255,255 } );
	if( pTexture == nullptr )
	{
		throw std::runtime_error( std::string( "Failed to load texture: " ) + SDL_GetError() );
	}

	float relativeW = pTexture->w / 640.f;
	float relativeH = pTexture->h / 480.f;

	vert[0].x = -relativeW;	vert[0].y = -relativeH;
	vert[1].x = -relativeW;	vert[1].y = relativeH;
	vert[2].x = relativeW;	vert[2].y = -relativeH;
	vert[3].x = relativeW;	vert[3].y = relativeH;

	return ICreateFromSurface(pTexture);
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
	if( m_Audio.find( file ) != m_Audio.end() )
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
		Mix_FreeChunk( mix.second.get() );
		//mix.second = nullptr;
	}
	//m_Audio.clear();
	m_Fonts.clear();
	for( auto& texture : m_Textures )
	{
		glDeleteTextures( 1, &texture.second );
	}
	m_Textures.clear();
}
