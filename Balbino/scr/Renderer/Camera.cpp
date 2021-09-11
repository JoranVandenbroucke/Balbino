#include "pch.h"
#include "Camera.h"

#include "../Managers/CameraManager.h"

void* Balbino::CCamera::operator new( const size_t size )
{
	if ( CCamera* pCam = static_cast<CCamera*>( ::operator new( size ) ) )
	{
		CCameraManager::GetInstance().AddCamera( pCam );
		return pCam;
	}
	return nullptr;
}

void* Balbino::CCamera::operator new( size_t size, int b, const char* f, int l)
{
	if (CCamera* pCam = static_cast<CCamera*>(::operator new(size, b, f, l)))
	{
		CCameraManager::GetInstance().AddCamera(pCam);
		return pCam;
	}
	return nullptr;
}

void Balbino::CCamera::operator delete( void* ptr ) noexcept
{
	if ( ptr )
		::operator delete( ptr );
	ptr = nullptr;
}

void Balbino::CCamera::operator delete( void* ptr, int b, const char* f, int l) noexcept
{
	if (ptr)
		::operator delete(ptr, b, f, l);
	ptr = nullptr;
}

void Balbino::CCamera::SetDrawIndex( const int index )
{
	m_index = index;
}

int Balbino::CCamera::GetDrawIndex() const
{
	return m_index;
}
