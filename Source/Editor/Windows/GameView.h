#pragma once
#include <Core.h>

struct ISystem;
namespace BalbinoScene
{
    class CScene;
}

namespace FawnForge
{
    class CSceneHierarchy;

    class CGameView final
    {
    public:
        CGameView()                              = default;
        ~CGameView()                             = default;
        CGameView( const CGameView& )            = delete;
        CGameView( CGameView&& )                 = delete;
        CGameView& operator=( const CGameView& ) = delete;
        CGameView& operator=( CGameView&& )      = delete;
        void Draw() noexcept;

        void SetContext( ISystem* pSystem, CSceneHierarchy* pSceneHierarchy ) noexcept;
        void SetSnap( bool snap ) noexcept;
        void SetGuizmo( int key ) noexcept;

        [[nodiscard]] int GetGuizmoType() const;

    private:
        CSceneHierarchy* m_pSceneHierarchy {BALBINO_NULL};
        ISystem* m_pSystem {BALBINO_NULL};
        BalbinoScene::CScene* m_pContext {BALBINO_NULL};

        int m_gizmoType { 7 };
        bool m_snap {};
    };
}// namespace FawnForge
