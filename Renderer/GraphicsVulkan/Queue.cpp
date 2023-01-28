#include "Queue.h"

#include "CommandPool.h"
#include "Device.h"
#include "Fence.h"
#include "Funtions.h"
#include "Instance.h"
#include "Semaphore.h"
#include "Swapchain.h"

FawnVision::CQueue::CQueue( const CDevice* device )
        : CDeviceObject{ device }
          , m_queueFamily{ 0 }
          , m_queue{ VK_NULL_HANDLE }
{
}

FawnVision::CQueue::~CQueue()
{
    m_queue = VK_NULL_HANDLE;
}

void FawnVision::CQueue::Initialize()
{
    if ( !m_queue )
    {
        uint32_t count;
        vkGetPhysicalDeviceQueueFamilyProperties( GetDevice()->GetPhysicalDeviceInfo()->device, &count, nullptr );
        const auto queues{ static_cast<VkQueueFamilyProperties*>( malloc( sizeof( VkQueueFamilyProperties ) * count )) };
        vkGetPhysicalDeviceQueueFamilyProperties( GetDevice()->GetPhysicalDeviceInfo()->device, &count, queues );
        for ( uint32_t i = 0; i < count; i++ )
        {
            if ( queues && queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT )
            {
                m_queueFamily = i;
                break;
            }
        }
        free( queues );
        vkGetDeviceQueue( GetDevice()->GetVkDevice(), m_queueFamily, 0, &m_queue );
    }
}

void FawnVision::CQueue::SubmitPass( const CSemaphore* signalSemaphore, const CSemaphore* waitableSemaphores, const CCommandPool* cmdList, const CFence* pFence ) const
{
    constexpr VkPipelineStageFlags pipelineStageFlags{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    const VkSubmitInfo             submitInfo{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO, .waitSemaphoreCount = 1, .pWaitSemaphores = &waitableSemaphores->Get(), .pWaitDstStageMask = &pipelineStageFlags, .commandBufferCount = 1, .pCommandBuffers = &cmdList->GetCommandBuffer(), .signalSemaphoreCount = 1, .pSignalSemaphores = &signalSemaphore->Get(),
    };
    CheckVkResult( vkQueueSubmit( m_queue, 1, &submitInfo, pFence->Get()));
}

bool FawnVision::CQueue::PresentToScreen( const CSwapchain* pSwapchain, const CSemaphore* signalSemaphore, uint32_t imageIndex ) const
{
    const VkPresentInfoKHR presentInfo{
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR, .waitSemaphoreCount = 1, .pWaitSemaphores = &signalSemaphore->Get(), .swapchainCount = 1, .pSwapchains = &pSwapchain->GetVkSwapchain(), .pImageIndices = &imageIndex,
    };
    const VkResult         result{ vkQueuePresentKHR( m_queue, &presentInfo ) };
    
    CheckVkResult( result, "", static_cast<VkResult>(VK_SUCCESS | VK_ERROR_OUT_OF_DATE_KHR | VK_SUBOPTIMAL_KHR));
    return ( result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR );
}

const VkQueue& FawnVision::CQueue::GetQueue() const
{
    return m_queue;
}

uint32_t FawnVision::CQueue::GetQueFamily() const
{
    return m_queueFamily;
}

void FawnVision::CQueue::WaitIdle() const
{
    vkQueueWaitIdle( m_queue );
}

FawnVision::CQueue* FawnVision::CQueue::CreateNew( const CDevice* pDevice )
{
    return new CQueue{ pDevice };
}
