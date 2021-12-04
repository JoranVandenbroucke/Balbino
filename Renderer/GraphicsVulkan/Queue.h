#pragma once
#include "Base.h"
#include "CommandPool.h"
#include "Semaphore.h"

namespace BalVulkan
{
	class CSwapchain;
	class CSemaphore;
	class CCommandPool;
	class CFence;

	class CQueue final : public CDeviceObject
	{
	public:
		explicit CQueue( const CDevice* device);
		~CQueue() override;

		void Initialize();
		void SubmitPass( const CSemaphore* signalSemaphore, const CSemaphore* waitableSemaphores, const CCommandPool* cmdList, const CFence* pFence ) const;
		void PresentToScreen( const CSwapchain* pSwapchain, const CSemaphore* signalSemaphore, uint32_t imageIndex ) const;
		const VkQueue& GetQueue() const;
		uint32_t GetQueFamily()const;
		void WaitIdle() const;
		static CQueue* CreateNew( const CDevice* pDevice );

	private:
		uint32_t m_queueFamily;
		VkQueue m_queue;
	};
};

