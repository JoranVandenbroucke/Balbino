#include "Sampler.h"

#include "Device.h"

DeerVulkan::CSampler::~CSampler()
{
    vkDestroySampler( GetDevice()->GetVkDevice(), m_sampler, VK_NULL_HANDLE );
}

void DeerVulkan::CSampler::Initialize( const FawnVision::SSamplerCreateInfo& createInfo )
{
     if(m_sampler)
    {
        return;
    }

    const VkSamplerCreateInfo samplerInfo {
        .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext                   = VK_NULL_HANDLE,
        .flags                   = 0,
        .magFilter               = (VkFilter)createInfo.magFilter,
        .minFilter               = (VkFilter)createInfo.minFilter,
        .mipmapMode              = (VkSamplerMipmapMode)createInfo.mipMapMode,
        .addressModeU            = (VkSamplerAddressMode)createInfo.addressModeU,
        .addressModeV            = (VkSamplerAddressMode)createInfo.addressModeV,
        .addressModeW            = (VkSamplerAddressMode)createInfo.addressModeW,
        .mipLodBias              = createInfo.mipLodBias,
        .anisotropyEnable        = GetDevice()->GetPhysicalDeviceInfo().deviceFeatures.samplerAnisotropy,
        .maxAnisotropy           = GetDevice()->GetPhysicalDeviceInfo().deviceFeatures.samplerAnisotropy == VK_TRUE ? createInfo.maxAnisotropy > 0 ? createInfo.maxAnisotropy : (float)GetDevice()->GetPhysicalDeviceInfo().GetMaxUsableSampleCount() : 1.f,
        .compareEnable           = createInfo.compareEnable,
        .compareOp               = VkCompareOp( createInfo.compareOperator ),
        .minLod                  = createInfo.minLod,
        .maxLod                  = createInfo.maxLod,
        .borderColor             = VkBorderColor( createInfo.borderColor ),
        .unnormalizedCoordinates = createInfo.unNormalizedCoordinates,
    };
    CheckVkResult( vkCreateSampler( GetDevice()->GetVkDevice(), &samplerInfo, VK_NULL_HANDLE, &m_sampler ), "failed to create texture sampler!" );
}