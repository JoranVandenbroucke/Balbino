#pragma once
#include <vulkan/vulkan.hpp>

#include "Base.h"
#include "Common.h"

namespace FawnVision
{
    class CSampler final : public CDeviceObject
    {
    public:
        explicit CSampler( const CDevice* pDevice );
        ~CSampler() override;
        
        void Initialize( int filterMode, int mipmapMode, ESamplerAddressMode::Enum samplerAddressModeU, ESamplerAddressMode::Enum samplerAddressModeV, ESamplerAddressMode::Enum samplerAddressModeW, int anisotropy, uint32_t mipLevels );
        
        VkSampler GetSampler() const;
    
    private:
        void Destroy() override; // Uses CDevice::DeferDestruction
        
        VkSampler m_sampler;
    };
}
