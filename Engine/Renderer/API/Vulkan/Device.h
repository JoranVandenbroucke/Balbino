#pragma once
#include "Base.h"

#include "Instance.h"

namespace FawnVision
{
    class CDeviceHolder
    {
    public:
        explicit CDeviceHolder();
        virtual ~CDeviceHolder();
        
        CDeviceHolder( CDeviceHolder&& ) noexcept = default;
        CDeviceHolder& operator=( CDeviceHolder&& ) noexcept = default;
    
    protected:
        VkDevice m_device;
    };
    
    class CDevice final : public CDeviceHolder, public CRefCounted
    {
    public:
        explicit CDevice( SPhysicalDeviceInfo deviceInfo );
        ~CDevice() override;
        
        void WaitIdle() const;
        void Initialize( const std::vector<const char*>& layersToEnable, const std::vector<const char*>& extensionsToEnable );
        
        const VkDevice& GetVkDevice() const;
        const SPhysicalDeviceInfo& GetPhysicalDeviceInfo() const;
    
    private:
        SPhysicalDeviceInfo m_deviceInfo;
    };
}
