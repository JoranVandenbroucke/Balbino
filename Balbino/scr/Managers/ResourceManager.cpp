#include "ResourceManager.h"

#include "FileParcer.h"
#include "Material.h"
#include "Texture.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/UBOStructs.h"

#include <fstream>

#include "IScene.h"

void CResourceManager::Initialize( const ISystem* pRenderer )
{
    m_pSystem = pRenderer;
}

void CResourceManager::Cleanup()
{
    for ( const auto& [ fst, snd ] : m_loadedMeshMap )
    {
        snd->Cleanup();
        delete snd;
    }
    for ( const auto& [ fst, snd ] : m_loadedMaterialMap )
    {
        snd->Cleanup();
        delete snd;
    }
    for ( const auto& [ fst, snd ] : m_loadedShaderMap )
    {
        snd->Release();
    }
    for ( const auto& [ fst, snd ] : m_loadedTextureMap )
    {
        snd->Cleanup();
        delete snd;
    }
    m_files.clear();
    m_loadedMeshMap.clear();
    m_loadedMaterialMap.clear();
    m_loadedShaderMap.clear();
    m_loadedTextureMap.clear();
}

Balbino::CTexture* CResourceManager::LoadTexture( const std::string_view assetPath )
{
    std::ifstream file( assetPath.data(), std::ios::in | std::ios::binary );
    if ( !file.is_open())
    {
        return nullptr;
    }
    
    uint64_t uuid;
    BinaryReadWrite::Read( file, uuid );
    if ( m_loadedTextureMap.contains( uuid ))
    {
        return m_loadedTextureMap.at( uuid );
    }
    uint8_t type;
    BinaryReadWrite::Read( file, type );
    if ( static_cast< EFileTypes >( type ) == EFileTypes::Image )
    {
        const auto pTextureComponent = new Balbino::CTexture{ g_pDevice, uuid };
        uint8_t    imageType{};
        uint32_t   imageFormat{};
        uint8_t    mips{};
        uint8_t    layers{};
        int        anisotropy{};
        int        sampleLevel{};
        int        mipmapMode{};
        int        filterMode{};
        int        wrapModeU{};
        int        wrapModeV{};
        int        wrapModeW{};
        uint32_t   width{};
        uint32_t   height{};
        uint32_t   depth{};
        uint8_t    pitch{};
        
        BinaryReadWrite::Read( file, imageType );
        BinaryReadWrite::Read( file, imageFormat );
        BinaryReadWrite::Read( file, mips );
        BinaryReadWrite::Read( file, layers );
        BinaryReadWrite::Read( file, anisotropy );
        BinaryReadWrite::Read( file, sampleLevel );
        BinaryReadWrite::Read( file, mipmapMode );
        BinaryReadWrite::Read( file, filterMode );
        BinaryReadWrite::Read( file, wrapModeU );
        BinaryReadWrite::Read( file, wrapModeV );
        BinaryReadWrite::Read( file, wrapModeW );
        BinaryReadWrite::Read( file, width );
        BinaryReadWrite::Read( file, height );
        BinaryReadWrite::Read( file, depth );
        BinaryReadWrite::Read( file, pitch );
        uint32_t size{ width * height * depth * pitch };
        void* pImageData = malloc( size );
        BinaryReadWrite::Read( file, pImageData, size );
        pTextureComponent->Initialize((BalVulkan::EImageViewType::Enum) imageType,
                                      (BalVulkan::EFormat::Enum) imageFormat,
                                      mips,
                                      layers,
                                      anisotropy,
                                      sampleLevel,
                                      mipmapMode,
                                      filterMode,
                                      (BalVulkan::ESamplerAddressMode::Enum) wrapModeU,
                                      (BalVulkan::ESamplerAddressMode::Enum) wrapModeV,
                                      (BalVulkan::ESamplerAddressMode::Enum) wrapModeW,
                                      width,
                                      height,
                                      depth,
                                      pitch,
                                      pImageData,
                                      g_pCommandPool,
                                      g_pQueue
        );
        free( pImageData );
        m_loadedTextureMap[uuid] = pTextureComponent;
        file.close();
        return pTextureComponent;
    }
    file.close();
    return nullptr;
}

