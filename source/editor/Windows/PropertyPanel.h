//
// Created by joran on 23/10/2022.
//

#pragma once
#include <Core.h>
#include <UUID.h>
#include <cstdint>

namespace FawnForge
{
    class CPropertyPanel final
    {
    public:
        CPropertyPanel();
        ~CPropertyPanel()                                  = default;
        CPropertyPanel( const CPropertyPanel& )            = delete;
        CPropertyPanel( CPropertyPanel&& )                 = delete;
        CPropertyPanel& operator=( const CPropertyPanel& ) = delete;
        CPropertyPanel& operator=( CPropertyPanel&& )      = delete;

        void Draw() noexcept;
        void ShowWindow() noexcept;

        [[nodiscard]] CUuid GetDefaultShader() const;
        [[nodiscard]] CUuid GetDefaultTransparentShader() const;
        [[nodiscard]] CUuid GetDefaultClothShader() const;
        [[nodiscard]] CUuid GetDefaultSkinShader() const;
        [[nodiscard]] CUuid GetDefaultHairShader() const;
        [[nodiscard]] CUuid GetDefaultDebugShader() const;
        [[nodiscard]] CUuid GetDefaultSkyboxShader() const;

        [[nodiscard]] CUuid GetDefaultMaterial() const;
        [[nodiscard]] CUuid GetDefaultTransparentMaterial() const;
        [[nodiscard]] CUuid GetDefaultClothMaterial() const;
        [[nodiscard]] CUuid GetDefaultSkinMaterial() const;
        [[nodiscard]] CUuid GetDefaultHairMaterial() const;
        [[nodiscard]] CUuid GetDefaultDebugMaterial() const;
        [[nodiscard]] CUuid GetDefaultSkyboxMaterial() const;
        [[nodiscard]] CUuid GetDefaultTexture() const;

    private:
        enum class property_type
        {
            audio,
            graphics,
            input_manager,
            physics,
            player,
            time,
            max
        };
        bool m_isVisible {};
        property_type m_currentPropertyType { property_type::audio };

#pragma region graphics
        enum shader_types : uint8_t
        {
            shader_types_default = 0,
            shader_types_transparent,
            shader_types_cloth,
            shader_types_skin,
            shader_types_hair,
            shader_types_skybox,
            shader_types_debug,
            shader_types_max
        };
        enum material_types : uint8_t
        {
            material_types_default = 0,
            material_types_transparent,
            material_types_cloth,
            material_types_skin,
            material_types_hair,
            material_types_skybox,
            material_types_debug,
            material_types_max
        };
        CUuid m_shaderIds[ shader_types_max ] {};
        CUuid m_materialIds[ material_types_max ] {};

        CUuid m_defaultTextureID {};
        const char* shaderNames[ shader_types_max ] {
            "Default Shader", "Transparent Shader", "Cloth Shader", "Skin Shader", "Hair Shader", "Skybox Shader", "Debug Shader",
        };
        const char* materialNames[ material_types_max ] {
            "Default Material", "Transparent Material", "Cloth Material", "Skin Material", "Hair Material", "Skybox Material", "Debug Material",
        };
#pragma endregion graphics

        void DrawAudioProperties() const noexcept;
        void DrawGraphicsProperties() noexcept;
        void DrawInputManagerProperties() const noexcept;
        void DrawPhysicsProperties() const noexcept;
        void DrawPlayerProperties() const noexcept;
        void DrawTimeProperties() const noexcept;
        void SaveToIni() const;
        void LoadFromIni() noexcept;
        static const char* ToString( property_type types );
    };
}// namespace FawnForge
