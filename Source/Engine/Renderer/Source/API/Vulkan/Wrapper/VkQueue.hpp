//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "../DeerVulkan_Core.hpp"

#include "VkDevice.hpp"
#include "VkSemaphore.hpp"
#include "VkSwapChain.hpp"

namespace DeerVulkan
{
struct SVkQueue
{
    VkQueue queue;
};

inline auto Initialize(const SVkDevice& device, SVkQueue& queue, const uint32_t family, const uint32_t index) noexcept -> int32_t
{
    if (device.device == VK_NULL_HANDLE)
    {
        return -1;
    }
    const VkDeviceQueueInfo2 info{
        .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
        .pNext            = VK_NULL_HANDLE,
        .flags            = 0,
        .queueFamilyIndex = family,
        .queueIndex       = index,
    };
    vkGetDeviceQueue2(device.device, &info, &queue.queue);
    return 0;
}
inline void Cleanup(SVkQueue& queue) noexcept
{
    queue.queue = VK_NULL_HANDLE;
}

inline void WaitIdle(const SVkQueue& queue) noexcept
{
    if (queue.queue == VK_NULL_HANDLE)
    {
        return;
    }
    vkQueueWaitIdle(queue.queue);
}
inline auto Present(const SVkQueue& queue, const SVkSwapChain& swapChain) noexcept -> int32_t
{
    if (queue.queue == VK_NULL_HANDLE || swapChain.swapchain == VK_NULL_HANDLE)
    {
        return -1;
    }

    if (const VkPresentInfoKHR info{
            .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext              = VK_NULL_HANDLE,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores    = VK_NULL_HANDLE,
            .swapchainCount     = 1U,
            .pSwapchains        = &swapChain.swapchain,
            .pImageIndices      = &swapChain.currentFrameIdx,
            .pResults           = VK_NULL_HANDLE,
        };
        !CheckVkResult(vkQueuePresentKHR(queue.queue, &info)))
    {
        return -1;
    }
    return 0;
}
inline auto Present(const SVkQueue& queue, const SVkSwapChain& swapChain, const SVkSemaphore& waitSemaphore) noexcept -> int32_t
{
    if (queue.queue == VK_NULL_HANDLE || swapChain.swapchain == VK_NULL_HANDLE)
    {
        return -1;
    }

    if (const VkPresentInfoKHR info{
            .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext              = VK_NULL_HANDLE,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores    = &waitSemaphore.semaphore,
            .swapchainCount     = 1U,
            .pSwapchains        = &swapChain.swapchain,
            .pImageIndices      = &swapChain.currentFrameIdx,
            .pResults           = VK_NULL_HANDLE,
        };
        !CheckVkResult(vkQueuePresentKHR(queue.queue, &info)))
    {
        return -1;
    }
    return 0;
}

inline auto QueueSubmit(const SVkQueue& queue, const SVkCommandBuffer& commandBuffer) noexcept -> int32_t
{
    if (queue.queue == VK_NULL_HANDLE || commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return -1;
    }
    const VkCommandBufferSubmitInfo commandBufferSubmitInfo{
        .sType         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .pNext         = VK_NULL_HANDLE,
        .commandBuffer = commandBuffer.commandBuffer,
        .deviceMask    = 0,
    };
    const VkSubmitInfo2 info{
        .sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .pNext                    = VK_NULL_HANDLE,
        .flags                    = 0,
        .waitSemaphoreInfoCount   = 0,
        .pWaitSemaphoreInfos      = VK_NULL_HANDLE,
        .commandBufferInfoCount   = 1,
        .pCommandBufferInfos      = &commandBufferSubmitInfo,
        .signalSemaphoreInfoCount = 0,
        .pSignalSemaphoreInfos    = VK_NULL_HANDLE,
    };
    if (!CheckVkResult(vkQueueSubmit2(queue.queue, 1, &info, VK_NULL_HANDLE)))
    {
        return -1;
    }
    return 0;
}
inline auto QueueSubmit(const SVkQueue& queue, const SVkCommandBuffer& commandBuffer, const SVkSemaphore& semaphore, const uint64_t waitValue) noexcept -> int32_t
{
    if (queue.queue == VK_NULL_HANDLE || commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return -1;
    }
    const VkCommandBufferSubmitInfo commandBufferSubmitInfo{
        .sType         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .pNext         = VK_NULL_HANDLE,
        .commandBuffer = commandBuffer.commandBuffer,
        .deviceMask    = 0,
    };
    const VkSemaphoreSubmitInfo waitSemaphoreInfo{
        .sType       = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .pNext       = VK_NULL_HANDLE,
        .semaphore   = semaphore.semaphore,
        .value       = waitValue,
        .stageMask   = 0,
        .deviceIndex = 0,
    };
    const VkSemaphoreSubmitInfo signalSemaphoreInfo{
        .sType       = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .pNext       = VK_NULL_HANDLE,
        .semaphore   = semaphore.semaphore,
        .value       = waitValue + 1,
        .stageMask   = 0,
        .deviceIndex = 0,
    };
    const VkSubmitInfo2 info{
        .sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .pNext                    = VK_NULL_HANDLE,
        .flags                    = 0,
        .waitSemaphoreInfoCount   = 1,
        .pWaitSemaphoreInfos      = &waitSemaphoreInfo,
        .commandBufferInfoCount   = 1,
        .pCommandBufferInfos      = &commandBufferSubmitInfo,
        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos    = &signalSemaphoreInfo,
    };
    if (!CheckVkResult(vkQueueSubmit2(queue.queue, 1, &info, VK_NULL_HANDLE)))
    {
        return -1;
    }
    return 0;
}
inline auto QueueSubmit(const SVkQueue& queue, const SVkCommandBuffer& commandBuffer, const SVkSemaphore& semaphore, const uint64_t value, const bool isWait) noexcept -> int32_t
{
    if (queue.queue == VK_NULL_HANDLE || commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return -1;
    }
    const VkCommandBufferSubmitInfo commandBufferSubmitInfo{
        .sType         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .pNext         = VK_NULL_HANDLE,
        .commandBuffer = commandBuffer.commandBuffer,
        .deviceMask    = 0,
    };
    const VkSemaphoreSubmitInfo semaphoreInfo{
        .sType       = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .pNext       = VK_NULL_HANDLE,
        .semaphore   = semaphore.semaphore,
        .value       = value,
        .stageMask   = 0,
        .deviceIndex = 0,
    };
    const VkSubmitInfo2 info{
        .sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .pNext                    = VK_NULL_HANDLE,
        .flags                    = 0,
        .waitSemaphoreInfoCount   = isWait ? 1U : 0U,
        .pWaitSemaphoreInfos      = isWait ? &semaphoreInfo : VK_NULL_HANDLE,
        .commandBufferInfoCount   = 1,
        .pCommandBufferInfos      = &commandBufferSubmitInfo,
        .signalSemaphoreInfoCount = isWait ? 0U : 1U,
        .pSignalSemaphoreInfos    = isWait ? VK_NULL_HANDLE : &semaphoreInfo,
    };
    if (!CheckVkResult(vkQueueSubmit2(queue.queue, 1, &info, VK_NULL_HANDLE)))
    {
        return -1;
    }
    return 0;
}
inline auto QueueSubmit(const SVkQueue& queue, const SVkCommandBuffer& commandBuffer, const SVkSemaphore& semaphore, const uint64_t waitValue, const uint64_t signalValue) noexcept -> int32_t
{
    if (queue.queue == VK_NULL_HANDLE || commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return -1;
    }
    const VkCommandBufferSubmitInfo commandBufferSubmitInfo{
        .sType         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .pNext         = VK_NULL_HANDLE,
        .commandBuffer = commandBuffer.commandBuffer,
        .deviceMask    = 0,
    };
    const VkSemaphoreSubmitInfo waitSemaphoreInfo{
        .sType       = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .pNext       = VK_NULL_HANDLE,
        .semaphore   = semaphore.semaphore,
        .value       = waitValue,
        .stageMask   = 0,
        .deviceIndex = 0,
    };
    const VkSemaphoreSubmitInfo signalSemaphoreInfo{
        .sType       = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .pNext       = VK_NULL_HANDLE,
        .semaphore   = semaphore.semaphore,
        .value       = signalValue,
        .stageMask   = 0,
        .deviceIndex = 0,
    };
    const VkSubmitInfo2 info{
        .sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .pNext                    = VK_NULL_HANDLE,
        .flags                    = 0,
        .waitSemaphoreInfoCount   = 1,
        .pWaitSemaphoreInfos      = &waitSemaphoreInfo,
        .commandBufferInfoCount   = 1,
        .pCommandBufferInfos      = &commandBufferSubmitInfo,
        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos    = &signalSemaphoreInfo,
    };
    if (!CheckVkResult(vkQueueSubmit2(queue.queue, 1, &info, VK_NULL_HANDLE)))
    {
        return -1;
    }
    return 0;
}
} // namespace DeerVulkan