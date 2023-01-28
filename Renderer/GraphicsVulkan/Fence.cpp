#include "Fence.h"

#include "Device.h"
#include "Funtions.h"

BalVulkan::CFence::CFence( const CDevice* pDevice )
        : CDeviceObject{ pDevice }
          , m_fence{ VK_NULL_HANDLE }
{
}

BalVulkan::CFence::~CFence()
{
    vkDestroyFence( GetDevice()->GetVkDevice(), m_fence, nullptr );
    m_fence = VK_NULL_HANDLE;
}

void BalVulkan::CFence::Initialize()
{
    VkFenceCreateInfo fenceInfo{
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };
    
    CheckVkResult(
            vkCreateFence( GetDevice()->GetVkDevice(), &fenceInfo, nullptr, &m_fence ),
            "Could not create a fence"
    );
}

VkFence BalVulkan::CFence::Get() const
{
    return m_fence;
}

void BalVulkan::CFence::Wait() const
{
    vkWaitForFences( GetDevice()->GetVkDevice(), 1, &m_fence, VK_TRUE, UINT64_MAX );
}

void BalVulkan::CFence::Reset() const
{
    vkResetFences( GetDevice()->GetVkDevice(), 1, &m_fence );
}

BalVulkan::CFence* BalVulkan::CFence::CreateNew( const CDevice* pDevice )
{
    return new CFence{ pDevice };
}
