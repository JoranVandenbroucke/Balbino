#include "ResourceManager.h"

#include "FileParcer.h"
#include <Texture.h>

#include <Mesh.h>
#include <Material.h>
#include <Renderer.h>
#include <UBOStructs.h>

#include <fstream>

#include "IScene.h"

void CResourceManager::Initialize( const ISystem* pSystem, FawnVision::CRenderer* pRenderer )
{
    m_pSystem   = pSystem;
    m_pRenderer = pRenderer;
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
        for ( const auto& resource : snd->GetShaderResourcesVector() )
        {
            if ( resource.type == FawnVision::EShaderResourceType::BufferUniform )
            {
                if ( resource.binding == 0 )
                {
                    FawnVision::CBuffer* pUbo = m_pSystem->GetCurrentActiveScene()->GetModelBuffer();
                    pUbo->Release();
                }
                else if ( resource.binding == 1 )
                {
                    FawnVision::CBuffer* pUbo = m_pSystem->GetCurrentActiveScene()->GetShadingBuffer();
                    pUbo->Release();
                }
            }
        }
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

CTexture* CResourceManager::LoadTexture( std::string_view assetPath )
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
    if ( static_cast< file_type >( type ) == file_type::file_type_image )
    {
        const auto pTextureComponent = new CTexture{ m_pRenderer->GetDevice(), uuid };
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
        pTextureComponent->Initialize((FawnVision::EImageViewType::Enum) imageType,
                                      (FawnVision::EFormat::Enum) imageFormat,
                                      mips,
                                      layers,
                                      anisotropy,
                                      sampleLevel,
                                      mipmapMode,
                                      filterMode,
                                      (FawnVision::ESamplerAddressMode::Enum) wrapModeU,
                                      (FawnVision::ESamplerAddressMode::Enum) wrapModeV,
                                      (FawnVision::ESamplerAddressMode::Enum) wrapModeW,
                                      width,
                                      height,
                                      depth,
                                      pitch,
                                      pImageData,
                                      m_pRenderer->GetCommandPool(),
                                      m_pRenderer->GetQueue());
        free( pImageData );
        m_loadedTextureMap[uuid] = pTextureComponent;
        file.close();
        return pTextureComponent;
    }
    file.close();
    return nullptr;
}

FawnVision::CShaderPipeline* CResourceManager::LoadShader( std::string_view assetPath )
{
    std::ifstream file( assetPath.data(), std::ios::in | std::ios::binary );
    if ( !file.is_open())
    {
        return nullptr;
    }
    std::cout << "Start loading shader " << assetPath.data() << std::endl;
    
    uint64_t uuid;
    BinaryReadWrite::Read( file, uuid );
    if ( m_loadedShaderMap.contains( uuid ))
    {
        return m_loadedShaderMap.at( uuid );
    }
    
    uint8_t type;
    BinaryReadWrite::Read( file, type );
    if ( type != (uint8_t) file_type::file_type_shader )
    {
        file.close();
        return nullptr;
    }
    
    int32_t                           cullmode;
    int32_t                           shaderTypes;    //eg. vertex
    uint8_t                           shaderCount;   //eg. 2 shaders
    uint64_t                          shaderSize;    //eg. 965 char long
    std::vector<uint64_t>             shadersSizes;
    std::vector<FawnVision::CShader*> shaderVector;
    BinaryReadWrite::Read( file, cullmode );
    BinaryReadWrite::Read( file, shaderTypes );
    BinaryReadWrite::Read( file, shaderCount );
    shadersSizes.reserve( shaderCount );
    shaderVector.reserve( shaderCount );
    for ( uint8_t i{}; i < shaderCount; ++i )
    {
        BinaryReadWrite::Read( file, shaderSize );
        shadersSizes.push_back( shaderSize );
    }
    for ( uint8_t i{}; i < shaderCount; ++i )
    {
        void* pData = malloc( sizeof( uint32_t ) * shadersSizes[i] );
        
        BinaryReadWrite::Read( file, pData, shadersSizes[i] * sizeof( uint32_t ));
        std::vector<uint32_t> shaderData( shadersSizes[i], 0 );
        shaderData.assign((uint32_t*) pData, (uint32_t*) pData + shadersSizes[i] );
        
        free( pData );
        
        shaderVector.emplace_back( new FawnVision::CShader{ m_pRenderer->GetDevice() });
        uint16_t j {FawnVision::shader_stage_vertex};
        
        for(int k{}; (k != (i+1)) && j < FawnVision::shader_stage_max; j *= 2 )
        {
            if(shaderTypes&j)
                ++k;
        }
        shaderVector.back()->Initialize(
                shaderData.data(), shaderData.size(), FawnVision::shader_stage( j / 2 ));
    }
    
    FawnVision::CShaderPipeline* pPipeline = new FawnVision::CShaderPipeline{ m_pRenderer->GetDevice() };
    
    pPipeline->Initialize(
            shaderTypes,
            shaderVector,
            *m_pRenderer->GetRenderPass(),
            shaderVector[0]->GetVertexComponents(),
            1, m_pRenderer->GetSwapChain(),
            FawnVision::ECullMode::Enum( cullmode ));
    for ( uint64_t i{}; i < shaderVector.size(); ++i )
    {
        shaderVector[i]->Release();
    }
    m_loadedShaderMap[uuid] = pPipeline;
    file.close();
    std::cout << "End loading Shader " << assetPath.data() << std::endl;
    return pPipeline;
}

