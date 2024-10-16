//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "../DeerVulkan_Core.hpp"

#include "VkInstance.hpp"

#include <SDL3/SDL_vulkan.h>

namespace DeerVulkan
{
struct SVkSurface
{
    VkSurfaceKHR surface{VK_NULL_HANDLE};
};
inline auto Initialize(SDL_Window* pWindow, const SVkInstance& instance, SVkSurface& surface) noexcept -> int32_t
{
    return SDL_Vulkan_CreateSurface(pWindow, instance.instance, VK_NULL_HANDLE, &surface.surface) ? 0 : -1;
}
inline void Cleanup(const SVkInstance& instance, SVkSurface surface) noexcept
{
    if (instance.instance != VK_NULL_HANDLE && surface.surface != VK_NULL_HANDLE)
    {
        SDL_Vulkan_DestroySurface(instance.instance, surface.surface, VK_NULL_HANDLE);
        surface.surface = VK_NULL_HANDLE;
    }
}
} // namespace DeerVulkan