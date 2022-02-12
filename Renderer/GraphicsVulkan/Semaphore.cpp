#include "pch.h"
#include "Semaphore.h"

#include "Device.h"
#include "Funtions.h"

BalVulkan::CSemaphore::CSemaphore( const CDevice* pDevice )
	: CDeviceObject{ pDevice }
	, m_semaphore{ VK_NULL_HANDLE }
{
}

BalVulkan::CSemaphore::~CSemaphore()
{
	vkDestroySemaphore( GetDevice()->GetVkDevice(), m_semaphore, nullptr );
	m_semaphore = VK_NULL_HANDLE;
}

void BalVulkan::CSemaphore::Initialize()
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	CheckVkResult( vkCreateSemaphore( GetDevice()->GetVkDevice(), &semaphoreInfo, nullptr, &m_semaphore ) );
}

const VkSemaphore& BalVulkan::CSemaphore::Get() const
{
	return m_semaphore;
}

BalVulkan::CSemaphore* BalVulkan::CSemaphore::CreateNew( const CDevice* pDevice )
{
	return new CSemaphore{ pDevice };
}
