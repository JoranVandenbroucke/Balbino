#include "pch.h"
#include "Entity.h"

#include "Components/IDComponent.h"
#include "Components/TransformComponent.h"

#include "Scene.h"

Balbino::CEntity::CEntity()
	: IEntity{ {}, nullptr }
	, m_isInitialized{ false }
	, m_isLoadedFromFile{ false }
{
}

Balbino::CEntity::CEntity( const entt::entity handle, IScene* scene )
	: IEntity{ handle, scene }
	, m_isInitialized{ false }
	, m_isLoadedFromFile{ false }
{
}

Balbino::CEntity::operator bool() const
{
	return GetEntity() != entt::null;
}

Balbino::CEntity::operator entt::entity() const
{
	return GetEntity();
}

Balbino::CEntity::operator unsigned() const
{
	return static_cast<uint32_t>( GetEntity() );
}

bool Balbino::CEntity::operator==( const CEntity& other ) const
{
	return GetEntity() == other.GetEntity();
}

bool Balbino::CEntity::operator!=( const CEntity& other ) const
{
	return !( *this == other );
}

const CUuid& Balbino::CEntity::GetId() const
{
	return GetComponent<CIDComponent>()->GetUUID();
}

bool Balbino::CEntity::IsInitialized() const
{
	return m_isInitialized;
}

bool Balbino::CEntity::IsLoadedFromLevelFile() const
{
	return m_isLoadedFromFile;
}

void Balbino::CEntity::AttachChild( IEntity* pChildEntity )
{
	if ( HasComponent<CTransformComponent>() )
		GetComponent<CTransformComponent>()->AddChild( pChildEntity->GetComponent<CTransformComponent>() );
}

void Balbino::CEntity::DetachAll()
{
	if ( HasComponent<CTransformComponent>() )
		GetComponent<CTransformComponent>()->RemoveAllChildren();
}

void Balbino::CEntity::DetachThis()
{
	if ( !HasComponent<CTransformComponent>() )
		return;
	const auto transformComponent = GetComponent<CTransformComponent>();
	transformComponent->GetParent()->RemoveChild( transformComponent );
	transformComponent->ClearParent();
}

int Balbino::CEntity::GetChildCount() const
{
	if ( HasComponent<CTransformComponent>() )
		return GetComponent<CTransformComponent>()->GetChildCount();
	return 0;
}

IEntity* Balbino::CEntity::GetChild( int index ) const
{
	if ( HasComponent<CTransformComponent>() )
		return GetComponent<CTransformComponent>()->GetChild( index )->GetEntity();
	return nullptr;
}

IEntity* Balbino::CEntity::GetParent() const
{
	if ( HasComponent<CTransformComponent>() )
		return GetComponent<CTransformComponent>()->GetParent()->GetEntity();
	return nullptr;
}

IEntity* Balbino::CEntity::GetLocalSimParent() const
{
	return nullptr;
}

bool Balbino::CEntity::IsUpdateEnabled()
{
	return false;
}

void Balbino::CEntity::RemoveAllComponents()
{
	//GetComponent<>()
	//m_pScene->GetRegistry().remove<T>( m_entityHandle );
}

const glm::vec3& Balbino::CEntity::GetPosition() const
{
	return GetComponent<CTransformComponent>()->GetTranslation();
}

const glm::quat& Balbino::CEntity::GetRotation() const
{
	return GetComponent<CTransformComponent>()->GetRotation();
}

const glm::vec3& Balbino::CEntity::GetScale() const
{
	return GetComponent<CTransformComponent>()->GetScale();
}

glm::vec3 Balbino::CEntity::GetWorldPosition() const
{
	glm::vec3 position{};
	if ( GetParent() )
		position += GetParent()->GetWorldPosition();
	position += GetPosition();
	return position;
}

glm::quat Balbino::CEntity::GetWorldRotation() const
{
	glm::quat rot{};
	if ( GetParent() )
		rot += GetParent()->GetWorldRotation();
	rot += GetRotation();
	return rot;
}

glm::vec3 Balbino::CEntity::GetWorldScale() const
{
	glm::vec3 scale{};
	if ( GetParent() )
		scale += GetParent()->GetWorldScale();
	scale += GetScale();
	return scale;
}

void Balbino::CEntity::SetPosition( const glm::vec3& position ) const
{
	GetComponent<CTransformComponent>()->SetTranslation( position );
}

void Balbino::CEntity::SetRotation( const glm::quat& rotation ) const
{
	GetComponent<CTransformComponent>()->SetRotation( rotation );
}

void Balbino::CEntity::SetScale( const glm::vec3& scale ) const
{
	GetComponent<CTransformComponent>()->SetScale( scale );
}

void Balbino::CEntity::SetPosRotScale( const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scale ) const
{
	CTransformComponent* comp = GetComponent<CTransformComponent>();
	comp->SetTranslation( pos );
	comp->SetScale( scale );
	comp->SetRotation( rot );
}
