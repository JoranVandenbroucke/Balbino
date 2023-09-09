#pragma once
#include "Base.h"
#include "Vulkan/Vulkan.hpp"

namespace DeerVulkan
{
    class CQueue final : public CDeviceObject
    {
    public:
        constexpr explicit CQueue( FawnVision::Device device )
            : CDeviceObject { device }
        {
        }
        ~CQueue() override;
        void Initialize();
        void SubmitPass( FawnVision::Semaphore signalSemaphore, FawnVision::Semaphore waitableSemaphores, FawnVision::CommandPool cmdList, FawnVision::Fence fence ) noexcept;
        bool PresentToScreen( FawnVision::Swapchain swapchain, FawnVision::Semaphore signalSemaphore, uint32_t imageIndex ) noexcept;
        void WaitIdle() const;

        constexpr const VkQueue& GetHandle() const
        {
            return m_queue;
        }
        constexpr uint32_t GetQueFamily() const
        {
            return m_queueFamily;
        }

    private:
        uint32_t m_queueFamily {};
        VkQueue m_queue { VK_NULL_HANDLE };
    };
};// namespace DeerVulkan
