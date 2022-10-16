//
// Created by joran on 20/08/2022.
//

#include "Exporter.h"
#include <filesystem>
#include <fstream>
#include <UUID.h>
#include <FileParcer.h>
#include <MeshMetadata.h>

bool BalEditor::Exporter::ExportShader( const std::string& assetName, const std::string& assetPath, uint16_t type, const std::vector<std::vector<uint32_t>>& compiledShaders, CUuid id )
{
    std::filesystem::path path{ assetPath + assetName + ".basset" };
    std::ofstream         file{ path, std::ios::out | std::ios::binary };
    
    if ( !file.is_open())
    {
        return false;
    }
    
    BinaryReadWrite::Write( file, (uint64_t) id );
    BinaryReadWrite::Write( file, (uint8_t) EFileTypes::Shader );
    BinaryReadWrite::Write( file, type );
    for ( uint8_t i{}; i < (uint8_t) compiledShaders.size(); ++i )
    {
        BinaryReadWrite::Write( file, (uint64_t) compiledShaders[i].size());
    }
    for ( uint8_t i{}; i < (uint8_t) compiledShaders.size(); ++i )
    {
        BinaryReadWrite::Write( file, compiledShaders[i].data(),
                                (uint64_t) compiledShaders[i].size() * sizeof( uint32_t ));
    }
    file.close();
    return true;
}

bool BalEditor::Exporter::ExportMesh( const std::string& assetName, const std::string& assetPath, const std::vector<uint32_t>& indices, const std::vector<BalVulkan::SVertex>& vertices, const std::vector<Balbino::SMeshMetadata>& pMetadata, CUuid id )
{
    std::filesystem::path path{ assetPath + assetName };
    path.replace_extension( ".basset" );
    std::ofstream file{ path, std::ios::out | std::ios::binary };
    
    if ( !file.is_open())
    {
        return false;
    }
    
    BinaryReadWrite::Write( file, (uint64_t) id );
    BinaryReadWrite::Write( file, (uint8_t) EFileTypes::Model );
    BinaryReadWrite::Write( file, (uint64_t) indices.size());
    BinaryReadWrite::Write( file, (uint64_t) vertices.size());
    BinaryReadWrite::Write( file, (uint64_t) pMetadata.size());
    BinaryReadWrite::Write( file, indices.data(), sizeof( uint32_t ) * indices.size());
    BinaryReadWrite::Write( file, vertices.data(), sizeof( BalVulkan::SVertex ) * vertices.size());
    for ( const auto& metadata : pMetadata )
    {
        BinaryReadWrite::Write( file, metadata );
    }
    
    file.close();
    return true;
}

bool BalEditor::Exporter::ExportMaterial( const std::string& assetName, const std::string& assetPath, CUuid shaderID, const std::unordered_map<std::string, BalVulkan::SShaderResource>& resources, CUuid id )
{
    std::vector<BalVulkan::SShaderResource> shaderResource;
    shaderResource.reserve( resources.size());
    /*** Copy all value fields from map to a vector using transform() & Lambda function ***/
    std::ranges::transform( resources, std::back_inserter( shaderResource ),
                            []( const std::pair<std::string, BalVulkan::SShaderResource>& pair )
                            {
                                return pair.second;
                            } );
    
    std::filesystem::path path{ assetPath + assetName + ".basset" };
    std::ofstream         file{ path, std::ios::out | std::ios::binary };
    
    if ( !file.is_open())
    {
        return false;
    }
    
    BinaryReadWrite::Write( file, (uint64_t) id );
    BinaryReadWrite::Write( file, (uint8_t) EFileTypes::Material );
    BinaryReadWrite::Write( file, (uint64_t) shaderID );
    BinaryReadWrite::Write( file, shaderResource );
    
    file.close();
    return true;
}

bool BalEditor::Exporter::ExportImage( const std::string& assetName, const std::string& assetPath, uint8_t imageType, uint32_t imageFormat, uint8_t mips, uint8_t layers, uint32_t width, uint32_t height, uint32_t depth, uint8_t pitch, void* pData, int anisotropy, int sampleLevel, int mipmapMode, int filterMode, int wrapModeU, int wrapModeV, int wrapModeW, CUuid id )
{
    std::filesystem::path path{ assetPath + assetName };
    path.replace_extension( ".basset" );
    std::ofstream file{ path, std::ios::out | std::ios::binary };
    
    if ( !file.is_open())
    {
        return false;
    }
    
    BinaryReadWrite::Write( file, (uint64_t) id );
    BinaryReadWrite::Write( file, (uint8_t) EFileTypes::Image );
    BinaryReadWrite::Write( file, imageType );
    BinaryReadWrite::Write( file, imageFormat );
    BinaryReadWrite::Write( file, mips );
    BinaryReadWrite::Write( file, layers );
    BinaryReadWrite::Write( file, anisotropy );
    BinaryReadWrite::Write( file, sampleLevel );
    BinaryReadWrite::Write( file, mipmapMode );
    BinaryReadWrite::Write( file, filterMode );
    BinaryReadWrite::Write( file, wrapModeU );
    BinaryReadWrite::Write( file, wrapModeV );
    BinaryReadWrite::Write( file, wrapModeW );
    BinaryReadWrite::Write( file, width );
    BinaryReadWrite::Write( file, height );
    BinaryReadWrite::Write( file, depth );
    BinaryReadWrite::Write( file, pitch );
    BinaryReadWrite::Write( file, pData, width * height * depth * pitch );
    
    file.close();
    return true;
}
