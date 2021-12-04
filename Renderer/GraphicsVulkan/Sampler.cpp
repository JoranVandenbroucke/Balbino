#include "pch.h"
#include "Sampler.h"

#include "Device.h"
#include "Funtions.h"
#include "Instance.h"

BalVulkan::CSampler::CSampler( const CDevice* pDevice )
	: CDeviceObject{ pDevice }
	, m_sampler{ VK_NULL_HANDLE }
{
}

BalVulkan::CSampler::~CSampler()
{
	vkDestroySampler( GetDevice()->GetVkDevice(), m_sampler, nullptr );
}

void BalVulkan::CSampler::Initialize( uint32_t mipLevels, EFilter filter, ESamplerAddressMode samplerAddressMode )
{
	if ( m_sampler )
		return;

	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties( GetDevice()->GetPhysicalDeviceInfo()->device, &properties );

	//VkPhysicalDeviceFeatures supportedFeatures;
	//vkGetPhysicalDeviceFeatures( GetDevice()->GetPhysicalDeviceInfo()->device, &supportedFeatures );


	const VkSamplerCreateInfo samplerInfo{
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.pNext = VK_NULL_HANDLE,
		.flags = 0,
		.magFilter = static_cast< VkFilter >( filter ),
		.minFilter = static_cast< VkFilter >( filter ),
		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.addressModeU = static_cast< VkSamplerAddressMode >( samplerAddressMode ),
		.addressModeV = static_cast< VkSamplerAddressMode >( samplerAddressMode ),
		.addressModeW = static_cast< VkSamplerAddressMode >( samplerAddressMode ),
		.mipLodBias = 0.0f,
		//.anisotropyEnable = supportedFeatures.samplerAnisotropy,
		//.maxAnisotropy = supportedFeatures.samplerAnisotropy == VK_TRUE? properties.limits.maxSamplerAnisotropy : 1,
		.anisotropyEnable = false,
		.maxAnisotropy = 1,
		.compareEnable = VK_FALSE,
		.compareOp = VK_COMPARE_OP_ALWAYS,
		.minLod = 0.0f,
		.maxLod = static_cast< float >( mipLevels ),
		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,
	};
	CheckVkResult( vkCreateSampler( GetDevice()->GetVkDevice(), &samplerInfo, nullptr, &m_sampler ), "failed to create texture sampler!" );
}

VkSampler BalVulkan::CSampler::GetSampler() const
{
	return m_sampler;
}

BalVulkan::CSampler* BalVulkan::CSampler::CreateNew( const CDevice* pDevice )
{
	return new CSampler{pDevice};
}

void BalVulkan::CSampler::Destroy()
{
	CDeviceObject::Destroy();
}
