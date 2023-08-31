#pragma once
#include "Base.h"
#include <vulkan/vulkan.hpp>

namespace DeerVulkan
{
    class CImageResource final : public CDeviceObject
    {
    public:
        constexpr explicit CImageResource( FawnVision::Device device )
            : CDeviceObject { device }
        {
        }
        ~CImageResource() override;

        VkResult InitFromSwapchain( const VkImage& image, VkImageLayout layout, uint32_t width, uint32_t height, VkFormat format );
        void Initialize( const FawnVision::SImageCreateInfo& createInfo );

        void TransitionImageLayout( uint32_t mipLevels, uint32_t layers, FawnVision::Buffer Command, FawnVision::image_layout newLayout ) noexcept;
        void GenerateMipMaps( uint32_t mipLevels, FawnVision::Buffer Command );

        constexpr VkImage GetImage() noexcept{return m_image;}
        constexpr const VkImage& GetImage() const{return m_image;}
        constexpr VkImageLayout GetImageLayout(){return m_imageLayout;}
        constexpr const VkImageLayout& GetImageLayout() const{return m_imageLayout;}
        constexpr const VkImageCreateInfo& GetImageCreateInfo() const{return m_createInfo;}
        constexpr VkImageType GetDimensionality() const{return m_createInfo.imageType;}
        constexpr uint32_t GetMipCount() const{return m_createInfo.mipLevels;}
        constexpr uint32_t GetLayerCount() const{return m_createInfo.arrayLayers;}
        constexpr uint32_t GetWidth() const{return m_createInfo.extent.width;}
        constexpr uint32_t GetHeight() const{return m_createInfo.extent.height;}
        constexpr uint32_t GetDepth() const{return m_createInfo.extent.depth;}
        constexpr uint8_t GetFormat() const{return m_format;}
    private:
        VkImage m_image { VK_NULL_HANDLE };
        VkDeviceMemory m_memory { VK_NULL_HANDLE };
        VkImageLayout m_imageLayout {};
        VkImageCreateInfo m_createInfo {};
        uint8_t m_format {};

        bool m_ownedBySwapchain {};
    };
}// namespace DeerVulkan
