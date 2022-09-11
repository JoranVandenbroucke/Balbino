#pragma once
#include <map>

#include "IMesh.h"

namespace Balbino
{
	class CMaterial;
	class CTexture;
}
namespace BalVulkan
{
	class CShaderPipeline;
}

struct IResourceManager
{
	virtual ~IResourceManager() = default;

	virtual Balbino::CTexture* LoadTexture( std::string_view assetPath ) = 0;
	virtual BalVulkan::CShaderPipeline* LoadShader( std::string_view assetPath ) = 0;
	virtual Balbino::CMaterial* LoadMaterial( std::string_view assetPath ) = 0;
	virtual Balbino::IMesh* LoadModel( std::string_view file ) = 0;

	virtual bool BindMaterial( uint64_t id ) = 0;
	virtual Balbino::CTexture* GetTexture( CUuid textureId, bool tryToCreateWhenNotFound = false ) = 0;
	virtual BalVulkan::CShaderPipeline* GetShader( CUuid shaderId, bool tryToCreateWhenNotFound = false ) = 0;
	virtual Balbino::CMaterial* GetMaterial( CUuid materialID, bool tryToCreateWhenNotFound = false ) = 0;
	virtual Balbino::IMesh* GetModel( CUuid meshId, bool tryToCreateWhenNotFound = false ) = 0;
	virtual const std::map<uint64_t, Balbino::CMaterial*>& GetAllLoadedMaterials() const = 0;
	virtual void ReloadAll( BalVulkan::CCommandPool* commandPool, BalVulkan::CQueue* queue ) = 0;

    virtual void UnloadMaterial(CUuid materialId) = 0;
};
