#pragma once
#include <vulkan/vulkan.hpp>
#include "Base.h"

namespace BalVulkan
{
    class CFence final : public CDeviceObject
    {
    public:
        explicit CFence( const CDevice* pDevice );
        CFence( CFence&& ) = default;
        CFence& operator=( CFence&& ) = default;
        ~CFence() override;
        void Initialize();
        VkFence Get() const;
        void Wait() const;
        void Reset() const;
        static CFence* CreateNew( const CDevice* pDevice );
    private:
        VkFence m_fence;
    };
}
