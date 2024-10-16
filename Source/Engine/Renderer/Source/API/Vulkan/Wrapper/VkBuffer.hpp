//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "../DeerVulkan_Core.hpp"

#include "VkDevice.hpp"

namespace DeerVulkan
{
struct SVkBuffer
{
    VkBuffer buffer{VK_NULL_HANDLE};
    VkDeviceMemory bufferMemory{VK_NULL_HANDLE};
    uint64_t size{0};
};

inline auto Initialize(const SVkDevice& device, SVkBuffer& buffer, const uint64_t size, const uint32_t usage, const uint32_t memoryProperty) noexcept -> int32_t
{
    if (device.device == VK_NULL_HANDLE || size <= 0)
    {
        return -1;
    }
    if (const VkBufferCreateInfo bufferInfo{
            .sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext                 = VK_NULL_HANDLE,
            .flags                 = 0,
            .size                  = size,
            .usage                 = usage,
            .sharingMode           = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices   = VK_NULL_HANDLE,
        };
        buffer.buffer != VK_NULL_HANDLE || !CheckVkResult(vkCreateBuffer(device.device, &bufferInfo, VK_NULL_HANDLE, &buffer.buffer), "Failed to create a Vulkan Buffer."))
    {
        return -1;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device.device, buffer.buffer, &memRequirements);

    if (const VkMemoryAllocateInfo allocInfo{
            .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext           = VK_NULL_HANDLE,
            .allocationSize  = memRequirements.size,
            .memoryTypeIndex = FindMemoryType(device, memRequirements.memoryTypeBits, memoryProperty),
        };
        buffer.bufferMemory != VK_NULL_HANDLE || !CheckVkResult(vkAllocateMemory(device.device, &allocInfo, VK_NULL_HANDLE, &buffer.bufferMemory), "Failed to allocate Vulkan Buffer Memory."))
    {
        return -1;
    }
    if (!CheckVkResult(vkBindBufferMemory(device.device, buffer.buffer, buffer.bufferMemory, 0), "Failed to bind Vulkan Buffer Memory to Vulkan Buffer."))
    {
        return -1;
    }
    buffer.size = size;
    return 0;
}
inline void Cleanup(const SVkDevice& device, SVkBuffer& buffer) noexcept
{
    if (device.device == VK_NULL_HANDLE)
    {
        return;
    }
    if (buffer.bufferMemory)
    {
        vkFreeMemory(device.device, buffer.bufferMemory, VK_NULL_HANDLE);
        buffer.bufferMemory = VK_NULL_HANDLE;
    }
    if (buffer.buffer)
    {
        vkDestroyBuffer(device.device, buffer.buffer, VK_NULL_HANDLE);
        buffer.buffer = VK_NULL_HANDLE;
    }
    buffer.size = 0;
}
template <typename T>
void CopyData(const SVkDevice& device, const SVkBuffer& buffer, const uint64_t size, const T* const pInData) noexcept
{
    if (device.device == VK_NULL_HANDLE || buffer.buffer == VK_NULL_HANDLE || size == 0U || pInData == nullptr)
    {
        return;
    }
    void* data;
    vkMapMemory(device.device, buffer.bufferMemory, 0, size, 0, &data);
    memcpy(data, std::bit_cast<const unsigned char*>(pInData), size);
    vkUnmapMemory(device.device, buffer.bufferMemory);
}
inline void Flush(const SVkDevice& device, const SVkBuffer& buffer, const uint64_t size, const uint64_t  offset ) noexcept
{
    const VkMappedMemoryRange mappedRange {
        .sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
        .pNext = VK_NULL_HANDLE,
        .memory = buffer.bufferMemory,
        .offset = offset,
        .size   = size,
    };
    vkFlushMappedMemoryRanges( device.device, 1, &mappedRange );
}
} // namespace DeerVulkan