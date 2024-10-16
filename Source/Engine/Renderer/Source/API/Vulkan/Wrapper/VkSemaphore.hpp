//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "../DeerVulkan_Core.hpp"

#include "VkDevice.hpp"

namespace DeerVulkan
{
struct SVkSemaphore
{
    VkSemaphore semaphore;
};

inline auto Initialize(const SVkDevice& device, SVkSemaphore& semaphore, const bool isTimeline) noexcept -> int32_t
{
    if (device.device == VK_NULL_HANDLE)
    {
        return -1;
    }
    constexpr VkSemaphoreTypeCreateInfo timelineCreateInfo{
        .sType         = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
        .pNext         = VK_NULL_HANDLE,
        .semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
        .initialValue  = 0,
    };

    if (const VkSemaphoreCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = isTimeline ? &timelineCreateInfo : VK_NULL_HANDLE,
            .flags = 0,
        };
        semaphore.semaphore != VK_NULL_HANDLE || !CheckVkResult(vkCreateSemaphore(device.device, &createInfo, VK_NULL_HANDLE, &semaphore.semaphore)))
    {
        return -1;
    }
    return 0;
}
inline void Cleanup(const SVkDevice& device, SVkSemaphore& semaphore) noexcept
{
    if (device.device == VK_NULL_HANDLE)
    {
        return;
    }
    if (semaphore.semaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(device.device, semaphore.semaphore, VK_NULL_HANDLE);
        semaphore.semaphore = VK_NULL_HANDLE;
    }
}
inline auto Value(const SVkDevice& device, const SVkSemaphore& semaphore, uint64_t& value) noexcept -> int32_t
{
    if (device.device == VK_NULL_HANDLE || semaphore.semaphore == VK_NULL_HANDLE || !CheckVkResult(vkGetSemaphoreCounterValue(device.device, semaphore.semaphore, &value)))
    {
        return -1;
    }
    return 0;
}
inline auto Wait(const SVkDevice& device, const SVkSemaphore& semaphore, const uint64_t& value) noexcept -> int32_t
{
    if (device.device == VK_NULL_HANDLE || semaphore.semaphore == VK_NULL_HANDLE)
    {
        return -1;
    }
    if (const VkSemaphoreWaitInfo waitInfo{
            .sType          = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
            .pNext          = VK_NULL_HANDLE,
            .flags          = 0,
            .semaphoreCount = 1,
            .pSemaphores    = &semaphore.semaphore,
            .pValues        = &value,

        };
        !CheckVkResult(vkWaitSemaphores(device.device, &waitInfo, UINT64_MAX)))
    {
        return -1;
    }
    return 0;
}
[[nodiscard]] constexpr auto SubmitInfo(const uint64_t& waitValue, const uint64_t& signalValue) noexcept -> VkTimelineSemaphoreSubmitInfo
{
    return {
        .sType                     = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO,
        .pNext                     = VK_NULL_HANDLE,
        .waitSemaphoreValueCount   = 1,
        .pWaitSemaphoreValues      = &waitValue,
        .signalSemaphoreValueCount = 1,
        .pSignalSemaphoreValues    = &signalValue,
    };
}
} // namespace DeerVulkan
