//
// Created by joran on 23/10/2022.
//

#ifndef GAME_PROPERTYPANEL_H
#define GAME_PROPERTYPANEL_H

#include <cstdint>
#include <UUID.h>

namespace BalEditor
{
    class CPropertyPanel final
    {
    public:
        CPropertyPanel();
        ~CPropertyPanel() = default;
        CPropertyPanel( const CPropertyPanel& ) = delete;
        CPropertyPanel( CPropertyPanel&& ) = delete;
        CPropertyPanel& operator=( const CPropertyPanel& ) = delete;
        CPropertyPanel& operator=( CPropertyPanel&& ) = delete;
    
        void Draw();
        void ShowWindow();
    
        [[nodiscard]] uint64_t GetDefaultShader() const;
        [[nodiscard]] uint64_t GetDefaultTransparentShader() const;
        [[nodiscard]] uint64_t GetDefaultClothShader() const;
        [[nodiscard]] uint64_t GetDefaultSkinShader() const;
        [[nodiscard]] uint64_t GetDefaultHairShader() const;
        [[nodiscard]] uint64_t GetDefaultDebugShader() const;
        [[nodiscard]] uint64_t GetDefaultSkyboxShader() const;
    
        [[nodiscard]] uint64_t GetDefaultMaterial() const;
        [[nodiscard]] uint64_t GetDefaultTransparentMaterial() const;
        [[nodiscard]] uint64_t GetDefaultClothMaterial() const;
        [[nodiscard]] uint64_t GetDefaultSkinMaterial() const;
        [[nodiscard]] uint64_t GetDefaultHairMaterial() const;
        [[nodiscard]] uint64_t GetDefaultDebugMaterial() const;
        [[nodiscard]] uint64_t GetDefaultSkyboxMaterial() const;
        [[nodiscard]] uint64_t GetDefaultTexture() const;
    private:
        struct EPropertyTypes
        {
            enum Enum
            {
                Audio,
                Graphics,
                InputManager,
                Physics,
                Player,
                Time,
                MaxItems
            };
        };
        bool m_isVisible;
        EPropertyTypes::Enum m_currentPropertyType;
#pragma region graphics
        uint64_t m_defaultShaderID;
        uint64_t m_defaultTransparentShaderID;
        uint64_t m_defaultClothShaderID;
        uint64_t m_defaultSkinShaderID;
        uint64_t m_defaultHairShaderID;
        uint64_t m_defaultDebugShaderID;
        uint64_t m_defaultSkyboxShaderID;
        
        uint64_t m_defaultMaterialID;
        uint64_t m_defaultTransparentMaterialID;
        uint64_t m_defaultClothMaterialID;
        uint64_t m_defaultSkinMaterialID;
        uint64_t m_defaultHairMaterialID;
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
        static const char* ToString( EPropertyTypes::Enum types );
    };
} // BalEditor

#endif //GAME_PROPERTYPANEL_H