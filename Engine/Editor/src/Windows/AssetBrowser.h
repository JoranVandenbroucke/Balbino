#pragma once

#include <filesystem>
#include <vector>

#include "../Tools/FilesSystem/FileSystem.h"
#include "Texture.h"

struct ISystem;

namespace FawnForge
{
    class CShaderEditor;
    
    class CMaterialEditor;
    
    class CPropertyPanel;
    
    class CAssetBrowser
    {
    public:
        CAssetBrowser()=default;
        ~CAssetBrowser();
        CAssetBrowser( const CAssetBrowser& ) = delete;
        CAssetBrowser( CAssetBrowser&& ) = delete;
        CAssetBrowser& operator=( const CAssetBrowser& ) = delete;
        CAssetBrowser& operator=( CAssetBrowser&& ) = delete;
        
        void Initialize( const ISystem* pSystem );
        void Draw();
        void Cleanup();
        void ShowWindow();
        void SetShaderGraphReference( CShaderEditor* pShaderGraph, CMaterialEditor* pMaterialEditor, CPropertyPanel* pPropertyPanel );
        [[nodiscard]] const char* GetCurrentDirectory()const;
    
    private:
        CTexture* m_pUnknownIcon{ nullptr };
        CTexture* m_pFolderIcon{ nullptr };
        CTexture* m_pBalbinoIcon{ nullptr };
        CTexture* m_pImageIcon{ nullptr };
        CTexture* m_pAudioIcon{ nullptr };
        CTexture* m_pModelIcon{ nullptr };
        CTexture* m_pPresetIcon{ nullptr };
        CTexture* m_pCodeIcon{ nullptr };
        CTexture* m_pFontIcon{ nullptr };
        CTexture* m_pMaterialIcon{ nullptr };
        CTexture* m_pShaderIcon{ nullptr };
        
        VkDescriptorSet m_pVkDescriptorSetUnknownIcon{ VK_NULL_HANDLE };
        VkDescriptorSet m_pVkDescriptorSetFolderIcon{ VK_NULL_HANDLE };
        VkDescriptorSet m_pVkDescriptorSetBalbinoIcon{ VK_NULL_HANDLE };
        VkDescriptorSet m_pVkDescriptorSetImageIcon{ VK_NULL_HANDLE };
        VkDescriptorSet m_pVkDescriptorSetAudioIcon{ VK_NULL_HANDLE };
        VkDescriptorSet m_pVkDescriptorSetModelIcon{ VK_NULL_HANDLE };
        VkDescriptorSet m_pVkDescriptorSetPresetIcon{ VK_NULL_HANDLE };
        VkDescriptorSet m_pVkDescriptorSetCodeIcon{ VK_NULL_HANDLE };
        VkDescriptorSet m_pVkDescriptorSetFontIcon{ VK_NULL_HANDLE };
        VkDescriptorSet m_pVkDescriptorSetMaterialIcon{ VK_NULL_HANDLE };
        VkDescriptorSet m_pVkDescriptorSetShaderIcon{ VK_NULL_HANDLE };
        
        CShaderEditor  * m_pShaderGraph{ nullptr };
        CMaterialEditor* m_pMaterialEditor{ nullptr };
        CPropertyPanel * m_pPropertyPanel{ nullptr };
        
        const ISystem* m_pSystem{ nullptr };
        
        float m_size{ 32.f };
        bool  m_isVisible{true};
        bool  m_updateCurrentDirectory{ true };
        
        bool m_isContextMenuOpen{};
        bool m_wasContextMenuOpen{};
        bool m_isItemSelected{};
        bool m_openItem{};
        
        SFile              m_itemToBeCreated{"",""};
        std::vector<SFile> m_currentDirectory;
        std::vector<SFile> m_files;
        std::string        m_currentDirectoryName;
        
        void FindAllFiles();
        void DrawTree( const std::string& path, uint32_t& nodeIdx );
        void DrawContextMenu( bool isSelected );
        void GetAllFilesInSelectedPath( std::string path, std::vector<SFile>& filesInDirectory );
        void HandelSelected( const SFile& currentFile, bool isSelected );
        void MoveFile( const SFile* pFile, const std::filesystem::path& destination )const;
        void Rename( const std::filesystem::path& oldName, const std::filesystem::path& newName )const;
        void CreateItem();
    };
}
