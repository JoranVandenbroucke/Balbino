#pragma once
#include "../IResourceManager.h"
#include <Serializer.h>

namespace BalbinoControl
{
    class CResourceManager final : public IResourceManager
    {
    public:
        CResourceManager()                                     = default;
        ~CResourceManager() override                           = default;
        CResourceManager( const CResourceManager& )            = delete;
        CResourceManager( CResourceManager&& )                 = delete;
        CResourceManager& operator=( const CResourceManager& ) = delete;
        CResourceManager& operator=( CResourceManager&& )      = delete;

        void Initialize( FawnVision::CRenderer* pRenderer ) override;
        void Cleanup() override;

        FawnVision::CTexture* GetTexture( const CUuid& textureId ) override;
        FawnVision::CTexture* GetTexture( std::string_view assetPath ) override;

        FawnVision::ShaderPipeline GetShader( const CUuid& shaderId ) override;
        FawnVision::ShaderPipeline GetShader( std::string_view assetPath ) override;

        FawnVision::CMaterial* GetMaterial( const CUuid& materialId ) override;
        FawnVision::CMaterial* GetMaterial( std::string_view assetPath ) override;

        FawnVision::CMesh* GetModel( const CUuid& meshId ) override;
        FawnVision::CMesh* GetModel( std::string_view assetPath ) override;

    private:
        std::unordered_map<CUuid, FawnVision::ShaderPipeline> m_loadedShaderMap {};

        std::vector<SFile> m_files {};
        FawnVision::CRenderer* m_pRenderer { nullptr };

        FawnVision::CTexture* LoadTexture( std::string_view assetPath );
        FawnVision::ShaderPipeline LoadShader( std::string_view assetPath );
        FawnVision::CMaterial* LoadMaterial( std::string_view assetPath );
        FawnVision::CMesh* LoadModel( std::string_view assetPath );

        const char* IdToFilePath( const CUuid& id );
        CUuid FilePathToUuid( std::string path );
        void FindAllFiles() noexcept;
        [[nodiscard]] static SFile CreateFileFromPath( const std::filesystem::path& path );
    };
}// namespace BalbinoControl