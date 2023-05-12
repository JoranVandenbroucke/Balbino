#pragma once
#include "Base.h"
#include "Common.h"
#include "ImageResource.h"

#include <vulkan/vulkan.hpp>

namespace FawnVision
{
    class CImageView final : public CDeviceObject
    {
    public:
        explicit CImageView( const CImageResource& pImage, EImageViewType::Enum type, uint32_t firstMip = 0, uint32_t numMips = VK_REMAINING_MIP_LEVELS, uint32_t firstLayer = 0, uint32_t numLayers = VK_REMAINING_ARRAY_LAYERS);
        ~CImageView() override;
        
        VkImageView GetImageView() const;
        
    private:
        VkImageView m_imageView;
        bool        m_ownedBySwapchain;
    };
}
