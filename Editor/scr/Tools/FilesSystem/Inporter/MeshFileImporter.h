//
// Created by joran on 13/06/2022.
//

#ifndef GAME_MESHFILEIMPORTER_H
#define GAME_MESHFILEIMPORTER_H

#include <filesystem>

namespace BalEditor
{
    class CMeshFileImporter
    {
    public:
        CMeshFileImporter();
        ~CMeshFileImporter() = default;

        void SetVisible(const std::filesystem::path& path, std::string pDestinationDirection = "../Data");
        bool IsVisible()const;
        bool DrawImportSettings();
        float GetImportPercentage();

    private:
        float m_importPercentage;
        bool m_isVisible;
        bool m_preset;
        bool m_meshes;
        bool m_materials;
        bool m_textures;
        bool m_lights;
        bool m_cameras;
        std::filesystem::path m_path;
        std::string m_destinationDirection;
        bool LoadMesh() const;
    };
}

#endif //GAME_MESHFILEIMPORTER_H
