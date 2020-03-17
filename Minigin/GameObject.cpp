#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

Balbino::GameObject::~GameObject() = default;

void Balbino::GameObject::Create()
{
	SetTexture( "background.jpg" );
}

void Balbino::GameObject::Update(){}

void Balbino::GameObject::Draw() const
{
	const auto pos = m_Transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void Balbino::GameObject::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void Balbino::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}
