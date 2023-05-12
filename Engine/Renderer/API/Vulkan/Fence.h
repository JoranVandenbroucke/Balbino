#pragma once
#include <vulkan/vulkan.hpp>
#include "Base.h"
#include "Device.h"

namespace FawnVision
{
    class CFence final : public CDeviceObject
    {
    public:
        explicit CFence( const CDevice* pDevice );
        ~CFence() override;
        
        void Initialize();
        VkFence Get() const;
        void Wait() const;
        void Reset() const;
    private:
        VkFence m_fence;
    };
}
