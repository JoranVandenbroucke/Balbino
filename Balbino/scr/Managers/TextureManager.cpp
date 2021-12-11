#include "pch.h"
#include "TextureManager.h"


#include "Common.h"
#include "../Renderer/Texture.h"

void Balbino::CTextureManager::SetRenderData( const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue )
{
	m_pDevice = pDevice;
	m_pCommandPool = pCommandPool;
	m_pQueue = pQueue;
}

void Balbino::CTextureManager::Initialize()
{
}

void Balbino::CTextureManager::Cleanup()
{
	for ( const auto& texture : m_textures )
	{
		texture.second->Cleanup();
		delete texture.second;
	}
	m_textures.clear();
}

Balbino::CTextureManager::CTextureManager()
	: m_pDevice{ nullptr }
	, m_pCommandPool{ nullptr }
	, m_pQueue{ nullptr }
{
}

Balbino::CTextureManager::~CTextureManager()
{
	if ( !m_textures.empty() )
		std::cerr << "Texture Manager not cleared" << std::endl;
};

Balbino::CTexture* Balbino::CTextureManager::AddTexture( const char* filePath )
{
	const std::filesystem::path path( std::filesystem::absolute( filePath ) );
	const uint32_t hash = static_cast< uint32_t >( std::hash<std::string>{}( path.relative_path().string() ) );
	if ( m_textures.find( hash ) != m_textures.end() )
		return m_textures.at( hash );

	std::ifstream fileChecker{ path };
	if ( !fileChecker.is_open() )
	{
		throw std::runtime_error( path.string() + " does not exist" );
	}
	fileChecker.close();

	void* pData;
	uint32_t width, height, mipmaps, layers, faces, size;
	const BalVulkan::EImageLayout layout = BalVulkan::EImageLayout::ColorAttachmentOptimal;
	BalVulkan::EImageViewType type = BalVulkan::EImageViewType::View2D;
	BalVulkan::EFormat format = BalVulkan::EFormat::R8G8B8A8Srgb;
	if ( path.string().find( ".dds" ) != std::string::npos )
	{
		gli::texture_cube texture{ gli::load( path.string().c_str() ) };
		if ( texture.empty() )
			throw std::runtime_error( std::string( "Failed to load texture" ) );

		width = texture.extent().x;
		height = texture.extent().y;
		mipmaps = static_cast<uint32_t>( texture.max_level() );
		faces = static_cast<uint32_t>( texture.max_face() );
		layers = static_cast<uint32_t>( texture.max_layer() );
		size = static_cast<uint32_t>( texture.size() );
		pData = texture.data();
		switch( texture.format() )
		{
			case gli::format::FORMAT_RGBA8_UNORM_PACK8:
				format = BalVulkan::EFormat::R8G8B8A8Unorm;
				break;
			case gli::format::FORMAT_R8_UNORM_PACK8:
				format = BalVulkan::EFormat::R8Unorm;
				break;
			case gli::format::FORMAT_R8_SRGB_PACK8:
				format = BalVulkan::EFormat::R8Srgb;
				break;
		}
		type = ( size / height / width ) > 1 ? BalVulkan::EImageViewType::Cube : type;
	}
	else
	{
		SDL_Surface* pSurface{ IMG_Load( path.string().c_str() ) };
		if ( pSurface == nullptr )
		{
			throw std::runtime_error( std::string( "Failed to load texture: " ) + SDL_GetError() );
		}
		if ( pSurface->pitch / pSurface->w != 4 )
			pSurface = SDL_ConvertSurfaceFormat( pSurface, SDL_PIXELFORMAT_RGBA32, 0 );

		width = pSurface->w;
		height = pSurface->h;
		mipmaps = layers = faces = 1;
		size = pSurface->pitch * height;
		pData = pSurface->pixels;
	}
	m_textures[hash] = DBG_NEW Balbino::CTexture{ m_pDevice };

	m_textures[hash]->Initialize( pData, width, height, mipmaps, layers, faces, size, layout, type, format, *m_pCommandPool, m_pQueue );
	return m_textures[hash];
}
