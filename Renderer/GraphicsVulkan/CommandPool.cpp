#include "CommandPool.h"

#include "DescriptorSet.h"
#include "Device.h"
#include "FrameBuffer.h"
#include "RenderPass.h"
#include "Funtions.h"
#include "ImageResource.h"
#include "ShaderPipeline.h"
#include "Swapchain.h"

BalVulkan::CCommandPool::CCommandPool( const CDevice* device )
        : CDeviceObject{ device },
          m_commandPool{ VK_NULL_HANDLE },
          m_commandBuffers{},
          m_currentFrameIndex{ 0 }
{
}

BalVulkan::CCommandPool::~CCommandPool()
{
    vkDestroyCommandPool( GetDevice()->GetVkDevice(), m_commandPool, nullptr );
    for ( const CImageResource* pImageResource : m_imageResources )
    {
        delete pImageResource;
    }
    m_imageResources.clear();
}

void BalVulkan::CCommandPool::Initialize( uint32_t queueFamilyIndex, const CSwapchain* pSwapchain )
{
    if ( !m_commandPool )
    {
        const VkCommandPoolCreateInfo poolInfo{
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, .queueFamilyIndex = queueFamilyIndex,
        };
        VkCommandPool                 cmdAllocator{ VK_NULL_HANDLE };
        CheckVkResult( vkCreateCommandPool( GetDevice()->GetVkDevice(), &poolInfo, nullptr, &cmdAllocator ),
                       "failed to create graphics command pool!" );
        
        m_commandPool = cmdAllocator;
    }
    
    if ( m_commandBuffers.empty())
    {
        uint32_t size;
        pSwapchain->GetImages( m_imageResources, size );
        m_commandBuffers.resize( size );
        const VkCommandBufferAllocateInfo bufferInfo{
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, .commandPool = m_commandPool, .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .commandBufferCount = size,
        };
        
        CheckVkResult( vkAllocateCommandBuffers( GetDevice()->GetVkDevice(), &bufferInfo, m_commandBuffers.data()),
                       "Could not create command list" );
    }
}

void BalVulkan::CCommandPool::UpdateFrameIndex()
{
    m_currentFrameIndex = ++m_currentFrameIndex % m_commandBuffers.size();
}

const VkCommandPool& BalVulkan::CCommandPool::GetCommandPool() const
{
    return m_commandPool;
}

const VkCommandBuffer& BalVulkan::CCommandPool::GetCommandBuffer() const
{
    return m_commandBuffers[m_currentFrameIndex];
}

void BalVulkan::CCommandPool::EndRender() const
{
    vkCmdEndRenderPass( m_commandBuffers[m_currentFrameIndex] );
    if ( vkEndCommandBuffer( m_commandBuffers[m_currentFrameIndex] ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to record command buffer!" );
    }
}

void BalVulkan::CCommandPool::BeginRender( CRenderPass* pRenderPass, CFrameBuffer* pFrameBuffer, CSwapchain* pSwapchain ) const
{
    const VkCommandBufferBeginInfo beginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };
    std::vector<VkClearValue>      clearColor;
    if ( pRenderPass->HasColorAttachments())
    {
        clearColor.emplace_back( VkClearValue{ .color = { 0.37647058823f, 0.37647058823f, 0.37647058823f, 1.0f }} );
    }
    if ( pRenderPass->HasDepthAttachment())
    {
        clearColor.emplace_back( VkClearValue{ .depthStencil = { 1.0f, 0 }} );
    }
    
    const VkRenderPassBeginInfo renderPassInfo{
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = pRenderPass->GetRenderPass(),
            .framebuffer = pFrameBuffer->GetFrameBuffer( m_currentFrameIndex ),
            .renderArea = {{ 0, 0 }, pSwapchain->GetExtend() },
            .clearValueCount = (uint32_t) clearColor.size(),
            .pClearValues = clearColor.data(),
    };
    vkBeginCommandBuffer( m_commandBuffers[m_currentFrameIndex], &beginInfo );
    vkCmdBeginRenderPass( m_commandBuffers[m_currentFrameIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );
}

void BalVulkan::CCommandPool::BindShader( CShaderPipeline* pPipeline, CDescriptorSet* pDescriptorSet ) const
{
    uint32_t offset{};
//    uint32_t offset{sizeof(glm::mat4) * 3};
    vkCmdBindPipeline( m_commandBuffers[m_currentFrameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS,
                       pPipeline->GetPipeline());
    vkCmdBindDescriptorSets( m_commandBuffers[m_currentFrameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS,
                             pPipeline->GetPipelineLayout(), 0, pDescriptorSet->GetDescriptorSetCount(),
                             pDescriptorSet->GetDescriptorSets(), pDescriptorSet->GetDynamicCount(), &offset );
}

BalVulkan::CCommandPool* BalVulkan::CCommandPool::CreateNew( const CDevice* pDevice )
{
    return new CCommandPool{ pDevice };
}

uint32_t BalVulkan::CCommandPool::GetCommandBufferCount() const
{
    return static_cast<uint32_t>( m_commandBuffers.size());
}

uint32_t BalVulkan::CCommandPool::GetCurrentIndex() const
{
    return m_currentFrameIndex;
}
void BalVulkan::CCommandPool::NextSubpass()
{
    vkCmdNextSubpass( m_commandBuffers[m_currentFrameIndex], VK_SUBPASS_CONTENTS_INLINE );
}
