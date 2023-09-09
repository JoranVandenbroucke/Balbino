#pragma once
#include "Base.h"
#include "Device.h"
#include "Vulkan/Vulkan.hpp"

namespace DeerVulkan
{
    class CFence final : public CDeviceObject
    {
    public:
        constexpr explicit CFence( FawnVision::Device device )
            : CDeviceObject { device }
        {
        }
        ~CFence() override;
        void Initialize();
        constexpr VkFence Get() const
        {
            return m_fence;
        }
        void Wait() const;
        void Reset() const;

    private:
        VkFence m_fence { VK_NULL_HANDLE };
    };
}// namespace DeerVulkan