BalVulkan::CShaderPipeline* CResourceManager::LoadShader( std::string_view assetPath )
{
    std::ifstream file( assetPath.data(), std::ios::in | std::ios::binary );
    if ( !file.is_open())
    {
        return nullptr;
    }
    
    uint64_t uuid;
    BinaryReadWrite::Read( file, uuid );
    if ( m_loadedShaderMap.contains( uuid ))
    {
        return m_loadedShaderMap.at( uuid );
    }
    
    uint8_t type;
    BinaryReadWrite::Read( file, type );
    if ( type != (uint8_t) EFileTypes::Shader )
    {
        file.close();
        return nullptr;
    }
    
    uint8_t               shaderCount;   //eg. 2 shaders
    uint8_t               shaderType;    //eg. vertex
    uint64_t              shaderSize;    //eg. 965 char long
    std::vector<uint8_t>  shaderTypes;
    std::vector<uint64_t> shadersSizes;
    BinaryReadWrite::Read( file, shaderCount );
    std::vector<BalVulkan::CShader*> shaderVector( shaderCount, nullptr );
    shadersSizes.reserve( shaderCount );
    shaderTypes.reserve( shaderCount );
    for ( uint8_t i{}; i < shaderCount; ++i )
    {
        BinaryReadWrite::Read( file, shaderSize );
        BinaryReadWrite::Read( file, shaderType );
        shadersSizes.push_back( shaderSize );
        shaderTypes.push_back( shaderType );
    }
    for ( uint8_t i{}; i < shaderCount; ++i )
    {
        void* pData = malloc( sizeof( uint32_t ) * shadersSizes[i] );
        
        BinaryReadWrite::Read( file, pData, shadersSizes[i] * sizeof( uint32_t ));
        std::vector<uint32_t> shaderData( shadersSizes[i], 0 );
        shaderData.assign((uint32_t*) pData, (uint32_t*) pData + shadersSizes[i] );
        
        free( pData );
        
        shaderVector[i] = BalVulkan::CShader::CreateNew( g_pDevice );
        shaderVector[i]->Initialize(
                shaderData.data(),
                shaderData.size(),
                BalVulkan::EShaderStage::Enum( 1 << shaderTypes[i] ));
    }
    
    BalVulkan::CShaderPipeline* pPipeline = BalVulkan::CShaderPipeline::CreateNew( g_pDevice );
    
    pPipeline->Initialize(
            shaderTypes, shaderVector, *g_pRenderPass, shaderVector[0]->GetVertexComponents(), 1, g_pSwapChain
    );
    for ( uint64_t i{}; i < shaderVector.size(); ++i )
    {
        shaderVector[i]->Release();
    }
    m_loadedShaderMap[uuid] = pPipeline;
    file.close();
    return pPipeline;
}

