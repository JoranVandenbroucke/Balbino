#include "Semaphore.h"

#include "Device.h"
#include "Funtions.h"

FawnVision::CSemaphore::CSemaphore( const CDevice* pDevice )
        : CDeviceObject{ pDevice }
          , m_semaphore{ VK_NULL_HANDLE }
{
}

FawnVision::CSemaphore::~CSemaphore()
{
    vkDestroySemaphore( GetDevice()->GetVkDevice(), m_semaphore, nullptr );
    m_semaphore = VK_NULL_HANDLE;
}

void FawnVision::CSemaphore::Initialize()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    CheckVkResult( vkCreateSemaphore( GetDevice()->GetVkDevice(), &semaphoreInfo, nullptr, &m_semaphore ));
}

const VkSemaphore& FawnVision::CSemaphore::Get() const
{
    return m_semaphore;
}
