#include "pch.h"
#include "Common.h"
#include "CommandPool.h"

BalVulkan::SDescriptorSet::SDescriptorSet( SDescriptorSet::EType buffer, CBuffer* pModelBuffer )
	: descriptorType{ buffer }
	, unition{ .buffer = pModelBuffer }
{
}

BalVulkan::SDescriptorSet::SDescriptorSet( SDescriptorSet::EType buffer, CImageView* pModelBuffer, CSampler* pSampler )
	: descriptorType{ buffer }
	, unition{.image = {pModelBuffer, pSampler}}
{
}

void BalVulkan::DrawMesh( const CCommandPool* command, uint32_t indexCount, uint32_t firstIndex, uint32_t firstInstance, uint32_t instanceCount )
{
	vkCmdDrawIndexed( command->GetCommandBuffer(), indexCount, instanceCount, firstIndex, 0, firstInstance );
}

BalVulkan::EImageUsageFlagBits BalVulkan::operator|( BalVulkan::EImageUsageFlagBits lhs, BalVulkan::EImageUsageFlagBits rhs )
{
	return ( EImageUsageFlagBits ) ( ( uint32_t ) lhs | ( uint32_t ) rhs );
}

BalVulkan::EMemoryPropertyFlagBits BalVulkan::operator|( BalVulkan::EMemoryPropertyFlagBits lhs, BalVulkan::EMemoryPropertyFlagBits rhs )
{
	return ( EMemoryPropertyFlagBits ) ( ( uint32_t ) lhs | ( uint32_t ) rhs );
}

BalVulkan::EBufferUsageFlagBits BalVulkan::operator|( BalVulkan::EBufferUsageFlagBits lhs, BalVulkan::EBufferUsageFlagBits rhs )
{
	return ( EBufferUsageFlagBits ) ( ( uint32_t ) lhs | ( uint32_t ) rhs );
}
