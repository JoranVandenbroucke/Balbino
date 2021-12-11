#pragma once
#include <SDL.h>

#include <vulkan/vulkan.hpp>

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
	class CAssetBrowser;
	class CGameView;
	class CMainScreen;

	class CInterface final
	{
	public:
		CInterface();
		~CInterface() = default;
		CInterface( const CInterface& ) = delete;
		CInterface( CInterface&& ) = delete;
		CInterface& operator=( const CInterface& ) = delete;
		CInterface& operator=( CInterface&& ) = delete;

		void Initialize( SDL_Window* pWindow, const int32_t w, const int32_t h, const BalVulkan::CDevice* pDevice, const BalVulkan::CQueue* pQueue, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CFrameBuffer* pFrameBuffer );
		void Draw( BalVulkan::CCommandPool* pCommandPool );
		void Cleanup() const;
		void ProcessEvent( SDL_Event e )const;
	private:
		CMainScreen* m_pMain;
		CGameView* m_pGameView;
		CAssetBrowser* m_pAssetBrowser;

		VkDescriptorPool m_descriptorPool;
		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorSet m_descriptorSet;
		VkPipelineCache m_pipelineCache;
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_pipeline;
		VkShaderModule m_shaderModuleVert;
		VkShaderModule m_shaderModuleFrag;
		const BalVulkan::CDevice* m_pDevice;
		BalVulkan::CImageResource* m_pResource;
		BalVulkan::CImageView* m_pView;
		BalVulkan::CSampler* m_pSampler;
		BalVulkan::CBuffer* m_pVertexBuffer;
		BalVulkan::CBuffer* m_pIndexBuffer;
		SDL_Window* m_pWindow;

		int m_vertexCount;
		int m_indexCount;

		void UpdateBuffers();
		void FrameRender( BalVulkan::CCommandPool* pCommandPool ) const;
		static void SetImGuiStyle();
	};
}
