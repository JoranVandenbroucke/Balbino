#pragma once
#include "FileParcer.h"
#include "Shader.h"

struct ISystem;

namespace Balbino
{
    class CMaterial;
}

namespace BalEditor
{
    class CPropertyPanel;
    
    class CMaterialEditor final
    {
    public:
        CMaterialEditor();
        ~CMaterialEditor() = default;
        CMaterialEditor( const CMaterialEditor& ) = delete;
        CMaterialEditor( CMaterialEditor&& ) = delete;
        CMaterialEditor& operator=( const CMaterialEditor& ) = delete;
        CMaterialEditor& operator=( CMaterialEditor&& ) = delete;
        
        void Initialize( const ISystem* pSystem, const CPropertyPanel* pPropertyPanel );
        void Draw();
        void Cleanup();
        void ShowWindow( const SFile& currentMaterial );
    private:
        bool  m_isVisible;
        SFile m_currentMaterial;
        
        uint64_t m_shaderID;
        uint64_t m_shaderPresetNr;
        
        const ISystem       * m_pSystem;
        const CPropertyPanel* m_pPropertyPanel;
        std::vector<BalVulkan::SShaderResource> m_shaderResources;
        std::vector<std::string>                m_defaultShaderNames;
        
        void LoadShader();
        void SaveMaterial() const;
        void LoadEditorFromData();
        void ReloadMaterial() const;
    };
}