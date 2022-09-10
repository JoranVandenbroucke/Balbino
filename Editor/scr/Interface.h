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

	class CInterface final
	{
	public:
		CInterface();
		~CInterface() = default;
		CInterface( const CInterface& ) = delete;
		CInterface( CInterface&& ) = delete;
		CInterface& operator=( const CInterface& ) = delete;
		CInterface& operator=( CInterface&& ) = delete;

		void Initialize( SDL_Window* pWindow, const int32_t w, const int32_t h, const BalVulkan::CDevice* pDevice, const BalVulkan::CQueue* pQueue, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CFrameBuffer* pFrameBuffer, const BalVulkan::CSwapchain* pSwapchain, ISystem* pSystem );
		void Draw( BalVulkan::CCommandPool* pCommandPool );
		void Cleanup() const;
		void ProcessEvent( SDL_Event e ) const;
		void SetContext( IScene* pScene );
		void Resize( VkInstance instance, VkQueue queue );
	private:
		CMainScreen* m_pMain;
		CGameView* m_pGameView;
		CAssetBrowser* m_pAssetBrowser;
		CSceneHierarchy* m_pSceneHierarchy;
		CShaderGraph* m_pShaderGraph;
		CMaterialEditor* m_pMaterialEditor;
        CMeshFileImporter* m_pMeshImporter;

		VkDescriptorPool m_descriptorPool;
		SDL_Window* m_pWindow;

		int m_vertexCount;
		int m_indexCount;
		IScene* m_pContext;
		const BalVulkan::CDevice* m_pDevice;

		static void SetImGuiStyle();
	};
}
