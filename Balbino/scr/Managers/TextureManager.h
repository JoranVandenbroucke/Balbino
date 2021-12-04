#pragma once

#include "../Core.h"

namespace BalVulkan
{
	class CDevice;
	class CCommandPool;
	class CQueue;
}
namespace Balbino
{
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

		void SetRenderData( const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue );
		void Initialize();
		void Cleanup();

		BALBINO_API CTexture* AddTexture(const char* filePath);
	private:
		std::unordered_map<uint32_t, Balbino::CTexture*> m_textures;
		const BalVulkan::CDevice* m_pDevice;
		const BalVulkan::CCommandPool* m_pCommandPool;
		const BalVulkan::CQueue* m_pQueue;
	};
}
