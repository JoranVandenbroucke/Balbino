//
// Created by joran on 13/06/2022.
//
#include "MeshFileImporter.h"
#include "CalcTangents.h"

#include <format>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "../../../EditorGUI/EditorGui.h"
#include "../Exporter.h"

BalEditor::CMeshFileImporter::CMeshFileImporter()
        : m_importPercentage{}
          , m_isVisible{}
          , m_importMeshes{ true }
          , m_applyTransforms{}
          , m_mergeMeshes{}
          , m_materials{}
          , m_textures{}
          , m_importLights{}
          , m_importCameras{}
          , m_path{}
          , m_destinationDirection{}
{
}

bool BalEditor::CMeshFileImporter::DrawImportSettings()
{
    if ( GUI::StartPopup( "Import Mesh" ))
    {
        GUI::DrawText( std::format( "Importing from {}", m_path.string()).c_str());
        GUI::DrawToggle( "Import Meshes", m_importMeshes, 200 );
        if ( m_importMeshes )
        {
            GUI::DrawToggle( "Apply transformation", m_applyTransforms, 200 );
            GUI::DrawToggle( "Try to merge meshes", m_mergeMeshes, 200 );
            GUI::DrawToggle( "Try to create materials", m_materials, 200 );
            GUI::DrawToggle( "Try to import related textures", m_textures, 200 );
            GUI::Separator();
        }
        GUI::DrawToggle( "Import Lights", m_importLights, 200 );
        GUI::DrawToggle( "Import Cameras", m_importCameras, 200 );
        if ( GUI::DrawButton( "Import" ))
        {
            m_isVisible = false;
            if ( !LoadMesh())
            { //TODO add task to thread pool
                std::cerr << "Failed to import file: " << m_path.string() << std::endl;
                GUI::EndPopup();
                return false;
            }
            GUI::EndPopup();
            return true;
        }
        if ( GUI::DrawButton( "Cancel" ))
        {
            GUI::EndPopup();
            return true;
        }
        GUI::EndPopup();
    }
    return false;
}

float BalEditor::CMeshFileImporter::GetImportPercentage()
{
    return m_importPercentage;
}

void BalEditor::CMeshFileImporter::SetVisible( const std::filesystem::path& path, const std::string& destinationDirection )
{
    m_isVisible            = true;
    m_path                 = path;
    m_destinationDirection = destinationDirection;
}

bool BalEditor::CMeshFileImporter::IsVisible() const
{
    return m_isVisible;
}

