//
// Created by joran on 13/06/2022.
//
#include "MeshFileImporter.h"

#include <format>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "../../../Renderer/EditorGui.h"
#include "../Exporter.h"

#include "CalcTangents.h"

bool FawnForge::CMeshFileImporter::DrawImportSettings() noexcept
{
    if ( Gui::StartPopup( "Import Mesh" ) )
    {
        Gui::Text( std::format( "Importing from {}", m_path.string() ) );
        Gui::Toggle( "Import Meshes", m_importMeshes, 200 );
        if ( m_importMeshes )
        {
            Gui::Toggle( "Apply transformation", m_applyTransforms, 200 );
            Gui::Toggle( "Try to merge meshes", m_mergeMeshes, 200 );
            Gui::Toggle( "Try to create materials", m_materials, 200 );
            Gui::Toggle( "Try to import related textures", m_textures, 200 );
            Gui::Separator();
        }
        Gui::Toggle( "Import Lights", m_importLights, 200 );
        Gui::Toggle( "Import Cameras", m_importCameras, 200 );
        if ( Gui::Button( "Import" ) )
        {
            m_isVisible = false;
            if ( !LoadMesh() )
            {
                //todo: task to thread pool
                std::cerr << "Failed to import file: " << m_path.string() << std::endl;
                Gui::EndPopup();
                return false;
            }
            Gui::EndPopup();
            return true;
        }
        if ( Gui::Button( "Cancel" ) )
        {
            Gui::EndPopup();
            return true;
        }
        Gui::EndPopup();
    }
    return false;
}

float FawnForge::CMeshFileImporter::GetImportPercentage() const
{
    return m_importPercentage;
}

void FawnForge::CMeshFileImporter::SetVisible( const std::filesystem::path& path, const std::string_view destinationDirection )
{
    m_isVisible            = true;
    m_path                 = path;
    m_destinationDirection = destinationDirection;
}

bool FawnForge::CMeshFileImporter::IsVisible() const
{
    return m_isVisible;
}

