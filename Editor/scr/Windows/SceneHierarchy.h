#pragma once

struct IEntity;
struct IScene;

namespace BalEditor
{
    class CSceneHierarchy
    {
    public:
        CSceneHierarchy() = default;
        ~CSceneHierarchy() = default;
        CSceneHierarchy( const CSceneHierarchy& ) = delete;
        CSceneHierarchy( CSceneHierarchy&& ) = delete;
        CSceneHierarchy& operator=( const CSceneHierarchy& ) = delete;
        CSceneHierarchy& operator=( CSceneHierarchy&& ) = delete;
        void Draw();
        void SetContext( IScene* pScene );


    private:
        IScene* m_pContext;
        IEntity* m_pSelectionContext;

        void DrawEntityNode( IEntity* pEntity );
        void DrawComponents( IEntity* pEntity ) const;
    };
}

