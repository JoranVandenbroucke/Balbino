#pragma once
#include "DeerVulkanCore.h"
#include <atomic>

namespace DeerVulkan
{
    class CRefCounted
    {
    public:
        constexpr CRefCounted() = default;
        constexpr CRefCounted( CRefCounted&& ) noexcept
        {
            /* refcount not copied on purpose*/
        }

        constexpr CRefCounted& operator=( CRefCounted&& ) noexcept
        {
            /* refcount not copied on purpose*/
            return *this;
        }
        constexpr CRefCounted( CRefCounted& other ) noexcept            = delete;
        constexpr CRefCounted& operator=( CRefCounted& other ) noexcept = delete;

        constexpr virtual ~CRefCounted() = default;

        inline virtual uint32_t AddRef() final
        {
            return m_refCount.fetch_add( 1 );
        }
        constexpr virtual uint32_t Release() final
        {
            if ( this == BALBINO_NULL )
            {
                return 0;
            }
            if ( m_refCount.fetch_sub( 1 ) == 1 )
            {
                Destroy();
            }
            return m_refCount;
        }

        inline bool IsDeletable() const
        {
            return m_refCount == 0;
        }
        inline bool IsUniquelyOwned() const
        {
            return m_refCount == 1;
        }

    protected:
        constexpr virtual void Destroy()
        {
            delete this;
        }

    private:
        std::atomic<uint32_t> m_refCount { 1 };
    };

    class CDeviceObject : public CRefCounted
    {
    public:
        constexpr explicit CDeviceObject( FawnVision::Device pDevice )
            : m_device { pDevice }
        {
        }
        constexpr CDeviceObject( CDeviceObject&& ) noexcept            = default;
        constexpr CDeviceObject& operator=( CDeviceObject&& ) noexcept = default;
        constexpr ~CDeviceObject() override                            = default;

        constexpr FawnVision::Device GetDevice() const
        {
            return m_device;
        }

    private:
        FawnVision::Device m_device { BALBINO_NULL };
    };
}// namespace DeerVulkan
