#include "BalbinoPCH.h"
#include "Component.h"
#include "../GameObject/GameObject.h"
#include "Transform.h"

Balbino::Component::Component( const GameObject* const origin )
	:m_pOrigin{ const_cast<GameObject* const>( origin ) }
{
}

void Balbino::Component::Create()
{
	m_pTransform = GetComponent<Transform>();
}

Balbino::GameObject* const Balbino::Component::GetGameObject() const
{
	return m_pOrigin;
}

