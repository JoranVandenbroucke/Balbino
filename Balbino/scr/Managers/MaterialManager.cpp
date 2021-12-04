#include "pch.h"
#include "MaterialManager.h"

Balbino::CMaterialManager::~CMaterialManager()
{
	if (!m_materials.empty())
		std::cout << "material manager not cleaned" << std::endl;
}

void Balbino::CMaterialManager::Initialize()
{
}

void Balbino::CMaterialManager::Cleanup()
{
	for ( const auto & material : m_materials )
	{
		material.Cleanup();
	}
	m_materials.clear();
}

void Balbino::CMaterialManager::AddReserve(uint32_t size)
{
	m_materials.reserve(m_materials.size() + size);
}

void Balbino::CMaterialManager::BindMaterial(uint32_t id) const
{
	if( id < 0 || id >= m_materials.size())
		return;
	m_materials[id].Bind();
}


Balbino::CMaterial* Balbino::CMaterialManager::AddMaterial( BalVulkan::CShaderPipeline* shaderPipeline, BalVulkan::CCommandPool* pPool, const std::vector<BalVulkan::SDescriptorSet>& descriptorSetsInfo, const BalVulkan::CDevice* pDevice )
{
	m_materials.emplace_back( shaderPipeline , pPool);
	m_materials.back().Initialize( descriptorSetsInfo , pDevice);
	return &m_materials.back();
}
