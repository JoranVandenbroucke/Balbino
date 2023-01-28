#pragma once

#include <SDL.h>

#include <vulkan/vulkan.hpp>

#include "ISystem.h"
#include "Swapchain.h"

struct IScene;
struct ISystem;

namespace FawnVision
{
    class CRenderPass;
    
    class CDevice;
    
    class CQueue;
    
    class CCommandPool;
    
    class CBuffer;
    
    class CImageResource;
    
    class CImageView;
    
    class CSampler;
}

struct SDL_Window;
struct ImNodesStyle;

namespace FawnForge
{
    class CSceneHierarchy;
    
    class CAssetBrowser;
    
    class CGameView;
    
    class CMainScreen;
    
    class CMaterialEditor;
    
    class CShaderGraph;
    
    class CMeshFileImporter;
    
    class CTextureFileImporter;
    
    class CPropertyPanel;
    
    class CInterface final
    {
    public:
        CInterface();
        
        ~CInterface() = default;
        CInterface( const CInterface& ) = delete;
        CInterface( CInterface&& ) = delete;
        CInterface& operator=( const CInterface& ) = delete;
        CInterface& operator=( CInterface&& ) = delete;
        
        void Initialize( SDL_Window* pWindow, const int32_t w, const int32_t h, const FawnVision::CDevice* pDevice, const FawnVision::CQueue* pQueue, const FawnVision::CCommandPool* pCommandPool, const FawnVision::CRenderPass* pRenderPass, const FawnVision::CSwapchain* pSwapchain, ISystem* pSystem );
        void Draw( FawnVision::CCommandPool* pCommandPool );
        void Cleanup() const;
        void ProcessEvent( SDL_Event e );
        void SetContext( IScene* pScene, ISystem* pSystem );
        void Resize( const int32_t w, const int32_t h );
    
    private:
        bool m_queueNextResource;
        
        CMainScreen         * m_pMain;
        CGameView           * m_pGameView;
        CAssetBrowser       * m_pAssetBrowser;
        CSceneHierarchy     * m_pSceneHierarchy;
        CShaderGraph        * m_pShaderGraph;
        CMaterialEditor     * m_pMaterialEditor;
        CMeshFileImporter   * m_pMeshImporter;
        CTextureFileImporter* m_pTextureImporter;
        CPropertyPanel      * m_pPropertyPanel;
        
        VkDescriptorPool m_descriptorPool;
        SDL_Window* m_pWindow;
        
        const FawnVision::CDevice* m_pDevice;
        
        std::vector<std::string> m_pendingResources;
        
        static void SetImGuiStyle( ImNodesStyle& imNodesStyle );
    };
}
