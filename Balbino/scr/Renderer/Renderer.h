#pragma once

#ifdef BALBINO_EDITOR
#include <scr/Interface.h>

namespace BalEditor
{
	class CInterface;
}
#endif

namespace BalVulkan
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
		~CRenderer();

		CRenderer( const CRenderer& ) = delete;
		CRenderer( CRenderer&& ) = delete;
		CRenderer& operator=( const CRenderer& ) = delete;
		CRenderer& operator=( CRenderer&& ) = delete;

#ifdef BALBINO_EDITOR
		void Setup( SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount, BalEditor::CInterface* pInterface, ISystem* pSystem );
#endif // BALBINO_EDITOR

		void Setup( SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount );
		void Cleanup();
		bool StartDraw();
        bool NextSubPass();
		bool EndDraw();
		[[nodiscard]] float GetAspectRatio() const;
		void GiveSceneRenderData( CScene* pScene);

	private:
		BalVulkan::CInstance* m_pInstance;
		BalVulkan::CDevice* m_pDevice;
		BalVulkan::CSwapchain* m_pSwapchain;
		BalVulkan::CFrameBuffer* m_pFrameBuffer;

		BalVulkan::CQueue* m_pQueue;
		BalVulkan::CCommandPool* m_pCommandPool;
		BalVulkan::CSemaphore* m_pSignalingSemaphore;
		BalVulkan::CSemaphore* m_pWaitingSemaphore;
		std::vector<BalVulkan::CFence*> m_pFences;
		std::vector<BalVulkan::CFence*> m_pInFlightFences;

#ifdef BALBINO_EDITOR
		BalEditor::CInterface* m_pInterface;
#endif

		int32_t m_width;
		int32_t m_height;
		uint32_t m_imageIndex;
		float m_aspectRation;

		SDL_Window* m_pWindow;
		CScene* m_pScene;
		void RecreateSwapChain();
	};
}
