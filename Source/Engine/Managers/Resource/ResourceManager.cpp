#include "ResourceManager.h"
#include <ErrorHandling.h>
#include <FawnVision.h>
#include <DeerVulkan.h>
#include <string>

using namespace std::string_literals;

namespace BalbinoControl
{
    void CResourceManager::Initialize( FawnVision::CRenderer* pRenderer )
    {
        m_pRenderer = pRenderer;
        FindAllFiles();
    }
    void CResourceManager::Cleanup()
    {
        for ( const auto& [ fst, snd ] : m_loadedShaderMap )
        {
            snd->Release();
        }

        m_files.clear();
        m_loadedShaderMap.clear();
    }

    FawnVision::CTexture* CResourceManager::GetTexture( const CUuid& id )
    {
        FawnVision::CTexture* pAsset { m_pRenderer->GetAsset<FawnVision::CTexture>( id ) };
        if ( pAsset )
        {
            return pAsset;
        }

        FindAllFiles();
        pAsset = LoadTexture( IdToFilePath( id ) );
        FawnTrace::Assert::IsNotNull( pAsset, "Could not load texture with id: "s + id.ToString() );
        return pAsset;
    }
    FawnVision::CTexture* CResourceManager::GetTexture( std::string_view assetPath )
    {
        FawnVision::CTexture* pAsset { m_pRenderer->GetAsset<FawnVision::CTexture>( FilePathToUuid( assetPath.data() ) ) };
        if ( pAsset )
        {
            return pAsset;
        }

        FindAllFiles();
        pAsset = LoadTexture( assetPath );
        FawnTrace::Assert::IsNotNull( pAsset, "Could not load texture with path: "s + assetPath.data() );
        return pAsset;
    }

    FawnVision::ShaderPipeline CResourceManager::GetShader( const CUuid& id )
    {
        if ( m_loadedShaderMap.contains( id ) )
        {
            return m_loadedShaderMap.at( id );
        }

        FindAllFiles();
        return LoadShader( IdToFilePath( id ) );
    }
    FawnVision::ShaderPipeline CResourceManager::GetShader( std::string_view assetPath )
    {
        if ( CUuid id { FilePathToUuid( assetPath.data() ) }; m_loadedShaderMap.contains( id ) )
        {
            return m_loadedShaderMap.at( id );
        }

        FindAllFiles();
        return LoadShader( assetPath );
    }

    FawnVision::CMaterial* CResourceManager::GetMaterial( const CUuid& id )
    {
        FawnVision::CMaterial* pAsset { m_pRenderer->GetAsset<FawnVision::CMaterial>( id ) };
        if ( pAsset )
        {
            return pAsset;
        }

        FindAllFiles();
        pAsset = LoadMaterial( IdToFilePath( id ) );
        FawnTrace::Assert::IsNotNull( pAsset, "Could not load Material with id: "s + id.ToString() );
        return pAsset;
    }
    FawnVision::CMaterial* CResourceManager::GetMaterial( std::string_view assetPath )
    {
        FawnVision::CMaterial* pAsset { m_pRenderer->GetAsset<FawnVision::CMaterial>( FilePathToUuid( assetPath.data() ) ) };
        if ( pAsset )
        {
            return pAsset;
        }

        FindAllFiles();
        pAsset = LoadMaterial( assetPath );
        FawnTrace::Assert::IsNotNull( pAsset, "Could not load Material with path: "s + assetPath.data() );
        return pAsset;
    }

    FawnVision::CMesh* CResourceManager::GetModel( const CUuid& id )
    {
        FawnVision::CMesh* pAsset { m_pRenderer->GetAsset<FawnVision::CMesh>( id ) };
        if ( pAsset )
        {
            return pAsset;
        }

        FindAllFiles();
        pAsset = LoadModel( IdToFilePath( id ) );
        FawnTrace::Assert::IsNotNull( pAsset, "Could not load LoadMaterial with id: "s + id.ToString() );
        return pAsset;
    }
    FawnVision::CMesh* CResourceManager::GetModel( std::string_view assetPath )
    {
        FawnVision::CMesh* pAsset { m_pRenderer->GetAsset<FawnVision::CMesh>( FilePathToUuid( assetPath.data() ) ) };
        if ( pAsset )
        {
            return pAsset;
        }

        FindAllFiles();
        pAsset = LoadModel( assetPath );
        FawnTrace::Assert::IsNotNull( pAsset, "Could not load LoadMaterial with path: "s + assetPath.data() );
        return pAsset;
    }

