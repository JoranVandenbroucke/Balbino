#include "CommandPool.h"

#include "Device.h"
#include "Funtions.h"
#include "ShaderPipeline.h"
#include "DescriptorSet.h"
#include "FrameBuffer.h"
#include "Swapchain.h"
#include "ImageResource.h"

FawnVision::CCommandPool::CCommandPool( const CDevice* pDevice )
        : CDeviceObject{ pDevice }
        , m_commandPool{ VK_NULL_HANDLE }
        , m_commandBuffers{}
        , m_currentFrameIndex{ 0 }
{
}

FawnVision::CCommandPool::~CCommandPool()
{
    vkFreeCommandBuffers(GetDevice()->GetVkDevice(), m_commandPool, (uint32_t)m_commandBuffers.size(), m_commandBuffers.data() );
    vkDestroyCommandPool( GetDevice()->GetVkDevice(), m_commandPool, nullptr );
    m_commandPool = VK_NULL_HANDLE;
    for ( CImageResource* imageResource : m_imageResources )
    {
        imageResource->Release();
    }
    m_commandBuffers.clear();
    m_imageResources.clear();
    m_currentFrameIndex = 0;
}

void FawnVision::CCommandPool::Initialize( uint32_t queueFamilyIndex, CSwapchain* swapchain )
{
    swapchain->AddRef();
    if ( !m_commandPool )
    {
        const VkCommandPoolCreateInfo poolInfo{
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, .queueFamilyIndex = queueFamilyIndex,
        };
        VkCommandPool                 cmdAllocator{ VK_NULL_HANDLE };
        CheckVkResult(
                vkCreateCommandPool( GetDevice()->GetVkDevice(), &poolInfo, nullptr, &cmdAllocator ),
                "failed to create graphics command pool!"
        );
        
        m_commandPool = cmdAllocator;
    }
    
    if ( m_commandBuffers.empty())
    {
        uint32_t size;
        swapchain->GetImages( m_imageResources, size );
        m_commandBuffers.resize( size );
        const VkCommandBufferAllocateInfo bufferInfo{
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, .commandPool = m_commandPool, .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .commandBufferCount = size,
        };
        
        CheckVkResult(
                vkAllocateCommandBuffers( GetDevice()->GetVkDevice(), &bufferInfo, m_commandBuffers.data()),
                "Could not create command list"
        );
    }
    swapchain->Release();
}

void FawnVision::CCommandPool::UpdateFrameIndex()
{
    m_currentFrameIndex = ++m_currentFrameIndex % m_commandBuffers.size();
}

const VkCommandPool& FawnVision::CCommandPool::GetHandle() const
{
    return m_commandPool;
}

const VkCommandBuffer& FawnVision::CCommandPool::GetCommandBuffer() const
{
    return m_commandBuffers[m_currentFrameIndex];
}

void FawnVision::CCommandPool::EndRender() const
{
    vkCmdEndRenderPass( m_commandBuffers[m_currentFrameIndex] );
    if ( vkEndCommandBuffer( m_commandBuffers[m_currentFrameIndex] ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to record command buffer!" );
    }
}

void FawnVision::CCommandPool::BeginRender( CRenderPass* renderPass, CFrameBuffer* frameBuffer, CSwapchain* swapchain ) const
{
    renderPass->AddRef();
    frameBuffer->AddRef();
    swapchain->AddRef();
    
    const VkCommandBufferBeginInfo beginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };
    std::vector<VkClearValue>      clearColor;
    if ( renderPass->HasColorAttachments())
    {
        clearColor.emplace_back( VkClearValue{ .color = { 0.37647058823f, 0.37647058823f, 0.37647058823f, 1.0f }} );
    }
    if ( renderPass->HasDepthAttachment())
    {
        clearColor.emplace_back( VkClearValue{ .depthStencil = { 1.0f, 0 }} );
    }
    
    const VkRenderPassBeginInfo renderPassInfo{
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO, .renderPass = renderPass->GetHandle(), .framebuffer = frameBuffer->GetFrameBuffer(
                    m_currentFrameIndex
            ), .renderArea = {{ 0, 0 }, swapchain->GetExtend()
            }, .clearValueCount = (uint32_t) clearColor.size(), .pClearValues = clearColor.data(),
    };
    vkBeginCommandBuffer( m_commandBuffers[m_currentFrameIndex], &beginInfo );
    vkCmdBeginRenderPass( m_commandBuffers[m_currentFrameIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );
    
    renderPass->Release();
    frameBuffer->Release();
    swapchain->Release();
}

void FawnVision::CCommandPool::BindShader( CShaderPipeline* pipeline, CDescriptorSet* descriptorSet ) const
{
    pipeline->AddRef();
    descriptorSet->AddRef();
    
    uint32_t offset{};
//    uint32_t offset{sizeof(glm::mat4) * 3};
    vkCmdBindPipeline(
            m_commandBuffers[m_currentFrameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipeline());
    vkCmdBindDescriptorSets(
            m_commandBuffers[m_currentFrameIndex],
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipeline->GetPipelineLayout(),
            0u,
            1u,  //todo
            &descriptorSet->GetDescriptorSets(),
            0u,  //todo
            &offset
    );
    pipeline->Release();
    descriptorSet->Release();
}

uint32_t FawnVision::CCommandPool::GetCommandBufferCount() const
{
    return static_cast<uint32_t>( m_commandBuffers.size());
}

uint32_t FawnVision::CCommandPool::GetCurrentIndex() const
{
    return m_currentFrameIndex;
}
void FawnVision::CCommandPool::NextSubPass()
{
    vkCmdNextSubpass( m_commandBuffers[m_currentFrameIndex], VK_SUBPASS_CONTENTS_INLINE );
}
