//
// Copyright (c) 2024.
// Author: Joran
//

#pragma once
#include "../DeerVulkan_Core.hpp"

#include <SDL3/SDL_vulkan.h>

#include <array>
#include <vector>

namespace DeerVulkan
{
struct SVkInstance
{
    VkInstance instance{VK_NULL_HANDLE};
#ifdef BALBINO_DEBUG
    VkDebugUtilsMessengerEXT debugMessenger{VK_NULL_HANDLE};
#endif
};

#ifdef BALBINO_DEBUG
VKAPI_ATTR inline VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSevirity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void*)
{
    std::string_view type{};
    switch (messageType)
    {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: type = "General"; break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT : type = "Validation"; break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT : type = "Performance"; break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT: type = "Device Address Binding"; break;
    default: break;
    }
    switch (messageSevirity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    std::cout << type << " Verbose: "<< pCallbackData->pMessage << std::endl;
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    std::cout << type << " Info: "<< pCallbackData->pMessage << std::endl;
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    std::clog << type << " Warning: "<< pCallbackData->pMessage << std::endl;
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    std::cerr << type << " Error: "<< pCallbackData->pMessage << std::endl;
        break;
    default: break;
    }

    return VK_FALSE;
}

inline auto InitializeDebugLayerCallback(SVkInstance& instance) noexcept -> int32_t
{
    if (instance.instance == VK_NULL_HANDLE)
    {
        return -1;
    }
    constexpr VkDebugUtilsMessengerCreateInfoEXT createInfo{
        .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext           = VK_NULL_HANDLE,
        .flags           = 0,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
        .pfnUserCallback = DebugCallback,
        .pUserData       = nullptr,
    };
    if (const auto pCreateCallback = std::bit_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance.instance, "vkCreateDebugUtilsMessengerEXT"));
        pCreateCallback == VK_NULL_HANDLE || instance.debugMessenger != VK_NULL_HANDLE || !CheckVkResult(pCreateCallback(instance.instance, &createInfo, VK_NULL_HANDLE, &instance.debugMessenger)))
    {
        return -1;
    }
    return 0;
}
#endif

inline auto InitializeInstance(SVkInstance& instance, const std::string_view appName, const uint32_t appVersion, const char* const* const pExtensions, const uint32_t extensionsCount) noexcept -> int32_t
{
    if (instance.instance != VK_NULL_HANDLE)
    {
        return -1;
    }
#ifdef BALBINO_DEBUG
    constexpr std::array layers{"VK_LAYER_KHRONOS_validation"};
    std::vector extensions{VK_KHR_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
#else
    constexpr std::array<const char*, 0> layers;
    std::vector extensions{VK_KHR_SURFACE_EXTENSION_NAME};
#endif
    for (uint32_t i{}; i < extensionsCount; ++i)
    {
        if (const char* const pExt{pExtensions[i]}; std::ranges::find_if(extensions,
                                                                         [pExt](const char* const pExtension)
                                                                         {
                                                                             return std::strcmp(pExt, pExtension) == 0;
                                                                         })
            == extensions.cend())
        {
            extensions.emplace_back(pExt);
        }
    }

    const VkApplicationInfo applicationInfo{
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext              = VK_NULL_HANDLE,
        .pApplicationName   = appName.data(),
        .applicationVersion = appVersion,
        .pEngineName        = "Balbino",
        .engineVersion      = VK_MAKE_VERSION(0, 2, 0),
        .apiVersion         = VK_API_VERSION_1_3,
    };
    VkInstanceCreateInfo createInfo{
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext                   = VK_NULL_HANDLE,
        .flags                   = 0,
        .pApplicationInfo        = &applicationInfo,
        .enabledLayerCount       = static_cast<uint32_t>(layers.size()),
        .ppEnabledLayerNames     = layers.empty() ? VK_NULL_HANDLE : layers.data(),
        .enabledExtensionCount   = static_cast<uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.empty() ? VK_NULL_HANDLE : extensions.data(),
    };
    VkResult result{vkCreateInstance(&createInfo, VK_NULL_HANDLE, &instance.instance)};
    if (result == VK_ERROR_LAYER_NOT_PRESENT)
    {
        createInfo.enabledLayerCount   = 0;
        createInfo.ppEnabledLayerNames = VK_NULL_HANDLE;
        result                         = vkCreateInstance(&createInfo, VK_NULL_HANDLE, &instance.instance);
    }
    return result == VK_SUCCESS ? 0 : -1;
}

inline auto Initialize(SVkInstance& instance) noexcept -> int32_t
{
    uint32_t extensionCount{};
    if (const char* const* const pExtensions{SDL_Vulkan_GetInstanceExtensions(&extensionCount)}; InitializeInstance(instance, "Deer", VK_MAKE_VERSION(0, 0, 0), pExtensions, extensionCount) != 0)
    {
        return -1;
    }
#ifdef BALBINO_DEBUG
    if (InitializeDebugLayerCallback(instance) != 0)
    {
        return -1;
    }
#endif
    return 0;
}

inline void Cleanup(SVkInstance& instance) noexcept
{
#ifdef BALBINO_DEBUG
    if (instance.debugMessenger != VK_NULL_HANDLE)
    {
        if (const auto pDestroyCallback{std::bit_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance.instance, "vkDestroyDebugUtilsMessengerEXT"))})
        {
            pDestroyCallback(instance.instance, instance.debugMessenger, VK_NULL_HANDLE);
            instance.debugMessenger = VK_NULL_HANDLE;
        }
    }
#endif
    if (instance.instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(instance.instance, VK_NULL_HANDLE);
        instance.instance = VK_NULL_HANDLE;
    }
}
} // namespace DeerVulkan