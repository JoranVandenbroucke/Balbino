//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include "API/Vulkan/DeerVulkan.hpp"

#include <cstddef>
#include <functional>
#include <vector>

export module FawnVision.RenderGraph;
import FawnAlgebra.Arithmetics;
import FawnVision.Enum;
import FawnVision.Texture;
import FawnVision.Renderer;
import FawnVision.RenderPass;
import FawnVision.RenderPassContext;
using namespace FawnAlgebra;

namespace FawnVision
{
#pragma region defenition
export struct SRenderGraph;
export struct IRenderPassHandle;
export template <class PassData>
void SetRenderFunc(SRenderGraph& renderGraph, IRenderPassHandle& handle, std::function<void(const PassData*, const SRenderPassContext&)>&& renderFunc) noexcept;
export template <class PassData>
constexpr IRenderPassHandle AddComputeRenderPass(SRenderGraph& renderGraph, PassData*& passData) noexcept;
export template <class PassData>
constexpr IRenderPassHandle AddRasterRenderPass(SRenderGraph& renderGraph, PassData*& passData) noexcept;
export constexpr void EnableRenderPass(SRenderGraph& renderGraph, const IRenderPassHandle& handle) noexcept;
export constexpr void DisableRenderPass(SRenderGraph& renderGraph, const IRenderPassHandle& handle) noexcept;
export constexpr void SetRenderTarget(SRenderGraph& renderGraph, const IRenderPassHandle& handle, STexture& color, STexture& depth) noexcept;
export constexpr void SetRenderColorTarget(SRenderGraph& renderGraph, const IRenderPassHandle& handle, STexture& color) noexcept;
export constexpr void SetRenderDepthTarget(SRenderGraph& renderGraph, const IRenderPassHandle& handle, STexture& depth) noexcept;
export constexpr void ExecuteAll(SRenderer& renderer, SRenderGraph& renderGraph) noexcept;
export constexpr void CleanupRenderGraph(SRenderGraph& renderGraph) noexcept;

constexpr void PreRenderPass(const SRenderPassContext& renderPassContext, const SRenderPassBase* renderPass) noexcept;
inline void PostRenderPass(const SRenderPassContext& renderPassContext, const SRenderPassBase* renderPass) noexcept;
#pragma endregion

#pragma region implementation
struct SRenderGraph
{
    std::vector<SRenderPassBase*> renderPasses;
    std::vector<SRenderPassBase*> compiledRenderPasses;
    bool dirty{};
};
struct IRenderPassHandle
{
    const DeerVulkan::SVkCommandBuffer* commandBuffer{nullptr};
    std::size_t renderPassIdx;
};

template <class PassData>
void SetRenderFunc(SRenderGraph& renderGraph, IRenderPassHandle& handle, std::function<void(const PassData*, const SRenderPassContext&)>&& renderFunc) noexcept
{
    SRenderPass<PassData>* renderPass = std::bit_cast<SRenderPass<PassData>*>(renderGraph.renderPasses[handle.renderPassIdx]);
    renderPass->renderFunction.swap(renderFunc);
}

template <class PassData>
constexpr IRenderPassHandle AddComputeRenderPass(SRenderGraph& renderGraph, PassData*& passData) noexcept
{
    // todo : implement proper memory management
    auto* renderPass = new SRenderPass<PassData>{};
    auto* renderData = new PassData{};

    Initialize(renderPass, true, static_cast<int>(renderGraph.renderPasses.size()), renderData);
    passData = renderPass->data;

    renderGraph.renderPasses.emplace_back(renderPass);
    renderGraph.dirty = true;
    return IRenderPassHandle{nullptr, renderPass};
}

template <class PassData>
constexpr IRenderPassHandle AddRasterRenderPass(SRenderGraph& renderGraph, PassData*& passData) noexcept
{
    // todo : implement proper memory management
    auto* renderPass = new SRenderPass<PassData>{};
    auto* renderData = new PassData{};

    Initialize(renderPass, true, static_cast<int>(renderGraph.renderPasses.size()), renderData);
    passData = renderPass->data;

    renderGraph.renderPasses.emplace_back(renderPass);
    renderGraph.dirty = true;
    return IRenderPassHandle{nullptr, renderPass->index};
}

constexpr void EnableRenderPass(SRenderGraph& renderGraph, const IRenderPassHandle& handle) noexcept
{
    renderGraph.renderPasses[handle.renderPassIdx]->isEnabled = false;
    renderGraph.dirty                                         = true;
}
constexpr void DisableRenderPass(SRenderGraph& renderGraph, const IRenderPassHandle& handle) noexcept
{
    renderGraph.renderPasses[handle.renderPassIdx]->isEnabled = true;
    renderGraph.dirty                                         = true;
}
constexpr void SetRenderTarget(SRenderGraph& renderGraph, const IRenderPassHandle& handle, STexture& color, STexture& depth) noexcept
{
    SetRenderColorTarget(renderGraph, handle, color);
    SetRenderDepthTarget(renderGraph, handle, depth);
    renderGraph.dirty = true;
}
constexpr void SetRenderColorTarget(SRenderGraph& renderGraph, const IRenderPassHandle& handle, STexture& color) noexcept
{
    renderGraph.renderPasses[handle.renderPassIdx]->colorImage = &color.image;
    renderGraph.dirty                                          = true;
}
constexpr void SetRenderDepthTarget(SRenderGraph& renderGraph, const IRenderPassHandle& handle, STexture& depth) noexcept
{
    renderGraph.renderPasses[handle.renderPassIdx]->colorImage = &depth.image;
    renderGraph.dirty                                          = true;
}

constexpr void ExecuteAll(SRenderer& renderer, SRenderGraph& renderGraph) noexcept
{
    if (renderGraph.dirty)
    {
        renderGraph.compiledRenderPasses.clear();
        for (SRenderPassBase* renderPass : renderGraph.renderPasses)
        {
            if (renderPass->isEnabled)
            {
                renderGraph.compiledRenderPasses.emplace_back(renderPass);
            }
        }
        std::ranges::sort(renderGraph.compiledRenderPasses,
                          [](const SRenderPassBase* pA, const SRenderPassBase* pB)
                          {
                              return pA->index < pB->index;
                          });
        renderGraph.dirty = false;
    }
    for (std::size_t i{}; i < renderGraph.compiledRenderPasses.size(); ++i)
    {
        SRenderPassBase* renderPass{renderGraph.compiledRenderPasses[i]};
        const SRenderPassContext renderContext{
            .device            = renderer.device,
            .commandBuffer     = renderer.commandBuffer,
            .swapChain         = renderer.swapChain,
            .timelineSemaphore = renderer.timelineSemaphore,
            .binarySemaphore   = renderer.binarySemaphore,
            .queue             = &renderer.queue[g_presentQueueId],
        };
        PreRenderPass(renderContext, renderPass);
        renderPass->Execute(renderContext);
        PostRenderPass(renderContext, renderPass);
    }
}
constexpr void CleanupRenderGraph(SRenderGraph& renderGraph) noexcept
{
    for (const SRenderPassBase* renderPass : renderGraph.renderPasses)
    {
        delete renderPass;
    }
    renderGraph.renderPasses.clear();
}

constexpr void PreRenderPass(const SRenderPassContext& renderPassContext, const SRenderPassBase* renderPass) noexcept
{
    if (!renderPass)
    {
        return;
    }
    if (BeginCommand(renderPassContext.commandBuffer) != 0)
    {
        return;
    }
    DeerVulkan::SVkImage* color               = renderPass->colorImage;
    DeerVulkan::SVkImage* depth               = renderPass->depthImage;
    const DeerVulkan::SVkImageView* colorView = renderPass->colorImageView;
    const DeerVulkan::SVkImageView* depthView = renderPass->depthImageView;
    if (!color && !depth)
    {
        NextImage(renderPassContext.device, renderPassContext.swapChain, renderPassContext.binarySemaphore);
        color     = &CurrentImage(renderPassContext.swapChain);
        colorView = &CurrentImageView(renderPassContext.swapChain);
    }
    if (color)
    {
        TransitionImageLayout(renderPassContext.commandBuffer, *color, static_cast<uint32>(image_layout::color_attachment_optimal));
    }
    if (depth)
    {
        TransitionImageLayout(renderPassContext.commandBuffer, *depth, static_cast<uint32>(image_layout::depth_stencil_attachment_optimal));
    }

    BeginRender(renderPassContext.commandBuffer,
                DeerVulkan::RenderParams{.colorImageView = colorView,
                                         .depthImageView = depthView,
                                         .clearColor     = {0, 0, 0},
                                         .depthClear     = 0,
                                         .xOffset        = 0,
                                         .yOffset        = 0,
                                         .width          = renderPassContext.swapChain.extent.width,
                                         .height         = renderPassContext.swapChain.extent.height,
                                         .stencilClear   = 0});
}
inline void PostRenderPass(const SRenderPassContext& renderPassContext, const SRenderPassBase* renderPass) noexcept
{
    EndRender(renderPassContext.commandBuffer);

    DeerVulkan::SVkImage* color = renderPass->colorImage;
    DeerVulkan::SVkImage* depth = renderPass->depthImage;

    const bool isSwapChain{!color && !depth};
    if (isSwapChain)
    {
        DeerVulkan::SVkImage& swapChainImage{CurrentImage(renderPassContext.swapChain)};
        TransitionImageLayout(renderPassContext.commandBuffer, swapChainImage, static_cast<uint32>(image_layout::present_src_khr));
    }
    else
    {
        if (color)
        {
            TransitionImageLayout(renderPassContext.commandBuffer, *color, static_cast<uint32>(image_layout::shader_read_only_optimal));
        }
        if (depth)
        {
            TransitionImageLayout(renderPassContext.commandBuffer, *depth, static_cast<uint32>(image_layout::depth_stencil_attachment_optimal));
        }
    }

    // End the command buffer and handle possible failure
    if (EndCommand(renderPassContext.commandBuffer) != 0)
    {
        return;
    }

    // Determine the wait and signal values for synchronization
    const bool wait{(renderPass->syncMode & SYNC_WAIT) != 0};
    const bool signal{(renderPass->syncMode & SYNC_SIGNAL) != 0};

    if (wait)
    {
        if (signal)
        {
            if (QueueSubmit(*renderPassContext.queue, renderPassContext.commandBuffer, renderPassContext.timelineSemaphore, renderPass->waitValue, renderPass->signalValue) != 0)
            {
                return;
            }
        }
        else
        {
            if (QueueSubmit(*renderPassContext.queue, renderPassContext.commandBuffer, renderPassContext.timelineSemaphore, renderPass->waitValue, true) != 0)
            {
                return;
            }
        }
    }
    else if (signal)
    {
        if (QueueSubmit(*renderPassContext.queue, renderPassContext.commandBuffer, renderPassContext.timelineSemaphore, renderPass->signalValue, false) != 0)
        {
            return;
        }
    }
    else
    {
        if (QueueSubmit(*renderPassContext.queue, renderPassContext.commandBuffer) != 0)
        {
            return;
        }
    }

    // Present to screen if no colour and depth image is provided
    if (isSwapChain)
    {
        if (Present(*renderPassContext.queue, renderPassContext.swapChain, renderPassContext.binarySemaphore) != 0)
        {
            return;
        }

        NextFrame(renderPassContext.swapChain);
    }
    WaitIdle(*renderPassContext.queue);
}
#pragma endregion
} // namespace FawnVision