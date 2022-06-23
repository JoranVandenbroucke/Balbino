#include "Importer.h"

#include "CalcTangents.h"
#include "hdrloader.h"

#include <Common.h>
#include <FileParcer.h>
#include <MeshMetadata.h>
#include <UUID.h>
#include <Vertex.h>

#include <fstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#pragma warning (push)
#pragma warning (disable : 4100)
#pragma warning (disable : 4201)
#pragma warning (disable : 4244)
#pragma warning (disable : 4458)
#pragma warning (disable : 5054)

#include <gli/gli.hpp>

bool BalEditor::ImportImage( const std::filesystem::path& path )
{
    std::filesystem::path p{ std::string( "../Data/" ) + path.filename().string() };
    p.replace_extension( ".basset" );
    std::ofstream file( p.string().c_str(), std::ios::out | std::ios::binary );
    if ( !file.is_open())
    {
        return false;
    }
    
    std::string extenstion = path.extension().string();
    std::ranges::for_each( extenstion, []( char& c )
    {
        c = static_cast< char >( toupper( c ));
    } );
    
    if ( extenstion == ".KTX" || extenstion == ".DDS" )
    {
        gli::texture_cube texture{ gli::load( path.string().c_str()) };
        if ( texture.empty())
        {
            return false;
        }
        
        BinaryReadWrite::Write( file, ( uint64_t ) CUuid());
        BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Image ));
        BinaryReadWrite::Write( file, texture.extent().x );
        BinaryReadWrite::Write( file, texture.extent().y );
        BinaryReadWrite::Write( file, texture.levels());
        BinaryReadWrite::Write( file, texture.faces());
        BinaryReadWrite::Write( file, texture.layers());
        BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EImageLayout::Undefined ));
        BinaryReadWrite::Write( file, texture.target());
        BinaryReadWrite::Write( file, texture.format());
        BinaryReadWrite::Write( file, texture.size());
        BinaryReadWrite::Write( file, static_cast< uint8_t* >( texture.data()), texture.size());
    }
    else if ( extenstion == ".hdr" )
    {
        HDRLoaderResult hdrLoaderResult{};
        if ( !HDRLoader::load( path.string().c_str(), hdrLoaderResult ))
        {
            return false;
        }
        
        BinaryReadWrite::Write( file, ( uint64_t ) CUuid());
        BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Image ));
        BinaryReadWrite::Write( file, hdrLoaderResult.width );
        BinaryReadWrite::Write( file, hdrLoaderResult.height );
        BinaryReadWrite::Write( file, 1 );
        BinaryReadWrite::Write( file, 6 );
        BinaryReadWrite::Write( file, 1 );
        BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EImageLayout::Undefined ));
        BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EImageViewType::Cube ));
        BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EFormat::R16G16B16Sfloat));
        BinaryReadWrite::Write( file, hdrLoaderResult.width * hdrLoaderResult.height * sizeof( float ) * 3 );
        BinaryReadWrite::Write( file, (uint8_t*)( hdrLoaderResult.cols ), hdrLoaderResult.width * hdrLoaderResult.height * sizeof( float ) * 3 );
    }
    else
    {
        SDL_Surface* pSurface{ IMG_Load( path.string().c_str()) };
        if ( pSurface == nullptr )
        {
            std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
            return false;
        }
        
        if ( pSurface->pitch / pSurface->w != 4 )
        {
            pSurface = SDL_ConvertSurfaceFormat( pSurface, SDL_PIXELFORMAT_RGBA8888, 0 );
        }
        
        BinaryReadWrite::Write( file, ( uint64_t ) CUuid());
        BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Image ));
        BinaryReadWrite::Write( file, pSurface->w );
        BinaryReadWrite::Write( file, pSurface->h );
        BinaryReadWrite::Write( file, 1 );
        BinaryReadWrite::Write( file, 1 );
        BinaryReadWrite::Write( file, 1 );
        BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EImageLayout::Undefined ));
        BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EImageViewType::View2D ));
        BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EFormat::R8G8B8A8Srgb ));
        BinaryReadWrite::Write( file, static_cast< uint64_t >( pSurface->h ) * pSurface->pitch );
        BinaryReadWrite::Write( file, static_cast< uint8_t* >( pSurface->pixels ), static_cast< uint64_t >( pSurface->h ) * pSurface->pitch );
        SDL_FreeSurface( pSurface );
    }
    file.close();
    return true;
}

