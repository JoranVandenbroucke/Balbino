#include "Sampler.h"

#include "Device.h"
#include "Funtions.h"

FawnVision::CSampler::CSampler( const CDevice* pDevice )
        : CDeviceObject{ pDevice }
          , m_sampler{ VK_NULL_HANDLE }
{
}

FawnVision::CSampler::~CSampler()
{
    vkDestroySampler( GetDevice()->GetVkDevice(), m_sampler, nullptr );
}

void FawnVision::CSampler::Initialize( int filterMode, int mipmapMode, ESamplerAddressMode::Enum samplerAddressModeU, ESamplerAddressMode::Enum samplerAddressModeV, ESamplerAddressMode::Enum samplerAddressModeW, int anisotropy, uint32_t mipLevels )
{
    if ( m_sampler )
    {
        return;
    }
    
    const VkSamplerCreateInfo samplerInfo{
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, .pNext = VK_NULL_HANDLE, .flags = 0, .magFilter = static_cast<VkFilter>( filterMode ), .minFilter = static_cast<VkFilter>( mipmapMode ), .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR, .addressModeU = static_cast<VkSamplerAddressMode>( samplerAddressModeU ), .addressModeV = static_cast<VkSamplerAddressMode>( samplerAddressModeV ), .addressModeW = static_cast<VkSamplerAddressMode>( samplerAddressModeW ), .mipLodBias = 0.0f, .anisotropyEnable = GetDevice()->GetPhysicalDeviceInfo().deviceFeatures.samplerAnisotropy, .maxAnisotropy = float(
                    ( GetDevice()->GetPhysicalDeviceInfo().deviceFeatures.samplerAnisotropy == VK_TRUE )
                    ? (( anisotropy > 0 )
                       ? anisotropy
                       : GetDevice()->GetPhysicalDeviceInfo().GetMaxUsableSampleCount())
                    : 1
            ), .compareEnable = VK_FALSE, .compareOp = VK_COMPARE_OP_ALWAYS, .minLod = 0.0f, .maxLod = static_cast<float>( mipLevels ), .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK, .unnormalizedCoordinates = VK_FALSE,
    };
    CheckVkResult(
            vkCreateSampler( GetDevice()->GetVkDevice(), &samplerInfo, nullptr, &m_sampler ),
            "failed to create texture sampler!"
    );
}

VkSampler FawnVision::CSampler::GetSampler() const
{
    return m_sampler;
}

void FawnVision::CSampler::Destroy()
{
    CDeviceObject::Destroy();
}
