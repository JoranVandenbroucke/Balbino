#pragma once
#include <Core.h>
#include <filesystem>
#include <vector>

#include "../Tools/FilesSystem/FileSystem.h"
#include <FawnVision.h>

struct ISystem;

namespace FawnForge
{
    class CShaderEditor;

    class CMaterialEditor;

    class CPropertyPanel;

    class CAssetBrowser final
    {
    public:
        CAssetBrowser() = default;
        ~CAssetBrowser();
        CAssetBrowser( const CAssetBrowser& )            = delete;
        CAssetBrowser( CAssetBrowser&& )                 = delete;
        CAssetBrowser& operator=( const CAssetBrowser& ) = delete;
        CAssetBrowser& operator=( CAssetBrowser&& )      = delete;

        void Initialize( ISystem* pSystem );
        void Draw() noexcept;
        void Cleanup();
        void ShowWindow() noexcept;
        void SetShaderGraphReference( CShaderEditor* pShaderGraph, CMaterialEditor* pMaterialEditor, CPropertyPanel* pPropertyPanel ) noexcept;
        [[nodiscard]] const char* GetCurrentDirectory() const;

    private:
        FawnVision::CTexture* m_pIcon[ (uint8_t)file_type::max ] { BALBINO_NULL };
        VkDescriptorSet m_descriptorIcon[ (uint8_t)file_type::max ] { VK_NULL_HANDLE };

        CShaderEditor* m_pShaderGraph { BALBINO_NULL };
        CMaterialEditor* m_pMaterialEditor { BALBINO_NULL };
        CPropertyPanel* m_pPropertyPanel { BALBINO_NULL };

        ISystem* m_pSystem { BALBINO_NULL };

        float m_size { 32.f };
        bool m_isVisible { true };
        bool m_updateCurrentDirectory { true };

        uint8_t m_isContextMenuOpen : 2 {};
        bool m_isItemSelected {};
        bool m_openItem {};

        SFile m_itemToBeCreated { "", "" };
        std::vector<SFile> m_currentDirectory {};
        std::vector<SFile> m_files {};
        std::string m_currentDirectoryName { "..\\Data" };

        void FindAllFiles() noexcept;
        void DrawTree( const std::string& path, uint32_t& nodeIdx );
        void DrawContextMenu( bool isSelected ) noexcept;
        void GetAllFilesInSelectedPath( std::string path, std::vector<SFile>& filesInDirectory ) noexcept;
        void HandelSelected( const SFile& currentFile, bool isSelected );
        void MoveFile( const SFile* pFile, const std::filesystem::path& destination ) const;
        void Rename( const std::filesystem::path& oldName, const std::filesystem::path& newName ) const;
        void CreateItem() noexcept;
    };
}// namespace FawnForge