Balbino::CMaterial* CResourceManager::LoadMaterial( std::string_view assetPath )
{
    std::ifstream file( assetPath.data(), std::ios::in | std::ios::binary );
    if ( !file.is_open())
    {
        return nullptr;
    }
    
    uint64_t uuid;
    BinaryReadWrite::Read( file, uuid );
    if ( m_loadedMaterialMap.contains( uuid ))
    {
        return m_loadedMaterialMap.at( uuid );
    }
    uint8_t type;
    BinaryReadWrite::Read( file, type );
    if ( static_cast< EFileTypes >( type ) == EFileTypes::Material )
    {
        uint64_t                                shaderId;
        uint64_t                                size;
        bool                                    hasOnlyShaderResources;
        std::vector<BalVulkan::SShaderResource> shaderResources;
        std::vector<BalVulkan::SDescriptorSet>  descriptorSets;
        BinaryReadWrite::Read( file, shaderId );
        BinaryReadWrite::Read( file, size );
        
        void* pData = malloc( size * sizeof( BalVulkan::SShaderResource ));
        BinaryReadWrite::Read( file, pData, size * sizeof( BalVulkan::SShaderResource ));
        shaderResources.assign((BalVulkan::SShaderResource*) pData, (BalVulkan::SShaderResource*) pData + size );
        free( pData );
        
        BinaryReadWrite::Read( file, pData, size * sizeof( BalVulkan::SShaderResource ));
        BinaryReadWrite::IsAtEnd( file, hasOnlyShaderResources );
        
        for ( const auto& resource : shaderResources )
        {
            if ( resource.type == BalVulkan::EShaderResourceType::BufferUniform )
            {
                if ( resource.binding == 0 )
                {
                    BalVulkan::CBuffer* pUBO = m_pSystem->GetCurrentActiveScene()->GetModelBuffer();
                    descriptorSets.emplace_back( BalVulkan::SDescriptorSet::EType::Buffer, pUBO, 0, 0 );
                }
                else if ( resource.binding == 1 )
                {
                    BalVulkan::CBuffer* pUBO = m_pSystem->GetCurrentActiveScene()->GetShadingBuffer();
                    descriptorSets.emplace_back( BalVulkan::SDescriptorSet::EType::Buffer, pUBO, 0, 1 );
                }
            }
            else if ( resource.type == BalVulkan::EShaderResourceType::Image || resource.type == BalVulkan::EShaderResourceType::ImageSampler )
            {
                const auto& texture = GetTexture( resource.resourceID, true );
                if ( texture )
                {
                    descriptorSets.emplace_back(
                            BalVulkan::SDescriptorSet::EType::Image,
                            texture->GetImageViewObject(),
                            texture->GetSamplerObject(),
                            resource.set,
                            resource.binding
                    );
                }
            }
        }
        
        file.close();
        Balbino::CMaterial* pMaterial = Balbino::CMaterial::CreateNew( uuid, shaderId, g_pCommandPool );
        pMaterial->Initialize( GetShader( shaderId, true ), descriptorSets, g_pDevice );
        m_loadedMaterialMap[uuid] = pMaterial;
        return pMaterial;
    }
    file.close();
    return nullptr;
}

Balbino::IMesh* CResourceManager::LoadModel( std::string_view assetPath )
{
    std::ifstream file( assetPath.data(), std::ios::in | std::ios::binary );
    if ( !file.is_open())
    {
        return nullptr;
    }
    
    uint64_t uuid;
    BinaryReadWrite::Read( file, uuid );
    if ( m_loadedMeshMap.contains( uuid ))
    {
        return m_loadedMeshMap.at( uuid );
    }
    uint8_t type;
    BinaryReadWrite::Read( file, type );
    if ( static_cast< EFileTypes >( type ) == EFileTypes::Model )
    {
        uint64_t                            indicesSize;
        uint64_t                            verticesSize;
        uint64_t                            metadataSize;
        std::vector<uint32_t>               indices;
        std::vector<BalVulkan::SVertex>     vertices;
        std::vector<Balbino::SMeshMetadata> metadata;
        
        BinaryReadWrite::Read( file, indicesSize );
        BinaryReadWrite::Read( file, verticesSize );
        BinaryReadWrite::Read( file, metadataSize );
        
        void* pData = malloc( indicesSize * sizeof( uint32_t ));
        BinaryReadWrite::Read( file, pData, indicesSize * sizeof( uint32_t ));
        indices.assign((uint32_t*) pData, (uint32_t*) pData + indicesSize );
        free( pData );
        
        pData = malloc( verticesSize * sizeof( BalVulkan::SVertex ));
        BinaryReadWrite::Read( file, pData, verticesSize * sizeof( BalVulkan::SVertex ));
        vertices.assign((BalVulkan::SVertex*) pData, (BalVulkan::SVertex*) pData + verticesSize );
        free( pData );
        
        metadata.resize( metadataSize );
        for ( auto& meta : metadata )
        {
            BinaryReadWrite::Read( file, meta );
        }
        
        Balbino::CMesh* pMesh = Balbino::CMesh::CreateNew( vertices, indices, metadata, uuid );
        pMesh->Initialize( g_pDevice, g_pCommandPool, g_pQueue );
        return m_loadedMeshMap[uuid] = pMesh;
    }
    file.close();
    
    return nullptr;
}

bool CResourceManager::BindMaterial( uint64_t id )
{
    if ( m_loadedMaterialMap.contains( id ))
    {
        m_loadedMaterialMap[id]->Bind();
        return true;
    }
    if ( !m_loadedMaterialMap.empty())
    {
        m_loadedMaterialMap.begin()->second->Bind();
        return true;
    }
    return false;
}

