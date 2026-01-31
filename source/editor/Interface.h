#pragma once
#include <FawnVision.h>
#include <cstdint>
#include <imnodes.h>
#include <string>
#include <vector>

union SDL_Event;
struct SDL_Window;

struct ISystem;

namespace FawnVision
{
    class CRenderer;
}
namespace FawnForge
{
    class CMainScreen;
    class CGameView;
    class CAssetBrowser;
    class CSceneHierarchy;
    class CShaderEditor;
    class CMaterialEditor;
    class CMeshFileImporter;
    class CTextureFileImporter;
    class CPropertyPanel;
    class CInterface final
    {
    public:
        CInterface() = default;

        ~CInterface()                              = default;
        CInterface( const CInterface& )            = delete;
        CInterface( CInterface&& )                 = delete;
        CInterface& operator=( const CInterface& ) = delete;
        CInterface& operator=( CInterface&& )      = delete;

        void Initialize( SDL_Window* pWindow, int32_t w, int32_t h, FawnVision::CRenderer* pRenderer, ISystem* pSystem );
        void Draw();
        void Cleanup();
        void ProcessEvent( SDL_Event e ) noexcept;
        void SetContext( ISystem* pSystem ) noexcept;
        static void Resize( int32_t w, int32_t h ) noexcept;

    private:
        bool m_queueNextResource { false };

        CMainScreen* m_pMain { nullptr };
        CGameView* m_pGameView { nullptr };
        CAssetBrowser* m_pAssetBrowser { nullptr };
        CSceneHierarchy* m_pSceneHierarchy { nullptr };
        CShaderEditor* m_pShaderGraph { nullptr };
        CMaterialEditor* m_pMaterialEditor { nullptr };
        CMeshFileImporter* m_pMeshImporter { nullptr };
        CTextureFileImporter* m_pTextureImporter { nullptr };
        CPropertyPanel* m_pPropertyPanel { nullptr };

        FawnVision::CRenderer* m_pRenderer { nullptr };

        SDL_Window* m_pWindow { nullptr };

        std::vector<std::string> m_pendingResources {};

        static void SetImGuiStyle( ImNodesStyle& imNodesStyle ) noexcept;
    };
}// namespace FawnForge
