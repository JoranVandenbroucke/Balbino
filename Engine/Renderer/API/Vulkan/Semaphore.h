#pragma once
#include <vulkan/vulkan.hpp>
#include "Base.h"

namespace FawnVision
{
    class CSemaphore final : public CDeviceObject
    {
    public:
        explicit CSemaphore( const CDevice* pDevice );
        ~CSemaphore() override;
        
        void Initialize();
        const VkSemaphore& Get() const;
    private:
        VkSemaphore m_semaphore;
    };
}
