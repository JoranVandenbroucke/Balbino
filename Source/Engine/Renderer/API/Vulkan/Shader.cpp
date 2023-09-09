#include "Shader.h"

#include <fstream>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_reflect.hpp>

#include "Device.h"

DeerVulkan::CShader::~CShader()
{
    vkDestroyShaderModule( GetDevice()->GetVkDevice(), m_shaderHandle, VK_NULL_HANDLE );
    m_resources.clear();
    m_vertexComponents = 0;
}

void DeerVulkan::CShader::Initialize( const FawnVision::SShaderCreateInfo& createInfo )
{
    VkShaderStageFlagBits stageBits { ConvertShaderStage( createInfo.type ) };

    std::vector<uint32_t> sprv;
    sprv.assign( createInfo.pData, createInfo.pData + createInfo.size );

    spirv_cross::CompilerReflection reflection { sprv };
    const spirv_cross::ShaderResources& resources { reflection.get_shader_resources() };

    ExtractSampledImages( stageBits, reflection, resources );
    ExtractSeparateImages( stageBits, reflection, resources );
    ExtractSeparateSamplers( stageBits, reflection, resources );
    ExtractStageInputs( stageBits, reflection, resources );
    ExtractStageOutputs( stageBits, reflection, resources );
    ExtractStorageBuffers( stageBits, reflection, resources );
    ExtractStorageItems( stageBits, reflection, resources );
    ExtractSubpassInputs( stageBits, reflection, resources );
    ExtractUniformBuffers( stageBits, reflection, resources );

    m_shaderHandle = CreateShaderModule( sprv );
}
void DeerVulkan::CShader::ExtractUniformBuffers( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept
{
    for ( const auto& uniformBuffer : resources.uniform_buffers )
    {
        const auto& bufferType { reflection.get_type( uniformBuffer.base_type_id ) };
        const uint32_t bufferSize { static_cast<uint32_t>( reflection.get_declared_struct_size( bufferType ) ) };
        const uint32_t set { reflection.get_decoration( uniformBuffer.id, spv::DecorationDescriptorSet ) };
        const uint32_t binding { reflection.get_decoration( uniformBuffer.id, spv::DecorationBinding ) };
        const auto& spirvType = reflection.get_type_from_variable( uniformBuffer.id );

        FawnVision::SShaderResource shaderResource {
            .stages = (uint16_t)stageBits,
            .type   = (uint8_t)FawnVision::shader_resource_type::buffer_uniform,
        };

        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type                           = reflection.get_type( uniformBuffer.type_id );
        const bool isPushConstant                  = reflection.get_storage_class( uniformBuffer.id ) == spv::StorageClassPushConstant;
        const bool isBlock                         = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) || reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID( uniformBuffer.base_type_id ) : SPIRV_CROSS_NAMESPACE::ID( uniformBuffer.id );
        shaderResource.name                        = !uniformBuffer.name.empty() ? uniformBuffer.name : reflection.get_fallback_name( fallbackId );

        shaderResource.size      = bufferSize;
        shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[ 0 ] : 1;
        shaderResource.set       = set;
        shaderResource.binding   = binding;
        //        shaderResource.mode = (shaderResource.set == 0 && shaderResource.binding == 0)? shader_resource_mode::dynamic_mode : shader_resource_mode::static;

        m_resources.push_back( shaderResource );
    }
}
void DeerVulkan::CShader::ExtractStorageBuffers( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept
{
    for ( const auto& storageBuffer : resources.storage_buffers )
    {
        const auto& spirvType = reflection.get_type_from_variable( storageBuffer.id );
        const uint32_t bufferSize { static_cast<uint32_t>( reflection.get_declared_struct_size( spirvType ) ) };
        const uint32_t set { reflection.get_decoration( storageBuffer.id, spv::DecorationDescriptorSet ) };
        const uint32_t binding { reflection.get_decoration( storageBuffer.id, spv::DecorationBinding ) };

        FawnVision::SShaderResource shaderResource {
            .stages = (uint16_t)stageBits,
            .type   = (uint8_t)FawnVision::shader_resource_type::buffer_storage,
        };

        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type                           = reflection.get_type( storageBuffer.type_id );
        const bool isPushConstant                  = reflection.get_storage_class( storageBuffer.id ) == spv::StorageClassPushConstant;
        const bool isBlock                         = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) || reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID( storageBuffer.base_type_id ) : SPIRV_CROSS_NAMESPACE::ID( storageBuffer.id );
        shaderResource.name                        = !storageBuffer.name.empty() ? storageBuffer.name : reflection.get_fallback_name( fallbackId );
        //

        shaderResource.size       = bufferSize;
        shaderResource.arraySize  = !spirvType.array.empty() ? spirvType.array[ 0 ] : 1;
        shaderResource.set        = set;
        shaderResource.binding    = binding;
        shaderResource.qualifiers = 3;
        m_resources.push_back( shaderResource );
    }
}
void DeerVulkan::CShader::ExtractSeparateSamplers( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept
{
    for ( const auto& sampler : resources.separate_samplers )
    {
        const auto& spirvType = reflection.get_type_from_variable( sampler.id );
        FawnVision::SShaderResource shaderResource {
            .stages = (uint16_t)stageBits,
            .type   = (uint8_t)FawnVision::shader_resource_type::sampler,
        };

        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type                           = reflection.get_type( sampler.type_id );
        const bool isPushConstant                  = reflection.get_storage_class( sampler.id ) == spv::StorageClassPushConstant;
        const bool isBlock                         = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) || reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID( sampler.base_type_id ) : SPIRV_CROSS_NAMESPACE::ID( sampler.id );
        shaderResource.name                        = !sampler.name.empty() ? sampler.name : reflection.get_fallback_name( fallbackId );
        //

        shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[ 0 ] : 1;
        shaderResource.set       = reflection.get_decoration( sampler.id, spv::DecorationDescriptorSet );
        shaderResource.binding   = reflection.get_decoration( sampler.id, spv::DecorationBinding );

        m_resources.push_back( shaderResource );
    }
}
void DeerVulkan::CShader::ExtractStorageItems( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept
{
    for ( const auto& imageStorage : resources.storage_images )
    {
        const auto& spirvType = reflection.get_type_from_variable( imageStorage.id );
        FawnVision::SShaderResource shaderResource {
            .stages = (uint16_t)stageBits,
            .type   = (uint8_t)FawnVision::shader_resource_type::image_storage,
        };

        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type                           = reflection.get_type( imageStorage.type_id );
        const bool isPushConstant                  = reflection.get_storage_class( imageStorage.id ) == spv::StorageClassPushConstant;
        const bool isBlock                         = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) || reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID( imageStorage.base_type_id ) : SPIRV_CROSS_NAMESPACE::ID( imageStorage.id );
        shaderResource.name                        = !imageStorage.name.empty() ? imageStorage.name : reflection.get_fallback_name( fallbackId );
        //

        shaderResource.arraySize  = !spirvType.array.empty() ? spirvType.array[ 0 ] : 1;
        shaderResource.set        = reflection.get_decoration( imageStorage.id, spv::DecorationDescriptorSet );
        shaderResource.binding    = reflection.get_decoration( imageStorage.id, spv::DecorationBinding );
        shaderResource.qualifiers = 3;

        m_resources.push_back( shaderResource );
    }
}
void DeerVulkan::CShader::ExtractSampledImages( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept
{
    for ( const auto& imageSampler : resources.sampled_images )
    {
        const auto& spirvType = reflection.get_type_from_variable( imageSampler.id );
        FawnVision::SShaderResource shaderResource {
            .stages = (uint16_t)stageBits,
            .type   = (uint8_t)FawnVision::shader_resource_type::image_sampler,
        };

        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type                           = reflection.get_type( imageSampler.type_id );
        const bool isPushConstant                  = reflection.get_storage_class( imageSampler.id ) == spv::StorageClassPushConstant;
        const bool isBlock                         = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) || reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID( imageSampler.base_type_id ) : SPIRV_CROSS_NAMESPACE::ID( imageSampler.id );
        shaderResource.name                        = !imageSampler.name.empty() ? imageSampler.name : reflection.get_fallback_name( fallbackId );
        //

        shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[ 0 ] : 1;
        shaderResource.set       = reflection.get_decoration( imageSampler.id, spv::DecorationDescriptorSet );
        shaderResource.binding   = reflection.get_decoration( imageSampler.id, spv::DecorationBinding );

        m_resources.push_back( shaderResource );
    }
}
void DeerVulkan::CShader::ExtractSeparateImages( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept
{
    for ( const auto& image : resources.separate_images )
    {
        const auto& spirvType = reflection.get_type_from_variable( image.id );
        FawnVision::SShaderResource shaderResource {
            .stages = (uint16_t)stageBits,
            .type   = (uint8_t)FawnVision::shader_resource_type::image,
        };
        shaderResource.name = reflection.get_name( image.id );

        shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[ 0 ] : 1;
        shaderResource.set       = reflection.get_decoration( image.id, spv::DecorationDescriptorSet );
        shaderResource.binding   = reflection.get_decoration( image.id, spv::DecorationBinding );

        m_resources.push_back( shaderResource );
    }
}
void DeerVulkan::CShader::ExtractStageOutputs( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept
{
    for ( const auto& output : resources.stage_outputs )
    {
        const auto& bufferType { reflection.get_type( output.base_type_id ) };
        const auto& spirvType = reflection.get_type_from_variable( output.id );

        FawnVision::SShaderResource shaderResource {
            .stages = (uint16_t)stageBits,
            .type   = (uint8_t)FawnVision::shader_resource_type::output,
        };

        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type                           = reflection.get_type( output.type_id );
        const bool isPushConstant                  = reflection.get_storage_class( output.id ) == spv::StorageClassPushConstant;
        const bool isBlock                         = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) || reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID( output.base_type_id ) : SPIRV_CROSS_NAMESPACE::ID( output.id );
        shaderResource.name                        = !output.name.empty() ? output.name : reflection.get_fallback_name( fallbackId );
        //

        shaderResource.vecSize   = bufferType.vecsize;
        shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[ 0 ] : 1;
        shaderResource.location  = reflection.get_decoration( output.id, spv::DecorationLocation );

        m_resources.push_back( shaderResource );
    }
}
void DeerVulkan::CShader::ExtractSubpassInputs( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept
{
    for ( const auto& inputAttachment : resources.subpass_inputs )
    {
        const auto& spirvType = reflection.get_type_from_variable( inputAttachment.id );

        FawnVision::SShaderResource shaderResource {
            .stages = (uint16_t)stageBits,
            .type   = (uint8_t)FawnVision::shader_resource_type::input_attachment,
        };

        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type                           = reflection.get_type( inputAttachment.type_id );
        const bool isPushConstant                  = reflection.get_storage_class( inputAttachment.id ) == spv::StorageClassPushConstant;
        const bool isBlock                         = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) || reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID( inputAttachment.base_type_id ) : SPIRV_CROSS_NAMESPACE::ID( inputAttachment.id );
        shaderResource.name                        = !inputAttachment.name.empty() ? inputAttachment.name : reflection.get_fallback_name( fallbackId );
        //

        shaderResource.arraySize            = !spirvType.array.empty() ? spirvType.array[ 0 ] : 1;
        shaderResource.inputAttachmentIndex = reflection.get_decoration( inputAttachment.id, spv::DecorationInputAttachmentIndex );
        shaderResource.set                  = reflection.get_decoration( inputAttachment.id, spv::DecorationDescriptorSet );
        shaderResource.binding              = reflection.get_decoration( inputAttachment.id, spv::DecorationBinding );
        m_resources.push_back( shaderResource );
    }
}
void DeerVulkan::CShader::ExtractStageInputs( const VkShaderStageFlagBits& stageBits, const spirv_cross::CompilerReflection& reflection, const spirv_cross::ShaderResources& resources ) noexcept
{
    for ( const auto& stageInput : resources.stage_inputs )
    {
        const auto& bufferType { reflection.get_type( stageInput.base_type_id ) };
        const auto& spirvType = reflection.get_type_from_variable( stageInput.id );

        FawnVision::SShaderResource shaderResource {
            .stages = (uint16_t)stageBits,
            .type   = (uint8_t)FawnVision::shader_resource_type::input,
        };

        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type = reflection.get_type( stageInput.type_id );

        const bool isPushConstant                  = reflection.get_storage_class( stageInput.id ) == spv::StorageClassPushConstant;
        const bool isBlock                         = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) || reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID( stageInput.base_type_id ) : SPIRV_CROSS_NAMESPACE::ID( stageInput.id );
        shaderResource.name                        = !stageInput.name.empty() ? stageInput.name : reflection.get_fallback_name( fallbackId );

        shaderResource.vecSize   = bufferType.vecsize;
        shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[ 0 ] : 1;
        shaderResource.location  = reflection.get_decoration( stageInput.id, spv::DecorationLocation );
        m_resources.push_back( shaderResource );

        switch ( shaderResource.location )
        {
            case 0: m_vertexComponents |= FawnVision::vertex_component_flag_position; break;
            case 1: m_vertexComponents |= FawnVision::vertex_component_flag_color; break;
            case 2: m_vertexComponents |= FawnVision::vertex_component_flag_uv; break;
            case 3: m_vertexComponents |= FawnVision::vertex_component_flag_normal; break;
            case 4: m_vertexComponents |= FawnVision::vertex_component_flag_tangent; break;
            default: break;
        }
    }
}

