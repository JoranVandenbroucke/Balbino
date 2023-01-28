//
// Created by joran on 09/10/2022.
//

#include "RenderPass.h"
#include "Device.h"
#include "Instance.h"
#include "Funtions.h"

namespace BalVulkan
{
    CRenderPass::CRenderPass( const CDevice* device )
            : CDeviceObject( device )
              , m_renderPass{ VK_NULL_HANDLE }
              , m_hasDepthAttachment{}
              , m_hasColorAttachment{}
              , m_hasInputAttachment{}
    {
    }
    void CRenderPass::Initialize( std::vector<BalVulkan::EFormat::Enum> formats, uint32_t inputCount, bool hasColor )
    {
        const uint32_t descriptionCount{ (uint32_t) formats.size() };
        m_hasInputAttachment   = inputCount != 0;
        m_hasColorAttachment   = hasColor != 0;
        m_hasDepthAttachment   = descriptionCount > ( inputCount + ( hasColor ? 1 : 0 ));
        m_inputAttachmentCount = inputCount;
        
        std::vector<VkAttachmentReference>   inputAttachmentRef( inputCount );
        std::vector<VkAttachmentDescription> attachmentDescs( formats.size());
        
        for ( uint32_t i{}; i < attachmentDescs.size(); ++i )
        {
            attachmentDescs[i].format         = (VkFormat) formats[i];
            attachmentDescs[i].samples        = VK_SAMPLE_COUNT_1_BIT;
            attachmentDescs[i].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachmentDescs[i].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescs[i].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachmentDescs[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachmentDescs[i].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
            attachmentDescs[i].finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            
            if ( i < inputCount )
            {
                inputAttachmentRef[i].attachment = i;
                inputAttachmentRef[i].layout     = ( formats[i] == EFormat::D16Unorm || formats[i] == EFormat::D32Sfloat )
                                                   ? VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
                                                   : ( formats[i] == EFormat::D16Unorms8Uint || formats[i] == EFormat::D24Unorms8Uint || formats[i] == EFormat::D32Sfloats8Uint )
                                                     ? VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL
                                                     : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            }
//            if ( m_hasDepthAttachment && i == attachmentDescs.size() - 1 )
//            {
//                attachmentDescs[i].samples = GetDevice()->GetPhysicalDeviceInfo()->GetMaxUsableSampleCount();
//            }
        }
        
        const VkAttachmentReference colorAttachmentRef{
                .attachment = (uint32_t) inputAttachmentRef.size(), .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        };
        const VkAttachmentReference depthAttachmentRef{
                .attachment = (uint32_t) inputAttachmentRef.size() + ( hasColor
                                                                       ? 1u
                                                                       : 0u ), .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        };
        
        const VkSubpassDescription surpass{
                .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS, .inputAttachmentCount = inputCount, .pInputAttachments = inputCount
                                                                                                                               ? inputAttachmentRef.data()
                                                                                                                               : nullptr, .colorAttachmentCount = hasColor
                                                                                                                                                                  ? 1u
                                                                                                                                                                  : 0u, .pColorAttachments = hasColor
                                                                                                                                                                                             ? &colorAttachmentRef
                                                                                                                                                                                             : nullptr, .pResolveAttachments = nullptr,     //todo support?
                .pDepthStencilAttachment = m_hasDepthAttachment
                                           ? &depthAttachmentRef
                                           : nullptr, .preserveAttachmentCount = 0,       //todo support?
                .pPreserveAttachments = nullptr,    //todo support?
        };
        
        const VkSubpassDependency dependency{
                .srcSubpass = VK_SUBPASS_EXTERNAL, .dstSubpass = 0, .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, .srcAccessMask = 0, .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        };
        
        const VkRenderPassCreateInfo renderPassInfo{
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, .attachmentCount = static_cast<uint32_t>( attachmentDescs.size()), .pAttachments = attachmentDescs.data(), .subpassCount = 1, .pSubpasses = &surpass, .dependencyCount = 1, .pDependencies = &dependency,
        };
        CheckVkResult(
                vkCreateRenderPass( GetDevice()->GetVkDevice(), &renderPassInfo, nullptr, &m_renderPass ),
                "failed to create render pass!"
        );
    }
    VkRenderPass CRenderPass::GetRenderPass() const
    {
        return m_renderPass;
    }
    CRenderPass* CRenderPass::CreateNew( const CDevice* pDevice )
    {
        return new CRenderPass{ pDevice };
    }
    bool CRenderPass::HasDepthAttachment() const
    {
        return m_hasDepthAttachment;
    }
    bool CRenderPass::HasColorAttachments() const
    {
        return m_hasColorAttachment;
    }
    bool CRenderPass::HasInputAttachments() const
    {
        return m_hasInputAttachment;
    }
    uint32_t CRenderPass::GetInputAttachmentCount() const
    {
        return m_inputAttachmentCount;
    }
    CRenderPass::~CRenderPass()
    {
        vkDestroyRenderPass( GetDevice()->GetVkDevice(), m_renderPass, nullptr );
        
    }
} // BalVulkan