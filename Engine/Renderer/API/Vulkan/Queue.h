#pragma once
#include <vulkan/vulkan.hpp>
#include "Base.h"

namespace FawnVision
{
    class CSemaphore;
    class CSwapchain;
    class CCommandPool;
    class CFence;
    class CQueue final : public CDeviceObject
    {
    public:
        explicit CQueue( const CDevice* pDevice );
        ~CQueue() override;
        
        void Initialize();
        void SubmitPass( CSemaphore* signalSemaphore, CSemaphore* waitableSemaphores, CCommandPool * cmdList, CFence* pFence );
        bool PresentToScreen( CSwapchain* pSwapchain, CSemaphore* signalSemaphore, uint32_t imageIndex );
        const VkQueue& GetHandle() const;
        uint32_t GetQueFamily() const;
        void WaitIdle() const;
    
    private:
        uint32_t m_queueFamily;
        VkQueue  m_queue;
    };
};
