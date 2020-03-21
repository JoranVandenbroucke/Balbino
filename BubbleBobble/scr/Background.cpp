#include "pch.h"
#include "Background.h"


BubbleBobble::Background::~Background() = default;

void BubbleBobble::Background::Create()
{
	SetTexture( "background.jpg" );
}

void BubbleBobble::Background::Update(){}

void BubbleBobble::Background::Draw() const
{
	const auto pos = m_Transform.GetPosition();
	Balbino::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void BubbleBobble::Background::SetTexture(const std::string& filename)
{
	m_Texture = Balbino::ResourceManager::GetInstance().LoadTexture(filename);
}

void BubbleBobble::Background::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}
