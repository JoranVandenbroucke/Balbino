#pragma once

#include "Base.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Vertex.h"

namespace BalVulkan
{
	class CSwapchain;
	class CShaderPipeline final : public CDeviceObject
	{
	public:
		explicit CShaderPipeline( const CDevice* const device )
			: CDeviceObject{ device }
			, m_descriptorSetLayout{ VK_NULL_HANDLE }
			, m_pipelineLayout{ VK_NULL_HANDLE }
			, m_pipelineCache{ VK_NULL_HANDLE }
			, m_pipeline{ VK_NULL_HANDLE }
		{
		}

		CShaderPipeline( CShaderPipeline&& ) = default;
		CShaderPipeline& operator=( CShaderPipeline&& ) = default;
		~CShaderPipeline() override;

		void Initialize( const std::array<CShader*, 3>& shaders, const CFrameBuffer& frameBuffer, const std::vector<EVertexComponent>& components, uint32_t blendAttachmentSize, const BalVulkan::CSwapchain* pSwapchain, ECullMode cullModeFlag = ECullMode::Front );
		VkPipelineLayout GetPipelineLayout()const;
		const VkDescriptorSetLayout& GetDescriptorSetLayout()const;
		VkPipeline GetPipeline();
		static CShaderPipeline* CreateNew( const CDevice* pDevice );
	private:
		VkDescriptorSetLayout m_descriptorSetLayout;
		VkPipelineLayout m_pipelineLayout;
		VkPipelineCache m_pipelineCache;
		VkPipeline m_pipeline;

		std::unordered_map<std::string, SShaderResource> m_shaderResources;
		std::unordered_map<uint32_t, std::vector<SShaderResource>> m_shaderSets;

		static VkDescriptorType FindDescriptorType( EShaderResourceType resource_type, bool dynamic );
	}; 
}
