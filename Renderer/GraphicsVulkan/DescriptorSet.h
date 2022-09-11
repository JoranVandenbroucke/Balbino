#pragma once

#include "Base.h"
#include "Common.h"

#include <vulkan/vulkan.hpp>

namespace BalVulkan
{
    class CImageResource;
    class CShaderPipeline;

    class CDescriptorSet final : public CDeviceObject
    {
    public:
        explicit CDescriptorSet(const CDevice* device);

        void Initialize(const CShaderPipeline* pShaderPipeline, const std::vector< SDescriptorSet >& descriptorSetsInfo);
        const VkDescriptorSet* GetDescriptorSets() const;

        static CDescriptorSet* CreateNew(const CDevice* pDevice);

        uint32_t GetDynamicCount() const;
        uint32_t GetDescriptorSetCount() const;

    private:
        uint32_t m_dynamicCount;

        VkDescriptorSet m_descriptorSet;
        VkDescriptorPool m_descriptorPool;
    };
}
