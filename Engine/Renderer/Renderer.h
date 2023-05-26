#pragma once
#include "API/Vulkan/Buffer.h"
#include "API/Vulkan/DescriptorSet.h"
#include "API/Vulkan/ImageResource.h"
#include "API/Vulkan/ImageView.h"
#include "API/Vulkan/Sampler.h"

#ifdef BALBINO_EDITOR
namespace FawnForge
{
    class CInterface;
}
struct ISystem;
#endif

struct SDL_Window;
struct IScene;

namespace FawnVision
{
    class CCommandPool;
    class CDevice;
    class CFence;
    class CFrameBuffer;
    class CImageResource;
    class CImageView;
    class CInstance;
    class CQueue;
    class CRenderPass;
    class CSemaphore;
    class CShader;
    class CShaderPipeline;
    class CSwapchain;
    
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
        void GiveSceneRenderData( IScene* pScene );
        
        [[nodiscard]] inline constexpr FawnVision::CDevice* GetDevice()
        {
            return m_pDevice;
        }
        [[nodiscard]] inline constexpr FawnVision::CDevice* GetDevice() const
        {
            return m_pDevice;
        }
        [[nodiscard]] inline constexpr FawnVision::CQueue* GetQueue()
        {
            return m_pQueue;
        }
        [[nodiscard]] inline constexpr FawnVision::CQueue* GetQueue() const
        {
            return m_pQueue;
        }
        [[nodiscard]] inline constexpr FawnVision::CSwapchain* GetSwapChain()
        {
            return m_pSwapchain;
        }
        [[nodiscard]] inline constexpr FawnVision::CSwapchain* GetSwatChain() const
        {
            return m_pSwapchain;
        }
        [[nodiscard]] inline constexpr FawnVision::CRenderPass* GetRenderPass()
        {
            return m_pRenderPass;
        }
        [[nodiscard]] inline constexpr FawnVision::CRenderPass* GetRenderPass() const
        {
            return m_pRenderPass;
        }
        [[nodiscard]] inline constexpr FawnVision::CCommandPool* GetCommandPool()
        {
            return m_pCommandPool;
        }
        [[nodiscard]] inline constexpr FawnVision::CCommandPool* GetCommandPool() const
        {
            return m_pCommandPool;
        }
        
        
        // Factory function to create a CBuffer object
        [[nodiscard]] inline CBuffer* CreateBuffer( uint64_t size, EBufferUsageFlagBits::Enum bufferUsage, EMemoryPropertyFlagBits::Enum memoryProperty )
        {
            auto buffer = new CBuffer( m_pDevice, m_pCommandPool, m_pQueue );
            buffer->Initialize( size, bufferUsage, memoryProperty );
            return buffer;
        }
        
        // Factory function to create a CImage object
        [[nodiscard]] inline CImageResource* CreateImage( FawnVision::EImageViewType::Enum type, FawnVision::EFormat::Enum format, uint32_t width, uint32_t height, uint32_t depth, uint8_t mips, uint8_t layers, int sampleLevel, EImageUsageFlagBits::Enum usage, FawnVision::EImageLayout::Enum layout = FawnVision::EImageLayout::Undefined )
        {
            auto image = new CImageResource{ m_pDevice };
            image->Initialize( type, format, width, height, depth, mips, layers, sampleLevel, usage, layout );
            return image;
        }
        
        // Factory function to create a CImageView object
        [[nodiscard]] inline CImageView* CreateImageView( const CImageResource& pImage, EImageViewType::Enum type, uint32_t firstMip = 0, uint32_t numMips = VK_REMAINING_MIP_LEVELS, uint32_t firstLayer = 0, uint32_t numLayers = VK_REMAINING_ARRAY_LAYERS)
        {
            auto imageView = new CImageView{ pImage, type, firstMip, numMips, firstLayer, numLayers };
            return imageView;
        }
        
        // Factory function to create a CImageSampler object
        [[nodiscard]] inline CSampler* CreateImageSampler( int filterMode, int mipmapMode, ESamplerAddressMode::Enum samplerAddressModeU, ESamplerAddressMode::Enum samplerAddressModeV, ESamplerAddressMode::Enum samplerAddressModeW, int anisotropy, uint32_t mipLevels )
        {
            auto imageSampler = new CSampler{ m_pDevice };
            imageSampler->Initialize(
                    filterMode,
                    mipmapMode,
                    samplerAddressModeU,
                    samplerAddressModeV,
                    samplerAddressModeW,
                    anisotropy,
                    mipLevels
            );
            return imageSampler;
        }
    
    private:
        FawnVision::CInstance   * m_pInstance;
        FawnVision::CDevice     * m_pDevice;
        FawnVision::CSwapchain  * m_pSwapchain;
        FawnVision::CRenderPass * m_pRenderPass{};
        FawnVision::CFrameBuffer* m_pFrameBuffer;
        
        FawnVision::CQueue      * m_pQueue;
        FawnVision::CCommandPool* m_pCommandPool;
        FawnVision::CSemaphore  * m_pSignalingSemaphore;
        FawnVision::CSemaphore  * m_pWaitingSemaphore;
        std::vector<FawnVision::CFence*>         m_pFences;
        std::vector<FawnVision::CFence*>         m_pInFlightFences;
        std::vector<FawnVision::CImageResource*> m_swapchainResources;
        std::vector<FawnVision::CImageView*>     m_swapchainViews;
        FawnVision::CImageResource* m_pDepthImage{};
        FawnVision::CImageView    * m_pDepthImageView{};
        
        #ifdef BALBINO_EDITOR
        FawnForge::CInterface* m_pInterface;
        bool m_firstFramePassed{};
        #endif
        
        int32_t  m_width;
        int32_t  m_height;
        uint32_t m_imageIndex;
        float    m_aspectRation;
        
        SDL_Window* m_pWindow;
        IScene    * m_pScene;
        void RecreateSwapChain();
    };
}