bool BalEditor::CMeshFileImporter::LoadMesh()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( m_path.string(),
                                              aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_RemoveRedundantMaterials | aiProcess_SplitLargeMeshes | aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_SortByPType | aiProcess_FindDegenerates | aiProcess_FindInvalidData );
    
    // If the import failed, report it
    if ( !scene )
    {
        std::cerr << importer.GetErrorString();
        return false;
    }
    if ( m_importMeshes )
    {
        if ( !scene->HasMeshes())
        {
            std::cerr << "No meshes were found\n";
            return false;
        }
        m_meshes.resize( scene->mNumMeshes );
        ProcessMeshRecursively( scene->mRootNode, scene );
        if ( m_mergeMeshes )
        {
            std::sort( m_meshes.begin(), m_meshes.end(), []( const SMesh& p1, const SMesh& p2 )
            {
                return p1.meshData.marerialIndex < p2.meshData.marerialIndex;
            } );
            if ( m_mergeMeshes )
            {
                std::vector<BalVulkan::SVertex>     vertices;
                std::vector<uint32_t>               indices;
                std::vector<Balbino::SMeshMetadata> meshData;
                for ( const auto& mesh : m_meshes )
                {
                    vertices.insert( vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
                    indices.insert( indices.end(), mesh.indices.begin(), mesh.indices.end());
                    meshData.push_back( mesh.meshData );
                }
                BalEditor::Exporter::ExportMesh( m_path.filename().string(), m_destinationDirection + "\\", indices,
                                                 vertices, meshData );
            }
        }
    }
    return true;
}
void BalEditor::CMeshFileImporter::ProcessMeshRecursively( const aiNode* pMesh, const aiScene* pScene, int index )
{
    const unsigned int meshCount{ pMesh->mNumMeshes };
    for ( unsigned int i = 0; i < meshCount; i++ )
    {
        aiMesh* mesh = pScene->mMeshes[pMesh->mMeshes[i]];
        m_meshes[index + i].meshData.marerialIndex = mesh->mMaterialIndex;
        ProcessMesh( mesh, m_meshes[index + i], pMesh->mTransformation );
    }
    for ( unsigned int i = 0; i < pMesh->mNumChildren; i++ )
    {
        ProcessMeshRecursively( pMesh->mChildren[i], pScene, index + meshCount );
    }
}
void BalEditor::CMeshFileImporter::ProcessMesh( const aiMesh* pMesh, SMesh& mesh, const aiMatrix4x4& transform ) const
{
    mesh.meshData.indexCount = pMesh->mNumFaces * 3;
    mesh.meshData.firstIndex = static_cast<uint32_t>( mesh.indices.size());
    const uint32_t prevSize{ static_cast<uint32_t>( mesh.vertices.size()) };
    mesh.vertices.resize( prevSize + pMesh->mNumVertices, BalVulkan::SVertex{} );
    mesh.indices.resize( mesh.meshData.firstIndex + mesh.meshData.indexCount, 0 );
    
    const bool hasColor{ pMesh->HasVertexColors( 0 ) && pMesh->mColors[0] != nullptr };
    const bool hasUV{ pMesh->HasTextureCoords( 0 ) && pMesh->mTextureCoords[0] != nullptr };
    
    for ( uint32_t i{}; i < pMesh->mNumVertices; ++i )
    {
        const uint32_t index{ prevSize + i };
        aiVector3D     position{ pMesh->mVertices[i] };
        aiVector3D     normals{ pMesh->mNormals[i] };
        aiVector3D     tangents{ pMesh->mTangents[i] };
        if ( m_applyTransforms )
        {
            position *= transform;
            normals *= aiMatrix3x3t( transform );
            tangents *= aiMatrix3x3t( transform );
        }
        
        if ( hasColor )
        {
            mesh.vertices[index].color.r = pMesh->mColors[0][i].r;
            mesh.vertices[index].color.g = pMesh->mColors[0][i].g;
            mesh.vertices[index].color.b = pMesh->mColors[0][i].b;
            mesh.vertices[index].color.a = pMesh->mColors[0][i].a;
        }
        if ( hasUV )
        {
            mesh.vertices[index].uv.x = pMesh->mTextureCoords[0][i].x;
            mesh.vertices[index].uv.y = pMesh->mTextureCoords[0][i].y;
        }
        
        mesh.vertices[index].position.x = position.x;
        mesh.vertices[index].position.y = position.y;
        mesh.vertices[index].position.z = position.z;
        
        mesh.vertices[index].normal.x = normals.x;
        mesh.vertices[index].normal.y = normals.y;
        mesh.vertices[index].normal.z = normals.z;
        
        mesh.vertices[index].tangent.x = tangents.x;
        mesh.vertices[index].tangent.y = tangents.y;
        mesh.vertices[index].tangent.z = tangents.z;
        mesh.vertices[index].tangent.w = 1;
    }
    for ( uint32_t i{}; i < pMesh->mNumFaces; ++i )
    {
        mesh.indices[mesh.meshData.firstIndex + i * 3 + 0] = ( prevSize + pMesh->mFaces[i].mIndices[0] );
        mesh.indices[mesh.meshData.firstIndex + i * 3 + 1] = ( prevSize + pMesh->mFaces[i].mIndices[1] );
        mesh.indices[mesh.meshData.firstIndex + i * 3 + 2] = ( prevSize + pMesh->mFaces[i].mIndices[2] );
    }
    
    mesh.meshData.boundingBox.min.x = pMesh->mAABB.mMin.x;
    mesh.meshData.boundingBox.max.x = pMesh->mAABB.mMax.x;
    mesh.meshData.boundingBox.min.y = pMesh->mAABB.mMin.y;
    mesh.meshData.boundingBox.max.y = pMesh->mAABB.mMax.y;
    mesh.meshData.boundingBox.min.z = pMesh->mAABB.mMin.z;
    mesh.meshData.boundingBox.max.z = pMesh->mAABB.mMax.z;
    
    mesh.meshData.boundingSphere.center = ( mesh.meshData.boundingBox.min + mesh.meshData.boundingBox.max ) * 0.5f;
    mesh.meshData.boundingSphere.radius = glm::distance( mesh.meshData.boundingBox.min,
                                                         mesh.meshData.boundingBox.max ) * 0.5f;
}
