#include "Semaphore.h"

#include "Device.h"

DeerVulkan::CSemaphore::~CSemaphore()
{
    vkDestroySemaphore( GetDevice()->GetVkDevice(), m_semaphore, VK_NULL_HANDLE );
    m_semaphore = VK_NULL_HANDLE;
}

void DeerVulkan::CSemaphore::Initialize()
{
    VkSemaphoreCreateInfo semaphoreInfo {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    CheckVkResult( vkCreateSemaphore( GetDevice()->GetVkDevice(), &semaphoreInfo, VK_NULL_HANDLE, &m_semaphore ) );
}