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
		CShader(CShader&&)=delete;
		CShader& operator=(const CShader&)=delete;
		CShader& operator=(CShader&&)=delete;

		void* operator new(size_t size);
		void* operator new(size_t size, int b, const char* f, int l);
		void* operator new[](size_t size) = delete;

		void operator delete(void* ptr);
		void operator delete(void* ptr, int b, const char* f, int l);
		void operator delete[](void* ptr) = delete;

		//https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Shader_modules
		void Initialize( const VkDevice& device, const VkExtent2D& swapChainExtent, const VkRenderPass& renderPass, VkDescriptorSetLayout& descriptorSetLayout, const VkAllocationCallbacks* callbacks);
		void Cleanup(const VkDevice& device, const VkAllocationCallbacks* callbacks);
		void Bind(const VkCommandBuffer& commandBuffer) const;
		const VkPipelineLayout& GetPipelineLayout()const;
		VkPipeline GetPipeline() const;

	private:
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_pipeline;

		static VkShaderModule CreateShaderModule( const std::string& code, const VkDevice& device, const VkAllocationCallbacks* callbacks);
		static std::string ReadFile(const std::string & filename);
	};

}