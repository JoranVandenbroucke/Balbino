#pragma once

#include "Base.h"
#include "RenderPass.h"
#include "Shader.h"

namespace DeerVulkan
{
    class CShaderPipeline final : public CDeviceObject
    {
    public:
        explicit CShaderPipeline( FawnVision::Device device )
        : CDeviceObject{device}
        {
        }
        ~CShaderPipeline() override;

        void Initialize( const FawnVision::SShaderPassCreateInfo& createInfo, const std::vector<FawnVision::Shader>& shaders, FawnVision::RenderPass renderPass, FawnVision::Swapchain pSwapchain );
        constexpr VkPipelineLayout GetPipelineLayout() const{return m_pipelineLayout;}
        constexpr const VkDescriptorSetLayout& GetDescriptorSetLayout() const{return m_descriptorSetLayout;}
        constexpr VkPipeline GetPipeline() noexcept{return m_pipeline;}
        constexpr const std::vector<FawnVision::SShaderResource>& GetShaderResources() const{return m_shaderResources;}

    private:
        VkDescriptorSetLayout m_descriptorSetLayout { VK_NULL_HANDLE };
        VkPipelineLayout m_pipelineLayout { VK_NULL_HANDLE };
        VkPipelineCache m_pipelineCache { VK_NULL_HANDLE };
        VkPipeline m_pipeline { VK_NULL_HANDLE };

        std::vector<FawnVision::SShaderResource> m_shaderResources{};
        std::unordered_map<uint32_t, std::vector<FawnVision::SShaderResource>> m_shaderSets{};

        constexpr static VkDescriptorType FindDescriptorType( FawnVision::shader_resource_type resourceType, bool dynamic );
    };
}// namespace DeerVulkan
