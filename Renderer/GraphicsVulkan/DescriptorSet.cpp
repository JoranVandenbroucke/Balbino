#include "pch.h"
#include "DescriptorSet.h"

#include "Device.h"
#include "Funtions.h"
#include "ImageResource.h"
#include "ShaderPipeline.h"

BalVulkan::CDescriptorSet::CDescriptorSet( const CDevice* pDevice )
	: CDeviceObject{ pDevice }
	, m_descriptorSet{}
	, m_descriptorPool{ VK_NULL_HANDLE }
{
}

void BalVulkan::CDescriptorSet::Initialize( const CShaderPipeline* pShaderPipeline, const std::vector<SDescriptorSet>& descriptorSetsInfo )
{
	std::vector<VkDescriptorPoolSize> poolSizes{};
	poolSizes.reserve( descriptorSetsInfo.size() );
	for ( const SDescriptorSet& i : descriptorSetsInfo )
	{
		if ( i.descriptorType == SDescriptorSet::EType::Buffer )
			poolSizes.emplace_back( VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 );
		else
			poolSizes.emplace_back( VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 );
	}

	const VkDescriptorPoolCreateInfo poolInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.maxSets = 3,
		.poolSizeCount = static_cast< uint32_t >( poolSizes.size() ),
		.pPoolSizes = poolSizes.data(),
	};
	if ( vkCreateDescriptorPool( GetDevice()->GetVkDevice(), &poolInfo, nullptr, &m_descriptorPool ) != VK_SUCCESS )
	{
		throw std::runtime_error( "failed to create descriptor pool!" );
	}

	const std::vector layouts( 1u, pShaderPipeline->GetDescriptorSetLayout() );
	VkDescriptorSetAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = m_descriptorPool,
		.descriptorSetCount = 1u,
		.pSetLayouts = layouts.data(),
	};

	if ( vkAllocateDescriptorSets( GetDevice()->GetVkDevice(), &allocInfo, &m_descriptorSet ) != VK_SUCCESS )
	{
		throw std::runtime_error( "failed to allocate descriptor sets!" );
	}

	std::vector<VkWriteDescriptorSet> descriptorWrites( descriptorSetsInfo.size() );
	for ( uint32_t i = 0; i < ( uint32_t ) descriptorSetsInfo.size(); ++i )
	{
		if ( descriptorSetsInfo[i].descriptorType == SDescriptorSet::EType::Buffer )
		{
			const VkDescriptorBufferInfo info{
				.buffer = descriptorSetsInfo[i].unition.buffer.buffer->GetBuffer(),
				.offset = 0,
				.range = descriptorSetsInfo[i].unition.buffer.buffer->GetRange(),
			};
			descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[i].dstSet = m_descriptorSet;
			descriptorWrites[i].dstBinding = i;
			descriptorWrites[i].dstArrayElement = 0;
			descriptorWrites[i].descriptorCount = 1;
			descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[i].pBufferInfo = &info;
		}
		else
		{
			const VkDescriptorImageInfo info{
				.sampler = descriptorSetsInfo[i].unition.image.sampler->GetSampler(),
				.imageView = descriptorSetsInfo[i].unition.image.view->GetImageView(),
				.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			};
			descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[i].dstSet = m_descriptorSet;
			descriptorWrites[i].dstBinding = i;
			descriptorWrites[i].dstArrayElement = 0;
			descriptorWrites[i].descriptorCount = 1;
			descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[i].pImageInfo = &info;
		}
	}

	vkUpdateDescriptorSets( GetDevice()->GetVkDevice(), static_cast< uint32_t >( descriptorWrites.size() ), descriptorWrites.data(), 0, nullptr );
}

void BalVulkan::CDescriptorSet::Initialize( const VkDescriptorPool& descriptorPool, const VkDescriptorSetLayout& descriptorSetLayout, CSampler* pSampler, CImageView* pImageView, CImageResource* pResource )
{
	if ( m_descriptorPool || descriptorPool == VK_NULL_HANDLE || descriptorSetLayout == VK_NULL_HANDLE || !pSampler || !pImageView || !pResource )
		return;
	m_descriptorPool = descriptorPool;
	// Create Descriptor Set:
	{
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &descriptorSetLayout;
		CheckVkResult( vkAllocateDescriptorSets(GetDevice()->GetVkDevice(), &allocInfo, &m_descriptorSet));
	}
	//VkDescriptorSetAllocateInfo allocInfo{
	//.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
	//.descriptorPool = m_descriptorPool,
	//.descriptorSetCount = 1,
	//.pSetLayouts = &descriptorSetLayout,
	//};
	//CheckVkResult(vkAllocateDescriptorSets( GetDevice()->GetVkDevice(), &allocInfo, &m_descriptorSet ));

	// Update the Descriptor Set:
	{
		VkDescriptorImageInfo descImage[1] = {};
		descImage[0].sampler = pSampler->GetSampler();
		descImage[0].imageView = pImageView->GetImageView();
		descImage[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		VkWriteDescriptorSet writeDesc[1] = {};
		writeDesc[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDesc[0].dstSet = m_descriptorSet;
		writeDesc[0].descriptorCount = 1;
		writeDesc[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeDesc[0].pImageInfo = descImage;
		vkUpdateDescriptorSets( GetDevice()->GetVkDevice(), 1, writeDesc, 0, NULL );
	}
	//VkDescriptorImageInfo fontDescriptor{
	//	.sampler = pSampler->GetSampler(),
	//	.imageView = pImageView->GetImageView(),
	//	.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
	//};
	//const std::vector writeDescriptorSets
	//{
	//	VkWriteDescriptorSet{
	//		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
	//		.dstSet = m_descriptorSet,
	//		.dstBinding = 0,
	//		.descriptorCount = 1,
	//		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
	//		.pImageInfo = &fontDescriptor,
	//	}
	//};
	//vkUpdateDescriptorSets( GetDevice()->GetVkDevice(), static_cast< uint32_t >( writeDescriptorSets.size() ), writeDescriptorSets.data(), 0, nullptr );
}

const VkDescriptorSet& BalVulkan::CDescriptorSet::GetDescriptorSet() const
{
	return m_descriptorSet;
}

BalVulkan::CDescriptorSet* BalVulkan::CDescriptorSet::CreateNew( const CDevice* pDevice )
{
	return new CDescriptorSet{ pDevice };
}
