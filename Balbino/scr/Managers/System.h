#pragma once
#include "ISystem.h"
#include "Core.h"

namespace Balbino
{
    class CInputHandler;
    
    class BALBINO_API CSystem final : public ISystem
    {
    public:
        CSystem( float w, float h );
        ~CSystem() override = default;
        CSystem( const CSystem& ) = delete;
        CSystem( CSystem&& ) = delete;
        CSystem& operator=( const CSystem& ) = delete;
        CSystem& operator=( CSystem&& ) = delete;
        
        void Initialize();
        bool Update( bool isPause ) override;
        void Cleanup();
        
        [[nodiscard]] IResourceManager* GetResourceManager() const override;
        [[nodiscard]] ICameraManager* GetCameraManager() const override;
        [[nodiscard]] IScene* GetCurrentActiveScene() const override;
        
        void SetInputHandler( CInputHandler* pInputHandler );
        [[nodiscard]]CInputHandler* GetInputHandler() const;
        void SetCurrentScene( IScene* pScene );
        
        float GetWindowWidth() override;
        float GetWindowHeight() override;
    
    private:
        CInputHandler   * m_pInputHandler;
        IResourceManager* m_pResourceManager;
        ICameraManager  * m_pCameraManager;
        IScene          * m_pCurrentScene;
        float m_windowWidth;
        float m_windowHeight;
    };
}
