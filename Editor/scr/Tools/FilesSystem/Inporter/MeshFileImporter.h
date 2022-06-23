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
        virtual ~CMeshFileImporter();

        bool DrawImportSettings(const std::filesystem::path& path);
        float GetImportPercentage();

    private:
        bool LoadFromPath(const std::filesystem::path& path);
        float m_importPercentage;
        bool m_isImporting;
        bool m_preset;

        enum class PresetPropertieTypes{
            Camera,
            Light,
            Mesh
        };
    };
}

#endif //GAME_MESHFILEIMPORTER_H