Balbino::CTexture* CResourceManager::GetTexture( CUuid getMeshId, bool tryToCreateWhenNotFound )
{
    if ( m_loadedTextureMap.contains((uint64_t) getMeshId ))
    {
        return m_loadedTextureMap[(uint64_t) getMeshId];
    }
    if ( tryToCreateWhenNotFound )
    {
        FindAllFiles();
        const auto texture{ std::find( m_files.begin(), m_files.end(), getMeshId ) };
        if ( texture != m_files.end())
        {
            return LoadTexture( texture->path );
        }
    }
    return nullptr;
}

BalVulkan::CShaderPipeline* CResourceManager::GetShader( CUuid getMeshId, bool tryToCreateWhenNotFound )
{
    if ( m_loadedShaderMap.contains((uint64_t) getMeshId ))
    {
        return m_loadedShaderMap[(uint64_t) getMeshId];
    }
    if ( tryToCreateWhenNotFound )
    {
        FindAllFiles();
        auto shader{ std::find( m_files.begin(), m_files.end(), getMeshId ) };
        if ( shader != m_files.end())
        {
            return LoadShader( shader->path );
        }
    }
    return nullptr;
}

Balbino::CMaterial* CResourceManager::GetMaterial( CUuid getMeshId, bool tryToCreateWhenNotFound )
{
    if ( m_loadedMaterialMap.contains((uint64_t) getMeshId ))
    {
        return m_loadedMaterialMap[(uint64_t) getMeshId];
    }
    if ( tryToCreateWhenNotFound )
    {
        FindAllFiles();
        auto material{ std::find( m_files.begin(), m_files.end(), getMeshId ) };
        if ( material != m_files.end())
        {
            return LoadMaterial( material->path );
        }
    }
    return nullptr;
}

Balbino::IMesh* CResourceManager::GetModel( CUuid getMeshId, bool tryToCreateWhenNotFound )
{
    if ( m_loadedMeshMap.contains((uint64_t) getMeshId ))
    {
        return m_loadedMeshMap[(uint64_t) getMeshId];
    }
    if ( tryToCreateWhenNotFound )
    {
        FindAllFiles();
        auto model{ std::find( m_files.begin(), m_files.end(), getMeshId ) };
        if ( model != m_files.end())
        {
            return LoadModel( model->path );
        }
    }
    return nullptr;
}

const std::map<uint64_t, Balbino::CMaterial*>& CResourceManager::GetAllLoadedMaterials() const
{
    return m_loadedMaterialMap;
}

