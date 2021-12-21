#pragma once
#include "Base.h"
#include <vulkan/vulkan.hpp>
namespace BalVulkan
{
    class CSemaphore final: public CDeviceObject
    {
    public:
        explicit CSemaphore( const CDevice* pDevice );
        CSemaphore( CSemaphore&& ) = default;
        CSemaphore& operator=( CSemaphore&& ) = default;
        ~CSemaphore() override;

        void Initialize();

        const VkSemaphore& Get() const;

        static CSemaphore* CreateNew( const CDevice* pDevice );
    private:
        VkSemaphore m_semaphore;
    };
}