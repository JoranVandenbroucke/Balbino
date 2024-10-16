//
// Copyright (c) 2024.
// Author: Joran.
//

module;

#include "API/Vulkan/DeerVulkan.hpp"
#include <vector>

export module FawnVision.Shader;
import FawnAlgebra.Arithmetics;
import FawnVision.Enum;
import FawnVision.Renderer;
import FawnVision.Descriptor;
using namespace FawnAlgebra;

namespace FawnVision
{
#pragma region defenition
struct SShaderData;
export using ShaderCreateInfo = std::vector<SShaderData>;
export struct SShader;

export inline auto CreateShader(const SRenderer& renderer, const ShaderCreateInfo& shaderCreateInfo, SShader& shader) noexcept -> int32;
export inline auto CreateShader(const SRenderer& renderer, const ShaderCreateInfo& shaderCreateInfo, const SDescriptor& descriptor, SShader& shader) noexcept -> int32;
export inline void Cleanup(const SRenderer& renderer, SShader& shader) noexcept;
#pragma endregion

#pragma region implementation
struct SShaderData
{
    shader_stage stage;
    std::vector<uint8> code;
};
struct SShader
{
    DeerVulkan::SVkShader shader;
};

inline auto CreateShader(const SRenderer& renderer, const ShaderCreateInfo& shaderCreateInfo, SShader& shader) noexcept -> int32
{
    std::vector<uint32> stages(shaderCreateInfo.size(), 0);
    std::vector<std::vector<uint8>> codes(shaderCreateInfo.size());

    for (std::size_t i = 0; i < shaderCreateInfo.size(); ++i)
    {
        stages[i] = static_cast<uint32>(shaderCreateInfo[i].stage);
        codes[i]  = shaderCreateInfo[i].code;
    }

    if (Initialize(renderer.device, stages, codes, shader.shader) != 0)
    {
        return -1;
    }
    return 0;
}
inline auto CreateShader(const SRenderer& renderer, const ShaderCreateInfo& shaderCreateInfo, const SDescriptor& descriptor, SShader& shader) noexcept -> int32
{
    std::vector<uint32> stages(shaderCreateInfo.size(), 0);
    std::vector<std::vector<uint8>> codes(shaderCreateInfo.size());

    for (std::size_t i = 0; i < shaderCreateInfo.size(); ++i)
    {
        stages[i] = static_cast<uint32>(shaderCreateInfo[i].stage);
        codes[i]  = shaderCreateInfo[i].code;
    }

    if (Initialize(renderer.device, stages, codes, shader.shader, descriptor.descriptor) != 0)
    {
        return -1;
    }
    return 0;
}
inline void Cleanup(const SRenderer& renderer, SShader& shader) noexcept
{
    Cleanup(renderer.device, shader.shader);
}
#pragma endregion
} // namespace FawnVision