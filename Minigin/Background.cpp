#include "MiniginPCH.h"
#include "Background.h"
#include "ResourceManager.h"
#include "Renderer.h"

Balbino::Background::~Background() = default;

void Balbino::Background::Create()
{
	SetTexture( "background.jpg" );
}

void Balbino::Background::Update(){}

void Balbino::Background::Draw() const
{
	const auto pos = m_Transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void Balbino::Background::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void Balbino::Background::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}
