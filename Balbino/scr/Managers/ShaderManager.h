#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>
#include "../Renderer/Shader.h"

namespace Balbino
{
	class CRenderer;
	class CShaderManager final
	{
	public:
		CShaderManager();
		~CShaderManager();
		CShaderManager(const CShaderManager&) = delete;
		CShaderManager(CShaderManager&&) = delete;
		CShaderManager& operator=(const CShaderManager&) = delete;
		CShaderManager& operator=(CShaderManager&&) = delete;

		void Cleanup( const VkDevice& device, const VkAllocationCallbacks* pAllocator );
		void BindShader(const uint32_t materialIndex, const VkCommandBuffer& commandBuffer, const VkDescriptorSet* descriptorSet) const;
		void SetRenderer( CRenderer* renderer );

		CShader* AddShader();
		const std::vector<CShader>& GetShaders() const;
	private:
		CRenderer* m_pRenderer;
		std::vector<CShader> m_shader;
	};
}

