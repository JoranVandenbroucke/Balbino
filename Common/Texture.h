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
                : m_uuid{ uuid },
                  m_pView{ nullptr },
                  m_pResource{ nullptr },
                  m_pSampler{ nullptr },
                  m_pDevice{ pDevice }
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
    
        [[nodiscard]] uint64_t GetID() const
        {
            return (uint64_t) m_uuid;
        }
    
        void Initialize( BalVulkan::EImageViewType::Enum type, BalVulkan::EFormat::Enum format, uint8_t mips, uint8_t layers, int anisotropy, int sampleLevel, int mipmapMode, int filterMode, BalVulkan::ESamplerAddressMode::Enum sampleU, BalVulkan::ESamplerAddressMode::Enum sampleV, BalVulkan::ESamplerAddressMode::Enum sampleW, uint32_t width, uint32_t height, uint32_t depth, uint8_t pitch, void* pData, BalVulkan::CCommandPool* pPool, BalVulkan::CQueue* pQueue )
        {
            const uint64_t imageSize{ width * height * depth * pitch };
            bool           needsMipGenerating{ sampleLevel == 1 && mips == (uint8_t) -1 };
            mips = needsMipGenerating ? static_cast<uint8_t>( std::floor(
                    std::log2( std::max( width, height )))) + 1 : ( sampleLevel ) ? 1u : mips;
        
            BalVulkan::CBuffer stagingBuffer{ m_pDevice, pPool, pQueue };
            stagingBuffer.Initialize( imageSize, BalVulkan::EBufferUsageFlagBits::TransferSrcBit,
                                      BalVulkan::EMemoryPropertyFlagBits::Enum(
                                              BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit ));
            stagingBuffer.UpdateData( pData, imageSize );
        
            m_pResource = BalVulkan::CImageResource::CreateNew( m_pDevice );
            m_pResource->Initialize( type, format, width, height, depth, mips, layers, sampleLevel,
                                     BalVulkan::EImageUsageFlagBits::Enum(
                                             BalVulkan::EImageUsageFlagBits::TransferDstBit | BalVulkan::EImageUsageFlagBits::SampledBit ));
            m_pResource->TransitionImageLayout( mips, &stagingBuffer, BalVulkan::EImageLayout::TransferDstOptimal );
            stagingBuffer.CopyBufferToImage( m_pResource );
            if ( needsMipGenerating )
            {
                m_pResource->GenerateMipMaps( mips, &stagingBuffer );
            }
            else
            {
                m_pResource->TransitionImageLayout( mips, &stagingBuffer,
                                                    BalVulkan::EImageLayout::ShaderReadOnlyOptimal );
            }
            
            m_pView    = BalVulkan::CImageView::CreateNew( *m_pResource, type, 0u, m_pResource->GetMipCount(), 0u,
                                                           m_pResource->GetLayerCount());
            m_pSampler = BalVulkan::CSampler::CreateNew( m_pDevice );
            m_pSampler->Initialize( filterMode, mipmapMode, sampleU, sampleV, sampleW, anisotropy,
                                    m_pResource->GetMipCount());
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
        
        [[nodiscard]] BalVulkan::CImageView* GetImageViewObject() const
        {
            return m_pView;
        }
        
        [[nodiscard]] BalVulkan::CSampler* GetSamplerObject() const
        {
            return m_pSampler;
        }
        
        [[nodiscard]] VkImageLayout GetImageLayout() const
        {
            return m_pResource->GetImageLayout();
        }
    
    private:
        CUuid m_uuid;
        
        BalVulkan::CImageView    * m_pView;
        BalVulkan::CImageResource* m_pResource;
        BalVulkan::CSampler      * m_pSampler;
        const BalVulkan::CDevice * m_pDevice;
    };
}