    FawnVision::CTexture* CResourceManager::LoadTexture( std::string_view assetPath )
    {
        std::ifstream file( assetPath.data(), std::ios::binary | std::ios::in );
        if ( !file.is_open() )
        {
            return nullptr;
        }
        CUuid id {};
        uint8_t type {};
        FawnVision::STextureCreateInfo createInfo;

        Serialization::Read( file, id );
        Serialization::Read( file, type );
        if ( static_cast<file_type>( type ) != file_type::image )
        {
            file.close();
            return nullptr;
        }
        Serialization::Read( file, createInfo );
        file.close();

        FawnVision::CTexture* pTextureComponent { m_pRenderer->CreateTexture( id, createInfo, createInfo.imageCreateInfo.mipLevels == ~0u ) };
        free( createInfo.pImageData );

        return pTextureComponent;
    }
    FawnVision::ShaderPipeline CResourceManager::LoadShader( std::string_view assetPath )
    {
        std::ifstream file( assetPath.data(), std::ios::binary | std::ios::in );
        if ( !file.is_open() )
        {
            return nullptr;
        }

        CUuid id {};
        uint8_t type {};

        Serialization::Read( file, id );
        Serialization::Read( file, type );
        if ( static_cast<file_type>( type ) != file_type::shader )
        {
            file.close();
            return nullptr;
        }

        std::vector<FawnVision::SShaderCreateInfo> shadersCreateInfo;
        std::vector<FawnVision::Shader> shaders;
        FawnVision::SShaderPassCreateInfo pipelineCreateInfo;
        int cullMode;
        Serialization::Read( file, shadersCreateInfo );
        Serialization::Read( file, cullMode );
        file.close();
        std::ranges::for_each( shadersCreateInfo, [ &shaders, &pipelineCreateInfo, &renderer = m_pRenderer ]( const FawnVision::SShaderCreateInfo& createInfo ) {
            shaders.emplace_back( renderer->CreateShader( createInfo ) );
            pipelineCreateInfo.types |= createInfo.type;
            pipelineCreateInfo.components |= shaders.back()->GetVertexComponents();
        } );
        pipelineCreateInfo.cullModeFlag = (uint8_t)cullMode;
        FawnVision::ShaderPipeline pShaderPipeline { m_pRenderer->CreateShaderPipeline( pipelineCreateInfo, shaders ) };

        m_loadedShaderMap[ id ] = pShaderPipeline;
        return pShaderPipeline;
    }
    FawnVision::CMaterial* CResourceManager::LoadMaterial( std::string_view assetPath )
    {
        std::ifstream file( assetPath.data(), std::ios::binary | std::ios::in );
        if ( !file.is_open() )
        {
            return nullptr;
        }

        CUuid id {};
        uint8_t type {};
        FawnVision::SMaterialCreateInfo createInfo;

        Serialization::Read( file, id );
        Serialization::Read( file, type );
        if ( type != static_cast<uint8_t>( file_type::material ) )
        {
            file.close();
            return nullptr;
        }
        Serialization::Read( file, createInfo );
        file.close();

        for ( const auto& resource : createInfo.shaderResources )
        {
            FawnVision::shader_resource_type resourceType { (FawnVision::shader_resource_type)resource.type };
            if ( resourceType == FawnVision::shader_resource_type::buffer_uniform )
            {
                if ( resource.binding == 0 )
                {
                    FawnVision::Buffer pUbo = m_pRenderer->GetModelBuffer();
                    pUbo->AddRef();
                    createInfo.descriptorSets.emplace_back( FawnVision::SDescriptorSet::type::buffer, pUbo, 0, 0 );
                }
                else if ( resource.binding == 1 )
                {
                    FawnVision::Buffer pUbo = m_pRenderer->GetShadingBuffer();
                    pUbo->AddRef();
                    createInfo.descriptorSets.emplace_back( FawnVision::SDescriptorSet::type::buffer, pUbo, 0, 1 );
                }
            }
            else if ( resourceType == FawnVision::shader_resource_type::image || resourceType == FawnVision::shader_resource_type::image_sampler )
            {
                FawnVision::CTexture* texture = GetTexture( resource.resourceId );
                if ( !texture )
                {
                    continue;
                }
                createInfo.descriptorSets.emplace_back( FawnVision::SDescriptorSet::type::image, texture->GetImageViewObject(), texture->GetSamplerObject(), resource.set, resource.binding );
            }
        }
        return m_pRenderer->CreateMaterial( id, GetShader( createInfo.shaderId ), createInfo );
    }
    FawnVision::CMesh* CResourceManager::LoadModel( std::string_view assetPath )
    {
        std::ifstream file( assetPath.data(), std::ios::binary | std::ios::in );
        if ( !file.is_open() )
        {
            return nullptr;
        }

        CUuid id {};
        uint8_t type {};

        Serialization::Read( file, id );
        Serialization::Read( file, type );
        if ( static_cast<file_type>( type ) != file_type::model )
        {
            file.close();
            return nullptr;
        }
        FawnVision::SMeshCreateInfo createInfo;
        Serialization::Read( file, createInfo );

        file.close();
        return m_pRenderer->CreateMesh( id, createInfo );
    }

