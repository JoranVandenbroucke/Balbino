#pragma once

#include <atomic>

namespace FawnVision
{
    class CDevice;
    
    class CRefCounted
    {
    public:
        CRefCounted()=default;
        CRefCounted( CRefCounted&& ) noexcept
        {
            /* refcount not copied on purpose*/
        }
        
        CRefCounted& operator=( CRefCounted&& ) noexcept
        {
            /* refcount not copied on purpose*/
            return *this;
        }
        CRefCounted( CRefCounted& other ) noexcept = delete;
        CRefCounted& operator=( CRefCounted& other ) noexcept = delete;
        
        virtual ~CRefCounted() = default;
        
        virtual uint32_t AddRef() final;
        virtual uint32_t Release() final;
        
        bool IsDeletable() const;
        bool IsUniquelyOwned() const;
    
    protected:
        virtual void Destroy();
    
    private:
        std::atomic<uint32_t> m_refCount{1};
    };
    
    class CDeviceObject : public CRefCounted
    {
    public:
        explicit CDeviceObject( const CDevice* pDevice );
        CDeviceObject( CDeviceObject&& ) noexcept = default;
        CDeviceObject& operator=( CDeviceObject&& ) noexcept = default;
        ~CDeviceObject() override = default;
        
        const CDevice* GetDevice() const;
    
    private:
        const CDevice* m_pDevice;
    };
}
