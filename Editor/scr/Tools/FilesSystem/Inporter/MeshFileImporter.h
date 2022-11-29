//
// Created by joran on 13/06/2022.
//

#ifndef GAME_MESHFILEIMPORTER_H
#define GAME_MESHFILEIMPORTER_H

#include <filesystem>
#include <MeshMetadata.h>
#include <Vertex.h>
#include <assimp/matrix4x4.h>
#include <UUID.h>

struct aiMesh;
struct aiNode;
struct aiScene;
namespace BalEditor
{
    class CMeshFileImporter
    {
    public:
        CMeshFileImporter();
        ~CMeshFileImporter() = default;
        
        void SetVisible( const std::filesystem::path& path, const std::string& pDestinationDirection = "../Data" );
        bool IsVisible() const;
        bool DrawImportSettings();
        float GetImportPercentage();
    
    private:
        struct SMesh
        {
            std::vector<BalVulkan::SVertex> vertices;
            std::vector<uint32_t>           indices;
            Balbino::SMeshMetadata          meshData;
        };
        
        float m_importPercentage;
        bool m_isVisible;
        bool m_importMeshes;
        bool m_applyTransforms;
        bool  m_mergeMeshes;
        bool  m_materials;
        bool m_textures;
        bool m_importLights;
        bool m_importCameras;
        
        std::vector<SMesh> m_meshes;
        
        std::filesystem::path m_path;
        std::string           m_destinationDirection;
        
        [[nodiscard]] bool LoadMesh();
        void ProcessMeshRecursively( const aiNode* pMesh, const aiScene* pScene, int index = 0 );
        void ProcessMesh( const aiMesh* pMesh, SMesh& mesh, const aiMatrix4x4& transform = {
                1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1
        } ) const;
    };
}

#endif //GAME_MESHFILEIMPORTER_H
