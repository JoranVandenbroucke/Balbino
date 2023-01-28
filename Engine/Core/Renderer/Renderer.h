#pragma once

#ifdef BALBINO_EDITOR
#include <scr/Interface.h>

namespace FawnForge
{
    class CInterface;
}
#endif

namespace FawnVision
{
    class CBuffer;
    
    class CFence;
    
    class CSemaphore;
    
    class CQueue;
    
    class CDevice;
    
    class CShaderPipeline;
    
    class CShader;
    
    class CFrameBuffer;
    
    class CSwapchain;
    
    class CInstance;
    
    class CRenderPass;
    
    class CImageResource;
    
    class CImageView;
    
    class CCommandPool;
}

struct SDL_Window;

namespace Balbino
{
    class CScene;
    
    class CInterface;
    
    class CRenderer
    {
    public:
        CRenderer();
        ~CRenderer() = default;
        
        CRenderer( const CRenderer& ) = delete;
        CRenderer( CRenderer&& ) = delete;
        CRenderer& operator=( const CRenderer& ) = delete;
        CRenderer& operator=( CRenderer&& ) = delete;
        
        #ifdef BALBINO_EDITOR
        void Setup( SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount, FawnForge::CInterface* pInterface, ISystem* pSystem );
        #endif // BALBINO_EDITOR
        
        void Setup( SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount );
        void Cleanup();
        bool StartDraw();
        bool NextSubPass();
        bool EndDraw();
        [[nodiscard]] float GetAspectRatio() const;
        void GiveSceneRenderData( CScene* pScene );
    
    private:
        FawnVision::CInstance   * m_pInstance;
        FawnVision::CDevice     * m_pDevice;
        FawnVision::CSwapchain  * m_pSwapchain;
        FawnVision::CRenderPass * m_pRenderPass;
        FawnVision::CFrameBuffer* m_pFrameBuffer;
        
        FawnVision::CQueue      * m_pQueue;
        FawnVision::CCommandPool* m_pCommandPool;
        FawnVision::CSemaphore  * m_pSignalingSemaphore;
        FawnVision::CSemaphore  * m_pWaitingSemaphore;
        std::vector<FawnVision::CFence*>         m_pFences;
        std::vector<FawnVision::CFence*>         m_pInFlightFences;
        std::vector<FawnVision::CImageResource*> m_swapchainResources;
        std::vector<FawnVision::CImageView*>     m_swapchainViews;
        FawnVision::CImageResource* m_pDepthImage;
        FawnVision::CImageView    * m_pDepthImageView;
        
        #ifdef BALBINO_EDITOR
        FawnForge::CInterface* m_pInterface;
        bool m_firstFramePassed;
        #endif
        
        int32_t  m_width;
        int32_t  m_height;
        uint32_t m_imageIndex;
        float    m_aspectRation;
        
        SDL_Window* m_pWindow;
        CScene    * m_pScene;
        void RecreateSwapChain();
    };
}
