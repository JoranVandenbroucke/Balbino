#pragma once

#include <unordered_map>
#include <vector>

#include "FawnVisionCore.h"
#include <UUID.h>

namespace FawnVision
{
    class CRenderer;
    class CMaterial final
    {
    public:
        CMaterial(CUuid uuid, FawnVision::CommandPool pool);
        
        constexpr ~CMaterial() = default;
        constexpr CMaterial(const CMaterial&) = delete;
        constexpr CMaterial(CMaterial&&) = default;
        constexpr CMaterial& operator=(const CMaterial&) = delete;
        constexpr CMaterial& operator=(CMaterial&&) = delete;
        
        void Initialize( FawnVision::ShaderPipeline pShaderPipeline, const FawnVision::SMaterialCreateInfo& createInfo, FawnVision::Device device);
        void Cleanup() const;
        void Bind() const;

        [[nodiscard]] const std::vector<FawnVision::SShaderResource>& GetShaderResourcesVector() const;
        inline const CUuid& GetId() const noexcept{return m_uuid;}
        inline CUuid GetId() noexcept{return m_uuid;}

    private:
        FawnVision::DescriptorSet  m_descriptorSet { BALBINO_NULL };
        FawnVision::ShaderPipeline m_shaderPipeline { BALBINO_NULL };
        FawnVision::CommandPool    m_commandPool { BALBINO_NULL };
        
        const CUuid m_uuid {};
    };
}
