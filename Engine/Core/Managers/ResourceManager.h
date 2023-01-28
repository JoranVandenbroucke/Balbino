#pragma once
#include "IResourceManager.h"

#include <map>

#include "FileParcer.h"

struct ISystem;

namespace Balbino
{
    class CRenderer;
    
    class CMaterial;
    
    class CTexture;
    
    class CMesh;
}

namespace FawnVision
{
    class CShaderPipeline;
    
    class CRenderPass;
    
    class CQueue;
    
    class CCommandPool;
    
    class CDevice;
    
    class CSwapchain;
}

inline FawnVision::CDevice     * g_pDevice{ nullptr };
inline FawnVision::CCommandPool* g_pCommandPool{ nullptr };
inline FawnVision::CQueue      * g_pQueue{ nullptr };
inline FawnVision::CRenderPass * g_pRenderPass{ nullptr };
inline FawnVision::CSwapchain  * g_pSwapChain{ nullptr };

class CResourceManager final : public IResourceManager
{
public:
    CResourceManager() = default;
    ~CResourceManager() override = default;
    CResourceManager( const CResourceManager& ) = delete;
    CResourceManager( CResourceManager&& ) = delete;
    CResourceManager& operator=( const CResourceManager& ) = delete;
    CResourceManager& operator=( CResourceManager&& ) = delete;
    
    void Initialize( const ISystem* pRenderer ) override;
    void Cleanup() override;
    
    Balbino::CTexture* LoadTexture( std::string_view assetPath ) override;
    FawnVision::CShaderPipeline* LoadShader( std::string_view assetPath ) override;
    Balbino::CMaterial* LoadMaterial( std::string_view assetPath ) override;
    Balbino::IMesh* LoadModel( std::string_view assetPath ) override;
    
    bool BindMaterial( uint64_t id ) override;
    Balbino::CTexture* GetTexture( CUuid getMeshId, bool tryToCreateWhenNotFound = false ) override;
    FawnVision::CShaderPipeline* GetShader( CUuid getMeshId, bool tryToCreateWhenNotFound = false ) override;
    Balbino::CMaterial* GetMaterial( CUuid getMeshId, bool tryToCreateWhenNotFound = false ) override;
    Balbino::IMesh* GetModel( CUuid getMeshId, bool tryToCreateWhenNotFound = false ) override;
    
    [[nodiscard]]const std::map<uint64_t, Balbino::CMaterial*>& GetAllLoadedMaterials() const override;
    void ReloadAll( FawnVision::CCommandPool* commandPool, FawnVision::CQueue* queue ) override;
    void UnloadMaterial( CUuid materialId ) override;
private:
    std::map<uint64_t, Balbino::CTexture*>          m_loadedTextureMap;
    std::map<uint64_t, FawnVision::CShaderPipeline*> m_loadedShaderMap;
    std::map<uint64_t, Balbino::CMaterial*>         m_loadedMaterialMap;
    std::map<uint64_t, Balbino::IMesh*>             m_loadedMeshMap;
    
    std::vector<SFile> m_files;
    const ISystem* m_pSystem{};
    
    void FindAllFiles();
    [[nodiscard]] static SFile GetData( const std::filesystem::path& path );
};
