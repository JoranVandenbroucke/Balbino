//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once

#include "VkInstance.hpp"
#include "VkSurface.hpp"

#include <array>
#include <cstddef>
#include <set>
#include <vector>

namespace DeerVulkan
{
struct SQueueFamily
{
    int32_t graphicsFamily{-1};
    int32_t presentFamily{-1};
};

[[nodiscard]] constexpr auto IsComplete(const SQueueFamily& queueFamily) noexcept -> bool
{
    return queueFamily.graphicsFamily >= 0 && queueFamily.presentFamily >= 0;
}

struct SPhysicalDeviceInfo final
{
    VkPhysicalDeviceProperties deviceProperties{};
    VkPhysicalDeviceMemoryProperties deviceMemoryProperties{};
    VkPhysicalDeviceFeatures deviceFeatures{};
    std::vector<VkQueueFamilyProperties> queueFamilyProperties{};
    VkPhysicalDevice device{VK_NULL_HANDLE};
};

[[nodiscard]] constexpr auto GetMaxUsableSampleCount(const SPhysicalDeviceInfo& physicalDevice) noexcept -> VkSampleCountFlagBits
{
    const VkSampleCountFlags counts{physicalDevice.deviceProperties.limits.framebufferColorSampleCounts & physicalDevice.deviceProperties.limits.framebufferDepthSampleCounts};
    if ((counts & VK_SAMPLE_COUNT_64_BIT) != 0U)
    {
        return VK_SAMPLE_COUNT_64_BIT;
    }
    if ((counts & VK_SAMPLE_COUNT_32_BIT) != 0U)
    {
        return VK_SAMPLE_COUNT_32_BIT;
    }
    if ((counts & VK_SAMPLE_COUNT_16_BIT) != 0U)
    {
        return VK_SAMPLE_COUNT_16_BIT;
    }
    if ((counts & VK_SAMPLE_COUNT_8_BIT) != 0U)
    {
        return VK_SAMPLE_COUNT_8_BIT;
    }
    if ((counts & VK_SAMPLE_COUNT_4_BIT) != 0U)
    {
        return VK_SAMPLE_COUNT_4_BIT;
    }
    if ((counts & VK_SAMPLE_COUNT_2_BIT) != 0U)
    {
        return VK_SAMPLE_COUNT_2_BIT;
    }
    return VK_SAMPLE_COUNT_1_BIT;
}

inline auto FindQueueFamily(const SVkSurface& surface, const std::vector<SPhysicalDeviceInfo>& physicalDevices, SQueueFamily& queueFamily) noexcept -> int32_t
{
    if (surface.surface == VK_NULL_HANDLE || physicalDevices.empty())
    {
        return -1;
    }
    for (const auto& [deviceProperties, deviceMemoryProperties, deviceFeatures, queueFamilyProperties, device] : physicalDevices)
    {
        int32_t index{};
        SQueueFamily family{};
        for (const auto& [queueFlags, queueCount, timestampValidBits, minImageTransferGranularity] : queueFamilyProperties)
        {
            if (queueCount <= 0)
            {
                continue;
            }
            const bool supportsGraphics = (queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0U;
            const bool supportsCompute  = (queueFlags & VK_QUEUE_COMPUTE_BIT) != 0;

            if (const bool hasEnoughSize = queueCount >= 2; supportsGraphics && supportsCompute && hasEnoughSize)
            {
                family.graphicsFamily = index;
            }

            VkBool32 presentSupport{};
            if (!CheckVkResult(vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface.surface, &presentSupport)))
            {
                return -1;
            }

            if (presentSupport == VK_TRUE)
            {
                family.presentFamily = index;
            }

            if (IsComplete(family))
            {
                queueFamily = family;
                return 0;
            }

            ++index;
        }
    }
    return -1;
}

template <std::size_t N>
auto InitializePhysicalDeviceInfos(const SVkInstance& instance, const std::array<const char*, N>& physicalDeviceExtensions, std::vector<SPhysicalDeviceInfo>& physicalDevices) noexcept -> int32_t
{
    if (instance.instance == VK_NULL_HANDLE)
    {
        return -1;
    }
    uint32_t deviceCount{};
    if (!CheckVkResult(vkEnumeratePhysicalDevices(instance.instance, &deviceCount, VK_NULL_HANDLE)))
    {
        return -1;
    }
    std::vector<VkPhysicalDevice> devices(deviceCount, VK_NULL_HANDLE);
    if (!CheckVkResult(vkEnumeratePhysicalDevices(instance.instance, &deviceCount, devices.data())))
    {
        return -1;
    }

    physicalDevices.reserve(deviceCount);
    for (const auto& device : devices)
    {
        uint32_t extensionCount{};
        if (!CheckVkResult(vkEnumerateDeviceExtensionProperties(device, VK_NULL_HANDLE, &extensionCount, VK_NULL_HANDLE)))
        {
            return -1;
        }
        std::vector<VkExtensionProperties> extensions(extensionCount);
        if (!CheckVkResult(vkEnumerateDeviceExtensionProperties(device, VK_NULL_HANDLE, &extensionCount, extensions.data())))
        {
            return -1;
        }

        std::set<std::string, std::less<>> requiredExtensions(physicalDeviceExtensions.begin(), physicalDeviceExtensions.end());
        for (const auto& [extensionName, specVersion] : extensions)
        {
            requiredExtensions.erase(extensionName);
        }

        if (!requiredExtensions.empty())
        {
            continue;
        }

        uint32_t queueFamilyCount{};
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, VK_NULL_HANDLE);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        SPhysicalDeviceInfo deviceInfo{
            .queueFamilyProperties = std::move(queueFamilies),
            .device                = device,
        };
        vkGetPhysicalDeviceProperties(device, &deviceInfo.deviceProperties);
        vkGetPhysicalDeviceMemoryProperties(device, &deviceInfo.deviceMemoryProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceInfo.deviceFeatures);

        physicalDevices.emplace_back(std::move(deviceInfo));
    }
    return 0;
}
} // namespace DeerVulkan
