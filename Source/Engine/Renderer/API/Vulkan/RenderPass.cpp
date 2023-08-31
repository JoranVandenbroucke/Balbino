//
// Created by joran on 09/10/2022.
//

#include "RenderPass.h"
#include "Device.h"


namespace DeerVulkan
{

    void CRenderPass::Initialize( const std::vector<FawnVision::image_format>& formats, uint32_t inputCount, bool hasColor ) noexcept
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
            attachmentDescs[i].format         = (VkFormat) ConvertImageFormat(formats[i]);
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
                inputAttachmentRef[i].layout     = ( formats[i] == FawnVision::image_format::d16_unorm || formats[i] == FawnVision::image_format::d32_sfloat )
                                                   ? VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
                                                   : ( formats[i] == FawnVision::image_format::d16_unorm_s8_uint || formats[i] == FawnVision::image_format::d24_unorm_s8_uint || formats[i] == FawnVision::image_format::d32_sfloat_s8_uint )
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
                .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                .inputAttachmentCount = inputCount,
                .pInputAttachments = inputCount
                        ? inputAttachmentRef.data()
                        : VK_NULL_HANDLE,
                .colorAttachmentCount = hasColor
                        ? 1u
                        : 0u,
                .pColorAttachments = hasColor
                        ? &colorAttachmentRef
                        : VK_NULL_HANDLE, .pResolveAttachments = VK_NULL_HANDLE,     //todo:support?
                .pDepthStencilAttachment = m_hasDepthAttachment
                                           ? &depthAttachmentRef
                                           : VK_NULL_HANDLE, .preserveAttachmentCount = 0,       //todo:support?
                .pPreserveAttachments = VK_NULL_HANDLE,    //todo:support?
        };
        
        const VkSubpassDependency dependency{
                .srcSubpass = VK_SUBPASS_EXTERNAL, .dstSubpass = 0, .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, .srcAccessMask = 0, .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        };
        
        const VkRenderPassCreateInfo renderPassInfo{
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, .attachmentCount = static_cast<uint32_t>( attachmentDescs.size()), .pAttachments = attachmentDescs.data(), .subpassCount = 1, .pSubpasses = &surpass, .dependencyCount = 1, .pDependencies = &dependency,
        };
        CheckVkResult(
                vkCreateRenderPass( GetDevice()->GetVkDevice(), &renderPassInfo, VK_NULL_HANDLE, &m_renderPass ),
                "failed to create render pass!"
        );
    }
    CRenderPass::~CRenderPass()
    {
        vkDestroyRenderPass( GetDevice()->GetVkDevice(), m_renderPass, VK_NULL_HANDLE );
    }
} // DeerVulkan