CMaterial* CResourceManager::LoadMaterial( std::string_view assetPath )
{
    std::ifstream file( assetPath.data(), std::ios::in | std::ios::binary );
    if ( !file.is_open())
    {
        return nullptr;
    }
    
    std::cout <<  "Start loading material with id " << assetPath.data() << std::endl;
    
    uint64_t uuid;
    BinaryReadWrite::Read( file, uuid );
    if ( m_loadedMaterialMap.contains( uuid ))
    {
        std::cout << "End loading material with id " << assetPath.data() << std::endl;
        return m_loadedMaterialMap.at( uuid );
    }
    uint8_t type;
    BinaryReadWrite::Read( file, type );
    if ( static_cast< file_type >( type ) == file_type::file_type_material )
    {
        uint64_t                                 shaderId;
        uint64_t                                 size;
        bool                                     hasOnlyShaderResources;
        std::vector<FawnVision::SShaderResource> shaderResources;
        std::vector<FawnVision::SDescriptorSet>  descriptorSets;
        BinaryReadWrite::Read( file, shaderId );
        BinaryReadWrite::Read( file, size );
        
        void* pData = malloc( size * sizeof( FawnVision::SShaderResource ));
        BinaryReadWrite::Read( file, pData, size * sizeof( FawnVision::SShaderResource ));
        shaderResources.assign((FawnVision::SShaderResource*) pData, (FawnVision::SShaderResource*) pData + size );
        free( pData );
        
        BinaryReadWrite::IsAtEnd( file, hasOnlyShaderResources );
        
        for ( const auto& resource : shaderResources )
        {
            if ( resource.type == FawnVision::EShaderResourceType::BufferUniform )
            {
                if ( resource.binding == 0 )
                {
                    FawnVision::CBuffer* pUbo = m_pSystem->GetCurrentActiveScene()->GetModelBuffer();
                    pUbo->AddRef();
                    descriptorSets.emplace_back( FawnVision::SDescriptorSet::EType::Buffer, pUbo, 0, 0 );
                }
                else if ( resource.binding == 1 )
                {
                    FawnVision::CBuffer* pUbo = m_pSystem->GetCurrentActiveScene()->GetShadingBuffer();
                    pUbo->AddRef();
                    descriptorSets.emplace_back( FawnVision::SDescriptorSet::EType::Buffer, pUbo, 0, 1 );
                }
            }
            else if ( resource.type == FawnVision::EShaderResourceType::Image || resource.type == FawnVision::EShaderResourceType::ImageSampler )
            {
                const auto& texture = GetTexture( resource.resourceID, true );
                if ( texture )
                {
                    descriptorSets.emplace_back(
                            FawnVision::SDescriptorSet::EType::Image,
                            texture->GetImageViewObject(),
                            texture->GetSamplerObject(),
                            resource.set,
                            resource.binding
                    );
                }
            }
        }
        
        file.close();
        CMaterial* pMaterial{ new CMaterial{ uuid, shaderId, m_pRenderer->GetCommandPool() }};
        pMaterial->Initialize( GetShader( shaderId, true ), descriptorSets, m_pRenderer->GetDevice());
        m_loadedMaterialMap[uuid] = pMaterial;
        return pMaterial;
    }
    file.close();
    std::cout << "End loading material with id " << assetPath.data() << std::endl;
    return nullptr;
}

