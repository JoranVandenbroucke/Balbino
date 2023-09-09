#include "Fence.h"

DeerVulkan::CFence::~CFence()
{
    vkDestroyFence( GetDevice()->GetVkDevice(), m_fence, VK_NULL_HANDLE );
    m_fence = VK_NULL_HANDLE;
}

void DeerVulkan::CFence::Initialize()
{
    VkFenceCreateInfo fenceInfo {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    CheckVkResult( vkCreateFence( GetDevice()->GetVkDevice(), &fenceInfo, VK_NULL_HANDLE, &m_fence ), "Could not create a fence" );
}

void DeerVulkan::CFence::Wait() const
{
    vkWaitForFences( GetDevice()->GetVkDevice(), 1, &m_fence, VK_TRUE, UINT64_MAX );
}

void DeerVulkan::CFence::Reset() const
{
    vkResetFences( GetDevice()->GetVkDevice(), 1, &m_fence );
}
