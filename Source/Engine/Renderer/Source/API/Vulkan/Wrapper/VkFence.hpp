//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "../DeerVulkan_Core.hpp"

#include "VkDevice.hpp"

namespace DeerVulkan
{
struct SVkFence
{
    VkFence fence;
};

inline auto Initialize(const SVkDevice& device, SVkFence& fence) noexcept -> int32_t
{
    if (device.device == VK_NULL_HANDLE)
    {
        return -1;
    }

    if (constexpr VkFenceCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = VK_NULL_HANDLE,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT,
        };
        fence.fence != VK_NULL_HANDLE || !CheckVkResult(vkCreateFence(device.device, &createInfo, VK_NULL_HANDLE, &fence.fence)))
    {
        return -1;
    }
    return 0;
}
inline void Cleanup(const SVkDevice& device, SVkFence& fence) noexcept
{
    if (device.device == VK_NULL_HANDLE)
    {
        return;
    }
    if (fence.fence == VK_NULL_HANDLE)
    {
        vkDestroyFence(device.device, fence.fence, nullptr);
        fence.fence = VK_NULL_HANDLE;
    }
}
inline auto Wait(const SVkDevice& device, const SVkFence& fence) noexcept -> int32_t
{
    if (device.device == VK_NULL_HANDLE || fence.fence == VK_NULL_HANDLE)
    {
        return -1;
    }
    if (!CheckVkResult(vkWaitForFences(device.device, 1, &fence.fence, VK_TRUE, UINT64_MAX)))
    {
        return -1;
    }
    return 0;
}
inline auto Reset(const SVkDevice& device, const SVkFence& fence) noexcept -> int32_t
{
    if (device.device == VK_NULL_HANDLE || fence.fence == VK_NULL_HANDLE)
    {
        return -1;
    }
    if (!CheckVkResult(vkResetFences(device.device, 1, &fence.fence)))
    {
        return -1;
    }
    return 0;
}
} // namespace DeerVulkan