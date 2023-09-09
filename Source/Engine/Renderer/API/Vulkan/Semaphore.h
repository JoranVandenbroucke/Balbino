#pragma once
#include "Base.h"
#include <vulkan/vulkan.hpp>

namespace DeerVulkan
{
    class CSemaphore final : public CDeviceObject
    {
    public:
        constexpr explicit CSemaphore( FawnVision::Device device )
            : CDeviceObject { device }
        {
        }
        ~CSemaphore() override;
        void Initialize();
        constexpr const VkSemaphore& Get() const
        {
            return m_semaphore;
        }

    private:
        VkSemaphore m_semaphore { VK_NULL_HANDLE };
    };
}// namespace DeerVulkan