void CResourceManager::ReloadAll( BalVulkan::CCommandPool* commandPool, BalVulkan::CQueue* queue )
{
    g_pCommandPool = commandPool;
    g_pQueue       = queue;
    for ( const auto& mesh : m_loadedMeshMap )
    {
        mesh.second->Cleanup();
        mesh.second->Initialize( g_pDevice, commandPool, queue );
    }
    for ( const auto& shader : m_loadedShaderMap )
    {
        shader.second->Release();
    }
    m_loadedShaderMap.clear();
    for ( const auto& material : m_loadedMaterialMap )
    {
        std::ifstream file(
                std::find( m_files.begin(), m_files.end(), material.first )->path, std::ios::in | std::ios::binary
        );
        if ( !file.is_open())
        {
            continue;
        }
        uint64_t                                uuid;
        uint8_t                                 type;
        uint64_t                                shaderId;
        bool                                    hasOnlyShaderResources;
        std::vector<BalVulkan::SShaderResource> shaderResources;
        std::vector<BalVulkan::SDescriptorSet>  descriptorSets;
        BinaryReadWrite::Read( file, uuid );
        BinaryReadWrite::Read( file, type );
        BinaryReadWrite::Read( file, shaderId );
        BinaryReadWrite::Read( file, shaderResources );
        BinaryReadWrite::IsAtEnd( file, hasOnlyShaderResources );
        
        for ( const auto& resource : shaderResources )
        {
            if ( resource.type == BalVulkan::EShaderResourceType::BufferUniform )
            {
                if ( resource.set == 0 && resource.binding == 0 )
                {
                    BalVulkan::CBuffer* pUBO = m_pSystem->GetCurrentActiveScene()->GetModelBuffer();
                    pUBO->Initialize(
                            sizeof( SModelObject ),
                            BalVulkan::EBufferUsageFlagBits::UniformBufferBit,
                            BalVulkan::EMemoryPropertyFlagBits::Enum(
                                    BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit
                            ));
                    descriptorSets.emplace_back(
                            BalVulkan::SDescriptorSet::EType::Buffer, pUBO, resource.set, resource.binding
                    );
                }
                else if ( resource.set == 0 && resource.binding == 1 )
                {
                    BalVulkan::CBuffer* pUBO = m_pSystem->GetCurrentActiveScene()->GetShadingBuffer();
                    pUBO->Initialize(
                            sizeof( SLightObject ),
                            BalVulkan::EBufferUsageFlagBits::UniformBufferBit,
                            BalVulkan::EMemoryPropertyFlagBits::Enum(
                                    BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit
                            ));
                    descriptorSets.emplace_back(
                            BalVulkan::SDescriptorSet::EType::DynamicBuffer, pUBO, resource.set, resource.binding
                    );
                }
            }
            else
            {
                const auto& texture = GetTexture( resource.resourceID, true );
                if ( texture )
                {
                    descriptorSets.emplace_back(
                            BalVulkan::SDescriptorSet::EType::Image,
                            texture->GetImageViewObject(),
                            texture->GetSamplerObject(),
                            resource.set,
                            resource.binding
                    );
                }
            }
        }
        file.close();
        material.second->Cleanup();
        delete material.second;
        m_loadedMaterialMap[uuid] = new Balbino::CMaterial{ uuid, shaderId, g_pCommandPool };
        m_loadedMaterialMap[uuid]->Initialize( GetShader( shaderId, true ), descriptorSets, g_pDevice );
    }
}

void CResourceManager::FindAllFiles()
{
    auto it = m_files.begin();
    while ( it != m_files.end())
    {
        if ( !std::filesystem::exists( it->path ))
        {
            //Remove
            it = m_files.erase( it );
        }
        else
        {
            ++it;
        }
    }
    
    for ( auto file = std::filesystem::recursive_directory_iterator(
            "..\\Data"
    ); file != std::filesystem::recursive_directory_iterator(); ++file )
    {
        if ( file->path().extension() != ".basset" )
        {
            continue;
        }
        auto currentFileLastWriteTime = std::filesystem::last_write_time( *file );
        
        // File creation
        auto filetIt = std::ranges::find_if(
                m_files, [ &file ]( const SFile& filePair ) -> bool
                {
                    return filePair.path == file->path();
                }
        );
        if ( filetIt == m_files.end())
        {
            //CreateNew
            m_files.push_back( GetData( relative( file->path())));
            m_files.back().lastWrittenTime = currentFileLastWriteTime;
            m_files.back().depth           = file.depth() + 1;
        }
        else
        {
            if ( filetIt->lastWrittenTime != currentFileLastWriteTime )
            {
                //Modify
                ( *filetIt ) = GetData( relative( file->path()));
                filetIt->lastWrittenTime = currentFileLastWriteTime;
                filetIt->depth           = file.depth() + 1;
            }
        }
    }
}

SFile CResourceManager::GetData( const std::filesystem::path& path )
{
    SFile         file{};
    std::ifstream fileStream( path, std::ios::in | std::ios::binary );
    if ( fileStream.is_open())
    {
        uint8_t value;
        BinaryReadWrite::Read( fileStream, file.uuid );
        BinaryReadWrite::Read( fileStream, value );
        
        file.type     = static_cast< EFileTypes > ( value );
        file.path     = path.string();
        file.fileName = path.filename().string();
        fileStream.close();
    }
    return file;
}

void CResourceManager::UnloadMaterial( CUuid materialId )
{
    if ( m_loadedMaterialMap.contains((uint64_t) materialId ))
    {
        Balbino::CMaterial* mat = m_loadedMaterialMap.at((uint64_t) materialId );
        mat->Cleanup();
        m_loadedMaterialMap.erase((uint64_t) materialId );
    }
}
