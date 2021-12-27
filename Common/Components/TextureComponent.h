#pragma once
#include <cstdint>
#include "Common.h"

#include "Device.h"
#include "DescriptorSet.h"
#include "CommandPool.h"
#include "ImageResource.h"
#include "ImageView.h"
#include "Buffer.h"
#include "Sampler.h"


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
	class CTextureComponent
	{
	public:
		explicit CTextureComponent( const BalVulkan::CDevice* pDevice )
			: m_pView{ nullptr }
			, m_pResource{ nullptr }
			, m_pSampler{ nullptr }
			, m_pDescriptorSet{ nullptr }
			, m_pDevice{ pDevice }
		{
		}

		~CTextureComponent()
		{
			m_pDescriptorSet->Release();
			m_pResource->Release();
			m_pView->Release();
			m_pSampler->Release();
			m_pDevice = nullptr;
		}
		VkImage GetImage() const
		{
			return m_pResource->GetImage();
		}

		VkImageView GetImageView() const
		{
			return m_pView->GetImageView();
		}
		CTextureComponent( const CTextureComponent& ) = delete;
		CTextureComponent( CTextureComponent&& ) = delete;
		CTextureComponent& operator=( const CTextureComponent& ) = delete;
		CTextureComponent& operator=( CTextureComponent&& ) = delete;
		void Initialize( const void* pData, uint32_t width, uint32_t height, uint32_t mipMaps, uint32_t layers, uint32_t faces, uint64_t size, BalVulkan::EImageLayout layout, BalVulkan::EImageViewType type, BalVulkan::EFormat format, const BalVulkan::CCommandPool* commandPool, const BalVulkan::CQueue* pQueue, const VkDescriptorPool descriptorPool = VK_NULL_HANDLE, const VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE )
		{
			const uint64_t imageSize{ size };
			const bool hasMips{ mipMaps != 0 };
			mipMaps = hasMips ? mipMaps : static_cast< uint32_t >( std::floor( std::log2( std::max( width, width ) ) ) ) + 1;

			BalVulkan::CBuffer stagingBuffer{ m_pDevice, commandPool, pQueue };
			stagingBuffer.Initialize( imageSize, BalVulkan::EBufferUsageFlagBits::TransferSrcBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
			stagingBuffer.UpdateData( pData, size );

			m_pResource = BalVulkan::CImageResource::CreateNew( m_pDevice );
			m_pResource->Initialize( width, height, mipMaps, faces * layers, layout, format, BalVulkan::EImageUsageFlagBits::TransferDstBit | BalVulkan::EImageUsageFlagBits::SampledBit );
			m_pResource->TransitionImageLayout( mipMaps, &stagingBuffer, BalVulkan::EImageLayout::TransferDstOptimal );
			stagingBuffer.CopyBufferToImage( m_pResource );

			if ( !hasMips )
				m_pResource->GenerateMipMaps( mipMaps, &stagingBuffer );

			m_pView = BalVulkan::CImageView::CreateNew( *m_pResource, type, 0u, m_pResource->GetMipCount(), 0u, m_pResource->GetLayerCount() );
			m_pSampler = BalVulkan::CSampler::CreateNew( m_pDevice );
			m_pSampler->Initialize( m_pResource->GetMipCount() );

			m_pDescriptorSet = BalVulkan::CDescriptorSet::CreateNew( m_pDevice );
			if ( descriptorSetLayout != VK_NULL_HANDLE && descriptorPool != VK_NULL_HANDLE )
				m_pDescriptorSet->Initialize( descriptorPool, descriptorSetLayout, m_pSampler, m_pView, m_pResource );
		}

		[[nodiscard]] VkDescriptorSet GetDescriptorSet()const
		{
			return m_pDescriptorSet->GetDescriptorSet();
		}
		void Cleanup() const
		{
			m_pResource->Release();
			m_pView->Release();
			m_pSampler->Release();
		}
	private:
		BalVulkan::CImageView* m_pView;
		BalVulkan::CImageResource* m_pResource;
		BalVulkan::CSampler* m_pSampler;
		BalVulkan::CDescriptorSet* m_pDescriptorSet;

		const BalVulkan::CDevice* m_pDevice;
	};
}