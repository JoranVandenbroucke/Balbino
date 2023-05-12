#pragma once

#include <unordered_map>
#include <vector>
#include "Common.h"

#include "CommandPool.h"
#include "DescriptorSet.h"
#include "ISystem.h"
#include "ShaderPipeline.h"

namespace Balbino
{
    struct SDescriptorSet;
    
    class CDevice;
    
    class CCommandPool;
    
    class CShaderPipeline;
    
    class CDescriptorSet;
}

class CTexture;

class CMaterial
{
public:
    CMaterial( CUuid uuid, CUuid shaderUUID, FawnVision::CCommandPool* pPool )
            : m_descriptorSet{ nullptr }
            , m_shaderPipeline{}
            , m_command{ pPool }
            , m_uuid{ uuid }
            , m_shaderUUID{ shaderUUID }
    {
    }
    
    ~CMaterial() = default;
    CMaterial( const CMaterial& ) = delete;
    CMaterial( CMaterial&& ) = default;
    CMaterial& operator=( const CMaterial& ) = delete;
    CMaterial& operator=( CMaterial&& ) = delete;
    
    void Initialize( FawnVision::CShaderPipeline* pShaderPipeline, const std::vector<FawnVision::SDescriptorSet>& descriptorSetsInfo, const FawnVision::CDevice* pDevice )
    {
        m_shaderPipeline = pShaderPipeline;
        m_descriptorSet  = new FawnVision::CDescriptorSet{ pDevice };
        m_descriptorSet->Initialize( m_shaderPipeline, descriptorSetsInfo );
        m_shaderPipeline->AddRef();
        m_command->AddRef();
    }
    
    void Cleanup() const
    {
        m_descriptorSet->Release();
        m_shaderPipeline->Release();
        m_command->Release();
    }
    
    void Bind() const
    {
        m_command->BindShader( m_shaderPipeline, m_descriptorSet );
    }
    
    [[nodiscard]] const std::vector<FawnVision::SShaderResource>& GetShaderResourcesVector() const
    {
        return m_shaderPipeline->GetShaderResources();
    }
    CUuid GetShaderID()
    {
        return m_shaderUUID;
    }
    
    uint64_t GetID()
    {
        return (uint64_t) m_uuid;
    }

private:
    FawnVision::CDescriptorSet * m_descriptorSet;
    FawnVision::CShaderPipeline* m_shaderPipeline;
    FawnVision::CCommandPool   * m_command;
    CUuid m_uuid{};
    CUuid m_shaderUUID{};
};
