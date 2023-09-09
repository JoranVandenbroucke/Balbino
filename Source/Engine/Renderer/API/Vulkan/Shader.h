#pragma once

#include "Base.h"
#include "Vulkan/Vulkan.hpp"
#include <filesystem>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_reflect.hpp>
#include <unordered_set>

namespace DeerVulkan
{
    class CShader final : public CDeviceObject
    {
    public:
        constexpr explicit CShader( FawnVision::Device device )
            : CDeviceObject { device }
        {
        }
        constexpr CShader( const CShader& other )
            : CDeviceObject { other.GetDevice() }
            , m_shaderHandle { other.m_shaderHandle }
            , m_resources { other.m_resources }
        {
        }
        ~CShader() override;

        void Initialize( const FawnVision::SShaderCreateInfo& createInfo );
        constexpr const std::vector<FawnVision::SShaderResource>& GetShaderResources() const
        {
            return m_resources;
        }
        constexpr const VkShaderModule& GetShaderModule() const
        {
            return m_shaderHandle;
        }
        [[nodiscard]] constexpr uint8_t GetVertexComponents() const
        {
            return m_vertexComponents;
        }

    private:
        VkShaderModule m_shaderHandle { VK_NULL_HANDLE };
        std::vector<FawnVision::SShaderResource> m_resources {};
        uint8_t m_vertexComponents {};

        VkShaderModule CreateShaderModule( const std::vector<uint32_t>& data ) const;
        static VkShaderStageFlagBits ConvertShaderStage( const uint32_t type );
        void ExtractStageInputs( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept;
        void ExtractSubpassInputs( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept;
        void ExtractStageOutputs( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept;
        void ExtractSeparateImages( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept;
        void ExtractSampledImages( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept;
        void ExtractStorageItems( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept;
        void ExtractSeparateSamplers( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept;
        void ExtractStorageBuffers( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept;
        void ExtractUniformBuffers( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept;
    };
}// namespace DeerVulkan
