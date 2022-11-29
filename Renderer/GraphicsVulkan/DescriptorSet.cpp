#include "DescriptorSet.h"

#include "Device.h"
#include "Funtions.h"
#include "ImageResource.h"
#include "ShaderPipeline.h"

BalVulkan::CDescriptorSet::CDescriptorSet( const CDevice* pDevice )
        : CDeviceObject{ pDevice },
          m_descriptorSet{},
          m_dynamicCount{},
          m_descriptorPool{ VK_NULL_HANDLE }
{
}

void BalVulkan::CDescriptorSet::Initialize( const CShaderPipeline* pShaderPipeline, const std::vector<SDescriptorSet>& descriptorSetsInfo )
{
    std::vector<VkDescriptorPoolSize> poolSizes{};
    poolSizes.reserve( descriptorSetsInfo.size());
    for ( const SDescriptorSet& i : descriptorSetsInfo )
    {
        if ( i.descriptorType == SDescriptorSet::EType::Buffer )
        {
            const auto& it = std::find_if( poolSizes.begin(), poolSizes.end(),
                                           []( const VkDescriptorPoolSize& poolSize )
                                           {
                                               return poolSize.type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                                           } );
            if ( it != poolSizes.cend())
            {
                it->descriptorCount += 1;
            }
            else
            {
                poolSizes.emplace_back( VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 );
            }
        }
        else if ( i.descriptorType == SDescriptorSet::EType::DynamicBuffer )
        {
            const auto& it = std::find_if( poolSizes.begin(), poolSizes.end(),
                                           []( const VkDescriptorPoolSize& poolSize )
                                           {
                                               return poolSize.type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
                                           } );
            if ( it != poolSizes.cend())
            {
                it->descriptorCount += 1;
            }
            else
            {
                poolSizes.emplace_back( VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1 );
            }
        }
        else if ( i.descriptorType == SDescriptorSet::EType::Image )
        {
            const auto& it = std::find_if( poolSizes.begin(), poolSizes.end(),
                                           []( const VkDescriptorPoolSize& poolSize )
                                           {
                                               return poolSize.type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                                           } );
            if ( it != poolSizes.cend())
            {
                it->descriptorCount += 1;
            }
            else
            {
                poolSizes.emplace_back( VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 );
            }
        }
        else if ( i.descriptorType == SDescriptorSet::EType::BufferStorage )
        {
            const auto& it = std::find_if( poolSizes.begin(), poolSizes.end(),
                                           []( const VkDescriptorPoolSize& poolSize )
                                           {
                                               return poolSize.type == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                                           } );
            if ( it != poolSizes.cend())
            {
                it->descriptorCount += 1;
            }
            else
            {
                poolSizes.emplace_back( VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1 );
            }
        }
    }
    
    const VkDescriptorPoolCreateInfo poolInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .maxSets = 3,
            .poolSizeCount = static_cast< uint32_t >( poolSizes.size()),
            .pPoolSizes = poolSizes.empty()? nullptr : poolSizes.data(),
    };
    CheckVkResult( vkCreateDescriptorPool( GetDevice()->GetVkDevice(), &poolInfo, nullptr, &m_descriptorPool ),
                   "failed to create descriptor pool!" );
    
    const std::vector           layouts( 1u, pShaderPipeline->GetDescriptorSetLayout());
    VkDescriptorSetAllocateInfo allocInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool = m_descriptorPool,
            .descriptorSetCount = 1u,
            .pSetLayouts = layouts.data(),
    };
    CheckVkResult( vkAllocateDescriptorSets( GetDevice()->GetVkDevice(), &allocInfo, &m_descriptorSet ),
                   "failed to allocate descriptor sets!" );
    
    std::vector<VkWriteDescriptorSet>   descriptorWrites( descriptorSetsInfo.size());
    std::vector<VkDescriptorBufferInfo> bufferInfos( descriptorSetsInfo.size());
    std::vector<VkDescriptorImageInfo>  imageInfos( descriptorSetsInfo.size());
    for ( uint32_t                      i = 0; i < static_cast< uint32_t >( descriptorSetsInfo.size()); ++i )
    {
        if ( descriptorSetsInfo[i].descriptorType == SDescriptorSet::EType::Buffer )
        {
            bufferInfos[i].buffer = descriptorSetsInfo[i].description.buffer.buffer->GetBuffer();
            bufferInfos[i].offset = 0;
            bufferInfos[i].range  = descriptorSetsInfo[i].description.buffer.buffer->GetRange();
            
            descriptorWrites[i].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[i].dstSet          = m_descriptorSet;
            descriptorWrites[i].dstBinding      = descriptorSetsInfo[i].binding;
            descriptorWrites[i].dstArrayElement = 0;
            descriptorWrites[i].descriptorCount = 1;
            descriptorWrites[i].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[i].pBufferInfo     = &bufferInfos[i];
        }
        else if ( descriptorSetsInfo[i].descriptorType == SDescriptorSet::EType::DynamicBuffer )
        {
            bufferInfos[i].buffer = descriptorSetsInfo[i].description.buffer.buffer->GetBuffer();
            bufferInfos[i].offset = 0;
            bufferInfos[i].range  = descriptorSetsInfo[i].description.buffer.buffer->GetRange();
            
            descriptorWrites[i].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[i].dstSet          = m_descriptorSet;
            descriptorWrites[i].dstBinding      = descriptorSetsInfo[i].binding;
            descriptorWrites[i].dstArrayElement = 0;
            descriptorWrites[i].descriptorCount = 1;
            descriptorWrites[i].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
            descriptorWrites[i].pBufferInfo     = &bufferInfos[i];
        }
        else if ( descriptorSetsInfo[i].descriptorType == SDescriptorSet::EType::BufferStorage )
        {
            bufferInfos[i].buffer = descriptorSetsInfo[i].description.buffer.buffer->GetBuffer();
            bufferInfos[i].offset = 0;
            bufferInfos[i].range  = descriptorSetsInfo[i].description.buffer.buffer->GetRange();
            
            descriptorWrites[i].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[i].dstSet          = m_descriptorSet;
            descriptorWrites[i].dstBinding      = descriptorSetsInfo[i].binding;
            descriptorWrites[i].dstArrayElement = 0;
            descriptorWrites[i].descriptorCount = 1;
            descriptorWrites[i].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            descriptorWrites[i].pBufferInfo     = &bufferInfos[i];
        }
        else
        {
            imageInfos[i].sampler     = descriptorSetsInfo[i].description.image.sampler->GetSampler();
            imageInfos[i].imageView   = descriptorSetsInfo[i].description.image.view->GetImageView();
            imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            
            descriptorWrites[i].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[i].dstSet          = m_descriptorSet;
            descriptorWrites[i].dstBinding      = descriptorSetsInfo[i].binding;
            descriptorWrites[i].dstArrayElement = 0;
            descriptorWrites[i].descriptorCount = 1;
            descriptorWrites[i].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[i].pImageInfo      = &imageInfos[i];
        }
    }
    
    vkUpdateDescriptorSets( GetDevice()->GetVkDevice(), static_cast< uint32_t >( descriptorWrites.size()),
                            descriptorWrites.data(), 0, nullptr );
}

const VkDescriptorSet* BalVulkan::CDescriptorSet::GetDescriptorSets() const
{
    return &m_descriptorSet;
}

BalVulkan::CDescriptorSet* BalVulkan::CDescriptorSet::CreateNew( const CDevice* pDevice )
{
    return new CDescriptorSet{ pDevice };
}

uint32_t BalVulkan::CDescriptorSet::GetDynamicCount() const
{
    return m_dynamicCount;
}

uint32_t BalVulkan::CDescriptorSet::GetDescriptorSetCount() const
{
    return 1u;
}
