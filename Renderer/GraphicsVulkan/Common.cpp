#include "Common.h"
#include "CommandPool.h"

FawnVision::SDescriptorSet::SDescriptorSet( EType::Enum buffer, CBuffer* pModelBuffer, uint32_t set, uint32_t binding )
        : descriptorType{ buffer }
          , description{ .buffer = { pModelBuffer }}
          , set( set )
          , binding{ binding }
{
}

FawnVision::SDescriptorSet::SDescriptorSet( EType::Enum buffer, CImageView* pModelBuffer, CSampler* pSampler, uint32_t set, uint32_t binding )
        : descriptorType{ buffer }
          , description{ .image = { pModelBuffer, pSampler }}
          , set( set )
          , binding{ binding }
{
}

void FawnVision::DrawMesh( const CCommandPool* command, uint32_t indexCount, uint32_t firstIndex, uint32_t firstInstance, uint32_t instanceCount )
{
    vkCmdDrawIndexed( command->GetCommandBuffer(), indexCount, instanceCount, firstIndex, 0, firstInstance );
}
