#include "pch.h"
#include "ShaderManager.h"

#include "../Renderer/Shader.h"
#include "../Renderer/Renderer.h"

const std::vector<Balbino::CShader*>& Balbino::CShaderManager::GetShaders()
{
	return GetInstance().IGetShaders();
}

void Balbino::CShaderManager::AddShader( CShader* shader)
{
	GetInstance().IAddShader(shader);
}

void Balbino::CShaderManager::BindShader( uint32_t materialIndex, const VkCommandBuffer& commandBuffer)
{
	return GetInstance().IBindShader(materialIndex, commandBuffer);
}

void Balbino::CShaderManager::SetRenderer( CRenderer* renderer )
{
	GetInstance().ISetRenderer(renderer);
}

void Balbino::CShaderManager::ISetRenderer( CRenderer* renderer )
{
	m_pRenderer = renderer;
}

Balbino::CShaderManager::CShaderManager()
{
}

Balbino::CShaderManager::~CShaderManager()
{
	if (m_pRenderer != nullptr || !m_shader.empty())
		std::cerr << "Texture Manager not cleared" << std::endl;
}

void Balbino::CShaderManager::Cleanup(const VkDevice& device, const VkAllocationCallbacks* pAllocator)
{
	for (const auto shader : m_shader)
	{
		shader->Cleanup(device, pAllocator);
		delete shader;
	}
	m_shader.clear();

	m_pRenderer = nullptr;
}

const std::vector<Balbino::CShader*>& Balbino::CShaderManager::IGetShaders() const
{
	return m_shader;
}

void Balbino::CShaderManager::IAddShader( CShader* shader)
{
	VkDevice device;
	VkExtent2D swapchainExtend;
	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkAllocationCallbacks* pCallbacks{nullptr};

	m_pRenderer->GetDevice(device);
	m_pRenderer->GetSwatChainExtend(swapchainExtend);
	m_pRenderer->GetRenderPass(renderPass);
	m_pRenderer->GetDescriptorSetLayout(descriptorSetLayout);
	m_pRenderer->GetAllocationCallbacks(pCallbacks);
	shader->Initialize(device, swapchainExtend, renderPass, descriptorSetLayout, pCallbacks);
	m_shader.push_back(shader);
}

void Balbino::CShaderManager::IBindShader( const uint32_t materialIndex, const VkCommandBuffer& commandBuffer ) const
{
	m_shader[materialIndex]->Bind(commandBuffer);
}