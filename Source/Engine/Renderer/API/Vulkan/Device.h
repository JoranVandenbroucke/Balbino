#pragma once
#include "Base.h"
#include "Instance.h"

namespace DeerVulkan
{
    class CDeviceHolder
    {
    public:
        constexpr explicit CDeviceHolder() = default;
        constexpr virtual ~CDeviceHolder();
        constexpr CDeviceHolder( CDeviceHolder&& ) noexcept  = default;
        CDeviceHolder& operator=( CDeviceHolder&& ) noexcept = default;

    protected:
        VkDevice m_device { VK_NULL_HANDLE };
    };

    class CDevice final : public CDeviceHolder, public CRefCounted
    {
    public:
        constexpr explicit CDevice( SPhysicalDeviceInfo deviceInfo )
            : CDeviceHolder {}
            , CRefCounted {}
            , m_deviceInfo( std::move( deviceInfo ) )
        {
        }
        ~CDevice() override;

        void Initialize( const std::vector<const char*>& layersToEnable, const std::vector<const char*>& extensionsToEnable );
        inline void WaitIdle()
        {
            vkDeviceWaitIdle( m_device );
        }
        [[nodiscard]] constexpr const VkDevice& GetVkDevice() const
        {
            return m_device;
        }
        [[nodiscard]] constexpr const SPhysicalDeviceInfo& GetPhysicalDeviceInfo() const
        {
            return m_deviceInfo;
        }

    private:
        SPhysicalDeviceInfo m_deviceInfo {};
    };
}// namespace DeerVulkan