VkShaderModule DeerVulkan::CShader::CreateShaderModule( const std::vector<uint32_t>& data ) const
{
    VkShaderModuleCreateInfo createInfo {};
    createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = data.size() * sizeof( uint32_t );
    createInfo.pCode    = data.data();

    VkShaderModule shaderModule;
    CheckVkResult( vkCreateShaderModule( GetDevice()->GetVkDevice(), &createInfo, VK_NULL_HANDLE, &shaderModule ), "failed to create shader module!" );

    return shaderModule;
}

VkShaderStageFlagBits DeerVulkan::CShader::ConvertShaderStage( const uint32_t type )
{
    switch ( type )
    {
        case FawnVision::shader_stage_vertex: return VK_SHADER_STAGE_VERTEX_BIT;
        case FawnVision::shader_stage_geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
        case FawnVision::shader_stage_fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
        case FawnVision::shader_stage_raygen: return VK_SHADER_STAGE_RAYGEN_BIT_KHR;
        case FawnVision::shader_stage_any_hit: return VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
        case FawnVision::shader_stage_closest_hit: return VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
        case FawnVision::shader_stage_miss: return VK_SHADER_STAGE_MISS_BIT_KHR;
        case FawnVision::shader_stage_intersection: return VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
        case FawnVision::shader_stage_callable: return VK_SHADER_STAGE_CALLABLE_BIT_KHR;
        case FawnVision::shader_stage_task: return VK_SHADER_STAGE_TASK_BIT_NV;
        case FawnVision::shader_stage_mesh: return VK_SHADER_STAGE_MESH_BIT_NV;
        case FawnVision::shader_stage_tessellation_control: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case FawnVision::shader_stage_tessellation_evaluation: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        case FawnVision::shader_stage_compute:
        case FawnVision::shader_stage_max: return {};
        default: throw std::invalid_argument( "Invalid shader type." );
    }
}
