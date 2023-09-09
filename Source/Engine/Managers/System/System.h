#pragma once
#include <Core.h>
#include "../ISystem.h"

namespace FawnVision
{
    class CRenderer;
}
namespace BalbinoControl
{
    class CInputHandler;

    class CSystem final : public ISystem
    {
    public:
        CSystem( int w, int h ) noexcept;
        ~CSystem() override                  = default;
        CSystem( const CSystem& )            = delete;
        CSystem( CSystem&& )                 = delete;
        CSystem& operator=( const CSystem& ) = delete;
        CSystem& operator=( CSystem&& )      = delete;

        void Initialize( FawnVision::CRenderer* pRenderer );
        bool Update( bool isPause ) override;
        void Cleanup();

        [[nodiscard]] inline BalbinoScene::CScene* GetCurrentActiveScene() const noexcept override{return m_pCurrentScene;}
        [[nodiscard]] inline IResourceManager* GetResourceManager() const noexcept override{return m_pResourceManager;}
        [[nodiscard]] inline FawnVision::CRenderer* GetRenderer() const noexcept override{return m_pCurrentRenderer;}
        [[nodiscard]] inline ICameraManager* GetCameraManager() const noexcept override{return m_pCameraManager;}
        [[nodiscard]] inline CInputHandler* GetInputHandler() const noexcept{return m_pInputHandler;}
        [[nodiscard]] inline int GetWindowHeight() noexcept override{return m_windowHeight;}
        [[nodiscard]] inline int GetWindowWidth() noexcept override{return m_windowWidth;}

        inline void SetInputHandler( CInputHandler* pInputHandler ) noexcept{m_pInputHandler = pInputHandler;};
        inline void SetCurrentScene( BalbinoScene::CScene* pScene ) noexcept{m_pCurrentScene = pScene;};

        inline void SetHeight( int h ) noexcept{m_windowHeight = h;};
        inline void SetWidth( int w ) noexcept{m_windowWidth = w;};

    private:
        CInputHandler* m_pInputHandler { nullptr };
        IResourceManager* m_pResourceManager { nullptr };
        ICameraManager* m_pCameraManager { nullptr };
        BalbinoScene::CScene* m_pCurrentScene { nullptr };
        FawnVision::CRenderer* m_pCurrentRenderer{ nullptr };
        int m_windowWidth;
        int m_windowHeight;
    };
}// namespace Balbino
