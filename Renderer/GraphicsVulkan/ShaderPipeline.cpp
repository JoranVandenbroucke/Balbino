#include "ShaderPipeline.h"

#include "Device.h"
#include "Funtions.h"
#include "Shader.h"
#include "Instance.h"
#include "Swapchain.h"

auto operator<=>( uint32_t lhs, BalVulkan::EShaderType rhs )
{
    return lhs <=> static_cast<uint32_t>( rhs );
}

BalVulkan::CShaderPipeline::~CShaderPipeline()
{
    vkDestroyPipeline( GetDevice()->GetVkDevice(), m_pipeline, nullptr );
    vkDestroyPipelineLayout( GetDevice()->GetVkDevice(), m_pipelineLayout, nullptr );
    vkDestroyDescriptorSetLayout( GetDevice()->GetVkDevice(), m_descriptorSetLayout, nullptr );
}

void BalVulkan::CShaderPipeline::Initialize( uint16_t type, const std::vector<CShader*>& shaders, const CRenderPass& renderPass, const std::vector<EVertexComponent>& components, uint32_t blendAttachmentSize, const CSwapchain* pSwapchain, ECullMode cullModeFlag )
{
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages{};
    std::vector<VkDescriptorSetLayoutBinding>    setLayoutBindings{};
    std::vector<VkWriteDescriptorSet>            writeDescriptor{};
    std::vector<VkDescriptorBindingFlagsEXT>     bindingFlags;
    
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindingsLookup;
    std::unordered_map<uint32_t, VkDescriptorBindingFlagsEXT>  bindingFlagsLookup;
    std::unordered_map<std::string, uint32_t>                  resourcesLookup;
    
    VkCullModeFlags cullmode{ VK_CULL_MODE_FLAG_BITS_MAX_ENUM };
    switch ( cullModeFlag )
    {
        case ECullMode::Back:
            cullmode = VK_CULL_MODE_BACK_BIT;
            break;
        case ECullMode::Front:
            cullmode = VK_CULL_MODE_FRONT_BIT;
            break;
        case ECullMode::None:
            cullmode = VK_CULL_MODE_NONE;
            break;
        default:;
    }
    switch ( type )
    {
        case 0:
            shaderStages.emplace_back( VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0,
                                       VK_SHADER_STAGE_VERTEX_BIT, shaders[0]->GetShaderModule(), "main", nullptr );
            break;
        case 1:
            shaderStages.emplace_back( VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0,
                                       VK_SHADER_STAGE_VERTEX_BIT, shaders[0]->GetShaderModule(), "main", nullptr );
            shaderStages.emplace_back( VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0,
                                       VK_SHADER_STAGE_GEOMETRY_BIT, shaders[1]->GetShaderModule(), "main", nullptr );
            shaderStages.emplace_back( VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0,
                                       VK_SHADER_STAGE_FRAGMENT_BIT, shaders[2]->GetShaderModule(), "main", nullptr );
            break;
        case 2:
            shaderStages.emplace_back( VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0,
                                       VK_SHADER_STAGE_VERTEX_BIT, shaders[0]->GetShaderModule(), "main", nullptr );
            shaderStages.emplace_back( VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0,
                                       VK_SHADER_STAGE_FRAGMENT_BIT, shaders[1]->GetShaderModule(), "main", nullptr );
            break;
        default:
            break;
    }
    for ( uint32_t i{}; i < shaders.size(); ++i )
    {
        const auto resources{ shaders[i]->GetShaderResources() };
        for ( const auto& shaderResource : resources )
        {
            std::string key = shaderResource.name;
            
            // Since 'Input' and 'Output' resources can have the same name, we modify the key string
            if ( shaderResource.type == EShaderResourceType::Input || shaderResource.type == EShaderResourceType::Output )
            {
                key = std::to_string( shaderResource.stages ) + "_" + key;
            }
            
            const auto it{ m_shaderResources.find( key ) };
            
            if ( it != m_shaderResources.end())
            {
                // Append stage flags if resource already exists
                it->second.stages |= shaderResource.stages;
            }
            else
            {
                // Create a new entry in the map
                m_shaderResources.emplace( key, shaderResource );
            }
        }
    }
    // Sift through the map of name indexed shader resources
    // Seperate them into their respective sets
    for ( const auto& it : m_shaderResources )
    {
        const auto& shaderResource = it.second;
        
        // Find binding by set index in the map.
        auto it2 = m_shaderSets.find( shaderResource.set );
        
        if ( it2 != m_shaderSets.end())
        {
            // Add resource to the found set index
            it2->second.push_back( shaderResource );
        }
        else
        {
            // Create a new set index and with the first resource
            m_shaderSets.emplace( shaderResource.set, std::vector<SShaderResource>{ shaderResource } );
        }
    }
    
    for ( const auto& shaderSet : m_shaderSets )
    {
        for ( auto& resource : shaderSet.second )
        {
            // Skip shader resources whitout a binding point
            if ( resource.type == EShaderResourceType::Input ||
                 resource.type == EShaderResourceType::Output ||
                 resource.type == EShaderResourceType::PushConstant ||
                 resource.type == EShaderResourceType::SpecializationConstant )
            {
                continue;
            }
            
            // Convert from EShaderResourceType to VkDescriptorType.
            const auto descriptorType = FindDescriptorType( resource.type,
                                                            resource.mode == EShaderResourceMode::Dynamic );
            
            if ( resource.mode == EShaderResourceMode::UpdateAfterBind )
            {
                bindingFlags.push_back( VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT );
            }
            else
            {
                // When creating a descriptor set layout, if we give a structure to create_info.pNext, each binding needs to have a binding flag
                // (pBindings[i] uses the flags in pBindingFlags[i])
                // Adding 0 ensures the bindings that don`t use any flags are mapped correctly.
                bindingFlags.push_back( 0 );
            }
            
            auto existing = std::ranges::find_if( setLayoutBindings,
                                                  [ &resource ]( const VkDescriptorSetLayoutBinding& binding )
                                                  {
                                                      return binding.binding == resource.binding;
                                                  } );
            if ( existing != setLayoutBindings.end())
            {
                // Store mapping between binding and the binding point
                bindingsLookup.emplace( resource.binding, *existing );
                existing->stageFlags |= static_cast<VkShaderStageFlags>( resource.stages );
            }
            else
            {
                // Convert ShaderResource to VkDescriptorSetLayoutBinding
                VkDescriptorSetLayoutBinding layoutBinding{};
                
                layoutBinding.binding         = resource.binding;
                layoutBinding.descriptorCount = resource.arraySize;
                layoutBinding.descriptorType  = descriptorType;
                layoutBinding.stageFlags      = static_cast<VkShaderStageFlags>( resource.stages );
                
                setLayoutBindings.push_back( layoutBinding );
                
                // Store mapping between binding and the binding point
                bindingsLookup.emplace( resource.binding, layoutBinding );
            }
            
            bindingFlagsLookup.emplace( resource.binding, bindingFlags.back());
            
            resourcesLookup.emplace( resource.name, resource.binding );
        }
    }
    
    const VkDescriptorSetLayoutCreateInfo descriptorLayout{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount = static_cast<uint32_t>( setLayoutBindings.size()),
            .pBindings = setLayoutBindings.data(),
    };
    CheckVkResult( vkCreateDescriptorSetLayout( GetDevice()->GetVkDevice(), &descriptorLayout, nullptr,
                                                &m_descriptorSetLayout ));
    
    // Shared pipeline layout used by all pipelines
    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 1,
            .pSetLayouts = &m_descriptorSetLayout,
    };
    CheckVkResult( vkCreatePipelineLayout( GetDevice()->GetVkDevice(), &pPipelineLayoutCreateInfo, nullptr,
                                           &m_pipelineLayout ), "failed to create pipeline layout!" );
    
    
    constexpr VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext = VK_NULL_HANDLE,
            .flags = 0,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE,
    };
    const VkPipelineRasterizationStateCreateInfo     rasterizationState{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .depthClampEnable = VK_FALSE,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = cullmode,
            .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
            .depthBiasEnable = VK_FALSE,
            .depthBiasConstantFactor = 0,
            .depthBiasClamp = 0,
            .depthBiasSlopeFactor = 0,
            .lineWidth = 1.0f,
    };
    
    const std::vector                           blendAttachmentStates( blendAttachmentSize,
                                                                       VkPipelineColorBlendAttachmentState{ .blendEnable = VK_FALSE, .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT, } );
    const VkPipelineColorBlendStateCreateInfo   colorBlendState{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = static_cast<uint32_t>( blendAttachmentStates.size()),
            .pAttachments = blendAttachmentStates.data(),
            .blendConstants = { 0.f, 0.f, 0.f, 0.f },
    };
    const VkPipelineDepthStencilStateCreateInfo depthStencilState{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .depthTestEnable = VK_TRUE,
            .depthWriteEnable = VK_TRUE,
            .depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
            .depthBoundsTestEnable = VK_FALSE,
            .stencilTestEnable = VK_FALSE,
    };
    VkViewport                                  viewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>( pSwapchain->GetExtend().width ),
            .height = static_cast<float>( pSwapchain->GetExtend().height ),
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
    };
    VkRect2D                                    scissor{
            .offset = { 0, 0 },
            .extent = pSwapchain->GetExtend(),
    };
    const VkPipelineViewportStateCreateInfo     viewportState{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .flags = 0,
            .viewportCount = 1,
            .pViewports = &viewport,
            .scissorCount = 1,
            .pScissors = &scissor,
    };
    
    const VkPipelineMultisampleStateCreateInfo         multisampleState{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .flags = 0,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable = VK_FALSE,
    };
    const std::vector<VkVertexInputBindingDescription> vertexInputBindingDescription{
            SVertex::GetBindingDescription(),
            InstanceBatch::GetBindingDescription()
    };
    std::vector                                        vertexInputAttributeDescriptions{
            SVertex::GetAttributeDescriptions( 0, components )
    };
    std::vector                                        vertexInstanceInputAttributeDescriptions{ InstanceBatch::GetAttributeDescriptions() };
    vertexInputAttributeDescriptions.insert( vertexInputAttributeDescriptions.end(),
                                             vertexInstanceInputAttributeDescriptions.begin(),
                                             vertexInstanceInputAttributeDescriptions.end());
    const VkPipelineVertexInputStateCreateInfo inputState{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .vertexBindingDescriptionCount = (uint32_t) vertexInputBindingDescription.size(),
            .pVertexBindingDescriptions = vertexInputBindingDescription.data(),
            .vertexAttributeDescriptionCount = static_cast<uint32_t>( vertexInputAttributeDescriptions.size()),
            .pVertexAttributeDescriptions = vertexInputAttributeDescriptions.data(),
    };
    const VkGraphicsPipelineCreateInfo         pipelineCI{
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext = VK_NULL_HANDLE,
            .flags = 0,
            .stageCount = static_cast<uint32_t>( shaderStages.size()),
            .pStages = shaderStages.data(),
            .pVertexInputState = &inputState,
            .pInputAssemblyState = &inputAssemblyState,
            .pViewportState = &viewportState,
            .pRasterizationState = &rasterizationState,
            .pMultisampleState = &multisampleState,
            .pDepthStencilState = &depthStencilState,
            .pColorBlendState = &colorBlendState,
            .layout = m_pipelineLayout,
            .renderPass = renderPass.GetRenderPass(),
            .subpass = 0,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = 0,
    };
    
    CheckVkResult( vkCreateGraphicsPipelines( GetDevice()->GetVkDevice(), m_pipelineCache, 1, &pipelineCI, nullptr,
                                              &m_pipeline ), "failed to create graphics pipeline!" );
}

