#pragma once
#include "Base.h"
#include "ImageResource.h"

#include <vulkan/vulkan.hpp>

namespace DeerVulkan
{
    class CImageView final : public CDeviceObject
    {
    public:
        constexpr explicit CImageView( FawnVision::Device device )
            : CDeviceObject { device }
        {
        }
        ~CImageView() override;
        void Initialize( const FawnVision::SImageViewCreateInfo& createInfo, FawnVision::ImageResource pImage );
        constexpr VkImageView GetImageView() const
        {
            return m_imageView;
        }

    private:
        VkImageView m_imageView { VK_NULL_HANDLE };
    };
}// namespace DeerVulkan
