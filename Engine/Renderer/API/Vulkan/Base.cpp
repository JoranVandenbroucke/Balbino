#include "Base.h"

void FawnVision::CRefCounted::Destroy()
{
    delete this;
}

uint32_t FawnVision::CRefCounted::AddRef()
{
    return m_refCount.fetch_add(1);
}

uint32_t FawnVision::CRefCounted::Release()
{
    if ( this == nullptr )
    {
        return 0;
    }
    if ( m_refCount.fetch_sub(1) == 1 )
    {
        Destroy();
    }
    return m_refCount;
}

bool FawnVision::CRefCounted::IsDeletable() const
{
    return m_refCount == 0;
}

bool FawnVision::CRefCounted::IsUniquelyOwned() const
{
    return m_refCount == 1;
}

FawnVision::CDeviceObject::CDeviceObject( const CDevice* pDevice )
        : m_pDevice{ pDevice }
{
}

const FawnVision::CDevice* FawnVision::CDeviceObject::GetDevice() const
{
    return m_pDevice;
}