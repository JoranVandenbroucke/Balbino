#include "BalbinoPCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fstream>
#include "Renderer/Renderer.h"
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

	SDL_Surface* pTexture = IMG_Load( fullPath.c_str() );
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
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	GLuint texture;
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		pTexture->w,
		pTexture->h,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		pTexture->pixels
	);
	// Set texture options
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	SDL_FreeSurface( pTexture );
	return texture;
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
	// Get pixel format information and translate to OpenGl format
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
	//Generate an array of textures.  We only want one texture (one element array), so trick
	//it by treating "texture" as array of length one.
	glGenTextures( 1, &id );

	//Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
	//All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
	glBindTexture( GL_TEXTURE_2D, id );
	// check for errors. Can happen if a texture is created while a static pointer is being initialized, even before the call to the main function.
	GLenum e = glGetError();
	if( e != GL_NO_ERROR )
	{
		std::cerr << "Texture::CreateFromSurface, error binding textures, Error id = " << e << '\n';
		std::cerr << "Can happen if a texture is created before performing the initialization code (e.g. a static Texture object).\n";
		std::cerr << "There might be a white rectangle instead of the image.\n";
	}

	// Specify the texture's data.  
	// This function is a bit tricky, and it's hard to find helpful documentation. 
	// A summary:
	//    GL_TEXTURE_2D:    The currently bound 2D texture (i.e. the one we just made)
	//                0:    The mipmap level.  0, since we want to update the base level mipmap image (i.e., the image itself,
	//                         not cached smaller copies)
	//          GL_RGBA:    Specifies the number of color components in the texture.
	//                     This is how OpenGL will store the texture internally (kinda)--
	//                     It's essentially the texture's type.
	//       surface->w:    The width of the texture
	//       surface->h:    The height of the texture
	//                0:    The border.  Don't worry about this if you're just starting.
	//      pixelFormat:    The format that the *data* is in--NOT the texture! 
	// GL_UNSIGNED_BYTE:    The type the data is in.  In SDL, the data is stored as an array of bytes, with each channel
	//                         getting one byte.  This is fairly typical--it means that the image can store, for each channel,
	//                         any value that fits in one byte (so 0 through 255).  These values are to be interpreted as
	//                         *unsigned* values (since 0x00 should be dark and 0xFF should be bright).
	//  surface->pixels:    The actual data.  As above, SDL's array of bytes.
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, pSurface->w, pSurface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, pSurface->pixels );

	// Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
	// *smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
	// each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
	// further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
	// them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
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
	m_Textures.clear();
}
