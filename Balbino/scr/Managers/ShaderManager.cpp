#include "pch.h"
#include "ShaderManager.h"

#include "../Renderer/Renderer.h"

void Balbino::CShaderManager::SetRenderer(CRenderer* renderer)
{
	m_pRenderer = renderer;
}

Balbino::CShaderManager::CShaderManager()
	: m_pRenderer{ nullptr }
{
}

Balbino::CShaderManager::~CShaderManager()
{
	if (m_pRenderer != nullptr || !m_shader.empty())
		std::cerr << "Texture Manager not cleared" << std::endl;
}

void Balbino::CShaderManager::Cleanup(const VkDevice& device, const VkAllocationCallbacks* pAllocator)
{
	for (auto& shader : m_shader)
	{
		shader.Cleanup(device, pAllocator);
	}
	m_shader.clear();

	m_pRenderer = nullptr;
}

const std::vector<Balbino::CShader>& Balbino::CShaderManager::GetShaders() const
{
	return m_shader;
}

Balbino::CShader* Balbino::CShaderManager::AddShader()
{
	VkDevice device;
	VkExtent2D swapchainExtend;
	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkAllocationCallbacks* pCallbacks{ nullptr };

	m_pRenderer->GetDevice(device);
	m_pRenderer->GetSwatChainExtend(swapchainExtend);
	m_pRenderer->GetRenderPass(renderPass);
	m_pRenderer->GetDescriptorSetLayout(descriptorSetLayout);
	m_pRenderer->GetAllocationCallbacks(pCallbacks);
	m_shader.emplace_back();
	m_shader.back().Initialize(device, swapchainExtend, renderPass, descriptorSetLayout, pCallbacks);
	return &m_shader.back();
}

void Balbino::CShaderManager::BindShader(const uint32_t materialIndex, const VkCommandBuffer& commandBuffer, const VkDescriptorSet* descriptorSet) const
{
	if (materialIndex < m_shader.size()) {
		m_shader[materialIndex].Bind(commandBuffer, descriptorSet);
		
	}
}