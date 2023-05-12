#pragma once

#include "Base.h"
#include "Common.h"

#include <vulkan/vulkan.hpp>

namespace FawnVision
{
    class CShaderPipeline;
    class CDescriptorSet final : public CDeviceObject
    {
    public:
        explicit CDescriptorSet( const CDevice* pDevice );
        ~CDescriptorSet() override;
        
        void Initialize( CShaderPipeline* pShaderPipeline, const std::vector<SDescriptorSet>& descriptorSetsInfo );
        const VkDescriptorSet& GetDescriptorSets() const;
    
    private:
        VkDescriptorSet  m_descriptorSet;
        VkDescriptorPool m_descriptorPool;
    };
}
