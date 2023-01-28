#pragma once
#include <entt/entt.hpp>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/glm.hpp>

#include "IScene.h"
#include "UUID.h"

struct IEntity
{
    IEntity( entt::entity entity, IScene* scene )
            : m_entityHandle{ entity }
              , m_pScene{ scene }
    {
    }
    
    IEntity( const IEntity& other ) = default;
    
    IEntity( IEntity&& other ) noexcept
            : m_entityHandle{ other.m_entityHandle }
              , m_pScene{ other.m_pScene }
    {
    }
    
    IEntity& operator=( const IEntity& other )
    {
        if ( *this != other )
        {
            this->m_pScene       = other.m_pScene;
            this->m_entityHandle = other.m_entityHandle;
        }
        return *this;
    }
    
    IEntity& operator=( IEntity&& other ) noexcept
    {
        if ( *this != other )
        {
            this->m_pScene       = other.m_pScene;
            this->m_entityHandle = other.m_entityHandle;
        }
        return *this;
    }
    
    virtual ~IEntity() = default;
    virtual operator bool() const = 0;
    virtual operator entt::entity() const = 0;
    virtual operator uint32_t() const = 0;
    
    bool operator==( const IEntity& other ) const
    {
        return m_entityHandle == other.m_entityHandle && m_pScene == other.m_pScene;
    }
    
    bool operator!=( const IEntity& other ) const
    {
        return !( *this == other );
    }
    
    [[nodiscard]] virtual const CUuid& GetId() const = 0;
    [[nodiscard]] virtual bool IsInitialized() const = 0;
    [[nodiscard]] virtual bool IsLoadedFromLevelFile() const = 0;
    virtual void AttachChild( IEntity* pChildEntity ) = 0;
    virtual void DetachAll() = 0;
    virtual void DetachThis() = 0;
    [[nodiscard]] virtual int GetChildCount() const = 0;
    [[nodiscard]] virtual IEntity* GetChild( int nIndex ) const = 0;
    [[nodiscard]] virtual IEntity* GetParent() const = 0;
    [[nodiscard]] virtual IEntity* GetLocalSimParent() const = 0;
    
    [[nodiscard]] virtual const glm::vec3& GetPosition() const = 0;
    [[nodiscard]] virtual const glm::quat& GetRotation() const = 0;
    [[nodiscard]] virtual const glm::vec3& GetScale() const = 0;
    [[nodiscard]] virtual glm::vec3 GetWorldPosition() const = 0;
    [[nodiscard]] virtual glm::quat GetWorldRotation() const = 0;
    [[nodiscard]] virtual glm::vec3 GetWorldScale() const = 0;
    
    virtual void SetPosition( const glm::vec3& position ) const = 0;
    virtual void SetRotation( const glm::quat& rotation ) const = 0;
    virtual void SetScale( const glm::vec3& scale ) const = 0;
    virtual void SetPosRotScale( const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scale ) const = 0;
    
    /////////////////////////////////////////////////
    
    virtual bool IsUpdateEnabled() = 0;
    
    template<typename ComponentType, typename ... Args>
    ComponentType* AddComponent( Args&& ... args );
    template<typename ComponentType>
    ComponentType* GetComponent() const;
    template<typename ComponentType, typename ... Args>
    ComponentType* CreateOrReplaceComponent( Args&& ... args );
    
    template<typename ComponentType>
    void RemoveComponent() const;
    template<typename ComponentType>
    [[nodiscard]] bool HasComponent() const;
    
    virtual void RemoveAllComponents() = 0;
    
    [[nodiscard]] const entt::entity& GetEntity() const
    {
        return m_entityHandle;
    }
    
    [[nodiscard]] const IScene* GetScene() const
    {
        return m_pScene;
    }

private:
    entt::entity m_entityHandle{ entt::null };
    IScene* m_pScene;
};


template<typename ComponentType, typename ... Args>
ComponentType* IEntity::AddComponent( Args&& ... args )
{
    if ( HasComponent<ComponentType>())
    {
        return GetComponent<ComponentType>();
    }
    ComponentType& component = m_pScene->GetRegistry().emplace<ComponentType>(
            m_entityHandle, this, std::forward<Args>( args )...
    );
    return &component;
}

template<typename ComponentType>
[[nodiscard]] ComponentType* IEntity::GetComponent() const
{
    if ( HasComponent<ComponentType>())
    {
        return &m_pScene->GetRegistry().get<ComponentType>( m_entityHandle );
    }
    return nullptr;
}

template<typename ComponentType, typename ... Args>
ComponentType* IEntity::CreateOrReplaceComponent( Args&& ... args )
{
    //ComponentType& component = m_pScene->GetRegistry().emplace_or_replace<ComponentType>( m_entityHandle, this, std::forward<Args>( args )... );
    ComponentType& component = m_pScene->GetRegistry().emplace_or_replace<ComponentType>(
            m_entityHandle, std::forward<Args>(
                    args
            )...
    );
    return &component;
}

template<typename ComponentType>
void IEntity::RemoveComponent() const
{
    if ( HasComponent<ComponentType>())
    {
        m_pScene->GetRegistry().remove<ComponentType>( m_entityHandle );
    }
}

template<typename ComponentType>
bool IEntity::HasComponent() const
{
    return m_pScene->GetRegistry().all_of<ComponentType>( m_entityHandle );
}
#pragma warning(pop)