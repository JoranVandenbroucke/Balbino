#pragma once

#include <SDL.h>

#include <vulkan/vulkan.hpp>

#include "IManager.h"
#include "Swapchain.h"

struct IScene;

namespace BalVulkan
{
    class CFrameBuffer;

    class CDevice;

    class CQueue;

    class CCommandPool;

    class CBuffer;

    class CImageResource;

    class CImageView;

    class CSampler;
}

struct SDL_Window;

namespace BalEditor
{
    class CSceneHierarchy;

    class CAssetBrowser;

    class CGameView;

    class CMainScreen;

    class CMaterialEditor;

    class CShaderGraph;

    class CMeshFileImporter;

    class CTextureFileImporter;

    class CInterface final
    {
    public:
        CInterface();

        ~CInterface() = default;

        CInterface( const CInterface& ) = delete;

        CInterface( CInterface&& ) = delete;

        CInterface& operator =( const CInterface& ) = delete;

        CInterface& operator =( CInterface&& ) = delete;

        void Initialize( SDL_Window* pWindow, const int32_t w, const int32_t h, const BalVulkan::CDevice* pDevice, const BalVulkan::CQueue* pQueue, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CFrameBuffer* pFrameBuffer, const BalVulkan::CSwapchain* pSwapchain, ISystem* pSystem );

        void Draw( BalVulkan::CCommandPool* pCommandPool );

        void Cleanup() const;

        void ProcessEvent( SDL_Event e );

        void SetContext( IScene* pScene );

        void Resize( VkInstance instance, VkQueue queue );

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

        VkDescriptorPool m_descriptorPool;
        SDL_Window* m_pWindow;

        const BalVulkan::CDevice* m_pDevice;

        std::vector<std::string> m_pendingResources;

        static void SetImGuiStyle();
    };
}
