#include "pch.h"
#include "TransformComponent.h"
#include <gtc/matrix_transform.hpp>
#include "IEntity.h"

Balbino::CTransformComponent::CTransformComponent( IEntity* pEntity, const glm::vec3& translation )
	: m_translate( translation )
	, m_scale{}
	, m_rotation{}
	, m_pParent{ nullptr }
	, m_pEntity{ pEntity }
{
}

glm::mat4 Balbino::CTransformComponent::GetTransform() const
{
	const glm::mat4 rotation{ glm::toMat4( m_rotation ) };

	return glm::translate( glm::mat4( 1.0f ), m_translate )
		* rotation
		* glm::scale( glm::mat4( 1.0f ), m_scale );
}

const glm::vec3& Balbino::CTransformComponent::GetTranslation() const
{
	return m_translate;
}

void Balbino::CTransformComponent::SetTranslation( const glm::vec3& translate )
{
	m_translate = translate;
}

const glm::vec3& Balbino::CTransformComponent::GetScale() const
{
	return m_scale;
}

void Balbino::CTransformComponent::SetScale( const glm::vec3& scale )
{
	m_scale = scale;
}

const glm::quat& Balbino::CTransformComponent::GetRotation() const
{
	return m_rotation;
}

void Balbino::CTransformComponent::SetRotation( const glm::quat& rotation )
{
	m_rotation = rotation;
}

Balbino::CTransformComponent* Balbino::CTransformComponent::GetParent() const
{
	return m_pParent;
}

Balbino::CTransformComponent* Balbino::CTransformComponent::GetChild( int index ) const
{
	return m_childeren[index];
}

int Balbino::CTransformComponent::GetChildCount() const
{
	return (int)m_childeren.size();
}

void Balbino::CTransformComponent::ClearParent()
{
	m_pParent = nullptr;
}

void Balbino::CTransformComponent::RemoveChild( CTransformComponent* transformComponent )
{
	(void)std::ranges::remove( m_childeren, transformComponent );
}

void Balbino::CTransformComponent::DeleteAllChildren()
{
	m_childeren.clear();
}

void Balbino::CTransformComponent::AddChild( CTransformComponent* entity )
{
	m_childeren.push_back( entity );
}

IEntity* Balbino::CTransformComponent::GetEntity() const
{
	return m_pEntity;
}

