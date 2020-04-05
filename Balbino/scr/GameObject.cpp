#include "BalbinoPCH.h"
#include "GameObject.h"
#include "Component.h"

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
	for( std::shared_ptr<Component> comp : m_Components )
	{
		comp->Update();
	}
}

void Balbino::GameObject::Draw() const
{
	for( std::shared_ptr<Component> comp : m_Components )
	{
		comp->Draw();
	}
}

void Balbino::GameObject::LoadComponents()
{
	for( std::shared_ptr<Component> comp : m_Components )
	{
		comp->Create();
	}
}
