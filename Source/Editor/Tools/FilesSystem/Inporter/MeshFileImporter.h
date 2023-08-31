//
// Created by joran on 13/06/2022.
//

#pragma once
#include "FawnVision.h"
#include <UUID.h>
#include <assimp/matrix4x4.h>
#include <filesystem>

struct aiMesh;
struct aiNode;
struct aiScene;

namespace FawnForge
{
    class CMeshFileImporter
    {
    public:
        CMeshFileImporter()  = default;
        ~CMeshFileImporter() = default;

        void SetVisible( const std::filesystem::path& path, const std::string_view pDestinationDirection = "../Data" );
        [[nodiscard]] bool IsVisible() const;
        bool DrawImportSettings() noexcept;
        float GetImportPercentage() const;

    private:
        struct SMesh final {
            std::vector<FawnVision::SVertex> vertices {};
            std::vector<uint32_t> indices {};
            SMeshMetadata meshData {};
        };

        float m_importPercentage {};
        bool m_isVisible {};
        bool m_importMeshes {};
        bool m_applyTransforms {};
        bool m_mergeMeshes {};
        bool m_materials {};
        bool m_textures {};
        bool m_importLights {};
        bool m_importCameras {};

        std::vector<SMesh> m_meshes {};

        std::filesystem::path m_path {};
        std::string m_destinationDirection {};

        [[nodiscard]] bool LoadMesh() noexcept;
        void ProcessMeshRecursively( const aiNode* pMesh, const aiScene* pScene, unsigned int index = 0 );
        void ProcessMesh( const aiMesh* pMesh, SMesh& mesh, const aiMatrix4x4& transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } ) const;
    };
}// namespace FawnForge
