#include "pch.h"
#include "CameraManager.h"

#include <algorithm>

#include "../Renderer/Camera.h"

Balbino::CCameraManager::CCameraManager()
	: m_pMainCamera{ nullptr }
{
}

Balbino::CCameraManager::~CCameraManager()
{
	if (m_pMainCamera != nullptr || !m_Cameras.empty())
		std::cerr << "Texture Manager not cleared" << std::endl;
}

const Balbino::CCamera* Balbino::CCameraManager::GetMainCamera()
{
	return GetInstance().IGetMainCamera();
}

const std::vector<Balbino::CCamera*>& Balbino::CCameraManager::GetCameras()
{
	return GetInstance().IGetCameras();
}

void Balbino::CCameraManager::AddCamera(  CCamera* cam )
{
	m_Cameras.push_back( cam );
	SortList();
}

void Balbino::CCameraManager::Cleanup()
{
	m_pMainCamera = nullptr;
	for (const CCamera* const camera : m_Cameras)
	{
		delete camera;
	}
	m_Cameras.clear();
}

void Balbino::CCameraManager::SortList()
{
	std::sort( m_Cameras.begin(), m_Cameras.end(), []( const CCamera* a, const CCamera* b )
	{
		return a->GetDrawIndex() > b->GetDrawIndex();
	} );

	const int mainPos{ static_cast<int>( std::find( m_Cameras.cbegin(), m_Cameras.cend(), m_pMainCamera ) - m_Cameras.cbegin()-1 ) };
	for( int i{mainPos}; i > 0; --i )
	{
		std::swap( m_Cameras[i], m_Cameras[i - 1] );
	}
}

const Balbino::CCamera* Balbino::CCameraManager::IGetMainCamera() const
{
	return m_pMainCamera;
}

const std::vector<Balbino::CCamera*>& Balbino::CCameraManager::IGetCameras() const
{
	return m_Cameras;
}
