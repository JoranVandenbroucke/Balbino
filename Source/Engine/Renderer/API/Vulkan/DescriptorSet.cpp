#include "DescriptorSet.h"

#include "Device.h"

#include "Buffer.h"
#include "ImageView.h"
#include "Sampler.h"
#include "ShaderPipeline.h"

DeerVulkan::CDescriptorSet::~CDescriptorSet()
{
    vkDestroyDescriptorPool( GetDevice()->GetVkDevice(), m_descriptorPool, VK_NULL_HANDLE );
}

void DeerVulkan::CDescriptorSet::Initialize( FawnVision::ShaderPipeline shaderPipeline, const std::vector<FawnVision::SDescriptorSet>& descriptorSetsInfo )
{
    shaderPipeline->AddRef();

    std::vector<VkDescriptorPoolSize> poolSizes {};
    poolSizes.reserve( descriptorSetsInfo.size() );
    for ( const FawnVision::SDescriptorSet& descSetInfo : descriptorSetsInfo )
    {
        VkDescriptorType descriptorType;

        switch ( descSetInfo.descriptorType )
        {
            case FawnVision::SDescriptorSet::type::buffer: descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; break;
            case FawnVision::SDescriptorSet::type::dynamic_buffer: descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC; break;
            case FawnVision::SDescriptorSet::type::image: descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; break;
            case FawnVision::SDescriptorSet::type::buffer_storage: descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; break;
            default: continue;// Skip unknown descriptor types
        }

        auto it = std::ranges::find_if( poolSizes, [ descriptorType ]( const VkDescriptorPoolSize& poolSize ) {
            return poolSize.type == descriptorType;
        } );

        if ( it != poolSizes.cend() )
        {
            it->descriptorCount += 1;
        }
        else
        {
            poolSizes.emplace_back( descriptorType, 1 );
        }
    }

    const VkDescriptorPoolCreateInfo poolInfo {
        .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets       = 3,
        .poolSizeCount = static_cast<uint32_t>( poolSizes.size() ),
        .pPoolSizes    = poolSizes.empty() ? VK_NULL_HANDLE : poolSizes.data(),
    };
    CheckVkResult( vkCreateDescriptorPool( GetDevice()->GetVkDevice(), &poolInfo, VK_NULL_HANDLE, &m_descriptorPool ), "failed to create descriptor pool!" );

    const std::vector layouts( 1u, shaderPipeline->GetDescriptorSetLayout() );
    VkDescriptorSetAllocateInfo allocInfo {
        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool     = m_descriptorPool,
        .descriptorSetCount = 1u,
        .pSetLayouts        = layouts.data(),
    };
    CheckVkResult( vkAllocateDescriptorSets( GetDevice()->GetVkDevice(), &allocInfo, &m_descriptorSet ), "failed to allocate descriptor sets!" );

    std::vector<VkWriteDescriptorSet> descriptorWrites( descriptorSetsInfo.size() );
    std::vector<VkDescriptorBufferInfo> bufferInfos( descriptorSetsInfo.size() );
    std::vector<VkDescriptorImageInfo> imageInfos( descriptorSetsInfo.size() );
    for ( uint32_t i = 0; i < static_cast<uint32_t>( descriptorSetsInfo.size() ); ++i )
    {
        descriptorWrites[ i ].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[ i ].dstSet          = m_descriptorSet;
        descriptorWrites[ i ].dstBinding      = descriptorSetsInfo[ i ].binding;
        descriptorWrites[ i ].dstArrayElement = 0;
        descriptorWrites[ i ].descriptorCount = 1;
        descriptorWrites[ i ].pBufferInfo     = &bufferInfos[ i ];

        if ( descriptorSetsInfo[ i ].descriptorType == FawnVision::SDescriptorSet::type::buffer )
        {
            bufferInfos[ i ].buffer = descriptorSetsInfo[ i ].description.buffer.buffer->GetBuffer();
            bufferInfos[ i ].offset = 0;
            bufferInfos[ i ].range  = descriptorSetsInfo[ i ].description.buffer.buffer->GetRange();

            descriptorWrites[ i ].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        }
        else if ( descriptorSetsInfo[ i ].descriptorType == FawnVision::SDescriptorSet::type::dynamic_buffer )
        {
            bufferInfos[ i ].buffer = descriptorSetsInfo[ i ].description.buffer.buffer->GetBuffer();
            bufferInfos[ i ].offset = 0;
            bufferInfos[ i ].range  = descriptorSetsInfo[ i ].description.buffer.buffer->GetRange();

            descriptorWrites[ i ].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        }
        else if ( descriptorSetsInfo[ i ].descriptorType == FawnVision::SDescriptorSet::type::buffer_storage )
        {
            bufferInfos[ i ].buffer = descriptorSetsInfo[ i ].description.buffer.buffer->GetBuffer();
            bufferInfos[ i ].offset = 0;
            bufferInfos[ i ].range  = descriptorSetsInfo[ i ].description.buffer.buffer->GetRange();

            descriptorWrites[ i ].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        }
        else
        {
            imageInfos[ i ].sampler   = descriptorSetsInfo[ i ].description.image.sampler->GetSampler();
            imageInfos[ i ].imageView = descriptorSetsInfo[ i ].description.image.view->GetImageView();

            descriptorWrites[ i ].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[ i ].pImageInfo     = &imageInfos[ i ];
        }
    }

    vkUpdateDescriptorSets( GetDevice()->GetVkDevice(), static_cast<uint32_t>( descriptorWrites.size() ), descriptorWrites.data(), 0, VK_NULL_HANDLE );

    shaderPipeline->Release();
}