    const char* CResourceManager::IdToFilePath( const CUuid& id )
    {
        return std::ranges::find_if( m_files,
                                     [ id ]( const SFile& f ) {
                                         return f.uuid == id;
                                     } )
            ->path.c_str();
    }
    CUuid CResourceManager::FilePathToUuid( std::string path )
    {
        std::ranges::replace( path, '\\', '/' );

        auto it = std::ranges::find_if( m_files, [ path ]( const SFile& f ) {
            return f.path == path;
        } );
        if ( it == m_files.cend() )
        {
            return {};
        }
        return it->uuid;
    }
    void CResourceManager::FindAllFiles() noexcept
    {
        auto it = m_files.begin();
        while ( it != m_files.end() )
        {
            if ( !std::filesystem::exists( it->path ) )
            {
                //Remove
                it = m_files.erase( it );
            }
            else
            {
                ++it;
            }
        }

        for ( auto file = std::filesystem::recursive_directory_iterator( "..\\Data" ); file != std::filesystem::recursive_directory_iterator(); ++file )
        {
            if ( file->path().extension() != ".basset" )
            {
                continue;
            }
            auto currentFileLastWriteTime = std::filesystem::last_write_time( *file );

            // File creation
            auto filetIt = std::ranges::find_if( m_files, [ &file ]( const SFile& filePair ) -> bool {
                return filePair.path == file->path();
            } );
            if ( filetIt == m_files.end() )
            {
                //CreateNew
                m_files.push_back( CreateFileFromPath( relative( file->path() ) ) );
                m_files.back().lastWrittenTime = currentFileLastWriteTime;
                m_files.back().depth           = file.depth() + 1;
            }
            else
            {
                if ( filetIt->lastWrittenTime != currentFileLastWriteTime )
                {
                    //Modify
                    ( *filetIt )             = CreateFileFromPath( relative( file->path() ) );
                    filetIt->lastWrittenTime = currentFileLastWriteTime;
                    filetIt->depth           = file.depth() + 1;
                }
            }
        }
    }
    SFile CResourceManager::CreateFileFromPath( const std::filesystem::path& path )
    {
        std::ifstream fileStream( path, std::ios::in | std::ios::binary );
        if ( !fileStream.is_open() )
        {
            return {};
        }

        SFile file {};
        uint8_t value;
        Serialization::Read( fileStream, file.uuid );
        Serialization::Read( fileStream, value );
        fileStream.close();

        file.type     = static_cast<file_type>( value );
        file.path     = path.string();
        file.fileName = path.filename().string();
        std::ranges::replace( file.path, '\\', '/' );
        return file;
    }
}// namespace BalbinoControl