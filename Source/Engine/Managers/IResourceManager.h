#pragma once
#include <FawnVision.h>
#include <UUID.h>
#include <map>

namespace FawnVision
{
    class CTexture;
    class CMaterial;
    class CMesh;
}// namespace FawnVision

struct ISystem;
struct IResourceManager {
    virtual ~IResourceManager() = default;

    virtual void Initialize( FawnVision::CRenderer* renderer ) = 0;
    virtual void Cleanup()                                     = 0;

    virtual FawnVision::CTexture* GetTexture( const CUuid& textureId )     = 0;
    virtual FawnVision::CTexture* GetTexture( std::string_view assetPath ) = 0;

    virtual FawnVision::ShaderPipeline GetShader( const CUuid& shaderId )      = 0;
    virtual FawnVision::ShaderPipeline GetShader( std::string_view assetPath ) = 0;

    virtual FawnVision::CMaterial* GetMaterial( const CUuid& materialId )    = 0;
    virtual FawnVision::CMaterial* GetMaterial( std::string_view assetPath ) = 0;

    virtual FawnVision::CMesh* GetModel( const CUuid& meshId )        = 0;
    virtual FawnVision::CMesh* GetModel( std::string_view assetPath ) = 0;
};