bool FawnForge::CMeshFileImporter::LoadMesh() noexcept
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( m_path.string(),
                                              aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_RemoveRedundantMaterials | aiProcess_SplitLargeMeshes | aiProcess_Triangulate | aiProcess_GenUVCoords
                                                  | aiProcess_SortByPType | aiProcess_FindDegenerates | aiProcess_FindInvalidData );

    // If the import failed, constexpr report it
    if ( !scene )
    {
        std::cerr << importer.GetErrorString();
        return false;
    }
    if ( !m_importMeshes )
    {
        return false;
    }
    if ( !scene->HasMeshes() )
    {
        std::cerr << "No meshes were found\n";
        return false;
    }
    m_meshes.resize( scene->mNumMeshes );
    ProcessMeshRecursively( scene->mRootNode, scene );
    if ( m_mergeMeshes )
    {
        std::ranges::sort( m_meshes, []( const SMesh& p1, const SMesh& p2 ) {
            return p1.meshData.materialIndex < p2.meshData.materialIndex;
        } );
        FawnVision::SMeshCreateInfo createInfo;
        for ( const auto& mesh : m_meshes )
        {
            createInfo.vertices.insert( createInfo.vertices.end(), mesh.vertices.begin(), mesh.vertices.end() );
            createInfo.indices.insert( createInfo.indices.end(), mesh.indices.begin(), mesh.indices.end() );
            createInfo.metadata.push_back( mesh.meshData );
        }
        FawnForge::Exporter::ExportMesh( m_path.filename().string(), m_destinationDirection, createInfo );
    }
    return true;
}
void FawnForge::CMeshFileImporter::ProcessMeshRecursively( const aiNode* pMesh, const aiScene* pScene, unsigned int index )
{
    const unsigned int meshCount { pMesh->mNumMeshes };
    for ( unsigned int i = 0; i < meshCount; i++ )
    {
        const aiMesh* mesh                           = pScene->mMeshes[ pMesh->mMeshes[ i ] ];
        m_meshes[ index + i ].meshData.materialIndex = mesh->mMaterialIndex;
        ProcessMesh( mesh, m_meshes[ index + i ], pMesh->mTransformation );
        STempMesh tempMesh { m_meshes[ index + i ].vertices, m_meshes[ index + i ].indices };
        CCalcTangents calcTangents {};
        calcTangents.Calc( &tempMesh );
        m_meshes[ index + i ].vertices = tempMesh.vertices;
        m_meshes[ index + i ].indices  = tempMesh.indices;
    }
    for ( unsigned int i = 0; i < pMesh->mNumChildren; i++ )
    {
        ProcessMeshRecursively( pMesh->mChildren[ i ], pScene, index + meshCount );
    }
}
void FawnForge::CMeshFileImporter::ProcessMesh( const aiMesh* pMesh, SMesh& mesh, const aiMatrix4x4& transform ) const
{
    mesh.meshData.indexCount = pMesh->mNumFaces * 3;
    mesh.meshData.firstIndex = static_cast<uint32_t>( mesh.indices.size() );
    const uint32_t prevSize { static_cast<uint32_t>( mesh.vertices.size() ) };
    mesh.vertices.resize( prevSize + pMesh->mNumVertices, FawnVision::SVertex {} );
    mesh.indices.resize( mesh.meshData.firstIndex + mesh.meshData.indexCount, 0 );

    const bool hasColor { pMesh->HasVertexColors( 0 ) && pMesh->mColors[ 0 ] != nullptr };
    const bool hasUv { pMesh->HasTextureCoords( 0 ) && pMesh->mTextureCoords[ 0 ] != nullptr };

    for ( uint32_t i {}; i < pMesh->mNumVertices; ++i )
    {
        const uint32_t index { prevSize + i };
        aiVector3D position { pMesh->mVertices[ i ] };
        aiVector3D normals { pMesh->mNormals[ i ] };
        aiVector3D tangents { pMesh->mTangents[ i ] };
        if ( m_applyTransforms )
        {
            position *= transform;
            normals *= aiMatrix3x3t( transform );
            tangents *= aiMatrix3x3t( transform );
        }

        if ( hasColor )
        {
            mesh.vertices[ index ].color.r = pMesh->mColors[ 0 ][ i ].r;
            mesh.vertices[ index ].color.g = pMesh->mColors[ 0 ][ i ].g;
            mesh.vertices[ index ].color.b = pMesh->mColors[ 0 ][ i ].b;
            mesh.vertices[ index ].color.a = pMesh->mColors[ 0 ][ i ].a;
        }
        if ( hasUv )
        {
            mesh.vertices[ index ].uv.x = pMesh->mTextureCoords[ 0 ][ i ].x;
            mesh.vertices[ index ].uv.y = pMesh->mTextureCoords[ 0 ][ i ].y;
        }

        mesh.vertices[ index ].position.x = position.x;
        mesh.vertices[ index ].position.y = position.y;
        mesh.vertices[ index ].position.z = position.z;

        mesh.vertices[ index ].normal.x = normals.x;
        mesh.vertices[ index ].normal.y = normals.y;
        mesh.vertices[ index ].normal.z = normals.z;

        mesh.vertices[ index ].tangent.x = tangents.x;
        mesh.vertices[ index ].tangent.y = tangents.y;
        mesh.vertices[ index ].tangent.z = tangents.z;
        mesh.vertices[ index ].tangent.w = 1;
    }
    for ( uint32_t i {}; i < pMesh->mNumFaces; ++i )
    {
        mesh.indices[ mesh.meshData.firstIndex + i * 3 + 0 ] = ( prevSize + pMesh->mFaces[ i ].mIndices[ 0 ] );
        mesh.indices[ mesh.meshData.firstIndex + i * 3 + 1 ] = ( prevSize + pMesh->mFaces[ i ].mIndices[ 1 ] );
        mesh.indices[ mesh.meshData.firstIndex + i * 3 + 2 ] = ( prevSize + pMesh->mFaces[ i ].mIndices[ 2 ] );
    }

    mesh.meshData.boundingBox.min.x = pMesh->mAABB.mMin.x;
    mesh.meshData.boundingBox.max.x = pMesh->mAABB.mMax.x;
    mesh.meshData.boundingBox.min.y = pMesh->mAABB.mMin.y;
    mesh.meshData.boundingBox.max.y = pMesh->mAABB.mMax.y;
    mesh.meshData.boundingBox.min.z = pMesh->mAABB.mMin.z;
    mesh.meshData.boundingBox.max.z = pMesh->mAABB.mMax.z;

    mesh.meshData.boundingSphere.center = ( mesh.meshData.boundingBox.min + mesh.meshData.boundingBox.max ) * 0.5f;
    mesh.meshData.boundingSphere.radius = BambiMath::Distance( mesh.meshData.boundingBox.min, mesh.meshData.boundingBox.max ) * 0.5f;
}