VkPipelineLayout BalVulkan::CShaderPipeline::GetPipelineLayout() const
{
    return m_pipelineLayout;
}

const VkDescriptorSetLayout& BalVulkan::CShaderPipeline::GetDescriptorSetLayout() const
{
    return m_descriptorSetLayout;
}

VkPipeline BalVulkan::CShaderPipeline::GetPipeline()
{
    return m_pipeline;
}

const std::unordered_map<std::string, BalVulkan::SShaderResource>& BalVulkan::CShaderPipeline::GetShaderResources() const
{
    return m_shaderResources;
}

BalVulkan::CShaderPipeline* BalVulkan::CShaderPipeline::CreateNew( const CDevice* pDevice )
{
    return new CShaderPipeline{ pDevice };
}

VkDescriptorType BalVulkan::CShaderPipeline::FindDescriptorType( EShaderResourceType resource_type, bool dynamic )
{
    switch ( resource_type )
    {
        case EShaderResourceType::InputAttachment:
            return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        case EShaderResourceType::Image:
            return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        case EShaderResourceType::ImageSampler:
            return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        case EShaderResourceType::ImageStorage:
            return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        case EShaderResourceType::Sampler:
            return VK_DESCRIPTOR_TYPE_SAMPLER;
        case EShaderResourceType::BufferUniform:
            if ( dynamic )
            {
                return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
            }
            return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case EShaderResourceType::BufferStorage:
            if ( dynamic )
            {
                return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
            }
            return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        default:
            throw std::runtime_error( "No conversion possible for the shader resource type." );
    }
}
