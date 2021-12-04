#include "pch.h"
#include "Material.h"

#include "CommandPool.h"
#include "DescriptorSet.h"
#include "ShaderPipeline.h"

Balbino::CMaterial::CMaterial( BalVulkan::CShaderPipeline* shaderPipeline, BalVulkan::CCommandPool* pPool )
	: m_descriptorSet{ nullptr }
	, m_shaderPipeline{ shaderPipeline }
	, m_command{ pPool }
{
}

void Balbino::CMaterial::Initialize( const std::vector<BalVulkan::SDescriptorSet>& descriptorSetsInfo, const BalVulkan::CDevice* pDevice )
{
	m_descriptorSet = BalVulkan::CDescriptorSet::CreateNew( pDevice );
	m_descriptorSet->Initialize( m_shaderPipeline, descriptorSetsInfo );
	m_shaderPipeline->AddRef();
	m_command->AddRef();
}

void Balbino::CMaterial::Cleanup() const
{
	m_descriptorSet->Release();
	m_shaderPipeline->Release();
	m_command->Release();
}

void Balbino::CMaterial::Bind() const
{
	m_command->BindShader( m_shaderPipeline, m_descriptorSet );
}
