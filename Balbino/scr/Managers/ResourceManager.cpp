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
        pTextureComponent->Initialize((BalVulkan::EImageViewType) imageType, (BalVulkan::EFormat) imageFormat, mips,
                                      layers, anisotropy, sampleLevel, mipmapMode, filterMode,
                                      (BalVulkan::ESamplerAddressMode) wrapModeU,
                                      (BalVulkan::ESamplerAddressMode) wrapModeV,
                                      (BalVulkan::ESamplerAddressMode) wrapModeW, width, height, depth, pitch,
                                      pImageData, g_pCommandPool, g_pQueue );
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
    //TODO RTX
    if ( static_cast< EFileTypes >( type ) == EFileTypes::Shader )
    {
        uint16_t shaderComboType;
        BinaryReadWrite::Read( file, shaderComboType );
        std::vector<std::vector<uint32_t>> shaders;
        switch ( shaderComboType )
        {
            case 0u:
            {
                uint64_t size;
                BinaryReadWrite::Read( file, size );

                void* pData = malloc( sizeof( uint32_t ) * size );
                BinaryReadWrite::Read( file, pData, size * sizeof( uint32_t ));

                std::vector<uint32_t> shaderData( size, 0 );
                shaderData.assign((uint32_t*) pData, (uint32_t*) pData + size );

                free( pData );

                shaders.push_back( shaderData );
                break;
            }
            case 1u:
            {
                uint64_t size1;
                uint64_t size2;
                uint64_t size3;

                BinaryReadWrite::Read( file, size1 );
                BinaryReadWrite::Read( file, size2 );
                BinaryReadWrite::Read( file, size3 );

                void* pData1 = malloc( sizeof( uint32_t ) * size1 );
                void* pData2 = malloc( sizeof( uint32_t ) * size2 );
                void* pData3 = malloc( sizeof( uint32_t ) * size3 );

                BinaryReadWrite::Read( file, pData1, size1 * sizeof( uint32_t ));
                BinaryReadWrite::Read( file, pData2, size2 * sizeof( uint32_t ));
                BinaryReadWrite::Read( file, pData3, size3 * sizeof( uint32_t ));

                std::vector<uint32_t> shaderData1( size1, 0 );
                std::vector<uint32_t> shaderData2( size2, 0 );
                std::vector<uint32_t> shaderData3( size3, 0 );

                shaderData1.assign((uint32_t*) pData1, (uint32_t*) pData1 + size1 );
                shaderData2.assign((uint32_t*) pData2, (uint32_t*) pData2 + size2 );
                shaderData3.assign((uint32_t*) pData3, (uint32_t*) pData3 + size3 );

                free( pData1 );
                free( pData2 );
                free( pData3 );

                shaders.push_back( shaderData1 );
                shaders.push_back( shaderData2 );
                shaders.push_back( shaderData3 );
                break;
            }
            case 2u:
            {
                uint64_t size1;
                uint64_t size2;

                BinaryReadWrite::Read( file, size1 );
                BinaryReadWrite::Read( file, size2 );

                void* pData1 = malloc( sizeof( uint32_t ) * size1 );
                void* pData2 = malloc( sizeof( uint32_t ) * size2 );

                BinaryReadWrite::Read( file, pData1, size1 * sizeof( uint32_t ));
                BinaryReadWrite::Read( file, pData2, size2 * sizeof( uint32_t ));

                std::vector<uint32_t> shaderData1( size1, 0 );
                std::vector<uint32_t> shaderData2( size2, 0 );

                shaderData1.assign((uint32_t*) pData1, (uint32_t*) pData1 + size1 );
                shaderData2.assign((uint32_t*) pData2, (uint32_t*) pData2 + size2 );

                free( pData1 );
                free( pData2 );

                shaders.push_back( shaderData1 );
                shaders.push_back( shaderData2 );
                break;
            }
            case 3u:
                break;
            default:
                break;
        }

        std::vector<BalVulkan::CShader*> shaderVector( shaders.size(), nullptr );
        for ( int                        i{}; i < shaders.size(); ++i )
        {
            shaderVector[i] = BalVulkan::CShader::CreateNew( g_pDevice );
            if ( shaderComboType == 0 )
            {
                shaderVector[i]->Initialize( shaders[i].data(), shaders[i].size(),
                                             BalVulkan::EShaderStage::VertexShader );
            }
            else if ( shaderComboType == 1 )
            {
                if ( i == 0 )
                {
                    shaderVector[i]->Initialize( shaders[i].data(), shaders[i].size(),
                                                 BalVulkan::EShaderStage::VertexShader );
                }
                else if ( i == 1 )
                {
                    shaderVector[i]->Initialize( shaders[i].data(), shaders[i].size(),
                                                 BalVulkan::EShaderStage::GeometryShader );
                }
                else
                {
                    shaderVector[i]->Initialize( shaders[i].data(), shaders[i].size(),
                                                 BalVulkan::EShaderStage::FragmentShader );
                }
            }

            else if ( shaderComboType == 2 )
            {
                if ( i == 0 )
                {
                    shaderVector[i]->Initialize( shaders[i].data(), shaders[i].size(),
                                                 BalVulkan::EShaderStage::VertexShader );
                }
                else
                {
                    shaderVector[i]->Initialize( shaders[i].data(), shaders[i].size(),
                                                 BalVulkan::EShaderStage::FragmentShader );
                }
            }
        }

        BalVulkan::CShaderPipeline* pPipeline = BalVulkan::CShaderPipeline::CreateNew( g_pDevice );

        pPipeline->Initialize( shaderComboType, shaderVector, *g_pFrameBuffer, {
                BalVulkan::EVertexComponent::Position,
                BalVulkan::EVertexComponent::Color,
                BalVulkan::EVertexComponent::UV,
                BalVulkan::EVertexComponent::Normal,
                BalVulkan::EVertexComponent::Tangent
        }, 1, g_pSwapChain );
        for ( int i{}; i < shaderVector.size(); ++i )
        {
            shaderVector[i]->Release();
        }
        m_loadedShaderMap[uuid] = pPipeline;
        file.close();
        return pPipeline;
    }
    file.close();
    return nullptr;
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
        bool                                    hasOnlyShaderResources;
        std::vector<BalVulkan::SShaderResource> shaderResources;
        std::vector<BalVulkan::SDescriptorSet>  descriptorSets;
        BinaryReadWrite::Read( file, shaderId );
        BinaryReadWrite::Read( file, shaderResources );
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
                    descriptorSets.emplace_back( BalVulkan::SDescriptorSet::EType::DynamicBuffer, pUBO, 0, 1 );
                }
            }
            else if ( resource.type == BalVulkan::EShaderResourceType::Image || resource.type == BalVulkan::EShaderResourceType::ImageSampler )
            {
                const auto& texture = GetTexture( resource.resourceID, true );
                if ( texture )
                {
                    descriptorSets.emplace_back( BalVulkan::SDescriptorSet::EType::Image, texture->GetImageViewObject(),
                                                 texture->GetSamplerObject(), resource.set, resource.binding );
                }
            }
            else if ( resource.type == BalVulkan::EShaderResourceType::BufferStorage )
            {
                BalVulkan::CBuffer* pUBO = m_pSystem->GetCurrentActiveScene()->GetShadingBuffer();
                descriptorSets.emplace_back( BalVulkan::SDescriptorSet::EType::BufferStorage, pUBO, 0, 1 );
            }
            //else if ( resource.type == BalVulkan::EShaderResourceType::Input && resource.location == 5 )
            //{
            //    BalVulkan::CBuffer* pUBO = m_pSystem->GetCurrentActiveScene()->GetInstanceBuffer();
            //    descriptorSets.emplace_back( BalVulkan::SDescriptorSet::EType::Input, pUBO, 0, 1 );
            //}
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
        uint64_t                        indicesSize;
        uint64_t                        verticesSize;
        std::vector<uint32_t>           indices;
        std::vector<BalVulkan::SVertex> vertices;
        Balbino::SMeshMetadata          root;
        Balbino::SMeshMetadata* pMeshMetaData{ &root };

        BinaryReadWrite::Read( file, indicesSize );
        BinaryReadWrite::Read( file, verticesSize );

        void* pData = malloc( indicesSize * sizeof( uint32_t ));
        BinaryReadWrite::Read( file, pData, indicesSize * sizeof( uint32_t ));
        indices.assign((uint32_t*) pData, (uint32_t*) pData + indicesSize );
        free( pData );

        pData = malloc( verticesSize * sizeof( BalVulkan::SVertex ));
        BinaryReadWrite::Read( file, pData, verticesSize * sizeof( BalVulkan::SVertex ));
        vertices.assign((BalVulkan::SVertex*) pData, (BalVulkan::SVertex*) pData + verticesSize );
        free( pData );

        bool isAtEnd;
        BinaryReadWrite::Read( file, pMeshMetaData->boundingBox.min.x );
        BinaryReadWrite::Read( file, pMeshMetaData->boundingBox.min.y );
        BinaryReadWrite::Read( file, pMeshMetaData->boundingBox.min.z );
        BinaryReadWrite::Read( file, pMeshMetaData->boundingBox.max.x );
        BinaryReadWrite::Read( file, pMeshMetaData->boundingBox.max.y );
        BinaryReadWrite::Read( file, pMeshMetaData->boundingBox.max.z );
        BinaryReadWrite::Read( file, pMeshMetaData->boundingSphere.center.x );
        BinaryReadWrite::Read( file, pMeshMetaData->boundingSphere.center.y );
        BinaryReadWrite::Read( file, pMeshMetaData->boundingSphere.center.z );
        BinaryReadWrite::Read( file, pMeshMetaData->boundingSphere.radius );
        BinaryReadWrite::Read( file, pMeshMetaData->firstIndex );
        BinaryReadWrite::Read( file, pMeshMetaData->indexCount );
        BinaryReadWrite::IsAtEnd( file, isAtEnd );
        while ( !isAtEnd )
        {
            Balbino::SMeshMetadata* pNewMeshMetaData = new Balbino::SMeshMetadata{};
            BinaryReadWrite::Read( file, pNewMeshMetaData->boundingBox.min.x );
            BinaryReadWrite::Read( file, pNewMeshMetaData->boundingBox.min.y );
            BinaryReadWrite::Read( file, pNewMeshMetaData->boundingBox.min.z );
            BinaryReadWrite::Read( file, pNewMeshMetaData->boundingBox.max.x );
            BinaryReadWrite::Read( file, pNewMeshMetaData->boundingBox.max.y );
            BinaryReadWrite::Read( file, pNewMeshMetaData->boundingBox.max.z );
            BinaryReadWrite::Read( file, pNewMeshMetaData->boundingSphere.center.x );
            BinaryReadWrite::Read( file, pNewMeshMetaData->boundingSphere.center.y );
            BinaryReadWrite::Read( file, pNewMeshMetaData->boundingSphere.center.z );
            BinaryReadWrite::Read( file, pNewMeshMetaData->boundingSphere.radius );
            BinaryReadWrite::Read( file, pNewMeshMetaData->firstIndex );
            BinaryReadWrite::Read( file, pNewMeshMetaData->indexCount );

            pMeshMetaData->pNext        = pNewMeshMetaData;
            pNewMeshMetaData->pPrevious = pMeshMetaData;
            pMeshMetaData = pNewMeshMetaData;
            BinaryReadWrite::IsAtEnd( file, isAtEnd );
        }
        Balbino::CMesh* pMesh = Balbino::CMesh::CreateNew( vertices, indices, root, uuid );
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
        std::ifstream file( std::find( m_files.begin(), m_files.end(), material.first )->path,
                            std::ios::in | std::ios::binary );
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
                    pUBO->Initialize( sizeof( SModelObject ), BalVulkan::EBufferUsageFlagBits::UniformBufferBit,
                                      BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
                    descriptorSets.emplace_back( BalVulkan::SDescriptorSet::EType::Buffer, pUBO, resource.set,
                                                 resource.binding );
                }
                else if ( resource.set == 0 && resource.binding == 1 )
                {
                    BalVulkan::CBuffer* pUBO = m_pSystem->GetCurrentActiveScene()->GetShadingBuffer();
                    pUBO->Initialize( sizeof( SLightObject ), BalVulkan::EBufferUsageFlagBits::UniformBufferBit,
                                      BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
                    descriptorSets.emplace_back( BalVulkan::SDescriptorSet::EType::DynamicBuffer, pUBO, resource.set,
                                                 resource.binding );
                }
            }
            else
            {
                const auto& texture = GetTexture( resource.resourceID, true );
                if ( texture )
                {
                    descriptorSets.emplace_back( BalVulkan::SDescriptorSet::EType::Image, texture->GetImageViewObject(),
                                                 texture->GetSamplerObject(), resource.set, resource.binding );
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
            "..\\Data" ); file != std::filesystem::recursive_directory_iterator(); ++file )
    {
        if ( file->path().extension() != ".basset" )
        {
            continue;
        }
        auto currentFileLastWriteTime = std::filesystem::last_write_time( *file );

        // File creation
        auto filetIt = std::ranges::find_if( m_files, [ &file ]( const SFile& filePair ) -> bool
        {
            return filePair.path == file->path();
        } );
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

        file.type     = static_cast< EFileTypes >( value );
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
