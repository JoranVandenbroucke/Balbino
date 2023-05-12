//
// Created by joran on 09/10/2022.
//

#ifndef GAME_RENDERPASS_H
#define GAME_RENDERPASS_H
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

#endif //GAME_RENDERPASS_H