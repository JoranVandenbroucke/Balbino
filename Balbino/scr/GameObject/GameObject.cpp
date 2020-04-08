#include "BalbinoPCH.h"
#include "GameObject.h"
#include "../Components/Component.h"

Balbino::GameObject::GameObject()
	: SceneObject{}
	, m_Components{}
{
}

void Balbino::GameObject::Create()
{
}

void Balbino::GameObject::Update()
{
	for( std::weak_ptr<Component> comp : m_Components )
	{
		comp.lock()->Update();
	}
}

void Balbino::GameObject::Draw() const
{
	for( std::weak_ptr<Component> comp : m_Components )
	{
		comp.lock()->Draw();
	}
}

void Balbino::GameObject::Destroy()
{
	m_Components.clear();
	m_Childeren.clear();
}

void Balbino::GameObject::LoadComponents()
{
	for( std::weak_ptr<Component> comp : m_Components )
	{
		comp.lock()->Create();
	}
}

void Balbino::GameObject::RemoveChild( std::shared_ptr<GameObject> child )
{
	m_Childeren.erase( std::remove( m_Childeren.begin(), m_Childeren.end(), child ) );
}
