#pragma once
#include "../Camera.h"

#include "../IEntity.h"

class CCameraComponent final
{
public:
	CCameraComponent() = default;
	explicit CCameraComponent( IEntity* pEntity )
		: m_primary{ true }
		, m_pEntity{ pEntity }
	{
	}

	~CCameraComponent() = default;
	CCameraComponent( const CCameraComponent& ) = default;
	CCameraComponent( CCameraComponent&& ) = default;
	CCameraComponent& operator=( const CCameraComponent& ) = default;
	CCameraComponent& operator=( CCameraComponent&& ) = default;

	void SetIsPrimary( const bool isPrimary )
	{
		m_primary = isPrimary;
	}

	Balbino::CCamera& GetCamera()
	{
		return m_camera;
	}

	[[nodiscard]] const Balbino::CCamera& GetCamera() const
	{
		return m_camera;
	}

	[[nodiscard]] bool IsPrimary() const
	{
		return m_primary;
	}

	[[nodiscard]] IEntity* GetEntity() const
	{
		return m_pEntity;
	}

    [[nodiscard]] const glm::mat4& GetProjection() const
    {
        return m_camera.GetProjection();
    }

private:
	bool m_primary;
	Balbino::CCamera m_camera;
	IEntity* m_pEntity;
};