bool BalEditor::ImportFont( const std::filesystem::path& path )
{
    TTF_Font* pFont = TTF_OpenFont( path.string().c_str(), 12 );
    if ( pFont == nullptr )
    {
        return false;
    }
    
    std::filesystem::path p{ std::string( "../Data/" ) + path.filename().string() };
    p.replace_extension( ".basset" );
    std::ofstream file( p.filename(), std::ios::out | std::ios::binary );
    
    if ( !file.is_open())
    {
        return false;
    }
    TTF_SetFontKerning( pFont, 0 );
    BinaryReadWrite::Write( file, CUuid());
    BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Font ));
    BinaryReadWrite::Write( file, TTF_FontLineSkip( pFont ));
    BinaryReadWrite::Write( file, TTF_GetFontHinting( pFont ));
    BinaryReadWrite::Write( file, TTF_FontHeight( pFont ));
    BinaryReadWrite::Write( file, TTF_FontAscent( pFont ));
    BinaryReadWrite::Write( file, TTF_FontDescent( pFont ));
    BinaryReadWrite::Write( file, TTF_FontLineSkip( pFont ));
    
    for ( uint16_t i{}; i < 256u; ++i )
    {
        const char c[1]{ static_cast< char >( i ) };
        SDL_Surface* pSurface = TTF_RenderText_Solid( pFont, c, { 255, 255, 255, 255 } );
        if ( pSurface == nullptr )
        {
            return false;
        }
        BinaryReadWrite::Write( file, pSurface->w );
        BinaryReadWrite::Write( file, pSurface->h );
        BinaryReadWrite::Write( file, pSurface->h * pSurface->pitch );
        BinaryReadWrite::Write( file, static_cast< uint8_t* >( pSurface->pixels ), static_cast< uint64_t >( pSurface->h ) * pSurface->pitch );
        TTF_CloseFont( pFont );
    }
    file.close();
    return true;
}

bool BalEditor::ImportAudio( const std::filesystem::path& path )
{
    Mix_Music* pMusic = Mix_LoadMUS( path.string().c_str());
    if ( pMusic == nullptr )
    {
        return false;
    }
    Mix_FreeMusic( pMusic );
    return true;
}

