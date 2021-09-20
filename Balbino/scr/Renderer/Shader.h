#pragma once
#include <vulkan/vulkan.hpp>

namespace Balbino
{
	class CShader
	{
	public:
		CShader();
		~CShader();
		CShader(const CShader&)=delete;
		CShader(CShader&&) noexcept;
		CShader& operator=(const CShader&)=delete;
		CShader& operator=(CShader&&)=delete;

		//https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Shader_modules
		void Initialize( const VkDevice& device, const VkExtent2D& swapChainExtent, const VkRenderPass& renderPass, VkDescriptorSetLayout& descriptorSetLayout, const VkAllocationCallbacks* callbacks);
		void Cleanup(const VkDevice& device, const VkAllocationCallbacks* callbacks);
		void Bind(const VkCommandBuffer& commandBuffer, const VkDescriptorSet* descriptorSet) const;
		const VkPipelineLayout& GetPipelineLayout()const;
		VkPipeline GetPipeline() const;

	private:
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_pipeline;

		static VkShaderModule CreateShaderModule( const std::string& code, const VkDevice& device, const VkAllocationCallbacks* callbacks);
		static std::string ReadFile(const std::string & filename);
	};

}