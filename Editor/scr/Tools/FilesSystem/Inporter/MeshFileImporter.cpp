//
// Created by joran on 13/06/2022.
//
#include "MeshFileImporter.h"
#include "CalcTangents.h"

#include <fstream>
#include <format>
#include <imgui.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <MeshMetadata.h>
#include <Vertex.h>
#include <FileParcer.h>
#include "../../../EditorGUI/EditorGui.h"

BalEditor::CMeshFileImporter::CMeshFileImporter()
{}

BalEditor::CMeshFileImporter::~CMeshFileImporter()
{

}

bool BalEditor::CMeshFileImporter::LoadFromPath( const std::filesystem::path& path )
{
    std::filesystem::path p{ std::string( "../Data/" ) + path.filename().string() };
    p.replace_extension( ".basset" );
    std::ofstream file( p.string().c_str(), std::ios::out | std::ios::binary );
    if ( !file.is_open())
    {
        return false;
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( path.string(), static_cast< unsigned >( aiProcess_Triangulate |
                                                                                      aiProcess_RemoveRedundantMaterials |
                                                                                      aiProcess_FlipUVs |
                                                                                      aiProcess_GenBoundingBoxes |
                                                                                      aiProcess_ValidateDataStructure ));

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
    unsigned int total{ scene->mNumMeshes };
    if ( m_preset )
    {
        total += scene->mNumCameras;
        total += scene->mNumLights;
        total += scene->mNumMeshes;
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
    CUuid meshID{};
    BinaryReadWrite::Write( file, ( uint64_t ) meshID );
    BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Model ));
    BinaryReadWrite::Write( file, indices.size());
    BinaryReadWrite::Write( file, vertices.size());
    BinaryReadWrite::Write( file, metadata.size());
    BinaryReadWrite::Write( file, indices.data(), indices.size() * sizeof( uint32_t ));
    BinaryReadWrite::Write( file, vertices.data(), vertices.size() * sizeof( BalVulkan::SVertex ));
    BinaryReadWrite::Write( file, metadata.data(), metadata.size() * sizeof( Balbino::SMeshMetadata ));
    file.close();


    if ( m_preset )
    {
        std::filesystem::path presetPath{ std::string( "../Data/" ) + path.filename().string() };
        presetPath.replace_filename( path.filename().string() + "_Preset" );
        presetPath.replace_extension( ".basset" );
        std::ofstream presetFile( presetPath, std::ios::out | std::ios::binary );

        if ( !file.is_open())
        {
            return false;
        }

        BinaryReadWrite::Write( presetFile, CUuid());
        BinaryReadWrite::Write( presetFile, static_cast< uint8_t >( EFileTypes::Preset ));
        int loadCount{};
        if ( scene->HasCameras())
        {
            uint32_t cameraCount{ scene->mNumCameras };
            for ( uint32_t i{}; i < cameraCount; ++i )
            {
                auto camera{ scene->mCameras[ i ] };
                BinaryReadWrite::Write( presetFile, static_cast< uint8_t >( PresetPropertieTypes::Camera ));
                BinaryReadWrite::Write( presetFile, camera->mAspect );
                BinaryReadWrite::Write( presetFile, camera->mClipPlaneFar );
                BinaryReadWrite::Write( presetFile, camera->mClipPlaneNear );
                BinaryReadWrite::Write( presetFile, camera->mLookAt.x );
                BinaryReadWrite::Write( presetFile, camera->mLookAt.y );
                BinaryReadWrite::Write( presetFile, camera->mLookAt.z );
                BinaryReadWrite::Write( presetFile, camera->mHorizontalFOV );
                BinaryReadWrite::Write( presetFile, camera->mOrthographicWidth );
                BinaryReadWrite::Write( presetFile, camera->mPosition.x );
                BinaryReadWrite::Write( presetFile, camera->mPosition.y );
                BinaryReadWrite::Write( presetFile, camera->mPosition.z );
                BinaryReadWrite::Write( presetFile, camera->mUp.x );
                BinaryReadWrite::Write( presetFile, camera->mUp.y );
                BinaryReadWrite::Write( presetFile, camera->mUp.z );
                ++loadCount;
            }
        }
        if ( scene->HasLights())
        {
            uint32_t lightCount{ scene->mNumLights };
            for ( uint32_t i{}; i < lightCount; ++i )
            {
                auto light{ scene->mLights[ i ] };
                BinaryReadWrite::Write( presetFile, static_cast< uint8_t >( PresetPropertieTypes::Light ));
                BinaryReadWrite::Write( presetFile, static_cast< uint8_t >(light->mType));
                ++loadCount;
            }
        }
        if ( scene->HasMeshes())
        {

        }
    }
    return true;
}

bool BalEditor::CMeshFileImporter::DrawImportSettings( const std::filesystem::path& path )
{
    if ( ImGui::BeginPopupModal( "BrokenComboBoxes", nullptr, 0 ))
    {
        ImGui::Text( "%s", std::format( "Importing from {}", path.string()).c_str());
        BalEditor::EditorGUI::DrawToggle( "Import as preset", m_preset );
        if ( BalEditor::EditorGUI::DrawButton( "Import" ))
        {
            if ( !LoadFromPath( path ))
                std::cerr << "Failed to import file: " << path.string() << std::endl;
            return true;
        }
        ImGui::SameLine();
        if ( BalEditor::EditorGUI::DrawButton( "Cancel" ))
        {
            return true;
        }
        ImGui::EndPopup();
    }
    return false;
}

float BalEditor::CMeshFileImporter::GetImportPercentage()
{
    return m_importPercentage;
}
