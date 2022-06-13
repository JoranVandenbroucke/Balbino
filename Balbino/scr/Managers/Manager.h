#pragma once

#include "IManager.h"

#include "Core.h"

class CResourceManager;

namespace Balbino
{
    class CRenderer;

    class CInputHandler;

    class CAssetManager;

    class BALBINO_API CSystem final
            : public ISystem
    {
    public:
        CSystem(float w, float h);
        ~CSystem() override = default;
        CSystem( const CSystem& ) = delete;
        CSystem( CSystem&& ) = delete;
        CSystem& operator=( const CSystem& ) = delete;
        CSystem& operator=( CSystem&& ) = delete;

        void Initialize();
        void Cleanup();

        void SetInputHandler( CInputHandler* pInputHandler );
        void SetCurrentScene( IScene* pScene );

        bool Update( bool isPause ) override;
        [[nodiscard]] IResourceManager* GetResourceManager() const override;
        [[nodiscard]] CInputHandler* GetInputHandler() const;
        [[nodiscard]] IScene* GetCurrentActiveScene() const override;

        [[nodiscard]] float GetWindowWidth() override;
        [[nodiscard]] float GetWindowHeight() override;
    private:
        CInputHandler* m_inputHandler;
        CResourceManager* m_resourceManager;
        IScene* m_pCurrentScene;
        float m_windowWidth;
        float m_windowHeight;
    };
}
