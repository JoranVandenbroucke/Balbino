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
          m_isVisible{}
{
}

bool BalEditor::CMeshFileImporter::LoadMesh() const
{
    std::filesystem::path p{ m_destinationDirection + m_path.filename().string() };
    p.replace_extension( ".basset" );
    std::ofstream file( p.string().c_str(), std::ios::out | std::ios::binary );
    if ( !file.is_open())
    {
        return false;
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( m_path.string(),
                                              static_cast< unsigned >( aiProcess_Triangulate | aiProcess_RemoveRedundantMaterials | aiProcess_FlipUVs | aiProcess_GenBoundingBoxes | aiProcess_ValidateDataStructure ));

    // If the import failed, report it
    if ( !scene )
    {
        std::cerr << importer.GetErrorString();
        file.close();
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
            std::vector<BalVulkan::SVertex> vertices;
            std::vector<uint32_t>           indices;
            Balbino::SMeshMetadata* pMeshData{ nullptr };
            for ( uint32_t i{}; i < meshCount; ++i )
            {
                aiMesh* pMesh{ meshes[i] };

                if ( pMeshData )
                {
                    Balbino::SMeshMetadata* pNewMeshData = new Balbino::SMeshMetadata{};
                    pMeshData->pNext        = pNewMeshData;
                    pNewMeshData->pPrevious = pMeshData;
                    pMeshData = pNewMeshData;

                }
                else
                {
                    pMeshData = new Balbino::SMeshMetadata{};
                }

                pMeshData->indexCount = pMesh->mNumFaces * 3;
                pMeshData->firstIndex = static_cast<uint32_t>( indices.size());
                const uint32_t prevSize{ static_cast<uint32_t>( vertices.size()) };
                vertices.resize( prevSize + pMesh->mNumVertices );
                indices.resize( pMeshData->firstIndex + pMeshData->indexCount );

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
                    indices[pMeshData->firstIndex + j * 3 + 0] = ( prevSize + pMesh->mFaces[j].mIndices[0] );
                    indices[pMeshData->firstIndex + j * 3 + 1] = ( prevSize + pMesh->mFaces[j].mIndices[1] );
                    indices[pMeshData->firstIndex + j * 3 + 2] = ( prevSize + pMesh->mFaces[j].mIndices[2] );
                }

                pMeshData->boundingBox.min.x = pMesh->mAABB.mMin.x;
                pMeshData->boundingBox.max.x = pMesh->mAABB.mMax.x;
                pMeshData->boundingBox.min.y = pMesh->mAABB.mMin.y;
                pMeshData->boundingBox.max.y = pMesh->mAABB.mMax.y;
                pMeshData->boundingBox.min.z = pMesh->mAABB.mMin.z;
                pMeshData->boundingBox.max.z = pMesh->mAABB.mMax.z;

                pMeshData->boundingSphere.center = ( pMeshData->boundingBox.min + pMeshData->boundingBox.max ) * 0.5f;
                pMeshData->boundingSphere.radius = glm::distance( pMeshData->boundingBox.min,
                                                                  pMeshData->boundingBox.max ) * 0.5f;

            }
            BalEditor::Exporter::ExportMesh( m_path.filename().string(), m_destinationDirection + "\\", indices,
                                             vertices,
                                             pMeshData );
            while ( pMeshData->pPrevious )
            {
                Balbino::SMeshMetadata* pTempMeshData{ pMeshData->pPrevious };

                delete pMeshData;
                pMeshData = pTempMeshData;
            }
            pMeshData->pNext = nullptr;
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
    ImGui::OpenPopup( "Import Mesh" );
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos( center, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ));
    if ( ImGui::BeginPopupModal( "Import Mesh", nullptr, ImGuiWindowFlags_AlwaysAutoResize ))
    {
        ImGui::Text( "%s", std::format( "Importing from {}", m_path.string()).c_str());
        BalEditor::EditorGUI::DrawToggle( "Import as preset", m_preset );
        BalEditor::EditorGUI::DrawToggle( "Import Meshes", m_meshes );
        if ( m_meshes )
        {
            BalEditor::EditorGUI::DrawToggle( "Try to create materials", m_materials );
            BalEditor::EditorGUI::DrawToggle( "Try to import related textures", m_textures );
        }
        if ( m_preset )
        {
            BalEditor::EditorGUI::DrawToggle( "Import Lights", m_lights );
            BalEditor::EditorGUI::DrawToggle( "Import Cameras", m_cameras );
        }
        if ( BalEditor::EditorGUI::DrawButton( "Import" ))
        {
            m_isVisible = false;
            if ( !LoadMesh())
            { //TODO add task to thread pool
                std::cerr << "Failed to import file: " << m_path.string() << std::endl;
                ImGui::EndPopup();
                return false;
            }
            ImGui::EndPopup();
            return true;
        }
        if ( BalEditor::EditorGUI::DrawButton( "Cancel" ))
        {
            ImGui::EndPopup();
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
