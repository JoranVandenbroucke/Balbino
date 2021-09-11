#pragma once
#include "Singleton.h"

#include <unordered_map>
#include <vulkan/vulkan_core.h>

namespace Balbino
{
	class CRenderer;
	class CTexture;

	class CTextureManager final: public CSingleton<CTextureManager>
	{
	public:
		static CTexture* AddTexture( CTexture* pTexture, const std::string& filePath);
		static CTexture* AddTexture( CTexture* pTexture, const char* filePath);

		void Initialize(CRenderer* pRenderer);
		void Cleanup( const VkDevice& device, const VkAllocationCallbacks* pAllocator );
		void SetRenderer( CRenderer* pRenderer );
	private:
		friend CSingleton<CTextureManager>;

		CTextureManager();
		virtual ~CTextureManager() override;
		CTextureManager(const CTextureManager&) = delete;
		CTextureManager(CTextureManager&&) = delete;
		CTextureManager& operator=(const CTextureManager&) = delete;
		CTextureManager& operator=(CTextureManager&&) = delete;

		CTexture* IAddTexture( CTexture* pTexture, const char* filePath);

		CRenderer* m_pRenderer;
		std::unordered_map<uint32_t, CTexture*> m_textures;
	};
}
