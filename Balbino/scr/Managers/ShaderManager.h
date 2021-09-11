#pragma once
#include "Singleton.h"
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Balbino
{
	class CRenderer;
	class CShader;
	class CShaderManager final: public CSingleton<CShaderManager>
	{
	public:
		static const std::vector<CShader*>& GetShaders();
		static void AddShader(CShader* shader);
		static void BindShader( uint32_t materialIndex, const VkCommandBuffer& commandBuffer );
		static void SetRenderer( CRenderer* renderer );
		void Cleanup( const VkDevice& device, const VkAllocationCallbacks* pAllocator );
	private:
		friend CSingleton<CShaderManager>;
		CShaderManager();
		virtual ~CShaderManager();
		CShaderManager(const CShaderManager&) = delete;
		CShaderManager(CShaderManager&&) = delete;
		CShaderManager& operator=(const CShaderManager&) = delete;
		CShaderManager& operator=(CShaderManager&&) = delete;

		const std::vector<Balbino::CShader*>& IGetShaders() const;
		void IAddShader(CShader* shader);
		void IBindShader( uint32_t materialIndex, const VkCommandBuffer& commandBuffer ) const;
		void ISetRenderer( CRenderer* renderer );

		CRenderer* m_pRenderer;
		std::vector<CShader*> m_shader;
	};
}

