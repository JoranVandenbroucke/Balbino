#pragma once
#include "Singleton.h"

#include <unordered_map>

namespace Balbino
{
	class CRenderer;
	class CTexture;

	class CTextureManager final: public CSingleton<CTextureManager>
	{
	public:
		static CTexture* AddTexture( CTexture* pTexture, const std::string& filePath);

		void Initialize(CRenderer* pRenderer);
		void Cleanup();
	private:
		friend CSingleton<CTextureManager>;

		CTextureManager();
		virtual ~CTextureManager() override;
		CTextureManager(const CTextureManager&) = delete;
		CTextureManager(CTextureManager&&) = delete;
		CTextureManager& operator=(const CTextureManager&) = delete;
		CTextureManager& operator=(CTextureManager&&) = delete;

		CTexture* IAddTexture( CTexture* pTexture, const std::string& filePath);

		CRenderer* m_pRenderer;
		std::unordered_map<uint32_t, CTexture*> m_textures;
	};
}