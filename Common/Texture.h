#pragma once
#include "Common.h"

#include <cstdint>

#include "Buffer.h"
#include "CommandPool.h"
#include "Device.h"
#include "ImageResource.h"
#include "ImageView.h"
#include "Sampler.h"
#include "UUID.h"


namespace BalVulkan
{
	class CQueue;
	class CImageView;
	class CImageResource;
	class CSampler;
	class CDevice;
}

namespace Balbino
{
	class CTexture
	{
	public:
		explicit CTexture( const BalVulkan::CDevice* pDevice, CUuid uuid )
			: m_uuid{ uuid }
			, m_pView{ nullptr }
			, m_pResource{ nullptr }
			, m_pSampler{ nullptr }
			, m_pDevice{ pDevice }
		{
		}

		~CTexture()
		{
			m_pResource->Release();
			m_pView->Release();
			m_pSampler->Release();
			m_pDevice = nullptr;
		}

		CTexture( const CTexture& ) = delete;
		CTexture( CTexture&& ) = delete;
		CTexture& operator=( const CTexture& ) = delete;
		CTexture& operator=( CTexture&& ) = delete;

		void Initialize( const void* pData, uint32_t width, uint32_t height, uint32_t mipMaps, uint32_t layers, uint32_t faces, uint64_t size, BalVulkan::EImageLayout layout, BalVulkan::EImageViewType type, BalVulkan::EFormat format, const BalVulkan::CCommandPool* commandPool, const BalVulkan::CQueue* pQueue )
		{
			const uint64_t imageSize{ size };
			const bool hasMips{ mipMaps != 0 };
			mipMaps = hasMips ? mipMaps : static_cast<uint32_t>( std::floor( std::log2( std::max( width, width ) ) ) ) + 1;

			BalVulkan::CBuffer stagingBuffer{ m_pDevice, commandPool, pQueue };
			stagingBuffer.Initialize( imageSize, BalVulkan::EBufferUsageFlagBits::TransferSrcBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
			stagingBuffer.UpdateData( pData, size );

			m_pResource = BalVulkan::CImageResource::CreateNew( m_pDevice );
			m_pResource->Initialize( width, height, mipMaps, faces * layers, layout, format, BalVulkan::EImageUsageFlagBits::TransferDstBit | BalVulkan::EImageUsageFlagBits::SampledBit );
			m_pResource->TransitionImageLayout( mipMaps, &stagingBuffer, BalVulkan::EImageLayout::TransferDstOptimal );
			stagingBuffer.CopyBufferToImage( m_pResource );
			m_pResource->TransitionImageLayout( mipMaps, &stagingBuffer, BalVulkan::EImageLayout::ShaderReadOnlyOptimal );

			if ( !hasMips )
				m_pResource->GenerateMipMaps( mipMaps, &stagingBuffer );

			m_pView = BalVulkan::CImageView::CreateNew( *m_pResource, type, 0u, m_pResource->GetMipCount(), 0u, m_pResource->GetLayerCount() );
			m_pSampler = BalVulkan::CSampler::CreateNew( m_pDevice );
			m_pSampler->Initialize( m_pResource->GetMipCount() );
		}

		void Cleanup() const
		{
			m_pResource->Release();
			m_pView->Release();
			m_pSampler->Release();
		}

		[[nodiscard]] VkImage GetImage() const
		{
			return m_pResource->GetImage();
		}

		[[nodiscard]] VkImageView GetImageView() const
		{
			return m_pView->GetImageView();
		}

		[[nodiscard]] VkSampler GetSampler() const
		{
			return m_pSampler->GetSampler();
		}

		[[nodiscard]] VkImageLayout GetImageLayout() const
		{
			return m_pResource->GetImageLayout();
		}

	private:
		CUuid m_uuid;

		BalVulkan::CImageView* m_pView;
		BalVulkan::CImageResource* m_pResource;
		BalVulkan::CSampler* m_pSampler;
		const BalVulkan::CDevice* m_pDevice;
	};
}
