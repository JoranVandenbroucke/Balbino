#pragma once
#include "Entity.h"
#include <string>

struct ISystem;
namespace BalbinoScene
{
    class CScene;
}

namespace FawnForge
{
    class CSceneHierarchy final
    {
    public:
        CSceneHierarchy()                                    = default;
        ~CSceneHierarchy()                                   = default;
        CSceneHierarchy( const CSceneHierarchy& )            = delete;
        CSceneHierarchy( CSceneHierarchy&& )                 = delete;
        CSceneHierarchy& operator=( const CSceneHierarchy& ) = delete;
        CSceneHierarchy& operator=( CSceneHierarchy&& )      = delete;
        void Draw() noexcept;
        void SetContext( const ISystem* pSystem ) noexcept;
        void ShowWindow() noexcept;

        [[nodiscard]] const BalbinoScene::CEntity& GetSelectedEntity() const;
        [[nodiscard]] BalbinoScene::CEntity& GetSelectedEntity();

    private:
        BalbinoScene::CEntity m_selectedEntity{};
        BalbinoScene::CScene* m_pScene { nullptr };
        const ISystem* m_pSystem { nullptr };
        bool m_isVisible { true };
        void DrawEntityNode( const BalbinoScene::CEntity& entity ) noexcept;
        static void DrawComponents( BalbinoScene::CEntity& pEntity ) ;

        template<typename T, typename... Args>
        void DisplayAddComponentEntry( const std::string& entryName, Args&&... args );
    };
}// namespace FawnForge