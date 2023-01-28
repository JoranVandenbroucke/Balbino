#pragma once

#include <atomic>

namespace BalVulkan
{
    class CDevice;
    
    class CRefCounted
    {
    public:
        CRefCounted();
        
        CRefCounted( CRefCounted&& ) noexcept
        {
            /* refcount not copied on purpose*/
        }
        
        CRefCounted& operator=( CRefCounted&& ) noexcept
        {
            /* refcount not copied on purpose*/
            return *this;
        }
        
        virtual ~CRefCounted() = default;
        
        virtual uint32_t AddRef() final;
        virtual uint32_t Release() final;
        
        bool IsDeletable() const;
        bool IsUniquelyOwned() const;
    
    protected:
        virtual void Destroy();
    
    private:
        std::atomic<uint32_t> m_refCount;
    };
    
    class CDeviceObject : public CRefCounted
    {
    public:
        CDeviceObject( const CDevice* device );
        CDeviceObject( CDeviceObject&& ) noexcept = default;
        CDeviceObject& operator=( CDeviceObject&& ) noexcept = default;
        ~CDeviceObject() override;
        
        const CDevice* GetDevice() const;
    private:
        const CDevice* m_pDevice;
    };
}
