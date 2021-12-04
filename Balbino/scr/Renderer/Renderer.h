#pragma once

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

		CRenderer(const CRenderer&) = delete;
		CRenderer(CRenderer&&) = delete;
		CRenderer& operator=(const CRenderer&) = delete;
		CRenderer& operator=(CRenderer&&) = delete;

		void Setup(SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount);
		void Cleanup();
		void Draw();
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

		float m_aspectRation;
	};
}