IMesh* CResourceManager::LoadModel( std::string_view assetPath )
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
    if ( static_cast< file_type >( type ) == file_type::file_type_model )
    {
        uint64_t                         indicesSize;
        uint64_t                         verticesSize;
        uint64_t                         metadataSize;
        std::vector<uint32_t>            indices;
        std::vector<FawnVision::SVertex> vertices;
        std::vector<SMeshMetadata>       metadata;
        
        BinaryReadWrite::Read( file, indicesSize );
        BinaryReadWrite::Read( file, verticesSize );
        BinaryReadWrite::Read( file, metadataSize );
        
        void* pData = malloc( indicesSize * sizeof( uint32_t ));
        BinaryReadWrite::Read( file, pData, indicesSize * sizeof( uint32_t ));
        indices.assign((uint32_t*) pData, (uint32_t*) pData + indicesSize );
        free( pData );
        
        pData = malloc( verticesSize * sizeof( FawnVision::SVertex ));
        BinaryReadWrite::Read( file, pData, verticesSize * sizeof( FawnVision::SVertex ));
        vertices.assign((FawnVision::SVertex*) pData, (FawnVision::SVertex*) pData + verticesSize );
        free( pData );
        
        metadata.resize( metadataSize );
        for ( auto& meta : metadata )
        {
            BinaryReadWrite::Read( file, meta );
        }
        
        FawnVision::CMesh* pMesh{ new FawnVision::CMesh{ vertices, indices, metadata, uuid }};
        pMesh->Initialize( m_pRenderer->GetDevice(), m_pRenderer->GetCommandPool(), m_pRenderer->GetQueue());
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

CTexture* CResourceManager::GetTexture( CUuid getMeshId, bool tryToCreateWhenNotFound )
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

FawnVision::CShaderPipeline* CResourceManager::GetShader( CUuid getMeshId, bool tryToCreateWhenNotFound )
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

CMaterial* CResourceManager::GetMaterial( CUuid materialId, bool tryToCreateWhenNotFound )
{
    if ( m_loadedMaterialMap.contains((uint64_t) materialId ))
    {
        return m_loadedMaterialMap[(uint64_t) materialId];
    }
    if ( tryToCreateWhenNotFound )
    {
        FindAllFiles();
        auto material{ std::find( m_files.begin(), m_files.end(), materialId ) };
        if ( material != m_files.end())
        {
            return LoadMaterial( material->path );
        }
    }
    return nullptr;
}

IMesh* CResourceManager::GetModel( CUuid getMeshId, bool tryToCreateWhenNotFound )
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

const std::map<uint64_t, CMaterial*>& CResourceManager::GetAllLoadedMaterials() const
{
    return m_loadedMaterialMap;
}

void CResourceManager::ReloadAll( FawnVision::CRenderer* pRenderer )
{
    m_pRenderer = pRenderer;
    for ( const auto& mesh : m_loadedMeshMap )
    {
        mesh.second->Cleanup();
        mesh.second->Initialize( m_pRenderer->GetDevice(), pRenderer->GetCommandPool(), pRenderer->GetQueue());
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
        uint64_t                                 uuid;
        uint8_t                                  type;
        uint64_t                                 shaderId;
        bool                                     hasOnlyShaderResources;
        std::vector<FawnVision::SShaderResource> shaderResources;
        std::vector<FawnVision::SDescriptorSet>  descriptorSets;
        BinaryReadWrite::Read( file, uuid );
        BinaryReadWrite::Read( file, type );
        BinaryReadWrite::Read( file, shaderId );
        BinaryReadWrite::Read( file, shaderResources );
        BinaryReadWrite::IsAtEnd( file, hasOnlyShaderResources );
        
        for ( const auto& resource : shaderResources )
        {
            if ( resource.type == FawnVision::EShaderResourceType::BufferUniform )
            {
                if ( resource.set == 0 && resource.binding == 0 )
                {
                    FawnVision::CBuffer* pUbo = m_pSystem->GetCurrentActiveScene()->GetModelBuffer();
                    pUbo->Initialize(
                            sizeof( SModelObject ),
                            FawnVision::EBufferUsageFlagBits::UniformBufferBit,
                            FawnVision::EMemoryPropertyFlagBits::Enum(
                                    FawnVision::EMemoryPropertyFlagBits::HostVisibleBit | FawnVision::EMemoryPropertyFlagBits::HostCoherentBit
                            ));
                    descriptorSets.emplace_back(
                            FawnVision::SDescriptorSet::EType::Buffer, pUbo, resource.set, resource.binding
                    );
                }
                else if ( resource.set == 0 && resource.binding == 1 )
                {
                    FawnVision::CBuffer* pUbo = m_pSystem->GetCurrentActiveScene()->GetShadingBuffer();
                    pUbo->Initialize(
                            sizeof( SLightObject ),
                            FawnVision::EBufferUsageFlagBits::UniformBufferBit,
                            FawnVision::EMemoryPropertyFlagBits::Enum(
                                    FawnVision::EMemoryPropertyFlagBits::HostVisibleBit | FawnVision::EMemoryPropertyFlagBits::HostCoherentBit
                            ));
                    descriptorSets.emplace_back(
                            FawnVision::SDescriptorSet::EType::DynamicBuffer, pUbo, resource.set, resource.binding
                    );
                }
            }
            else
            {
                const auto& texture = GetTexture( resource.resourceID, true );
                if ( texture )
                {
                    descriptorSets.emplace_back(
                            FawnVision::SDescriptorSet::EType::Image,
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
        m_loadedMaterialMap[uuid] = new CMaterial{ uuid, shaderId, m_pRenderer->GetCommandPool() };
        m_loadedMaterialMap[uuid]->Initialize( GetShader( shaderId, true ), descriptorSets, m_pRenderer->GetDevice());
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
        
        file.type     = static_cast< file_type > ( value );
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
        m_loadedMaterialMap[(uint64_t) materialId]->Cleanup();
    }
}
