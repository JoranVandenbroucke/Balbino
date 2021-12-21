#pragma once
#include <glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>

struct IEntity;

namespace Balbino
{
    class CTransformComponent final
    {
    public:
        CTransformComponent() = default;
        explicit CTransformComponent( IEntity* pEntity, const glm::vec3& translation );

        ~CTransformComponent() = default;
        CTransformComponent( const CTransformComponent& ) = default;
        CTransformComponent( CTransformComponent&& ) = default;
        CTransformComponent& operator=( const CTransformComponent& ) = default;
        CTransformComponent& operator=( CTransformComponent&& ) = default;

        [[nodiscard]] glm::mat4 GetTransform() const;
        [[nodiscard]] const glm::vec3& GetTranslation()const;
        [[nodiscard]] const glm::vec3& GetScale()const;
        [[nodiscard]] const glm::quat& GetRotation()const;

        void SetTranslation( const glm::vec3& translate );
        void SetScale( const glm::vec3& scale );
        void SetRotation( const glm::quat& rotation );

        [[nodiscard]] CTransformComponent* GetParent() const;
        [[nodiscard]] CTransformComponent* GetChild( int index ) const;
        [[nodiscard]] int GetChildCount() const;
        void ClearParent();
        void RemoveChild( CTransformComponent* transformComponent );
        void DeleteAllChildren();
        void AddChild( CTransformComponent* entity );

        IEntity* GetEntity()const;
    private:
        glm::vec3 m_translate;
        glm::vec3 m_scale;
        glm::quat m_rotation;

        CTransformComponent* m_pParent;
        std::vector<CTransformComponent*> m_childeren;
        IEntity* m_pEntity;
    };

}
