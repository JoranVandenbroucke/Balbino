#pragma once
#include "entt/entt.hpp"
#include "IEntity.h"
#include "IScene.h"

namespace Balbino
{
    class CEntity final : public IEntity
    {
    public:
        CEntity();
        explicit CEntity( entt::entity handle, IScene* scene );
        ~CEntity() override = default;
        CEntity( const CEntity& ) = default;
        CEntity( CEntity&& ) = default;
        CEntity& operator=( const CEntity& ) = default;
        CEntity& operator=( CEntity&& ) = default;
        
        operator bool() const override;
        operator entt::entity() const override;
        operator unsigned() const override;
        
        bool operator==( const CEntity& other ) const;
        bool operator!=( const CEntity& other ) const;
        
        [[nodiscard]] const CUuid& GetId() const override;
        [[nodiscard]] bool IsInitialized() const override;
        [[nodiscard]] bool IsLoadedFromLevelFile() const override;
        
        void AttachChild( IEntity* pChildEntity ) override;
        void DetachAll() override;
        void DetachThis() override;
        
        [[nodiscard]] int GetChildCount() const override;
        [[nodiscard]] IEntity* GetChild( int index ) const override;
        [[nodiscard]] IEntity* GetParent() const override;
        [[nodiscard]] IEntity* GetLocalSimParent() const override;
        
        bool IsUpdateEnabled() override;
        void RemoveAllComponents() override;
        [[nodiscard]] const glm::vec3& GetPosition() const override;
        [[nodiscard]] const glm::quat& GetRotation() const override;
        [[nodiscard]] const glm::vec3& GetScale() const override;
        [[nodiscard]] glm::vec3 GetWorldPosition() const override;
        [[nodiscard]] glm::quat GetWorldRotation() const override;
        [[nodiscard]] glm::vec3 GetWorldScale() const override;
        void SetPosition( const glm::vec3& position ) const override;
        void SetRotation( const glm::quat& rotation ) const override;
        void SetScale( const glm::vec3& scale ) const override;
        void SetPosRotScale( const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scale ) const override;
    private:
        bool m_isInitialized;
        bool m_isLoadedFromFile;
    };
}
