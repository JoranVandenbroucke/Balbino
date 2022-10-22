#pragma once

#include <unordered_map>
#include <vector>
#include "Common.h"

#include "CommandPool.h"
#include "DescriptorSet.h"
#include "ISystem.h"
#include "ShaderPipeline.h"

namespace BalVulkan
{
    struct SDescriptorSet;
    
    class CDevice;
    
    class CCommandPool;
    
    class CShaderPipeline;
    
    class CDescriptorSet;
}

namespace Balbino
{
    class CTexture;
    
    class CMaterial
    {
    public:
        CMaterial( CUuid uuid, CUuid shaderUUID, BalVulkan::CCommandPool* pPool )
                : m_descriptorSet{ nullptr },
                  m_shaderPipeline{},
                  m_command{ pPool },
                  m_uuid{ uuid },
                  m_shaderUUID{ shaderUUID }
        {
        }
        
        ~CMaterial() = default;
        CMaterial( const CMaterial& ) = delete;
        CMaterial( CMaterial&& ) = default;
        CMaterial& operator=( const CMaterial& ) = delete;
        CMaterial& operator=( CMaterial&& ) = delete;
        static CMaterial* CreateNew( CUuid uuid, CUuid shaderUUID, BalVulkan::CCommandPool* pPool )
        {
            return new CMaterial{ uuid, shaderUUID, pPool };
        }
        void Initialize( BalVulkan::CShaderPipeline* pShaderPipeline, const std::vector<BalVulkan::SDescriptorSet>& descriptorSetsInfo, const BalVulkan::CDevice* pDevice )
        {
            m_shaderPipeline = pShaderPipeline;
            m_descriptorSet  = BalVulkan::CDescriptorSet::CreateNew( pDevice );
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
        
        std::vector<BalVulkan::SShaderResource> GetShaderResourcesVector() const
        {
            std::vector<BalVulkan::SShaderResource> shaderResources;
            const auto& resources = m_shaderPipeline->GetShaderResources();
            shaderResources.reserve( resources.size());
            for ( const auto& resource : resources )
            {
                shaderResources.push_back( resource.second );
            }
            return shaderResources;
        }
        const std::unordered_map<std::string, BalVulkan::SShaderResource>& GetShaderResourcesMap() const
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
        BalVulkan::CDescriptorSet * m_descriptorSet;
        BalVulkan::CShaderPipeline* m_shaderPipeline;
        BalVulkan::CCommandPool   * m_command;
        CUuid m_uuid;
        CUuid m_shaderUUID;
    };
}
