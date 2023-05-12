#pragma once

#include "Base.h"
#include "RenderPass.h"
#include "Shader.h"
#include "Vertex.h"

namespace FawnVision
{
    class CShader;
    class CSwapchain;
    class CShaderPipeline final : public CDeviceObject
    {
    public:
        explicit CShaderPipeline( const CDevice* pDevice )
                : CDeviceObject{ pDevice }
                  , m_descriptorSetLayout{ VK_NULL_HANDLE }
                  , m_pipelineLayout{ VK_NULL_HANDLE }
                  , m_pipelineCache{ VK_NULL_HANDLE }
                  , m_pipeline{ VK_NULL_HANDLE }
        {
        }
        ~CShaderPipeline() override;
        
        void Initialize( uint32_t types, const std::vector<CShader*>& shaders, const CRenderPass& frameBuffer, const std::vector<EVertexComponent::Enum>& components, uint32_t blendAttachmentSize, CSwapchain* pSwapchain, ECullMode::Enum cullModeFlag = ECullMode::Back );
        
        VkPipelineLayout GetPipelineLayout() const;
        
        const VkDescriptorSetLayout& GetDescriptorSetLayout() const;
        
        VkPipeline GetPipeline();
        
        const std::vector<SShaderResource>& GetShaderResources() const;
        
    private:
        VkDescriptorSetLayout m_descriptorSetLayout;
        VkPipelineLayout      m_pipelineLayout;
        VkPipelineCache       m_pipelineCache;
        VkPipeline            m_pipeline;
        
        std::vector<SShaderResource>                               m_shaderResources;
        std::unordered_map<uint32_t, std::vector<SShaderResource>> m_shaderSets;
        
        static VkDescriptorType FindDescriptorType( EShaderResourceType::Enum resourceType, bool dynamic );
    };
}
