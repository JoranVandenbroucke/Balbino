#pragma once
#include "ISystem.h"
#include "IScene.h"

namespace FawnForge
{
    class CSceneHierarchy;
    
    class CGameView
    {
    public:
        CGameView() = default;
        ~CGameView() = default;
        CGameView( const CGameView& ) = delete;
        CGameView( CGameView&& ) = delete;
        CGameView& operator=( const CGameView& ) = delete;
        CGameView& operator=( CGameView&& ) = delete;
        void Draw();
        
        void SetContext( ISystem* pSystem, IScene* pContext, CSceneHierarchy* pSceneHierarchy );
        void SetSnap( bool snap );
        void SetGuizmo( int key );
        
        [[nodiscard]] int GetGuizmoType() const;
    private:
        CSceneHierarchy* m_pSceneHierarchy{};
        IScene         * m_pContext{};
        int  m_gizmoType {7};
        bool m_snap{};
        ISystem* m_pSystem{};
    };
}
