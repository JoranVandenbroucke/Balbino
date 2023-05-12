#include "Fence.h"

#include "Funtions.h"

FawnVision::CFence::CFence( const CDevice* pDevice )
        : CDeviceObject{ pDevice }
          , m_fence{ VK_NULL_HANDLE }
{
}

FawnVision::CFence::~CFence()
{
    vkDestroyFence( GetDevice()->GetVkDevice(), m_fence, nullptr );
    m_fence = VK_NULL_HANDLE;
}

void FawnVision::CFence::Initialize()
{
    VkFenceCreateInfo fenceInfo{
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };
    
    CheckVkResult(
            vkCreateFence( GetDevice()->GetVkDevice(), &fenceInfo, nullptr, &m_fence ),
            "Could not create a fence"
    );
}

VkFence FawnVision::CFence::Get() const
{
    return m_fence;
}

void FawnVision::CFence::Wait() const
{
    vkWaitForFences( GetDevice()->GetVkDevice(), 1, &m_fence, VK_TRUE, UINT64_MAX );
}

void FawnVision::CFence::Reset() const
{
    vkResetFences( GetDevice()->GetVkDevice(), 1, &m_fence );
}
