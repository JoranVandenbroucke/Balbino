//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include "API/Vulkan/DeerVulkan.hpp"

#include <array>
export module FawnVision.Renderer;
import FawnAlgebra.Arithmetics;
import FawnVision.Enum;
import FawnVision.Window;
using namespace FawnAlgebra;

namespace FawnVision
{
#pragma region defenition
export struct SRenderer;

export inline auto CreateRenderer(const SWindow& window, SRenderer& renderer) noexcept -> int32;
export inline void ReleaseRenderer(SRenderer& renderer) noexcept;
export inline auto RecreateRenderer(const SWindow& window, SRenderer& renderer) noexcept -> int32;
#pragma endregion

#pragma region implementation
struct SRenderer
{
    DeerVulkan::SVkInstance instance;
    DeerVulkan::SVkSurface surface;
    DeerVulkan::SVkDevice device;

    std::array<DeerVulkan::SVkQueue, g_queueCount> queue;
    DeerVulkan::SVkSwapChain swapChain;
    DeerVulkan::SVkSemaphore timelineSemaphore;
    DeerVulkan::SVkSemaphore binarySemaphore;
    DeerVulkan::SVkFence fence;
    DeerVulkan::SVkCommandPool commandPool;
    DeerVulkan::SVkCommandBuffer commandBuffer;
};
inline auto InitializeComponents(const SWindow& window, SRenderer& renderer) -> int32
{
    if (Initialize(renderer.device, renderer.queue[g_graphicsQueueId], renderer.device.queueFamily.graphicsFamily, 0U) != 0)
    {
        return -1;
    }
    if (Initialize(renderer.device, renderer.queue[g_computeQueueId], renderer.device.queueFamily.graphicsFamily, 1U) != 0)
    {
        return -1;
    }
    if (Initialize(renderer.device, renderer.queue[g_presentQueueId], renderer.device.queueFamily.presentFamily, renderer.device.queueFamily.graphicsFamily == renderer.device.queueFamily.presentFamily ? 2U : 0U) != 0)
    {
        return -1;
    }
    if (Initialize(renderer.device, renderer.surface, renderer.swapChain, window.width, window.height) != 0)
    {
        return -1;
    }
    if (Initialize(renderer.device, renderer.timelineSemaphore, true) != 0)
    {
        return -1;
    }
    if (Initialize(renderer.device, renderer.binarySemaphore, false) != 0)
    {
        return -1;
    }
    if (Initialize(renderer.device, renderer.commandPool, renderer.device.queueFamily.graphicsFamily) != 0)
    {
        return -1;
    }
    if (CreateCommandBuffer(renderer.instance, renderer.device, renderer.commandPool, renderer.commandBuffer, 1U) != 0)
    {
        return -1;
    }
    return 0;
}
inline void CleanupComponents(SRenderer& renderer) noexcept
{
    Cleanup(renderer.device, renderer.commandPool, renderer.commandBuffer);
    Cleanup(renderer.device, renderer.commandPool);
    Cleanup(renderer.device, renderer.binarySemaphore);
    Cleanup(renderer.device, renderer.timelineSemaphore);
    Cleanup(renderer.device, renderer.swapChain);
    Cleanup(renderer.queue[g_presentQueueId]);
    Cleanup(renderer.queue[g_computeQueueId]);
    Cleanup(renderer.queue[g_graphicsQueueId]);
}

inline auto CreateRenderer(const SWindow& window, SRenderer& renderer) noexcept -> int32
{
    if (Initialize(renderer.instance) != 0)
    {
        return -1;
    }
    if (Initialize(window.pWindow, renderer.instance, renderer.surface) != 0)
    {
        return -1;
    }
    if (Initialize(renderer.instance, renderer.surface, renderer.device) != 0)
    {
        return -1;
    }
    if (InitializeComponents(window, renderer) != 0)
    {
        return -1;
    }
    return 0;
}
inline void ReleaseRenderer(SRenderer& renderer) noexcept
{
    CleanupComponents(renderer);
    Cleanup(renderer.device);
    Cleanup(renderer.instance, renderer.surface);
    Cleanup(renderer.instance);
}
inline auto RecreateRenderer(const SWindow& window, SRenderer& renderer) noexcept -> int32
{
    CleanupComponents(renderer);
    if (InitializeComponents(window, renderer) != 0)
    {
        return -1;
    }
    return 0;
}
#pragma endregion
} // namespace FawnVision
