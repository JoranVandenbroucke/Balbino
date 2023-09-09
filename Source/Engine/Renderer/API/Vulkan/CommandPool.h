#pragma once
#include "Base.h"
#include <vulkan/vulkan.hpp>

namespace DeerVulkan
{
    class CCommandPool final : public CDeviceObject
    {
    public:
        constexpr explicit CCommandPool( FawnVision::Device device )
            : CDeviceObject { device }
        {
        }

        ~CCommandPool() override;

        void Initialize( uint32_t queueFamilyIndex, FawnVision::Swapchain& swapchain ) noexcept;
        void BeginRender( FawnVision::RenderPass& renderPass, FawnVision::FrameBuffer& frameBuffer, FawnVision::Swapchain& swapchain ) const;
        void NextSubPass() noexcept;
        void EndRender() const;
        void BindShader( FawnVision::ShaderPipeline shaderPipeline, FawnVision::DescriptorSet descriptorSet ) const;
        inline void UpdateFrameIndex() noexcept
        {
            m_currentFrameIndex = ++m_currentFrameIndex % m_commandBuffers.size();
        };
        constexpr uint32_t GetCommandBufferCount() const
        {
            return (uint32_t)m_commandBuffers.size();
        }
        constexpr uint32_t GetCurrentIndex() const
        {
            return m_currentFrameIndex;
        }
        constexpr const VkCommandPool& GetHandle() const
        {
            return m_commandPool;
        }
        constexpr const VkCommandBuffer& GetCommandBuffer() const
        {
            return m_commandBuffers[ m_currentFrameIndex ];
        }

    private:
        std::vector<VkCommandBuffer> m_commandBuffers;
        std::vector<FawnVision::ImageResource> m_imageResources;
        VkCommandPool m_commandPool { VK_NULL_HANDLE };
        uint32_t m_currentFrameIndex {};
    };
}// namespace DeerVulkan
