#pragma once
#include <unordered_map>
#include <vulkan/vulkan.h>
#include "../Renderer/Texture.h"

namespace Balbino
{
	class CRenderer;
	class CTexture;

	class CTextureManager final
	{
	public:
		CTextureManager();
		~CTextureManager();
		CTextureManager(const CTextureManager&) = delete;
		CTextureManager(CTextureManager&&) = delete;
		CTextureManager& operator=(const CTextureManager&) = delete;
		CTextureManager& operator=(CTextureManager&&) = delete;

		void Initialize(CRenderer* pRenderer);
		void Cleanup( const VkDevice& device, const VkAllocationCallbacks* pAllocator );
		void SetRenderer( CRenderer* pRenderer );

		CTexture* AddTexture( const char* filePath);
	private:
		CRenderer* m_pRenderer;
		std::unordered_map<uint32_t, CTexture> m_textures;
	};
}
