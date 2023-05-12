#pragma once
#include <vulkan/vulkan.hpp>
#include "Base.h"


namespace FawnVision
{
    class CSwapchain;
    class CRenderPass;
    class CFrameBuffer;
    class CShaderPipeline;
    class CDescriptorSet;
    class CImageResource;
    
    class CCommandPool final : public CDeviceObject
    {
    public:
        explicit CCommandPool( const CDevice* pDevice );
        ~CCommandPool() override;
        
        void Initialize( uint32_t queueFamilyIndex, CSwapchain* swapchain );
        void UpdateFrameIndex();
        void BeginRender( CRenderPass* renderPass, CFrameBuffer* frameBuffer, CSwapchain* swapchain ) const;
        void NextSubPass();
        void BindShader( CShaderPipeline* pipeline, CDescriptorSet* descriptorSet ) const;
        uint32_t GetCommandBufferCount() const;
        uint32_t GetCurrentIndex() const;
        
        const VkCommandPool& GetHandle() const;
        const VkCommandBuffer& GetCommandBuffer() const;
        void EndRender() const;
        
    private:
        VkCommandPool                m_commandPool;
        std::vector<VkCommandBuffer> m_commandBuffers;
        std::vector<CImageResource*> m_imageResources;
        uint32_t                     m_currentFrameIndex;
    };
}
