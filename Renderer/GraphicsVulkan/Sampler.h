#pragma once

#include "Base.h"
#include "Common.h"

#include <vulkan/vulkan.hpp>

namespace BalVulkan
{
    class CSampler final : public CDeviceObject
    {
    public:
        explicit CSampler( const CDevice* pDevice );

        CSampler( CSampler&& ) = default;

        CSampler& operator =( CSampler&& ) = default;

        ~CSampler() override;

        void Initialize( int filterMode, int mipmapMode, ESamplerAddressMode samplerAddressModeU, ESamplerAddressMode samplerAddressModeV, ESamplerAddressMode samplerAddressModeW, int anisotropy, uint32_t mipLevels );

        VkSampler GetSampler() const;

        static CSampler* CreateNew( const CDevice* pDevice );

    private:
        void Destroy() override; // Uses CDevice::DeferDestruction

        VkSampler m_sampler;
    };
}
