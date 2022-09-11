#pragma once
#include "../IEntity.h"

#pragma warning(push)
#pragma warning(disable:4201)
#pragma warning(disable:4324)
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class CTransformComponent final
{
public:
	CTransformComponent() = default;

	explicit CTransformComponent( IEntity* pEntity, const glm::vec3& translation )
		: m_translate( translation )
		, m_scale{ 1, 1, 1 }
		, m_rotation{}
		, m_pParent{ nullptr }
		, m_pEntity{ pEntity }
	{
	}

	~CTransformComponent() = default;
	CTransformComponent( const CTransformComponent& ) = default;
	CTransformComponent( CTransformComponent&& ) = default;
	CTransformComponent& operator=( const CTransformComponent& ) = default;
	CTransformComponent& operator=( CTransformComponent&& ) = default;

	[[nodiscard]] glm::mat4 GetTransform() const
	{
		const glm::mat4 rotation{ toMat4( m_rotation ) };

		return translate( glm::mat4( 1.0f ), m_translate )
			* rotation
			* scale( glm::mat4( 1.0f ), m_scale );
	}

	[[nodiscard]] const glm::vec3& GetTranslation() const
	{
		return m_translate;
	}

	[[nodiscard]] const glm::vec3& GetScale() const
	{
		return m_scale;
	}

	[[nodiscard]] const glm::quat& GetRotation() const
	{
		return m_rotation;
	}

	void SetTranslation( const glm::vec3& translate )
	{
		m_translate = translate;
	}

	void SetScale( const glm::vec3& scale )
	{
		m_scale = scale;
	}

	void SetRotation( const glm::quat& rotation )
	{
		m_rotation = rotation;
	}

	[[nodiscard]] CTransformComponent* GetParent() const
	{
		return m_pParent;
	}

	[[nodiscard]] CTransformComponent* GetChild( int index ) const
	{
		return m_childeren[index];
	}

	[[nodiscard]] uint32_t GetChildCount() const
	{
		return static_cast<uint32_t>( m_childeren.size() );
	}

	void RemoveChild( CTransformComponent* transformComponent )
	{
		( void ) std::ranges::remove( m_childeren, transformComponent );
	}

	void RemoveAllChildren()
	{
		m_childeren.clear();
	}

	void ClearParent()
	{
		m_pParent = nullptr;
	}

	void AddChild( CTransformComponent* entity )
	{
		m_childeren.push_back( entity );
	}

	[[nodiscard]] IEntity* GetEntity() const
	{
		return m_pEntity;
	}

private:
	glm::vec3 m_translate;
	glm::vec3 m_scale;
	glm::quat m_rotation;

	CTransformComponent* m_pParent;
	std::vector<CTransformComponent*> m_childeren;
	IEntity* m_pEntity;
};
#pragma warning(pop)