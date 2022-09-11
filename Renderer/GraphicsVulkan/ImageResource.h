#pragma once

#include <vulkan/vulkan.hpp>
#include "Base.h"
#include "Common.h"

namespace BalVulkan
{
    class CBuffer;

    class CImageResource final : public CDeviceObject
    {
    public:
        explicit CImageResource( const CDevice* device );

        CImageResource( CImageResource&& ) = default;

        CImageResource& operator =( CImageResource&& ) = default;

        ~CImageResource() override;

        VkResult InitFromSwapchain( VkImage image, VkImageLayout layout, uint32_t width, uint32_t height, VkFormat format );

        VkResult Initialize( BalVulkan::EImageViewType type, BalVulkan::EFormat format, uint32_t width, uint32_t height, uint32_t depth, uint8_t mips, uint8_t layers, int sampleLevel, EImageUsageFlagBits usage, BalVulkan::EImageLayout layout = BalVulkan::EImageLayout::Undefined );

        VkImage GetImage();

        const VkImage& GetImage() const;

        const VkImageLayout& GetImageLayout() const;

        const VkImageCreateInfo& GetImageCreateInfo() const;

        VkImageType GetDimensionality() const;

        VkFormat GetFormat() const;

        uint32_t GetMipCount() const;

        uint32_t GetLayerCount() const;

        uint32_t GetWidth() const;

        uint32_t GetHeight() const;

        uint32_t GetDepth() const;

        void TransitionImageLayout( uint32_t mipLevels, const CBuffer* pCommand, EImageLayout newLayout );

        void GenerateMipMaps( uint32_t mipLevels, const CBuffer* pCommand );

        VkImageLayout GetLayout() const;

        static CImageResource* CreateNew( const CDevice* pDevice );

        [[maybe_unused]] static CImageResource* LoadFromFile( const std::string& path );

    private:
        bool     m_ownedBySwapchain;
        uint32_t m_mipLevels;

        VkImage           m_image;
        VkImageLayout     m_imageLayout;
        VkImageCreateInfo m_createInfo;
        VkDeviceMemory    m_memory;
        VkFormat          m_format;
    };
}
