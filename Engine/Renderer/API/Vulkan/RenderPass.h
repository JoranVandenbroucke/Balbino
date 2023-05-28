//
// Created by joran on 09/10/2022.
//

#pragma once
#include "Base.h"
#include "Common.h"

#include <vulkan/vulkan.hpp>

namespace FawnVision
{
    class CRenderPass final : public CDeviceObject
    {
    public:
        explicit CRenderPass( const CDevice* pDevice );
        ~CRenderPass() override;
        
        void Initialize( std::vector<FawnVision::EFormat::Enum> formats, uint32_t inputCount, bool hasColor );
        VkRenderPass GetHandle() const;
        
        bool HasDepthAttachment() const;
        bool HasColorAttachments() const;
        bool HasInputAttachments() const;
        uint32_t GetInputAttachmentCount() const;
    private:
        bool         m_hasDepthAttachment{};
        bool         m_hasColorAttachment{};
        bool         m_hasInputAttachment{};
        uint32_t     m_inputAttachmentCount{};
        VkRenderPass m_renderPass{VK_NULL_HANDLE};
    };
} // FawnVision

