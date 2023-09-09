#include "FrameBuffer.h"

#include "Device.h"

#include "ImageView.h"
#include "RenderPass.h"

DeerVulkan::CFrameBuffer::~CFrameBuffer()
{
    for ( auto& frameBuffer : m_frameBuffer )
    {
        vkDestroyFramebuffer( GetDevice()->GetVkDevice(), frameBuffer, VK_NULL_HANDLE );
    }
    m_frameBuffer.clear();
}

void DeerVulkan::CFrameBuffer::Initialize( FawnVision::RenderPass renderPass, uint32_t width, uint32_t height, const std::vector<FawnVision::ImageView>& renderTargets, FawnVision::ImageView depth )
{
    renderPass->AddRef();
    if ( depth )
        depth->AddRef();

    bool hasRenderTargets { !renderTargets.empty() };
    if ( !hasRenderTargets && !depth || !width || !height || ( renderPass->HasDepthAttachment() != ( depth != BALBINO_NULL ) ) || ( renderPass->HasColorAttachments() != hasRenderTargets ) )
    {
        if ( depth )
            depth->Release();
        renderPass->Release();
        return;
    }

    std::vector<VkImageView> imageViews;

    if ( hasRenderTargets )
    {
        imageViews.push_back( VK_NULL_HANDLE );
    }
    if ( depth )
    {
        imageViews.push_back( depth->GetImageView() );
    }
    const VkFramebufferCreateInfo bufCreateInfo {
        .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext           = VK_NULL_HANDLE,
        .flags           = 0,
        .renderPass      = renderPass->GetHandle(),
        .attachmentCount = static_cast<uint32_t>( imageViews.size() ),
        .pAttachments    = imageViews.data(),
        .width           = width,
        .height          = height,
        .layers          = 1,
    };
    m_frameBuffer.resize( hasRenderTargets ? renderTargets.size() : 1, VK_NULL_HANDLE );
    if ( hasRenderTargets )
    {
        for ( uint32_t i = 0; i < renderTargets.size(); i++ )
        {
            imageViews[ 0 ] = renderTargets[ i ]->GetImageView();
            CheckVkResult( vkCreateFramebuffer( GetDevice()->GetVkDevice(), &bufCreateInfo, VK_NULL_HANDLE, &m_frameBuffer[ i ] ), "failed to create frame buffer!" );
        }
        if ( depth )
            depth->Release();
        renderPass->Release();
        return;
    }
    CheckVkResult( vkCreateFramebuffer( GetDevice()->GetVkDevice(), &bufCreateInfo, VK_NULL_HANDLE, &m_frameBuffer[ 0 ] ), "failed to create frame buffer!" );
    if ( depth )
        depth->Release();
    renderPass->Release();
}
