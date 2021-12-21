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
		void Setup( SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount, BalEditor::CInterface* pInterface );
#endif // BALBINO_EDITOR

		void Setup( SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount );
		void Cleanup();
		void StartDraw();
		void EndDraw();
		[[nodiscard]] float GetAspectRatio() const;

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
		BalVulkan::CBuffer* m_pModelBuffer;
		BalVulkan::CBuffer* m_pShadingBuffer;

#ifdef BALBINO_EDITOR
		BalEditor::CInterface* m_pInterface;
#endif

		int32_t m_width;
		int32_t m_height;
		float m_aspectRation;
		uint32_t m_imageIndex;
	};
}