bool BalEditor::ImportMesh( const std::filesystem::path& path )
{
    std::filesystem::path p{ std::string( "../Data/" ) + path.filename().string() };
    p.replace_extension( ".basset" );
    std::ofstream file( p.string().c_str(), std::ios::out | std::ios::binary );
    if ( !file.is_open())
    {
        return false;
    }
    
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( path.string(), static_cast< unsigned >( aiProcess_Triangulate | aiProcess_RemoveRedundantMaterials | aiProcess_FlipUVs | aiProcess_GenBoundingBoxes | aiProcess_ValidateDataStructure ));
    
    // If the import failed, report it
    if ( !scene )
    {
        std::cerr << importer.GetErrorString();
        file.close();
        return false;
    }
    // Now we can access the file's contents.
    if ( !scene->HasMeshes())
    {
        throw std::runtime_error( "No mesh inside file" );
    }
    
    const uint32_t nrOfMeshes{ scene->mNumMeshes };
    std::vector<BalVulkan::SVertex> vertices;
    std::vector<Balbino::SMeshMetadata> metadata( nrOfMeshes );
    std::vector<uint32_t> indices;
    for ( uint32_t i{}; i < nrOfMeshes; ++i )
    {
        const aiMesh* pMesh{ scene->mMeshes[ i ] };
        if ( !pMesh->HasTextureCoords( 0 ))
        {
            importer.ApplyPostProcessing( aiProcess_GenUVCoords );
        }
        if ( !pMesh->HasNormals())
        {
            importer.ApplyPostProcessing( aiProcess_GenNormals | aiProcess_FixInfacingNormals );
        }
        if ( !pMesh->HasTangentsAndBitangents())
        {
            importer.ApplyPostProcessing( aiProcess_CalcTangentSpace );
        }
        importer.ApplyPostProcessing( aiProcess_JoinIdenticalVertices );
        
        metadata[ i ].indexCount = pMesh->mNumFaces * 3;
        metadata[ i ].indexStart = static_cast<uint32_t>( indices.size());
        const uint32_t prevSize{ static_cast<uint32_t>( vertices.size()) };
        vertices.resize( prevSize + pMesh->mNumVertices, BalVulkan::SVertex{} );
        indices.resize( metadata[ i ].indexStart + metadata[ i ].indexCount, 0 );
        
        const bool hasColor{ pMesh->HasVertexColors( 0 ) && pMesh->mColors[ 0 ] != nullptr };
        const bool hasUV{ pMesh->HasTextureCoords( 0 ) && pMesh->mTextureCoords[ 0 ] != nullptr };
        const bool hasNormals{ pMesh->HasNormals() };
        const bool hasTangents{ pMesh->HasTangentsAndBitangents() };
        for ( uint32_t j{}; j < pMesh->mNumVertices; ++j )
        {
            const uint32_t index{ prevSize + j };
            vertices[ index ].position.x = pMesh->mVertices[ j ].x;
            vertices[ index ].position.y = pMesh->mVertices[ j ].y;
            vertices[ index ].position.z = pMesh->mVertices[ j ].z;
            
            if ( hasColor )
            {
                vertices[ index ].color.r = pMesh->mColors[ 0 ][ j ].r;
                vertices[ index ].color.g = pMesh->mColors[ 0 ][ j ].g;
                vertices[ index ].color.b = pMesh->mColors[ 0 ][ j ].b;
                vertices[ index ].color.a = pMesh->mColors[ 0 ][ j ].a;
            }
            if ( hasUV )
            {
                vertices[ index ].uv.x = pMesh->mTextureCoords[ 0 ][ j ].x;
                vertices[ index ].uv.y = pMesh->mTextureCoords[ 0 ][ j ].y;
            }
            if ( hasNormals )
            {
                vertices[ index ].normal.x = pMesh->mNormals[ j ].x;
                vertices[ index ].normal.y = pMesh->mNormals[ j ].y;
                vertices[ index ].normal.z = pMesh->mNormals[ j ].z;
            }
            if ( hasTangents )
            {
                vertices[ index ].tangent.x = pMesh->mTangents[ j ].x;
                vertices[ index ].tangent.y = pMesh->mTangents[ j ].y;
                vertices[ index ].tangent.z = pMesh->mTangents[ j ].z;
                vertices[ index ].tangent.w = 1;
            }
        }
        for ( uint32_t j{}; j < pMesh->mNumFaces; ++j )
        {
            indices[ metadata[ i ].indexStart + j * 3 + 0 ] = ( prevSize + pMesh->mFaces[ j ].mIndices[ 0 ] );
            indices[ metadata[ i ].indexStart + j * 3 + 1 ] = ( prevSize + pMesh->mFaces[ j ].mIndices[ 1 ] );
            indices[ metadata[ i ].indexStart + j * 3 + 2 ] = ( prevSize + pMesh->mFaces[ j ].mIndices[ 2 ] );
        }
        //m_importPercentage = i / float( total );
    }
    CCalcTangents calcTangents{};
    STempMesh tempMesh{ vertices, indices };
    calcTangents.Calc( &tempMesh );
    BinaryReadWrite::Write( file, CUuid());
    BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Model ));
    BinaryReadWrite::Write( file, indices.size());
    BinaryReadWrite::Write( file, vertices.size());
    BinaryReadWrite::Write( file, metadata.size());
    BinaryReadWrite::Write( file, indices.data(), indices.size() * sizeof( uint32_t ));
    BinaryReadWrite::Write( file, vertices.data(), vertices.size() * sizeof( BalVulkan::SVertex ));
    BinaryReadWrite::Write( file, metadata.data(), metadata.size() * sizeof( Balbino::SMeshMetadata ));
    
    file.close();
    return true;
}

#pragma warning (pop)
