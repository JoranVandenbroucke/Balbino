#pragma once
#include <map>
#include <UUID.h>

class CTexture;
class CMaterial;
struct IMesh;

namespace FawnVision
{
    class CRenderer;
    class CShaderPipeline;
    class CQueue;
    class CCommandPool;
}

struct ISystem;

struct IResourceManager
{
    virtual ~IResourceManager() = default;
    
    virtual void Initialize( const ISystem* pSystem, FawnVision::CRenderer* pRenderere ) = 0;
    virtual void Cleanup() = 0;
    
    virtual CTexture* LoadTexture( std::string_view assetPath ) = 0;
    virtual FawnVision::CShaderPipeline* LoadShader( std::string_view assetPath ) = 0;
    virtual CMaterial* LoadMaterial( std::string_view assetPath ) = 0;
    virtual IMesh* LoadModel( std::string_view file ) = 0;
    
    virtual bool BindMaterial( uint64_t id ) = 0;
    virtual CTexture* GetTexture( CUuid textureId, bool tryToCreateWhenNotFound = false ) = 0;
    virtual FawnVision::CShaderPipeline* GetShader( CUuid shaderId, bool tryToCreateWhenNotFound = false ) = 0;
    virtual CMaterial* GetMaterial( CUuid materialId, bool tryToCreateWhenNotFound = false ) = 0;
    virtual IMesh* GetModel( CUuid meshId, bool tryToCreateWhenNotFound = false ) = 0;
    
    [[nodiscard]]virtual const std::map<uint64_t, CMaterial*>& GetAllLoadedMaterials() const = 0;
    virtual void ReloadAll( FawnVision::CRenderer* pRenderere ) = 0;
    virtual void UnloadMaterial( CUuid materialId ) = 0;
};
