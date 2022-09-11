#include "Base.h"

BalVulkan::CRefCounted::CRefCounted()
	: m_refCount{ 1 }
{
}

uint32_t BalVulkan::CRefCounted::AddRef()
{
	return ++m_refCount;
}

uint32_t BalVulkan::CRefCounted::Release()
{
	if ( this == nullptr )
		return 0;
	const uint32_t newRefCount{ --m_refCount };
	if ( newRefCount == 0 )
	{
		Destroy();
	}
	return newRefCount;
}

bool BalVulkan::CRefCounted::IsDeletable() const
{
	return m_refCount == 0;
}

bool BalVulkan::CRefCounted::IsUniquelyOwned() const
{
	return m_refCount == 1;
}

void BalVulkan::CRefCounted::Destroy()
{
	delete this;
}

BalVulkan::CDeviceObject::CDeviceObject( const CDevice* device )
	: m_pDevice{ device }
{
}

BalVulkan::CDeviceObject::~CDeviceObject()
= default;

const BalVulkan::CDevice* BalVulkan::CDeviceObject::GetDevice() const
{
	return m_pDevice;
}
