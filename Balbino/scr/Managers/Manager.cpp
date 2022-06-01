#include "Manager.h"

#include "IResourceManager.h"
#include "ResourceManager.h"
#include "../Input/InputHandler.h"

Balbino::CSystem::CSystem()
        : m_inputHandler{ nullptr }
          , m_resourceManager{ nullptr }
          , m_pCurrentScene{ nullptr }
{
}

void Balbino::CSystem::Initialize()
{
    m_resourceManager = new CResourceManager{};
    m_resourceManager->Initialize( this );
}

void Balbino::CSystem::Cleanup()
{
    if( m_inputHandler )
    {
        m_inputHandler->Cleanup();
        delete m_inputHandler;
        m_inputHandler = nullptr;
    }
    if( m_resourceManager    )
    {
        m_resourceManager->Cleanup();
        delete m_resourceManager;
        m_resourceManager = nullptr;
    }
    m_pCurrentScene = nullptr;
}

void Balbino::CSystem::SetInputHandler(CInputHandler* pInputHandler)
{
    m_inputHandler = pInputHandler;
}

void Balbino::CSystem::SetCurrentScene(IScene* pScene)
{
    m_pCurrentScene = pScene;
}

bool Balbino::CSystem::Update(bool isPause)
{
    (void) isPause;
    return false;
}

IResourceManager* Balbino::CSystem::GetResourceManager() const
{
    return m_resourceManager;
}

Balbino::CInputHandler* Balbino::CSystem::GetInputHandler() const
{
    return m_inputHandler;
}

IScene* Balbino::CSystem::GetCurrentActiveScene() const
{
    return m_pCurrentScene;
}
