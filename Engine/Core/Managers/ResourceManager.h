#pragma once
#include <CommandPool.h>
#include <Queue.h>
#include <FileParcer.h>

#include "IResourceManager.h"

class CResourceManager final : public IResourceManager
{
public:
    CResourceManager() = default;
    ~CResourceManager() override = default;
    CResourceManager( const CResourceManager& ) = delete;
    CResourceManager( CResourceManager&& ) = delete;
    CResourceManager& operator=( const CResourceManager& ) = delete;
    CResourceManager& operator=( CResourceManager&& ) = delete;
    
    void Initialize( const ISystem* pSystem, FawnVision::CRenderer* pRenderer ) override;
    void Cleanup() override;
    
    CTexture* LoadTexture( std::string_view assetPath ) override;
    FawnVision::CShaderPipeline* LoadShader( std::string_view assetPath ) override;
    CMaterial* LoadMaterial( std::string_view assetPath ) override;
    IMesh* LoadModel( std::string_view file ) override;
    
    bool BindMaterial( uint64_t id ) override;
    
    CTexture* GetTexture( CUuid textureId, bool tryToCreateWhenNotFound ) override;
    FawnVision::CShaderPipeline* GetShader( CUuid shaderId, bool tryToCreateWhenNotFound ) override;
    CMaterial* GetMaterial( CUuid materialId, bool tryToCreateWhenNotFound ) override;
    IMesh* GetModel( CUuid meshId, bool tryToCreateWhenNotFound ) override;
    
    [[nodiscard]] const std::map<uint64_t, CMaterial*>& GetAllLoadedMaterials() const override;
    void ReloadAll( FawnVision::CRenderer* pRenderer ) override;
    void UnloadMaterial( CUuid materialId ) override;
private:
    std::map<uint64_t, CTexture*>        m_loadedTextureMap{};
    std::map<uint64_t, FawnVision::CShaderPipeline*> m_loadedShaderMap{};
    std::map<uint64_t, CMaterial*>       m_loadedMaterialMap{};
    std::map<uint64_t, IMesh*>           m_loadedMeshMap{};
    
    std::vector<SFile> m_files{};
    const ISystem* m_pSystem{ nullptr };
    FawnVision::CRenderer* m_pRenderer{ nullptr };
    
    void FindAllFiles();
    [[nodiscard]] static SFile GetData( const std::filesystem::path& path );
};
