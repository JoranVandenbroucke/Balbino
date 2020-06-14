#include "BalbinoPCH.h"
#include "Component.h"
#include "../GameObject/GameObject.h"
#include "Transform.h"

Balbino::Component::Component( const GameObject* const origin )
	:m_pOrigin{ const_cast<GameObject* const>( origin ) }
	, m_Created{ false }
	, m_pTransform{ nullptr }
{
}

Balbino::Component::~Component()
{
	m_pOrigin = nullptr;
	m_pTransform = nullptr;
}

void Balbino::Component::Create()
{
	m_Created = true;
	m_pTransform = GetComponent<Transform>();
}

void Balbino::Component::OnTriggerEnter( GameObject* pGameObject )
{
	(void*) pGameObject;
}

void Balbino::Component::OnTriggerExit( GameObject* pGameObject )
{
	(void*) pGameObject;
}

void Balbino::Component::OnCollisionEnter( GameObject* pGameObject )
{
	(void*) pGameObject;
}

void Balbino::Component::OnCollisionExit( GameObject* pGameObject )
{
	(void*) pGameObject;
}

Balbino::GameObject* const Balbino::Component::GetGameObject() const
{
	return m_pOrigin;
}

