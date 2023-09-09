#pragma once
#include "Vulkan/Vulkan.hpp"

#include "Base.h"

namespace DeerVulkan
{
    class CSampler final : public CDeviceObject
    {
    public:
        constexpr explicit CSampler( FawnVision::Device device )
            : CDeviceObject { device }
        {
        }
        ~CSampler() override;
        void Initialize( const FawnVision::SSamplerCreateInfo& createInfo );
        constexpr VkSampler GetSampler() const
        {
            return m_sampler;
        }

    private:
        VkSampler m_sampler { VK_NULL_HANDLE };
    };
}// namespace DeerVulkan
