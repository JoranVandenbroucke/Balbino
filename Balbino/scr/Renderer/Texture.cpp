#include "pch.h"
#include "Texture.h"

#include <CommandPool.h>
#include <Device.h>
#include <ImageResource.h>
#include <ImageView.h>
#include <Sampler.h>

#include "Buffer.h"

Balbino::CTexture::CTexture( const BalVulkan::CDevice* pDevice )
	: m_pView{ nullptr }
	, m_pResource{ nullptr }
	, m_pSampler{ nullptr }
	, m_pDevice{ pDevice }
{
}

Balbino::CTexture::~CTexture()
{
	delete m_pResource;
	delete m_pView;
	delete m_pSampler;
	m_pDevice = nullptr;
}

void Balbino::CTexture::Initialize( const void* pData, const uint32_t width, const uint32_t height, uint32_t mipMaps, const uint32_t layers, const uint32_t faces, const uint32_t size, const BalVulkan::EImageLayout layout, const BalVulkan::EImageViewType type, const BalVulkan::EFormat format, const BalVulkan::CCommandPool& commandPool, const BalVulkan::CQueue* pQueue )
{
	const uint64_t imageSize{ size };
	const bool hasMips{ mipMaps != 0 };
	mipMaps = hasMips ? mipMaps : static_cast< uint32_t >( std::floor( std::log2( std::max( width, width ) ) ) ) + 1;

	BalVulkan::CBuffer stagingBuffer{ m_pDevice, &commandPool, pQueue };
	stagingBuffer.Initialize( imageSize, BalVulkan::EBufferUsageFlagBits::TransferSrcBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
	stagingBuffer.UpdateData( pData, size );

	m_pResource = BalVulkan::CImageResource::CreateNew( m_pDevice );
	m_pResource->Initialize( width, height, mipMaps, faces * layers, layout, format);
	m_pResource->TransitionImageLayout( mipMaps, &stagingBuffer, BalVulkan::EImageLayout::TransferDstOptimal );
	stagingBuffer.CopyBufferToImage( m_pResource );

	if ( !hasMips )
		m_pResource->GenerateMipMaps( mipMaps, &stagingBuffer );

	m_pView = BalVulkan::CImageView::CreateNew( *m_pResource, type, 0u, m_pResource->GetMipCount(), 0u, m_pResource->GetLayerCount() );
	m_pSampler = BalVulkan::CSampler::CreateNew( m_pDevice );
	m_pSampler->Initialize( m_pResource->GetMipCount() );
}

void Balbino::CTexture::Cleanup() const
{
	m_pResource->Release();
	m_pView->Release();
	m_pSampler->Release();
}
