//
// Copyright (c) 2024.
// Author: Joran.
//

export module DepthOnlyPass;
import FawnAlgebra.arithmetics;
import FawnVision.Enum;
import FawnVision.RenderGraph;
import FawnVision.RenderPassContext;
import FawnVision.Shader;
import FawnVision.Texture;
using namespace FawnAlgebra;

namespace FawnVision
{
#pragma region defenition
export struct DepthOnlyPass;
export inline void Initialize(SRenderGraph& renderGraph, DepthOnlyPass& depthOnlyPass);
#pragma endregion

#pragma region implementation
struct DepthPassData
{
    STexture depthTexture;
    SShader depthShader;
};
struct DepthOnlyPass
{
    DepthPassData* data;
    IRenderPassHandle renderPassHandle;
};
inline void Initialize(SRenderGraph& renderGraph, DepthOnlyPass& depthOnlyPass)
{
    if (depthOnlyPass.data != nullptr)
    {
        return;
    }

    depthOnlyPass.renderPassHandle = AddRasterRenderPass<DepthPassData>(renderGraph, depthOnlyPass.data);
    SetRenderFunc<DepthPassData>(renderGraph, depthOnlyPass.renderPassHandle,
                  [](const DepthPassData* data, const SRenderPassContext& ctx) noexcept
                  {
                      SetViewport(ctx, 0, 0, 1920, 1080);
                      SetScissor(ctx, 1920, 1080, 0, 0);
                      SetDepthTestEnable(ctx, true);
                      SetDepthWriteEnable(ctx, true);
                      SetColorWriteMask(ctx, static_cast<color_component>(0)); // Disable color writing

                      BindShader(ctx, data->depthShader);

                      // todo : get renderlist of meshes.

                      // Draw(ctx, indexCount, 0, instanceCount, 0);
                  });
}
#pragma endregion
} // namespace FawnVision