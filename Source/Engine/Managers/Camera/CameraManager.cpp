//
// Created by joran on 22/10/2022.
//

#include "CameraManager.h"
#include <algorithm>

namespace BalbinoControl
{
    void CCameraManager::Initialize()
    {
        //no special things to do yet.
    }
    void CCameraManager::Cleanup()
    {
        m_pActive = nullptr;
        m_cameras.clear();
    }
    void CCameraManager::AddCamera( BalbinoComponent::CCameraComponent* pComponent ) noexcept
    {
        if ( m_cameras.empty() )
        {
            m_pActive = pComponent;
        }
        m_cameras.push_back( pComponent );
    }
    void CCameraManager::SwitchCameraToActive( BalbinoComponent::CCameraComponent* pComponent ) noexcept
    {
        m_pActive = pComponent;
    }
    void CCameraManager::RemoveCamera( BalbinoComponent::CCameraComponent* pComponent ) noexcept
    {
        if ( pComponent == m_pActive )
        {
            m_pActive = nullptr;
        }

        const auto ret = std::ranges::remove_if( m_cameras, [ pComponent ]( const BalbinoComponent::CCameraComponent* pCamera ) constexpr {
            return pComponent == pCamera;
        } );
        m_cameras.erase( ret.begin(), ret.end() );
    }
    bool CCameraManager::IsThisCameraActive( const BalbinoComponent::CCameraComponent* pComponent ) noexcept
    {
        return pComponent == m_pActive;
    }
    const BalbinoComponent::CCameraComponent* CCameraManager::GetActiveCamera() const noexcept
    {
        return m_pActive;
    }
    BalbinoComponent::CCameraComponent* CCameraManager::GetActiveCamera() noexcept
    {
        return m_pActive;
    }
}// namespace Balbino