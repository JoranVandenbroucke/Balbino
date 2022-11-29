//
// Created by joran on 09/10/2022.
//

#ifndef GAME_RENDERPASS_H
#define GAME_RENDERPASS_H
#include "Base.h"
#include "Common.h"

#include <vulkan/vulkan.hpp>

namespace BalVulkan
{
    class CRenderPass final : public CDeviceObject
    {
    public:
        explicit CRenderPass( const CDevice* device );
        ~CRenderPass() override;
        CRenderPass( const CRenderPass& ) = delete;
        CRenderPass( CRenderPass&& ) = delete;
        CRenderPass& operator=( const CRenderPass& ) = delete;
        CRenderPass operator=( CRenderPass&& ) = delete;
        
        void Initialize( std::vector<BalVulkan::EFormat::Enum> formats, uint32_t inputCount, bool hasColor );
        VkRenderPass GetRenderPass() const;
        static CRenderPass* CreateNew( const CDevice* pDevice );
        
        bool HasDepthAttachment() const;
        bool HasColorAttachments() const;
        bool HasInputAttachments() const;
        uint32_t GetInputAttachmentCount() const;
    private:
        bool         m_hasDepthAttachment;
        bool         m_hasColorAttachment;
        bool         m_hasInputAttachment;
        uint32_t     m_inputAttachmentCount;
        VkRenderPass m_renderPass;
    };
} // BalVulkan

#endif //GAME_RENDERPASS_H