#pragma once

#include "Base.h"

#include "Vulkan/Vulkan.hpp"

namespace DeerVulkan
{
    class CShaderPipeline;
    class CDescriptorSet final : public CDeviceObject
    {
    public:
        constexpr explicit CDescriptorSet( FawnVision::Device device )
            : CDeviceObject { device }
        {
        }
        ~CDescriptorSet() override;

        void Initialize( FawnVision::ShaderPipeline ShaderPipeline, const std::vector<FawnVision::SDescriptorSet>& descriptorSetsInfo );
        constexpr const VkDescriptorSet& GetDescriptorSets() const
        {
            return m_descriptorSet;
        }

    private:
        VkDescriptorPool m_descriptorPool { VK_NULL_HANDLE };
        VkDescriptorSet m_descriptorSet {};
    };
}// namespace DeerVulkan
