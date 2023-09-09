#include "ShaderPipeline.h"

#include "Device.h"

#include "Shader.h"
#include "Swapchain.h"

DeerVulkan::CShaderPipeline::~CShaderPipeline()
{
    vkDestroyPipelineCache( GetDevice()->GetVkDevice(), m_pipelineCache, VK_NULL_HANDLE );
    vkDestroyPipeline( GetDevice()->GetVkDevice(), m_pipeline, VK_NULL_HANDLE );
    vkDestroyPipelineLayout( GetDevice()->GetVkDevice(), m_pipelineLayout, VK_NULL_HANDLE );
    vkDestroyDescriptorSetLayout( GetDevice()->GetVkDevice(), m_descriptorSetLayout, VK_NULL_HANDLE );
};
void DeerVulkan::CShaderPipeline::Initialize( const FawnVision::SShaderPassCreateInfo& createInfo, const std::vector<FawnVision::Shader>& shaders, FawnVision::RenderPass renderPass, FawnVision::Swapchain pSwapchain )
{
    pSwapchain->AddRef();

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages {};
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings {};
    std::vector<VkWriteDescriptorSet> writeDescriptor {};
    std::vector<VkDescriptorBindingFlagsEXT> bindingFlags;

    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindingsLookup;
    std::unordered_map<uint32_t, VkDescriptorBindingFlagsEXT> bindingFlagsLookup;
    std::unordered_map<std::string, uint32_t> resourcesLookup;

    VkCullModeFlags cullMode { VK_CULL_MODE_FLAG_BITS_MAX_ENUM };
    switch ( (FawnVision::cull_mode)createInfo.cullModeFlag )
    {
        case FawnVision::cull_mode::back: cullMode = VK_CULL_MODE_BACK_BIT; break;
        case FawnVision::cull_mode::front: cullMode = VK_CULL_MODE_FRONT_BIT; break;
        case FawnVision::cull_mode::none: cullMode = VK_CULL_MODE_NONE; break;
        default: break;
    }

    int j {};

    for ( int32_t i { FawnVision::shader_stage_vertex }; i < FawnVision::shader_stage_max; i <<= 1 )
    {
        if ( ( createInfo.types & i ) == 0 )
        {
            continue;
        }
        VkShaderStageFlagBits type;
        switch ( FawnVision::shader_stage( createInfo.types & i ) )
        {
            case FawnVision::shader_stage_vertex: type = VK_SHADER_STAGE_VERTEX_BIT; break;
            case FawnVision::shader_stage_tessellation_control: type = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT; break;
            case FawnVision::shader_stage_tessellation_evaluation: type = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT; break;
            case FawnVision::shader_stage_geometry: type = VK_SHADER_STAGE_GEOMETRY_BIT; break;
            case FawnVision::shader_stage_fragment: type = VK_SHADER_STAGE_FRAGMENT_BIT; break;
            case FawnVision::shader_stage_compute: type = VK_SHADER_STAGE_COMPUTE_BIT; break;
            case FawnVision::shader_stage_raygen: type = VK_SHADER_STAGE_RAYGEN_BIT_KHR; break;
            case FawnVision::shader_stage_any_hit: type = VK_SHADER_STAGE_ANY_HIT_BIT_KHR; break;
            case FawnVision::shader_stage_closest_hit: type = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR; break;
            case FawnVision::shader_stage_miss: type = VK_SHADER_STAGE_MISS_BIT_KHR; break;
            case FawnVision::shader_stage_intersection: type = VK_SHADER_STAGE_INTERSECTION_BIT_KHR; break;
            case FawnVision::shader_stage_callable: type = VK_SHADER_STAGE_CALLABLE_BIT_KHR; break;
            case FawnVision::shader_stage_task: type = VK_SHADER_STAGE_TASK_BIT_NV; break;
            case FawnVision::shader_stage_mesh: type = VK_SHADER_STAGE_MESH_BIT_NV; break;
            default: break;
        }
        shaderStages.emplace_back( VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, VK_NULL_HANDLE, 0, type, shaders[ j ]->GetShaderModule(), "main", VK_NULL_HANDLE );
        ++j;
    }
    for ( uint32_t i {}; i < shaders.size(); ++i )
    {
        const auto resources { shaders[ i ]->GetShaderResources() };
        for ( const auto& shaderResource : resources )
        {
            const auto it { std::ranges::find_if( m_shaderResources, [ &shaderResource ]( const FawnVision::SShaderResource& resource ) {
                if ( ( shaderResource.type == (uint8_t)FawnVision::shader_resource_type::input && resource.type == (uint8_t)FawnVision::shader_resource_type::input )
                     || ( shaderResource.type == (uint8_t)FawnVision::shader_resource_type::output && resource.type == (uint8_t)FawnVision::shader_resource_type::output ) )
                {
                    return false;
                }
                return shaderResource.name == resource.name;
            } ) };

            if ( it != m_shaderResources.end() )
            {
                // Append stage flags if resource already exists
                it->stages |= shaderResource.stages;
            }
            else
            {
                // Create a new entry in the map
                m_shaderResources.emplace_back( shaderResource );
            }
        }
    }
    // Sift through the map of name indexed shader resources
    // constexpr Seperate them into their respective sets
    for ( const auto& it : m_shaderResources )
    {
        const auto& shaderResource = it;

        // Find binding by set index in the map.
        auto it2 = m_shaderSets.find( shaderResource.set );

        if ( it2 != m_shaderSets.end() )
        {
            // Add resource to the found set index
            it2->second.push_back( shaderResource );
        }
        else
        {
            // Create a new set index and with the first resource
            m_shaderSets.try_emplace( shaderResource.set, std::vector<FawnVision::SShaderResource> { shaderResource } );
        }
    }

    for ( const auto& [ id, resouces ] : m_shaderSets )
    {
        for ( auto& resource : resouces )
        {
            // Skip shader resources whitout a binding point
            if ( resource.type == (uint8_t)FawnVision::shader_resource_type::input || resource.type == (uint8_t)FawnVision::shader_resource_type::output || resource.type == (uint8_t)FawnVision::shader_resource_type::push_constant
                 || resource.type == (uint8_t)FawnVision::shader_resource_type::specialization_constant )
            {
                continue;
            }

            // Convert from shader_resource_type to VkDescriptorType.
            const auto descriptorType = FindDescriptorType( (FawnVision::shader_resource_type)resource.type, resource.mode == (uint8_t)FawnVision::shader_resource_mode::dynamic_mode );

            if ( resource.mode == (uint8_t)FawnVision::shader_resource_mode::update_after_bind_mode )
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

            auto existing = std::ranges::find_if( setLayoutBindings, [ &resource ]( const VkDescriptorSetLayoutBinding& binding ) constexpr {
                return binding.binding == resource.binding;
            } );
            if ( existing != setLayoutBindings.end() )
            {
                // Store mapping between binding and the binding point
                bindingsLookup.try_emplace( resource.binding, *existing );
                existing->stageFlags |= resource.stages;
            }
            else
            {
                // Convert ShaderResource to VkDescriptorSetLayoutBinding
                VkDescriptorSetLayoutBinding layoutBinding {};

                layoutBinding.binding         = resource.binding;
                layoutBinding.descriptorCount = resource.arraySize;
                layoutBinding.descriptorType  = descriptorType;
                layoutBinding.stageFlags      = resource.stages;

                setLayoutBindings.push_back( layoutBinding );

                // Store mapping between binding and the binding point
                bindingsLookup.try_emplace( resource.binding, layoutBinding );
            }

            bindingFlagsLookup.try_emplace( resource.binding, bindingFlags.back() );

            resourcesLookup.try_emplace( resource.name, resource.binding );
        }
    }

    const VkDescriptorSetLayoutCreateInfo descriptorLayout {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<uint32_t>( setLayoutBindings.size() ),
        .pBindings    = setLayoutBindings.data(),
    };
    CheckVkResult( vkCreateDescriptorSetLayout( GetDevice()->GetVkDevice(), &descriptorLayout, VK_NULL_HANDLE, &m_descriptorSetLayout ) );

    // Shared pipeline layout used by all pipelines
    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo {
        .sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 1,
        .pSetLayouts    = &m_descriptorSetLayout,
    };
    CheckVkResult( vkCreatePipelineLayout( GetDevice()->GetVkDevice(), &pPipelineLayoutCreateInfo, VK_NULL_HANDLE, &m_pipelineLayout ), "failed to create pipeline layout!" );

    constexpr VkPipelineInputAssemblyStateCreateInfo inputAssemblyState {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext                  = VK_NULL_HANDLE,
        .flags                  = 0,
        .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    };
    const VkPipelineRasterizationStateCreateInfo rasterizationState {
        .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext                   = VK_NULL_HANDLE,
        .flags                   = 0,
        .depthClampEnable        = VK_FALSE,
        .polygonMode             = VK_POLYGON_MODE_FILL,
        .cullMode                = cullMode,
        .frontFace               = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable         = VK_FALSE,
        .depthBiasConstantFactor = 0,
        .depthBiasClamp          = 0,
        .depthBiasSlopeFactor    = 0,
        .lineWidth               = 1.0f,
    };

    const std::vector blendAttachmentStates { createInfo.blendAttachmentSize,
                                              VkPipelineColorBlendAttachmentState {
                                                  .blendEnable    = VK_FALSE,
                                                  .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
                                              } };
    const VkPipelineColorBlendStateCreateInfo colorBlendState {
        .sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable   = VK_FALSE,
        .logicOp         = VK_LOGIC_OP_COPY,
        .attachmentCount = static_cast<uint32_t>( blendAttachmentStates.size() ),
        .pAttachments    = blendAttachmentStates.data(),
        .blendConstants  = { 0.f, 0.f, 0.f, 0.f },
    };
    const VkPipelineDepthStencilStateCreateInfo depthStencilState {
        .sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable       = VK_TRUE,
        .depthWriteEnable      = VK_TRUE,
        .depthCompareOp        = VK_COMPARE_OP_LESS_OR_EQUAL,
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable     = VK_FALSE,
    };

    VkViewport viewport {
        .x        = 0.0f,
        .y        = 0.0f,
        .width    = static_cast<float>( pSwapchain->GetExtend().width ),
        .height   = static_cast<float>( pSwapchain->GetExtend().height ),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };

    VkRect2D scissor {
        .offset = { 0, 0 },
        .extent = pSwapchain->GetExtend(),
    };

    const VkPipelineViewportStateCreateInfo viewportState {
        .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .flags         = 0,
        .viewportCount = 1,
        .pViewports    = &viewport,
        .scissorCount  = 1,
        .pScissors     = &scissor,
    };

    const VkPipelineMultisampleStateCreateInfo multisampleState {
        .sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .flags                = 0,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable  = VK_FALSE,
    };

    const std::vector<VkVertexInputBindingDescription> vertexInputBindingDescription { FawnVision::GetBindingDescription<FawnVision::SVertex>(), FawnVision::GetBindingDescription<FawnVision::SInstanceBatch>() };

    std::vector vertexInputAttributeDescriptions { FawnVision::GetAttributeDescriptions<FawnVision::SVertex>( 0, (FawnVision::vertex_component_flags)createInfo.components ) };

    std::vector vertexInstanceInputAttributeDescriptions { FawnVision::GetAttributeDescriptions<FawnVision::SInstanceBatch>() };
    vertexInputAttributeDescriptions.insert( vertexInputAttributeDescriptions.end(), vertexInstanceInputAttributeDescriptions.begin(), vertexInstanceInputAttributeDescriptions.end() );
    const VkPipelineVertexInputStateCreateInfo inputState {
        .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext                           = VK_NULL_HANDLE,
        .flags                           = 0,
        .vertexBindingDescriptionCount   = (uint32_t)vertexInputBindingDescription.size(),
        .pVertexBindingDescriptions      = vertexInputBindingDescription.data(),
        .vertexAttributeDescriptionCount = static_cast<uint32_t>( vertexInputAttributeDescriptions.size() ),
        .pVertexAttributeDescriptions    = vertexInputAttributeDescriptions.data(),
    };
    const VkGraphicsPipelineCreateInfo pipelineCi {
        .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext               = VK_NULL_HANDLE,
        .flags               = 0,
        .stageCount          = static_cast<uint32_t>( shaderStages.size() ),
        .pStages             = shaderStages.data(),
        .pVertexInputState   = &inputState,
        .pInputAssemblyState = &inputAssemblyState,
        .pViewportState      = &viewportState,
        .pRasterizationState = &rasterizationState,
        .pMultisampleState   = &multisampleState,
        .pDepthStencilState  = &depthStencilState,
        .pColorBlendState    = &colorBlendState,
        .layout              = m_pipelineLayout,
        .renderPass          = renderPass->GetHandle(),
        .subpass             = 0,
        .basePipelineHandle  = VK_NULL_HANDLE,
        .basePipelineIndex   = 0,
    };

    CheckVkResult( vkCreateGraphicsPipelines( GetDevice()->GetVkDevice(), m_pipelineCache, 1, &pipelineCi, VK_NULL_HANDLE, &m_pipeline ), "failed to create graphics pipeline!" );
    pSwapchain->Release();
}

constexpr VkDescriptorType DeerVulkan::CShaderPipeline::FindDescriptorType( FawnVision::shader_resource_type resourceType, bool dynamic )
{
    switch ( resourceType )
    {
        case FawnVision::shader_resource_type::input_attachment: return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        case FawnVision::shader_resource_type::image: return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        case FawnVision::shader_resource_type::image_sampler: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        case FawnVision::shader_resource_type::image_storage: return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        case FawnVision::shader_resource_type::sampler: return VK_DESCRIPTOR_TYPE_SAMPLER;
        case FawnVision::shader_resource_type::buffer_uniform:
            if ( dynamic )
            {
                return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
            }
            return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case FawnVision::shader_resource_type::buffer_storage:
            if ( dynamic )
            {
                return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
            }
            return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        default: throw std::invalid_argument( R"('resourceType' in 'FindDescriptorType( shader_resource_type resourceType, bool dynamic_mode )' has no possible convestion set up.)" );
    }
}
