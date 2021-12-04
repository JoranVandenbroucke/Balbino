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
