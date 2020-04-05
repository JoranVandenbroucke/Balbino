#include "BalbinoPCH.h"
#include "Component.h"
#include "../GameObject/GameObject.h"
#include "Transform.h"

Balbino::Component::Component( const std::weak_ptr<GameObject> origin )
	:m_Origin{ origin }
{
}

void Balbino::Component::Create()
{
	m_Transform = GetComponent<Transform>();
}
