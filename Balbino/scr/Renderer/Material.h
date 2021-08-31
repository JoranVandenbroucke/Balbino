#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>

namespace Balbino
{
	class CMaterial
	{
	public:
		CMaterial();

		//https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Shader_modules
		void Compile( const VkDevice& device, const VkExtent2D& swapChainExtent, const VkRenderPass& renderPass, const VkAllocationCallbacks* callbacks);
		void Cleanup(const VkDevice& device, const VkAllocationCallbacks* callbacks) const;
		void Bind(const VkCommandBuffer& commandBuffer) const;
	private:
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_pipeline;

		static VkShaderModule CreateShaderModule( const std::vector<char>& code, const VkDevice& device, const VkAllocationCallbacks* callbacks);
		static std::vector<char> ReadFile( const std::string& filename );
	};

}