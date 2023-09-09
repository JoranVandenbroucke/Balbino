#include "Queue.h"

#include "Device.h"

#include "CommandPool.h"
#include "Fence.h"
#include "Semaphore.h"
#include "Swapchain.h"

DeerVulkan::CQueue::~CQueue()
{
    m_queue = VK_NULL_HANDLE;
}

void DeerVulkan::CQueue::Initialize()
{
    if ( !m_queue )
    {
        uint32_t count;
        vkGetPhysicalDeviceQueueFamilyProperties( GetDevice()->GetPhysicalDeviceInfo().device, &count, VK_NULL_HANDLE );
        const auto queues { static_cast<VkQueueFamilyProperties*>( malloc( sizeof( VkQueueFamilyProperties ) * count ) ) };
        vkGetPhysicalDeviceQueueFamilyProperties( GetDevice()->GetPhysicalDeviceInfo().device, &count, queues );
        for ( uint32_t i = 0; i < count; i++ )
        {
            if ( queues && queues[ i ].queueFlags & VK_QUEUE_GRAPHICS_BIT )
            {
                m_queueFamily = i;
                break;
            }
        }
        free( queues );
        vkGetDeviceQueue( GetDevice()->GetVkDevice(), m_queueFamily, 0, &m_queue );
    }
}

void DeerVulkan::CQueue::SubmitPass( FawnVision::Semaphore signalSemaphore, FawnVision::Semaphore waitableSemaphores, FawnVision::CommandPool cmdList, FawnVision::Fence fence ) noexcept
{
    signalSemaphore->AddRef();
    waitableSemaphores->AddRef();
    cmdList->AddRef();
    fence->AddRef();

    constexpr VkPipelineStageFlags pipelineStageFlags { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    const VkSubmitInfo submitInfo {
        .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount   = 1,
        .pWaitSemaphores      = &waitableSemaphores->Get(),
        .pWaitDstStageMask    = &pipelineStageFlags,
        .commandBufferCount   = 1,
        .pCommandBuffers      = &cmdList->GetCommandBuffer(),
        .signalSemaphoreCount = 1,
        .pSignalSemaphores    = &signalSemaphore->Get(),
    };
    CheckVkResult( vkQueueSubmit( m_queue, 1, &submitInfo, fence->Get() ) );

    signalSemaphore->Release();
    waitableSemaphores->Release();
    cmdList->Release();
    fence->Release();
}

bool DeerVulkan::CQueue::PresentToScreen( FawnVision::Swapchain swapchain, FawnVision::Semaphore signalSemaphore, uint32_t imageIndex ) noexcept
{
    swapchain->AddRef();
    signalSemaphore->AddRef();

    const VkPresentInfoKHR presentInfo {
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores    = &signalSemaphore->Get(),
        .swapchainCount     = 1,
        .pSwapchains        = &swapchain->GetHandle(),
        .pImageIndices      = &imageIndex,
    };
    const VkResult result { vkQueuePresentKHR( m_queue, &presentInfo ) };

    CheckVkResult( result, "", static_cast<VkResult>( VK_SUCCESS | VK_ERROR_OUT_OF_DATE_KHR | VK_SUBOPTIMAL_KHR ) );

    swapchain->Release();
    signalSemaphore->Release();

    return ( result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR );
}
void DeerVulkan::CQueue::WaitIdle() const
{
    vkQueueWaitIdle( m_queue );
}
