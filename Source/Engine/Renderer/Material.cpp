//
// Created by joran on 29/07/2023.
//

#include "Material.h"

#include "Texture.h"
#include <DeerVulkan.h>

FawnVision::CMaterial::CMaterial( CUuid uuid, FawnVision::CommandPool pool )
    : m_commandPool { pool }
    , m_uuid { uuid }
{
}

void FawnVision::CMaterial::Initialize( FawnVision::ShaderPipeline pShaderPipeline, const FawnVision::SMaterialCreateInfo& createInfo, FawnVision::Device device )
{
    m_shaderPipeline = pShaderPipeline;

    m_descriptorSet = FawnVision::CreateDescriptorSet( device, m_shaderPipeline, createInfo.descriptorSets );
    m_shaderPipeline->AddRef();
    m_commandPool->AddRef();
}
void FawnVision::CMaterial::Cleanup() const
{
    m_commandPool->Release();
    m_descriptorSet->Release();
    m_shaderPipeline->Release();
}

void FawnVision::CMaterial::Bind() const
{
    m_commandPool->BindShader( m_shaderPipeline, m_descriptorSet );
}
[[nodiscard]] const std::vector<FawnVision::SShaderResource>& FawnVision::CMaterial::GetShaderResourcesVector() const
{
    return m_shaderPipeline->GetShaderResources();
}
