#pragma once
#include "Base.h"
#include "Vulkan/Vulkan.hpp"

namespace DeerVulkan
{
    class CFrameBuffer final : public CDeviceObject
    {
    public:
        constexpr explicit CFrameBuffer( FawnVision::Device device )
            : CDeviceObject { device }
        {
        }

        ~CFrameBuffer() override;

        //the framebuffer assumes that rendering is forward
        void Initialize( FawnVision::RenderPass RenderPass, uint32_t width, uint32_t height, const std::vector<FawnVision::ImageView>& renderTargets, FawnVision::ImageView Depth );
        constexpr VkFramebuffer GetFrameBuffer( uint32_t idx ) const
        {
            return m_frameBuffer[ idx ];
        }

    private:
        std::vector<VkFramebuffer> m_frameBuffer { VK_NULL_HANDLE };
    };
}// namespace DeerVulkan
