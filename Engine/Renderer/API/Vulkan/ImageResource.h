#pragma once
#include <vulkan/vulkan.hpp>
#include "Base.h"
#include "Common.h"

namespace FawnVision
{
    class CBuffer;
    class CImageResource final : public CDeviceObject
    {
    public:
        explicit CImageResource( const CDevice* pDevice );
        ~CImageResource() override;
        
        VkResult InitFromSwapchain( const VkImage& image, VkImageLayout layout, uint32_t width, uint32_t height, VkFormat format );
        VkResult Initialize( FawnVision::EImageViewType::Enum type, FawnVision::EFormat::Enum format, uint32_t width, uint32_t height, uint32_t depth, uint8_t mips, uint8_t layers, int sampleLevel, EImageUsageFlagBits::Enum usage, FawnVision::EImageLayout::Enum layout = FawnVision::EImageLayout::Undefined );
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
        
        void TransitionImageLayout( uint32_t mipLevels, uint32_t layers, CBuffer* pCommand, EImageLayout::Enum newLayout );
        void GenerateMipMaps( uint32_t mipLevels, CBuffer* pCommand );
        VkImageLayout GetLayout() const;
        
    private:
        bool     m_ownedBySwapchain;
        
        VkImage           m_image;
        VkImageLayout     m_imageLayout;
        VkImageCreateInfo m_createInfo;
        VkDeviceMemory    m_memory;
        VkFormat          m_format;
    };
}
