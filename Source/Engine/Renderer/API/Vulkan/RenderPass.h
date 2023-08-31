//
// Created by joran on 09/10/2022.
//

#pragma once
#include "Base.h"

#include <vulkan/vulkan.hpp>

namespace DeerVulkan
{
    class CRenderPass final : public CDeviceObject
    {
    public:
        constexpr explicit CRenderPass( FawnVision::Device device )
            : CDeviceObject{device}
        {
        }
        ~CRenderPass() override;

        void Initialize( const std::vector<FawnVision::image_format>& formats, uint32_t inputCount, bool hasColor ) noexcept;
        constexpr uint32_t GetInputAttachmentCount() const
        {
            return m_inputAttachmentCount;
        }
        constexpr VkRenderPass GetHandle() const
        {
            return m_renderPass;
        }
        constexpr bool HasDepthAttachment() const
        {
            return m_hasDepthAttachment;
        }
        constexpr bool HasColorAttachments() const
        {
            return m_hasColorAttachment;
        }
        constexpr bool HasInputAttachments() const
        {
            return m_hasInputAttachment;
        }

    private:
        bool m_hasDepthAttachment {};
        bool m_hasColorAttachment {};
        bool m_hasInputAttachment {};
        uint32_t m_inputAttachmentCount {};
        VkRenderPass m_renderPass { VK_NULL_HANDLE };
    };
}// namespace DeerVulkan
