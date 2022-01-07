#pragma once

struct IEntity;
struct IScene;

namespace BalEditor
{
    class CSceneHierarchy
    {
    public:
        CSceneHierarchy();
        ~CSceneHierarchy() = default;
        CSceneHierarchy( const CSceneHierarchy& ) = delete;
        CSceneHierarchy( CSceneHierarchy&& ) = delete;
        CSceneHierarchy& operator=( const CSceneHierarchy& ) = delete;
        CSceneHierarchy& operator=( CSceneHierarchy&& ) = delete;
        void Draw();
        void SetContext( IScene* pScene );
        void ShowWindow();

        [[nodiscard]] IEntity* GetSelectedEntity()const;

    private:
        IScene* m_pContext;
        IEntity* m_pSelectionContext;
        bool m_isVisible;
        void DrawEntityNode( IEntity* pEntity );
        void DrawComponents( IEntity* pEntity ) const;
    };
}

