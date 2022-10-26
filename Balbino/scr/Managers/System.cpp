#include "System.h"

#include "ResourceManager.h"
#include "CameraManager.h"
#include "Input/InputHandler.h"

Balbino::CSystem::CSystem( float w, float h )
        : m_pInputHandler{ nullptr },
          m_pResourceManager{ nullptr },
          m_pCurrentScene{ nullptr },
          m_pCameraManager{ nullptr },
          m_windowWidth{ w },
          m_windowHeight{ h }
{
}

void Balbino::CSystem::Initialize()
{
    m_pResourceManager = new CResourceManager{};
    m_pCameraManager   = new CCameraManager{};
    m_pResourceManager->Initialize( this );
}
bool Balbino::CSystem::Update( bool isPause )
{
    (void) isPause;
    return false;
}
void Balbino::CSystem::Cleanup()
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
void Balbino::CSystem::SetInputHandler( CInputHandler* pInputHandler )
{
    m_pInputHandler = pInputHandler;
}

void Balbino::CSystem::SetCurrentScene( IScene* pScene )
{
    m_pCurrentScene = pScene;
}
IResourceManager* Balbino::CSystem::GetResourceManager() const
{
    return m_pResourceManager;
}
ICameraManager* Balbino::CSystem::GetCameraManager() const
{
    return m_pCameraManager;
}
IScene* Balbino::CSystem::GetCurrentActiveScene() const
{
    return m_pCurrentScene;
}
float Balbino::CSystem::GetWindowWidth()
{
    return m_windowWidth;
}
float Balbino::CSystem::GetWindowHeight()
{
    return m_windowHeight;
}
Balbino::CInputHandler* Balbino::CSystem::GetInputHandler() const
{
    return m_pInputHandler;
}
void Balbino::CSystem::SetHeight( int i )
{
    m_windowHeight = (float) i;
}
void Balbino::CSystem::SetWidth( int i )
{
    m_windowWidth = (float) i;
}
