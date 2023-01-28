#include "Base.h"

FawnVision::CRefCounted::CRefCounted()
        : m_refCount{ 1 }
{
}

uint32_t FawnVision::CRefCounted::AddRef()
{
    return ++m_refCount;
}

uint32_t FawnVision::CRefCounted::Release()
{
    if ( this == nullptr )
    {
        return 0;
    }
    const uint32_t newRefCount{ --m_refCount };
    if ( newRefCount == 0 )
    {
        Destroy();
    }
    return newRefCount;
}

bool FawnVision::CRefCounted::IsDeletable() const
{
    return m_refCount == 0;
}

bool FawnVision::CRefCounted::IsUniquelyOwned() const
{
    return m_refCount == 1;
}

void FawnVision::CRefCounted::Destroy()
{
    delete this;
}

FawnVision::CDeviceObject::CDeviceObject( const CDevice* device )
        : m_pDevice{ device }
{
}

FawnVision::CDeviceObject::~CDeviceObject() = default;

const FawnVision::CDevice* FawnVision::CDeviceObject::GetDevice() const
{
    return m_pDevice;
}
