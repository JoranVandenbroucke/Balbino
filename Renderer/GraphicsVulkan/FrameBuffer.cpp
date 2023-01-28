#include "FrameBuffer.h"

#include "Device.h"
#include "Funtions.h"
#include "Instance.h"
#include "RenderPass.h"

#include "ImageResource.h"
#include "ImageView.h"

FawnVision::CFrameBuffer::CFrameBuffer( const CDevice* device )
        : CDeviceObject{ device }
{
}

FawnVision::CFrameBuffer::~CFrameBuffer()
{
    for ( auto& frameBuffer : m_frameBuffer )
    {
        vkDestroyFramebuffer( GetDevice()->GetVkDevice(), frameBuffer, VK_NULL_HANDLE );
    }
    m_frameBuffer.clear();
}

void FawnVision::CFrameBuffer::Initialize( FawnVision::CRenderPass* pRenderPass, uint32_t width, uint32_t height, const std::vector<CImageView*>& renderTargets, CImageView* pDepth )
{
    bool hasRenderTargets{ !renderTargets.empty() };
    if ( !hasRenderTargets && !pDepth || !width || !height || ( pRenderPass->HasDepthAttachment() && !pDepth ) || ( !pRenderPass->HasDepthAttachment() && pDepth ) || ( pRenderPass->HasColorAttachments() && !hasRenderTargets ) || ( !pRenderPass->HasColorAttachments() && hasRenderTargets ))
    {
        return;
    }
    
    std::vector<VkImageView> imageViews;
    
    if ( hasRenderTargets )
    {
        imageViews.push_back( VK_NULL_HANDLE );
    }
    if ( pDepth )
    {
        imageViews.push_back( pDepth->GetImageView());
    }
    VkFramebufferCreateInfo bufCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, .pNext = VK_NULL_HANDLE, .flags = 0, .renderPass = pRenderPass->GetRenderPass(), .attachmentCount = static_cast<uint32_t>( imageViews.size()), .pAttachments = imageViews.data(), .width = width, .height = height, .layers = 1,
    };
    m_frameBuffer.resize( hasRenderTargets ? renderTargets.size() : 1, VK_NULL_HANDLE );
    if ( hasRenderTargets )
    {
        for ( uint32_t i = 0; i < renderTargets.size(); i++ )
        {
            imageViews[0] = renderTargets[i]->GetImageView();
            CheckVkResult(
                    vkCreateFramebuffer( GetDevice()->GetVkDevice(), &bufCreateInfo, nullptr, &m_frameBuffer[i] ),
                    "failed to create frame buffer!"
            );
        }
        return;
    }
    CheckVkResult(
            vkCreateFramebuffer( GetDevice()->GetVkDevice(), &bufCreateInfo, nullptr, &m_frameBuffer[0] ),
            "failed to create frame buffer!"
    );
}


VkFramebuffer FawnVision::CFrameBuffer::GetFrameBuffer( const uint32_t idx ) const
{
    return m_frameBuffer[idx];
}

FawnVision::CFrameBuffer* FawnVision::CFrameBuffer::CreateNew( const CDevice* pDevice )
{
    return new CFrameBuffer{ pDevice };
}
