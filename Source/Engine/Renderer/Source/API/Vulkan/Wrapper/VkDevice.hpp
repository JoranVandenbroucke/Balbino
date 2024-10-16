//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "../DeerVulkan_Core.hpp"

#include "VkPhysicalDevice.hpp"

#include <array>
#include <cstddef>
#include <vector>

namespace DeerVulkan
{
struct SVkDevice
{
    VkDevice device{VK_NULL_HANDLE};
    VkDescriptorPool descriptorPool{VK_NULL_HANDLE};
    SPhysicalDeviceInfo deviceInfo;
    SQueueFamily queueFamily;
};

inline auto Initialize(const SVkInstance& instance, const SVkSurface& surface, SVkDevice& device) noexcept -> int32_t
{
    if (instance.instance == VK_NULL_HANDLE || surface.surface == VK_NULL_HANDLE)
    {
        return -1;
    }
    if (device.device == VK_NULL_HANDLE)
    {
#ifdef BALBINO_DEBUG
        constexpr std::array layers{"VK_LAYER_KHRONOS_validation"};
        constexpr std::array extensions{VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME, VK_EXT_SHADER_OBJECT_EXTENSION_NAME, VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME,
                                        VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,      VK_KHR_SWAPCHAIN_EXTENSION_NAME,     VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
                                        VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME};
#else
        constexpr std::array<const char*, 0> layers;
        constexpr std::array extensions{VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME, VK_EXT_SHADER_OBJECT_EXTENSION_NAME, VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME,
                                        VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,      VK_KHR_SWAPCHAIN_EXTENSION_NAME,     VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
                                        VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME};
#endif

        std::vector<SPhysicalDeviceInfo> physicalDevices;
        if (InitializePhysicalDeviceInfos(instance, extensions, physicalDevices) != 0)
        {
            return -1;
        }

        if (FindQueueFamily(surface, physicalDevices, device.queueFamily) != 0)
        {
            return -1;
        }

        device.deviceInfo = physicalDevices[device.queueFamily.presentFamily];

        VkPhysicalDeviceShaderObjectFeaturesEXT enabledShaderObjectFeatures{
            .sType        = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_OBJECT_FEATURES_EXT,
            .pNext        = VK_NULL_HANDLE,
            .shaderObject = VK_TRUE,
        };
        VkPhysicalDeviceSynchronization2FeaturesKHR synchronization2Features{
            .sType            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR,
            .pNext            = &enabledShaderObjectFeatures,
            .synchronization2 = VK_TRUE,
        };
        VkPhysicalDeviceTimelineSemaphoreFeaturesKHR timelineSemaphoreFeatures{
            .sType             = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR,
            .pNext             = &synchronization2Features,
            .timelineSemaphore = VK_TRUE,
        };
        const VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeature{
            .sType            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
            .pNext            = &timelineSemaphoreFeatures,
            .dynamicRendering = VK_TRUE,
        };

        constexpr VkPhysicalDeviceFeatures deviceFeatures{
            .robustBufferAccess                      = VK_FALSE,
            .fullDrawIndexUint32                     = VK_FALSE,
            .imageCubeArray                          = VK_FALSE,
            .independentBlend                        = VK_FALSE,
            .geometryShader                          = VK_FALSE,
            .tessellationShader                      = VK_FALSE,
            .sampleRateShading                       = VK_TRUE,
            .dualSrcBlend                            = VK_FALSE,
            .logicOp                                 = VK_FALSE,
            .multiDrawIndirect                       = VK_FALSE,
            .drawIndirectFirstInstance               = VK_FALSE,
            .depthClamp                              = VK_FALSE,
            .depthBiasClamp                          = VK_FALSE,
            .fillModeNonSolid                        = VK_FALSE,
            .depthBounds                             = VK_FALSE,
            .wideLines                               = VK_FALSE,
            .largePoints                             = VK_FALSE,
            .alphaToOne                              = VK_FALSE,
            .multiViewport                           = VK_FALSE,
            .samplerAnisotropy                       = VK_TRUE,
            .textureCompressionETC2                  = VK_FALSE,
            .textureCompressionASTC_LDR              = VK_FALSE,
            .textureCompressionBC                    = VK_FALSE,
            .occlusionQueryPrecise                   = VK_FALSE,
            .pipelineStatisticsQuery                 = VK_FALSE,
            .vertexPipelineStoresAndAtomics          = VK_FALSE,
            .fragmentStoresAndAtomics                = VK_FALSE,
            .shaderTessellationAndGeometryPointSize  = VK_FALSE,
            .shaderImageGatherExtended               = VK_FALSE,
            .shaderStorageImageExtendedFormats       = VK_FALSE,
            .shaderStorageImageMultisample           = VK_FALSE,
            .shaderStorageImageReadWithoutFormat     = VK_FALSE,
            .shaderStorageImageWriteWithoutFormat    = VK_FALSE,
            .shaderUniformBufferArrayDynamicIndexing = VK_FALSE,
            .shaderSampledImageArrayDynamicIndexing  = VK_FALSE,
            .shaderStorageBufferArrayDynamicIndexing = VK_FALSE,
            .shaderStorageImageArrayDynamicIndexing  = VK_FALSE,
            .shaderClipDistance                      = VK_FALSE,
            .shaderCullDistance                      = VK_FALSE,
            .shaderFloat64                           = VK_FALSE,
            .shaderInt64                             = VK_FALSE,
            .shaderInt16                             = VK_FALSE,
            .shaderResourceResidency                 = VK_FALSE,
            .shaderResourceMinLod                    = VK_FALSE,
            .sparseBinding                           = VK_FALSE,
            .sparseResidencyBuffer                   = VK_FALSE,
            .sparseResidencyImage2D                  = VK_FALSE,
            .sparseResidencyImage3D                  = VK_FALSE,
            .sparseResidency2Samples                 = VK_FALSE,
            .sparseResidency4Samples                 = VK_FALSE,
            .sparseResidency8Samples                 = VK_FALSE,
            .sparseResidency16Samples                = VK_FALSE,
            .sparseResidencyAliased                  = VK_FALSE,
            .variableMultisampleRate                 = VK_FALSE,
            .inheritedQueries                        = VK_FALSE,
        };
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::vector queueFamilies{device.queueFamily.graphicsFamily};
        std::vector<std::vector<float>> queuePriorties{{1.0F, 1.0F}};
        if (device.queueFamily.graphicsFamily != device.queueFamily.presentFamily)
        {
            queueFamilies.emplace_back(device.queueFamily.presentFamily);
            queuePriorties.push_back({1.0F});
        }
        else
        {
            queuePriorties.back().emplace_back(1.0F);
        }
        for (std::size_t i{}; i < queueFamilies.size(); i++)
        {
            queueCreateInfos.emplace_back(VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, VK_NULL_HANDLE, 0, static_cast<uint32_t>(device.queueFamily.graphicsFamily), static_cast<uint32_t>(queuePriorties[i].size()), queuePriorties[i].data());
        }

        if (const VkDeviceCreateInfo createInfo{
                .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .pNext                   = &dynamicRenderingFeature,
                .flags                   = 0,
                .queueCreateInfoCount    = static_cast<uint32_t>(queueCreateInfos.size()),
                .pQueueCreateInfos       = queueCreateInfos.data(),
                .enabledLayerCount       = static_cast<uint32_t>(layers.size()),
                .ppEnabledLayerNames     = layers.data(),
                .enabledExtensionCount   = static_cast<uint32_t>(extensions.size()),
                .ppEnabledExtensionNames = extensions.data(),
                .pEnabledFeatures        = &deviceFeatures,
            };
            !CheckVkResult(vkCreateDevice(device.deviceInfo.device, &createInfo, VK_NULL_HANDLE, &device.device)))
        {
            return -1;
        }
    }
    if (device.descriptorPool != VK_NULL_HANDLE)
    {
        return -1;
    }
    constexpr uint32_t poolSize{1024U};
    constexpr std::array poolSizes{
        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_SAMPLER, .descriptorCount = poolSize},
        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = poolSize},
        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, .descriptorCount = poolSize},
        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, .descriptorCount = poolSize},
        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, .descriptorCount = poolSize},
        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, .descriptorCount = poolSize},
        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = poolSize},
        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, .descriptorCount = poolSize},
        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, .descriptorCount = poolSize},
        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, .descriptorCount = poolSize},
        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, .descriptorCount = poolSize},
    };

    if (const VkDescriptorPoolCreateInfo descriptorPoolInfo{
            .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext         = VK_NULL_HANDLE,
            .flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            .maxSets       = 64U,
            .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
            .pPoolSizes    = poolSizes.data(),
        };
        !CheckVkResult(vkCreateDescriptorPool(device.device, &descriptorPoolInfo, VK_NULL_HANDLE, &device.descriptorPool)))
    {
        return -1;
    }
    return 0;
}
[[nodiscard]] inline auto FindMemoryType(const SVkDevice& device, const uint32_t memoryType, const uint32_t memoryProperty) noexcept -> uint32_t
{
    for ( uint32_t i = 0; i < device.deviceInfo.deviceMemoryProperties.memoryTypeCount; ++i )
        {
            if ( ( memoryType & ( 1 << i ) ) && ( device.deviceInfo.deviceMemoryProperties.memoryTypes[ i ].propertyFlags & memoryProperty ) == memoryProperty )
            {
                return i;
            }
        }
    return 0;
}
inline void Cleanup(SVkDevice& device) noexcept
{
    if (device.device == VK_NULL_HANDLE)
    {
        return;
    }
    if (device.descriptorPool != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(device.device, device.descriptorPool, VK_NULL_HANDLE);
        device.descriptorPool = VK_NULL_HANDLE;
    }
    vkDeviceWaitIdle(device.device);
    vkDestroyDevice(device.device, VK_NULL_HANDLE);
    device.device = VK_NULL_HANDLE;
}
} // namespace DeerVulkan