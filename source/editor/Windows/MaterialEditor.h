#pragma once
#include <Core.h>
#include <UUID.h>
#include <FawnVision.h>
#include <Serializer.h>

struct ISystem;

namespace FawnForge
{
    class CPropertyPanel;

    class CMaterialEditor final
    {
    public:
        CMaterialEditor()                                    = default;
        ~CMaterialEditor()                                   = default;
        CMaterialEditor( const CMaterialEditor& )            = delete;
        CMaterialEditor( CMaterialEditor&& )                 = delete;
        CMaterialEditor& operator=( const CMaterialEditor& ) = delete;
        CMaterialEditor& operator=( CMaterialEditor&& )      = delete;

        void Initialize( ISystem* pSystem, const CPropertyPanel* pPropertyPanel );
        void Draw() noexcept;
        void Cleanup();
        void ShowWindow( const SFile& currentMaterial );

    private:
        bool m_isVisible {};
        SFile m_currentMaterial {};

        CUuid m_shaderId {};
        uint64_t m_shaderPresetNr {};

        ISystem* m_pSystem { BALBINO_NULL };
        const CPropertyPanel* m_pPropertyPanel { BALBINO_NULL };

        std::vector<FawnVision::SShaderResource> m_shaderResources {};
        const std::vector<std::string> m_defaultShaderNames { "Default", "Transparent", "Cloth", "Skin", "Hair", "Debug", "Skybox", "Custom" };

        void LoadShader() noexcept;
        void SaveMaterial() const;
        void LoadEditorFromData() noexcept;
        void ReloadMaterial() const;
    };
}// namespace FawnForge