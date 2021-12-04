#include "pch.h"
#include "CameraManager.h"

#include <algorithm>

#include "../Camera.h"
#include "../Renderer/Renderer.h"

Balbino::CCameraManager::CCameraManager()
	: m_pMainCamera{nullptr}
	, m_pRenderer{nullptr}
{
}

Balbino::CCameraManager::~CCameraManager()
{
	if (m_pMainCamera != nullptr || !m_cameras.empty())
		std::cerr << "Texture Manager not cleared" << std::endl;
}

void Balbino::CCameraManager::Update(float deltaTime) const
{
	m_pMainCamera->Update(deltaTime);
}

Balbino::CCamera* Balbino::CCameraManager::AddCamera(const glm::vec3& pos, float xAngle, float yAngle)
{
	m_cameras.emplace_back();
	m_cameras.back().Initialize(m_pRenderer->GetAspectRatio(), pos, xAngle, yAngle);
	if (!m_pMainCamera)
		m_pMainCamera = &m_cameras.back();
	SortList();
	return &m_cameras.back();
}

void Balbino::CCameraManager::SetRenderer(const CRenderer* renderer)
{
	m_pRenderer = renderer;
}

void Balbino::CCameraManager::Cleanup()
{
	m_pMainCamera = nullptr;
	m_cameras.clear();
}

void Balbino::CCameraManager::SortList()
{
	std::sort(m_cameras.begin(), m_cameras.end(), [](const CCamera& a, const CCamera& b)
	{
		return a.GetDrawIndex() > b.GetDrawIndex();
	});

	const int mainPos{
		static_cast<int>(std::find(m_cameras.cbegin(), m_cameras.cend(), *m_pMainCamera) - m_cameras.cbegin() - 1)
	};
	for (int i{mainPos}; i > 0; --i)
	{
		const CCamera cam{m_cameras[i]};
		m_cameras[i] = m_cameras[i - 1];
		m_cameras[i - 1] = cam;
	}
}

const Balbino::CCamera* Balbino::CCameraManager::GetMainCamera() const
{
	return m_pMainCamera;
}

const std::vector<Balbino::CCamera>& Balbino::CCameraManager::GetCameras() const
{
	return m_cameras;
}
