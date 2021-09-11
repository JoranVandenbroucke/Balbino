#include "pch.h"
#include "TextureSampler.h"

Balbino::CTextureSampler::CTextureSampler()
	: m_textureImageView{ VK_NULL_HANDLE }
	, m_textureSampler{ VK_NULL_HANDLE }
	, m_descriptorSetLayout{}
	, m_descriptorPoolSize{}
{
}

Balbino::CTextureSampler::~CTextureSampler() {}

void Balbino::CTextureSampler::Initialize( const VkDevice& device, const VkAllocationCallbacks* pCallbacks, uint32_t mipLevels)
{

	const VkSamplerCreateInfo samplerInfo{
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.magFilter = VK_FILTER_LINEAR,
		.minFilter = VK_FILTER_LINEAR,
		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.mipLodBias = 0.0f,
		.anisotropyEnable = VK_FALSE,
		.maxAnisotropy = 0,
		.compareEnable = VK_FALSE,
		.compareOp = VK_COMPARE_OP_LESS,
		.minLod = 0.0f,
		.maxLod = static_cast<float>(mipLevels),
		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,
	};
	if (vkCreateSampler(device, &samplerInfo, pCallbacks, &m_textureSampler) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture sampler!");
	}

	m_descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
}

void Balbino::CTextureSampler::Cleanup( const VkDevice& device, const VkAllocationCallbacks* pCallbacks )
{
	vkDestroySampler( device, m_textureSampler, pCallbacks );
	vkDestroyImageView( device, m_textureImageView, pCallbacks );
}

const VkDescriptorSetLayoutBinding& Balbino::CTextureSampler::GetDescriptorLayoutBinding()
{
	m_descriptorSetLayout.binding = 1;
	m_descriptorSetLayout.descriptorCount = 1;
	m_descriptorSetLayout.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	m_descriptorSetLayout.pImmutableSamplers = nullptr;
	m_descriptorSetLayout.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	return m_descriptorSetLayout;
}

const VkDescriptorPoolSize& Balbino::CTextureSampler::GetDescriptorPoolSize(uint32_t size)
{
	m_descriptorPoolSize.descriptorCount = size;
	return m_descriptorPoolSize;
}

const VkSampler& Balbino::CTextureSampler::GetSampler()
{
	return m_textureSampler;
}
