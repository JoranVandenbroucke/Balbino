#pragma once
#include <filesystem>
#include <vector>

#include "../Tools/FilesSystem/FIleSystem.h"

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

		void Initialize( BalVulkan::CDevice* pDevice );
		void Draw();
		void Cleanup();

	private:
		BalVulkan::CImageResource* m_pUnknownIcon;
		BalVulkan::CImageResource* m_pFolderIcon;
		BalVulkan::CImageResource* m_pBalbinoIcon;
		BalVulkan::CImageResource* m_pImageIcon;
		BalVulkan::CImageResource* m_pAudioIcon;
		BalVulkan::CImageResource* m_pModelIcon;
		BalVulkan::CImageResource* m_pPresetIcon;
		BalVulkan::CImageResource* m_pCodeIcon;

		std::vector<SFile> m_selected;

		void MoveIn( const std::filesystem::path& path, std::vector<SFile>& selectedPath );
	};
}
