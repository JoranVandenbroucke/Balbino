#pragma once
#include <filesystem>
#include <vector>

#include "../Tools/FilesSystem/FIleSystem.h"
#include "Components/TextureComponent.h"

namespace BalVulkan
{
	class CDevice;
	class CImageResource;
}

namespace BalEditor
{
	class CAssetBrowser
	{
	public:
		CAssetBrowser();
		~CAssetBrowser();
		CAssetBrowser( const CAssetBrowser& ) = delete;
		CAssetBrowser( CAssetBrowser&& ) = delete;
		CAssetBrowser& operator=( const CAssetBrowser& ) = delete;
		CAssetBrowser& operator=( CAssetBrowser&& ) = delete;

		void Initialize( const VkDescriptorPool& descriptorPool, const VkDescriptorSetLayout& descriptorSetLayout );
		void Draw();
		void Cleanup();

	private:
		Balbino::CTextureComponent* m_pUnknownIcon;
		Balbino::CTextureComponent* m_pFolderIcon;
		Balbino::CTextureComponent* m_pBalbinoIcon;
		Balbino::CTextureComponent* m_pImageIcon;
		Balbino::CTextureComponent* m_pAudioIcon;
		Balbino::CTextureComponent* m_pModelIcon;
		Balbino::CTextureComponent* m_pPresetIcon;
		Balbino::CTextureComponent* m_pCodeIcon;

		std::vector<SFile> m_selected;

		void MoveIn( const std::filesystem::path& path, std::vector<SFile>& selectedPath );
	};
}
