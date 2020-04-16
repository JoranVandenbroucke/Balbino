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
	AddComponent<Transform>();
	if( m_Components.size() > 1 )
	{
		for( int i = int( m_Components.size() - 1 ); i > 0; --i )
		{
			std::swap( m_Components[i], m_Components[i - 1] );
		}
	}
	for( std::weak_ptr<Component> comp : m_Components )
	{
		comp.lock()->Create();
	}
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

void Balbino::GameObject::DrawInspector()
{
	for( std::weak_ptr<Component> comp : m_Components )
	{
		comp.lock()->DrawInpector();
	}
}

void Balbino::GameObject::Destroy()
{
	m_Components.clear();
}

void Balbino::GameObject::LoadComponents()
{
	for( std::weak_ptr<Component> comp : m_Components )
	{
		comp.lock()->Create();
	}
}

void Balbino::GameObject::SetName( const char* newName )
{
	m_Name = newName;
}

const char* Balbino::GameObject::GetName() const
{
	return m_Name.c_str();
}
