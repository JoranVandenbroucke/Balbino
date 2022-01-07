#pragma once
#include <filesystem>
#include <vector>

#include "../Tools/FilesSystem/FileSystem.h"
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

		void Initialize();
		void Draw();
		void Cleanup();
		void ShowWindow();
	private:
		Balbino::CTextureComponent* m_pUnknownIcon;
		Balbino::CTextureComponent* m_pFolderIcon;
		Balbino::CTextureComponent* m_pBalbinoIcon;
		Balbino::CTextureComponent* m_pImageIcon;
		Balbino::CTextureComponent* m_pAudioIcon;
		Balbino::CTextureComponent* m_pModelIcon;
		Balbino::CTextureComponent* m_pPresetIcon;
		Balbino::CTextureComponent* m_pCodeIcon;
		Balbino::CTextureComponent* m_pFontIcon;


		VkDescriptorSet m_pVkDescriptorSetUnknownIcon;
		VkDescriptorSet m_pVkDescriptorSetFolderIcon;
		VkDescriptorSet m_pVkDescriptorSetBalbinoIcon;
		VkDescriptorSet m_pVkDescriptorSetImageIcon;
		VkDescriptorSet m_pVkDescriptorSetAudioIcon;
		VkDescriptorSet m_pVkDescriptorSetModelIcon;
		VkDescriptorSet m_pVkDescriptorSetPresetIcon;
		VkDescriptorSet m_pVkDescriptorSetCodeIcon;
		VkDescriptorSet m_pVkDescriptorSetFontIcon;

		bool m_isVisible;
		float m_size;
		std::vector<SFile> m_selected;

		void MoveIn( const std::filesystem::path& path, std::vector<SFile>& selectedPath );
	};
}
