#pragma once
#include "Base.h"
#include "Common.h"

#include <vulkan/vulkan.hpp>

namespace FawnVision
{
    class CImageResource;
    
    class CImageView final : public CDeviceObject
    {
    public:
        explicit CImageView( const CImageResource& pImage, EImageViewType::Enum type, uint32_t firstMip = 0, uint32_t numMips = VK_REMAINING_MIP_LEVELS, uint32_t firstLayer = 0, uint32_t numLayers = VK_REMAINING_ARRAY_LAYERS);
        
        CImageView( CImageView&& ) = default;
        CImageView& operator=( CImageView&& ) = default;
        ~CImageView() override;
        
        VkImageView GetImageView() const;
        
        void Destroy() override;
        static CImageView* CreateNew( const CImageResource& pResource, EImageViewType::Enum type, uint32_t firstMip = 0, uint32_t numMips = VK_REMAINING_MIP_LEVELS, uint32_t firstLayer = 0, uint32_t numLayers = VK_REMAINING_ARRAY_LAYERS);
    private:
        VkImageView m_imageView;
        bool        m_ownedBySwapchain;
    };
}
