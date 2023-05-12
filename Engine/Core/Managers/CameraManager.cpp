//
// Created by joran on 22/10/2022.
//

#include "CameraManager.h"

namespace Balbino
{
    void CCameraManager::Initialize()
    {
        //no special things to do yet.
    }
    void CCameraManager::AddCamera( CCameraComponent* pComponent )
    {
        if ( m_cameras.empty())
        {
            m_pActive = pComponent;
        }
        m_cameras.push_back( pComponent );
    }
    void CCameraManager::SwitchCameraToActive( CCameraComponent* pComponent )
    {
        m_pActive = pComponent;
    }
    void CCameraManager::RemoveCamera( CCameraComponent* pComponent )
    {
        if ( pComponent == m_pActive )
        {
            m_pActive = nullptr;
        }
        
        const auto ret = std::ranges::remove_if(
                m_cameras, [ pComponent ]( const CCameraComponent* pCamera )
                {
                    return pComponent == pCamera;
                }
        );
        m_cameras.erase( ret.begin(), ret.end());
    }
    bool CCameraManager::IsThisCameraActive( const CCameraComponent* pComponent )
    {
        return pComponent == m_pActive;
    }
    const CCameraComponent* CCameraManager::GetActiveCamera() const
    {
        return m_pActive;
    }
    CCameraComponent* CCameraManager::GetActiveCamera()
    {
        return m_pActive;
    }
    void CCameraManager::Cleanup()
    {
        m_pActive = nullptr;
        m_cameras.clear();
    }
} // Balbino