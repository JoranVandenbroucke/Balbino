#include "System.h"

#include "../Camera/CameraManager.h"
#include "../Input/InputHandler.h"
#include "../Resource/ResourceManager.h"

BalbinoControl::CSystem::CSystem( int w, int h ) noexcept
    : m_windowWidth { w }
    , m_windowHeight { h }
{
}

void BalbinoControl::CSystem::Initialize( FawnVision::CRenderer* pRenderer )
{
    m_pResourceManager = new BalbinoControl::CResourceManager {};
    m_pCameraManager   = new BalbinoControl::CCameraManager {};
    m_pResourceManager->Initialize( pRenderer );
    m_pCurrentRenderer = pRenderer;
}
bool BalbinoControl::CSystem::Update( bool isPause )
{
    (void)isPause;
    return false;
}
void BalbinoControl::CSystem::Cleanup()
{
    if ( m_pInputHandler )
    {
        m_pInputHandler->Cleanup();
        delete m_pInputHandler;
        m_pInputHandler = nullptr;
    }
    if ( m_pResourceManager )
    {
        m_pResourceManager->Cleanup();
        delete m_pResourceManager;
        m_pResourceManager = nullptr;
    }
    if ( m_pCameraManager )
    {
        m_pCameraManager->Cleanup();
        delete m_pCameraManager;
        m_pCameraManager = nullptr;
    }
    m_pCurrentScene = nullptr;
}
