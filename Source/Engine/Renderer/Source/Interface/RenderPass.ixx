//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include "API/Vulkan/Wrapper/VkImage.hpp"
#include "API/Vulkan/Wrapper/VkImageView.hpp"

#include <functional>

export module FawnVision.RenderPass;
import FawnAlgebra.Arithmetics;
import FawnVision.RenderPassContext;
using namespace FawnAlgebra;

namespace FawnVision
{
#pragma region defenition
export struct SRenderPassBase;

export template <class PassData>
struct SRenderPass;

export template <class PassData>
constexpr void Initialize(SRenderPass<PassData>* renderPass, bool isComputePass, int passIndex, PassData* passData) noexcept;
export template <class PassData>
constexpr void Execute(SRenderPass<PassData>* renderPass, const SRenderPassContext& renderContext) noexcept;
export template <class PassData>
constexpr void Release(SRenderPass<PassData>* renderPass) noexcept;

export constexpr uint8 SYNC_NONE        = 0;
export constexpr uint8 SYNC_WAIT        = 1;
export constexpr uint8 SYNC_SIGNAL      = 2;
export constexpr uint8 SYNC_WAIT_SIGNAL = 3;
#pragma endregion

#pragma region implementation
struct SRenderPassBase
{
    virtual ~SRenderPassBase() = default;

    virtual void Execute(const SRenderPassContext&) = 0;

    DeerVulkan::SVkImage* colorImage{nullptr};
    DeerVulkan::SVkImage* depthImage{nullptr};
    DeerVulkan::SVkImageView* colorImageView{nullptr};
    DeerVulkan::SVkImageView* depthImageView{nullptr};
    uint64 waitValue{};
    uint64 signalValue{};
    uint32 index{};
    uint8 syncMode{};
    bool isCompute{false};
    bool isEnabled{true};
};

template <class PassData>
struct SRenderPass final : SRenderPassBase
{
    ~SRenderPass() override = default;
    std::function<void(const PassData*, const SRenderPassContext&)> renderFunction{};
    PassData* data{nullptr};
    void Execute(const SRenderPassContext& renderContext) override
    {
        if (data && renderFunction)
        {
            renderFunction(data, renderContext);
        }
    }
};

template <class PassData>
constexpr void Initialize(SRenderPass<PassData>* renderPass, const bool isComputePass, const int passIndex, PassData* passData) noexcept
{
    renderPass->isCompute = isComputePass;
    renderPass->index     = passIndex;
    renderPass->data      = passData;
}
template <class PassData>
constexpr void Execute(SRenderPass<PassData>* renderPass, const SRenderPassContext& renderContext) noexcept
{
    if (renderPass && renderPass->renderFunction)
    {
        renderPass->renderFunction(renderPass->data, renderContext);
    }
}
template <class PassData>
constexpr void Release(SRenderPass<PassData>* renderPass) noexcept
{
    renderPass->allowGlobalState            = false;
    renderPass->allowPassCulling            = true;
    renderPass->allowRendererListCulling    = true;
    renderPass->enableAsyncCompute          = false;
    renderPass->enableFoveatedRasterization = false;
    renderPass->index                       = -1;
}
#pragma endregion
} // namespace FawnVision
