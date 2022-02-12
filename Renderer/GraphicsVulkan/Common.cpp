#include "pch.h"
#include "Common.h"
#include "CommandPool.h"

BalVulkan::SDescriptorSet::SDescriptorSet( EType buffer, CBuffer* pModelBuffer )
	: descriptorType{ buffer }
	, description{ .buffer = pModelBuffer }
{
}

BalVulkan::SDescriptorSet::SDescriptorSet( EType buffer, CImageView* pModelBuffer, CSampler* pSampler )
	: descriptorType{ buffer }
	, description{ .image = { pModelBuffer, pSampler } }
{
}

void BalVulkan::DrawMesh( const CCommandPool* command, uint32_t indexCount, uint32_t firstIndex, uint32_t firstInstance, uint32_t instanceCount )
{
	vkCmdDrawIndexed( command->GetCommandBuffer(), indexCount, instanceCount, firstIndex, 0, firstInstance );
}

BalVulkan::EImageUsageFlagBits BalVulkan::operator|( EImageUsageFlagBits lhs, EImageUsageFlagBits rhs )
{
	return static_cast<EImageUsageFlagBits>( ( uint32_t ) lhs | ( uint32_t ) rhs );
}

BalVulkan::EMemoryPropertyFlagBits BalVulkan::operator|( EMemoryPropertyFlagBits lhs, EMemoryPropertyFlagBits rhs )
{
	return static_cast<EMemoryPropertyFlagBits>( ( uint32_t ) lhs | ( uint32_t ) rhs );
}

BalVulkan::EBufferUsageFlagBits BalVulkan::operator|( EBufferUsageFlagBits lhs, EBufferUsageFlagBits rhs )
{
	return static_cast<EBufferUsageFlagBits>( ( uint32_t ) lhs | ( uint32_t ) rhs );
}
