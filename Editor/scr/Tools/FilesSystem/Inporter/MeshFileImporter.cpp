//
// Created by joran on 13/06/2022.
//
#include "MeshFileImporter.h"
#include "CalcTangents.h"

#include <fstream>
#include <format>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <MeshMetadata.h>
#include "../../../EditorGUI/EditorGui.h"
#include "../Exporter.h"

BalEditor::CMeshFileImporter::CMeshFileImporter()
        : m_importPercentage{},
          m_preset{},
          m_meshes{ true },
          m_materials{},
          m_textures{},
          m_lights{},
          m_cameras{},
          m_path{},
          m_destinationDirection{},
          m_isVisible{}
{
}

bool BalEditor::CMeshFileImporter::LoadMesh() const
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( m_path.string(),
                                              static_cast< unsigned >( aiProcess_Triangulate | aiProcess_RemoveRedundantMaterials | aiProcess_FlipUVs | aiProcess_GenBoundingBoxes | aiProcess_ValidateDataStructure ));
    
    // If the import failed, report it
    if ( !scene )
    {
        std::cerr << importer.GetErrorString();
        return false;
    }
    if ( !m_preset )
    {
        if ( m_meshes )
        {
            if ( !scene->HasMeshes())
            {
                std::cerr << "No meshes were found\n";
                return false;
            }
            
            uint32_t meshCount{ scene->mNumMeshes };
            aiMesh** meshes{ scene->mMeshes };
            std::vector<BalVulkan::SVertex>     vertices;
            std::vector<uint32_t>               indices;
            std::vector<Balbino::SMeshMetadata> metadata( meshCount );
            
            for ( uint32_t i{}; i < meshCount; ++i )
            {
                aiMesh* pMesh{ meshes[i] };
                
                metadata[i].indexCount = pMesh->mNumFaces * 3;
                metadata[i].firstIndex = static_cast<uint32_t>( indices.size());
                const uint32_t prevSize{ static_cast<uint32_t>( vertices.size()) };
                vertices.resize( prevSize + pMesh->mNumVertices, BalVulkan::SVertex{} );
                indices.resize( metadata[i].firstIndex + metadata[i].indexCount, 0 );
                
                const bool hasColor{ pMesh->HasVertexColors( 0 ) && pMesh->mColors[0] != nullptr };
                const bool hasUV{ pMesh->HasTextureCoords( 0 ) && pMesh->mTextureCoords[0] != nullptr };
                const bool hasNormals{ pMesh->HasNormals() };
                const bool hasTangents{ pMesh->HasTangentsAndBitangents() };
                
                for ( uint32_t j{}; j < pMesh->mNumVertices; ++j )
                {
                    const uint32_t index{ prevSize + j };
                    vertices[index].position.x = pMesh->mVertices[j].x;
                    vertices[index].position.y = pMesh->mVertices[j].y;
                    vertices[index].position.z = pMesh->mVertices[j].z;
                    
                    if ( hasColor )
                    {
                        vertices[index].color.r = pMesh->mColors[0][j].r;
                        vertices[index].color.g = pMesh->mColors[0][j].g;
                        vertices[index].color.b = pMesh->mColors[0][j].b;
                        vertices[index].color.a = pMesh->mColors[0][j].a;
                    }
                    if ( hasUV )
                    {
                        vertices[index].uv.x = pMesh->mTextureCoords[0][j].x;
                        vertices[index].uv.y = pMesh->mTextureCoords[0][j].y;
                    }
                    if ( hasNormals )
                    {
                        vertices[index].normal.x = pMesh->mNormals[j].x;
                        vertices[index].normal.y = pMesh->mNormals[j].y;
                        vertices[index].normal.z = pMesh->mNormals[j].z;
                    }
                    if ( hasTangents )
                    {
                        vertices[index].tangent.x = pMesh->mTangents[j].x;
                        vertices[index].tangent.y = pMesh->mTangents[j].y;
                        vertices[index].tangent.z = pMesh->mTangents[j].z;
                        vertices[index].tangent.w = 1;
                    }
                }
                for ( uint32_t j{}; j < pMesh->mNumFaces; ++j )
                {
                    indices[metadata[i].firstIndex + j * 3 + 0] = ( prevSize + pMesh->mFaces[j].mIndices[0] );
                    indices[metadata[i].firstIndex + j * 3 + 1] = ( prevSize + pMesh->mFaces[j].mIndices[1] );
                    indices[metadata[i].firstIndex + j * 3 + 2] = ( prevSize + pMesh->mFaces[j].mIndices[2] );
                }
                
                metadata[i].boundingBox.min.x = pMesh->mAABB.mMin.x;
                metadata[i].boundingBox.max.x = pMesh->mAABB.mMax.x;
                metadata[i].boundingBox.min.y = pMesh->mAABB.mMin.y;
                metadata[i].boundingBox.max.y = pMesh->mAABB.mMax.y;
                metadata[i].boundingBox.min.z = pMesh->mAABB.mMin.z;
                metadata[i].boundingBox.max.z = pMesh->mAABB.mMax.z;
                
                metadata[i].boundingSphere.center = ( metadata[i].boundingBox.min + metadata[i].boundingBox.max ) * 0.5f;
                metadata[i].boundingSphere.radius = glm::distance( metadata[i].boundingBox.min,
                                                                   metadata[i].boundingBox.max ) * 0.5f;
                
            }
            BalEditor::Exporter::ExportMesh( m_path.filename().string(), m_destinationDirection + "\\", indices,
                                             vertices, metadata );
        }
    }
    else
    {
        std::clog << "Warning cant save as preset yet!\n";  //todo think of and implement presets
    }
    
    return true;
}

bool BalEditor::CMeshFileImporter::DrawImportSettings()
{
    if ( BalEditor::EditorGUI::StartPopup( "Import Mesh" ))
    {
        BalEditor::EditorGUI::DrawText( std::format( "Importing from {}", m_path.string()).c_str());
        BalEditor::EditorGUI::DrawToggle( "Import Meshes", m_meshes, 200 );
        BalEditor::EditorGUI::DrawToggle( "Import as preset", m_preset, 200 );
        if ( m_meshes )
        {
            BalEditor::EditorGUI::DrawToggle( "Try to create materials", m_materials, 200 );
            BalEditor::EditorGUI::DrawToggle( "Try to import related textures", m_textures, 200 );
        }
        if ( m_preset )
        {
            BalEditor::EditorGUI::DrawToggle( "Import Lights", m_lights, 200 );
            BalEditor::EditorGUI::DrawToggle( "Import Cameras", m_cameras, 200 );
        }
        if ( BalEditor::EditorGUI::DrawButton( "Import" ))
        {
            m_isVisible = false;
            if ( !LoadMesh())
            { //TODO add task to thread pool
                std::cerr << "Failed to import file: " << m_path.string() << std::endl;
                BalEditor::EditorGUI::EndPopup();
                return false;
            }
            BalEditor::EditorGUI::EndPopup();
            return true;
        }
        if ( BalEditor::EditorGUI::DrawButton( "Cancel" ))
        {
            BalEditor::EditorGUI::EndPopup();
            return true;
        }
        BalEditor::EditorGUI::EndPopup();
    }
    return false;
}

float BalEditor::CMeshFileImporter::GetImportPercentage()
{
    return m_importPercentage;
}

void BalEditor::CMeshFileImporter::SetVisible( const std::filesystem::path& path, std::string destinationDirection )
{
    m_isVisible            = true;
    m_path                 = path;
    m_destinationDirection = destinationDirection;
}

bool BalEditor::CMeshFileImporter::IsVisible() const
{
    return m_isVisible;
}
