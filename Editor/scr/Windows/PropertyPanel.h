//
// Created by joran on 23/10/2022.
//

#ifndef GAME_PROPERTYPANEL_H
#define GAME_PROPERTYPANEL_H

#include <cstdint>

namespace BalEditor
{
    class CPropertyPanel final
    {
    public:
        CPropertyPanel() = default;
        ~CPropertyPanel() = default;
        CPropertyPanel( const CPropertyPanel& ) = delete;
        CPropertyPanel( CPropertyPanel&& ) = delete;
        CPropertyPanel& operator=( const CPropertyPanel& ) = delete;
        CPropertyPanel& operator=( CPropertyPanel&& ) = delete;
        
        void Draw();
        void ShowWindow();
    private:
        enum class EPropertyTypes
        {
            Audio,
            Graphics,
            InputManager,
            Physics,
            Player,
            Time,
            MaxItems
        }    m_currentPropertyType;
        bool m_isVisible;

#pragma region graphics
        uint64_t m_defaultShaderID;
        uint64_t m_defaultTransparentShaderID;
        uint64_t m_defaultDebugShaderID;
        uint64_t m_defaultSkyboxShaderID;
        
        uint64_t m_defaultMaterialID;
        uint64_t m_defaultTransparentMaterialID;
        uint64_t m_defaultDebugMaterialID;
        uint64_t m_defaultSkyboxMaterialID;
        
        uint64_t m_defaultTextureID;
#pragma endregion graphics
        
        void DrawAudioProperties();
        void DrawGraphicsProperties();
        void DrawInputManagerProperties();
        void DrawPhysicsProperties();
        void DrawPlayerProperties();
        void DrawTimeProperties();
        void SaveToIni() const;
        void LoadFromIni();
        static const char* ToString( EPropertyTypes types );
    };
} // BalEditor

#endif //GAME_PROPERTYPANEL_H