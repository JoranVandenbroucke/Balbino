#ifndef SCENE_HIERARCHY_H
#define SCENE_HIERARCHY_H

#include <string>

struct IEntity;
struct IScene;
struct ISystem;

namespace FawnForge
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
        void SetContext( IScene* pScene, ISystem* pSystem );
        void ShowWindow();
        
        [[nodiscard]] IEntity* GetSelectedEntity() const;
    
    private:
        IScene * m_pContext;
        IEntity* m_pSelectionContext;
        ISystem* m_pSystem;
        
        bool m_isVisible;
        void DrawEntityNode( IEntity* pEntity );
        void DrawComponents( IEntity* pEntity ) const;
        
        template<typename T, typename ... Args>
        void DisplayAddComponentEntry( const std::string& entryName, Args&& ... args );
    };
}
#endif