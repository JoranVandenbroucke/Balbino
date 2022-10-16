#pragma once

#include <filesystem>
#include <vector>

#include "../Tools/FilesSystem/FileSystem.h"
#include "Texture.h"

struct ISystem;

namespace BalVulkan
{
    class CDevice;
    
    class CImageResource;
}

namespace BalEditor
{
    class CShaderGraph;
    
    class CMaterialEditor;
    
    class CAssetBrowser
    {
    public:
        CAssetBrowser();
        
        ~CAssetBrowser();
        
        CAssetBrowser( const CAssetBrowser& ) = delete;
        
        CAssetBrowser( CAssetBrowser&& ) = delete;
        
        CAssetBrowser& operator=( const CAssetBrowser& ) = delete;
        
        CAssetBrowser& operator=( CAssetBrowser&& ) = delete;
        
        void Initialize( const ISystem* pSystem );
        
        void Draw();
        
        void Cleanup();
        
        void ShowWindow();
        
        void SetShaderGraphReference( CShaderGraph* pShaderGraph, CMaterialEditor* pMaterialEditor );
        
        const char* GetCurrentDirectory();
    
    private:
        Balbino::CTexture* m_pUnknownIcon;
        Balbino::CTexture* m_pFolderIcon;
        Balbino::CTexture* m_pBalbinoIcon;
        Balbino::CTexture* m_pImageIcon;
        Balbino::CTexture* m_pAudioIcon;
        Balbino::CTexture* m_pModelIcon;
        Balbino::CTexture* m_pPresetIcon;
        Balbino::CTexture* m_pCodeIcon;
        Balbino::CTexture* m_pFontIcon;
        Balbino::CTexture* m_pMaterialIcon;
        Balbino::CTexture* m_pShaderIcon;
        
        VkDescriptorSet m_pVkDescriptorSetUnknownIcon;
        VkDescriptorSet m_pVkDescriptorSetFolderIcon;
        VkDescriptorSet m_pVkDescriptorSetBalbinoIcon;
        VkDescriptorSet m_pVkDescriptorSetImageIcon;
        VkDescriptorSet m_pVkDescriptorSetAudioIcon;
        VkDescriptorSet m_pVkDescriptorSetModelIcon;
        VkDescriptorSet m_pVkDescriptorSetPresetIcon;
        VkDescriptorSet m_pVkDescriptorSetCodeIcon;
        VkDescriptorSet m_pVkDescriptorSetFontIcon;
        VkDescriptorSet m_pVkDescriptorSetMaterialIcon;
        VkDescriptorSet m_pVkDescriptorSetShaderIcon;
        
        CShaderGraph   * m_pShaderGraph;
        CMaterialEditor* m_pMaterialEditor;
        
        const ISystem* m_pSystem;
        
        uint64_t m_newFileFronID;
        float    m_size;
        bool     m_isVisible;
        bool     m_newFile;
        bool     m_updateCurrentDirectory;
        
        std::vector<SFile> m_currentDirectory;
        std::vector<SFile> m_files;
        std::string        m_currentDirectoryName;
        std::string        m_currentName;
        
        void FindAllFiles();
        
        void CreateMaterial( const SFile& file, std::string_view name ) const;
        
        void DrawTree( const std::string& path, uint32_t& nodeIdx );
        
        void GetAllFilesInSelectedPath( std::string path, std::vector<SFile>& filesInDirectory );
        
        void HandelSelected( const SFile& currentFile, bool isSelected );
        
        void MoveFile( SFile* pFile, const std::filesystem::path& destination );
        
        void Rename( const std::filesystem::path& oldName, std::filesystem::path newName );
    };
}
