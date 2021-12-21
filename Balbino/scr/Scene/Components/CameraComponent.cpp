#include "pch.h"
#include "CameraComponent.h"
#include "IEntity.h"

Balbino::CCameraComponent::CCameraComponent( IEntity* pEntity )
	: m_primary{ true }
	, m_pEntity{ pEntity }
{
}

const Balbino::CCamera& Balbino::CCameraComponent::GetCamera() const
{
	return m_camera;
}

Balbino::CCamera& Balbino::CCameraComponent::GetCamera()
{
	return m_camera;
}

bool Balbino::CCameraComponent::GetIsPrimary() const
{
	return m_primary;
}

void Balbino::CCameraComponent::SetCamera( const CCamera& Camera )
{
	m_camera = Camera;
}

void Balbino::CCameraComponent::SetIsPrimary( const bool isPrimary )
{
	m_primary = isPrimary;
}
