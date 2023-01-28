#include "Shader.h"

#include <fstream>
#include <spirv_reflect.hpp>
#include <spirv_cross.hpp>
#include <shaderc/shaderc.hpp>

#include "Device.h"
#include "Funtions.h"


BalVulkan::CShader::~CShader()
{
    vkDestroyShaderModule( GetDevice()->GetVkDevice(), m_shaderHandle, nullptr );
    m_resources.clear();
}


BalVulkan::CShader::CShader( const CDevice* const device )
        : CDeviceObject{ device }
        , m_shaderHandle{ VK_NULL_HANDLE }
{
}

BalVulkan::CShader::CShader( const CShader& other )
        : CDeviceObject{ other.GetDevice() }
        , m_shaderHandle{ other.m_shaderHandle }
        , m_resources{ other.m_resources }
{
}

void BalVulkan::CShader::Initialize( const void* pShaderCode, size_t shaderCodeSize, EShaderStage::Enum stage )
{
    VkShaderStageFlagBits stageBits{};
    switch ( stage )
    {
        case EShaderStage::Vertex:
            stageBits = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        case EShaderStage::Geometry:
            stageBits = VK_SHADER_STAGE_GEOMETRY_BIT;
            break;
        case EShaderStage::Fragment:
            stageBits = VK_SHADER_STAGE_FRAGMENT_BIT;
            break;
        case EShaderStage::Raygen:
            stageBits = VK_SHADER_STAGE_RAYGEN_BIT_KHR;
            break;
        case EShaderStage::AnyHit:
            stageBits = VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
            break;
        case EShaderStage::ClosestHit:
            stageBits = VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
            break;
        case EShaderStage::Miss:
            stageBits = VK_SHADER_STAGE_MISS_BIT_KHR;
            break;
        case EShaderStage::Intersection:
            stageBits = VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
            break;
        case EShaderStage::Callable:
            stageBits = VK_SHADER_STAGE_CALLABLE_BIT_KHR;
            break;
        case EShaderStage::Task:
            stageBits = VK_SHADER_STAGE_TASK_BIT_NV;
            break;
        case EShaderStage::Mesh:
            stageBits = VK_SHADER_STAGE_MESH_BIT_NV;
            break;
        case EShaderStage::TessellationControl:
        case EShaderStage::TessellationEvaluation:
        case EShaderStage::Compute:
        case EShaderStage::max:
            break;
    }
    
    std::vector<uint32_t> sprv;
    sprv.assign((uint32_t*) pShaderCode, (uint32_t*) pShaderCode + shaderCodeSize );
    
    spirv_cross::CompilerReflection reflection{ sprv };
    const spirv_cross::ShaderResources& resources{ reflection.get_shader_resources() };
    
    for ( const auto& stageInput : resources.stage_inputs )
    {
        const auto& bufferType{ reflection.get_type( stageInput.base_type_id ) };
        const auto& spirvType = reflection.get_type_from_variable( stageInput.id );
        
        SShaderResource shaderResource{};
        shaderResource.type   = EShaderResourceType::Input;
        shaderResource.stages = stageBits;
        
        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type = reflection.get_type( stageInput.type_id );
        const bool                      isPushConstant = reflection.get_storage_class(
                stageInput.id
        ) == spv::StorageClassPushConstant;
        const bool                      isBlock        = reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBlock
        ) || reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBufferBlock
        );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId     = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID(
                stageInput.base_type_id
        ) : SPIRV_CROSS_NAMESPACE::ID( stageInput.id );
        shaderResource.name = !stageInput.name.empty() ? stageInput.name : reflection.get_fallback_name( fallbackId );
        
        shaderResource.vecSize   = bufferType.vecsize;
        shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
        shaderResource.location  = reflection.get_decoration( stageInput.id, spv::DecorationLocation );
        m_resources.push_back( shaderResource );
        
        switch ( shaderResource.location )
        {
            case 0:
                m_vertexComponents.push_back( EVertexComponent::Position );
                break;
            case 1:
                m_vertexComponents.push_back( EVertexComponent::Color );
                break;
            case 2:
                m_vertexComponents.push_back( EVertexComponent::UV );
                break;
            case 3:
                m_vertexComponents.push_back( EVertexComponent::Normal );
                break;
            case 4:
                m_vertexComponents.push_back( EVertexComponent::Tangent );
                break;
        }
    }
    for ( const auto& inputAttachment : resources.subpass_inputs )
    {
        const auto& spirvType = reflection.get_type_from_variable( inputAttachment.id );
        
        SShaderResource shaderResource{};
        shaderResource.type   = EShaderResourceType::InputAttachment;
        shaderResource.stages = stageBits;
        
        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type = reflection.get_type( inputAttachment.type_id );
        const bool                      isPushConstant = reflection.get_storage_class(
                inputAttachment.id
        ) == spv::StorageClassPushConstant;
        const bool                      isBlock        = reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBlock
        ) || reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBufferBlock
        );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId     = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID(
                inputAttachment.base_type_id
        ) : SPIRV_CROSS_NAMESPACE::ID( inputAttachment.id );
        shaderResource.name = !inputAttachment.name.empty() ? inputAttachment.name : reflection.get_fallback_name(
                fallbackId
        );
        //
        
        shaderResource.arraySize            = !spirvType.array.empty() ? spirvType.array[0] : 1;
        shaderResource.inputAttachmentIndex = reflection.get_decoration(
                inputAttachment.id, spv::DecorationInputAttachmentIndex
        );
        shaderResource.set                  = reflection.get_decoration(
                inputAttachment.id, spv::DecorationDescriptorSet
        );
        shaderResource.binding              = reflection.get_decoration( inputAttachment.id, spv::DecorationBinding );
        m_resources.push_back( shaderResource );
    }
    for ( const auto& output : resources.stage_outputs )
    {
        const auto& bufferType{ reflection.get_type( output.base_type_id ) };
        const auto& spirvType = reflection.get_type_from_variable( output.id );
        
        SShaderResource shaderResource{};
        shaderResource.type   = EShaderResourceType::Output;
        shaderResource.stages = stageBits;
        
        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type = reflection.get_type( output.type_id );
        const bool                      isPushConstant = reflection.get_storage_class(
                output.id
        ) == spv::StorageClassPushConstant;
        const bool                      isBlock        = reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBlock
        ) || reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBufferBlock
        );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId     = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID(
                output.base_type_id
        ) : SPIRV_CROSS_NAMESPACE::ID( output.id );
        shaderResource.name = !output.name.empty() ? output.name : reflection.get_fallback_name( fallbackId );
        //
        
        shaderResource.vecSize   = bufferType.vecsize;
        shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
        shaderResource.location  = reflection.get_decoration( output.id, spv::DecorationLocation );
        
        m_resources.push_back( shaderResource );
    }
    for ( const auto& image : resources.separate_images )
    {
        const auto& spirvType = reflection.get_type_from_variable( image.id );
        SShaderResource shaderResource{};
        shaderResource.type   = EShaderResourceType::Image;
        shaderResource.stages = stageBits;
        //shaderResource.name = stageInput.name;
        shaderResource.name   = reflection.get_name( image.id );
        
        shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
        shaderResource.set       = reflection.get_decoration( image.id, spv::DecorationDescriptorSet );
        shaderResource.binding   = reflection.get_decoration( image.id, spv::DecorationBinding );
        
        m_resources.push_back( shaderResource );
    }
    for ( const auto& imageSampler : resources.sampled_images )
    {
        const auto& spirvType = reflection.get_type_from_variable( imageSampler.id );
        SShaderResource shaderResource{};
        shaderResource.type   = EShaderResourceType::ImageSampler;
        shaderResource.stages = stageBits;
        
        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type = reflection.get_type( imageSampler.type_id );
        const bool                      isPushConstant = reflection.get_storage_class(
                imageSampler.id
        ) == spv::StorageClassPushConstant;
        const bool                      isBlock        = reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBlock
        ) || reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBufferBlock
        );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId     = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID(
                imageSampler.base_type_id
        ) : SPIRV_CROSS_NAMESPACE::ID( imageSampler.id );
        shaderResource.name = !imageSampler.name.empty() ? imageSampler.name : reflection.get_fallback_name(
                fallbackId
        );
        //
        
        shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
        shaderResource.set       = reflection.get_decoration( imageSampler.id, spv::DecorationDescriptorSet );
        shaderResource.binding   = reflection.get_decoration( imageSampler.id, spv::DecorationBinding );
        
        m_resources.push_back( shaderResource );
    }
    for ( const auto& imageStorage : resources.storage_images )
    {
        const auto& spirvType = reflection.get_type_from_variable( imageStorage.id );
        SShaderResource shaderResource{};
        shaderResource.type   = EShaderResourceType::ImageStorage;
        shaderResource.stages = stageBits;
        
        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type = reflection.get_type( imageStorage.type_id );
        const bool                      isPushConstant = reflection.get_storage_class(
                imageStorage.id
        ) == spv::StorageClassPushConstant;
        const bool                      isBlock        = reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBlock
        ) || reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBufferBlock
        );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId     = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID(
                imageStorage.base_type_id
        ) : SPIRV_CROSS_NAMESPACE::ID( imageStorage.id );
        shaderResource.name = !imageStorage.name.empty() ? imageStorage.name : reflection.get_fallback_name(
                fallbackId
        );
        //
        
        shaderResource.arraySize  = !spirvType.array.empty() ? spirvType.array[0] : 1;
        shaderResource.set        = reflection.get_decoration( imageStorage.id, spv::DecorationDescriptorSet );
        shaderResource.binding    = reflection.get_decoration( imageStorage.id, spv::DecorationBinding );
        shaderResource.qualifiers = 3;
        
        m_resources.push_back( shaderResource );
    }
    for ( const auto& sampler : resources.separate_samplers )
    {
        const auto& spirvType = reflection.get_type_from_variable( sampler.id );
        SShaderResource shaderResource{};
        shaderResource.type   = EShaderResourceType::Sampler;
        shaderResource.stages = stageBits;
        
        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type = reflection.get_type( sampler.type_id );
        const bool                      isPushConstant = reflection.get_storage_class(
                sampler.id
        ) == spv::StorageClassPushConstant;
        const bool                      isBlock        = reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBlock
        ) || reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBufferBlock
        );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId     = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID(
                sampler.base_type_id
        ) : SPIRV_CROSS_NAMESPACE::ID( sampler.id );
        shaderResource.name = !sampler.name.empty() ? sampler.name : reflection.get_fallback_name( fallbackId );
        //
        
        shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
        shaderResource.set       = reflection.get_decoration( sampler.id, spv::DecorationDescriptorSet );
        shaderResource.binding   = reflection.get_decoration( sampler.id, spv::DecorationBinding );
        
        m_resources.push_back( shaderResource );
    }
    for ( const auto& uniformBuffer : resources.uniform_buffers )
    {
        const auto& bufferType{ reflection.get_type( uniformBuffer.base_type_id ) };
        const uint32_t bufferSize{ static_cast<uint32_t>( reflection.get_declared_struct_size( bufferType )) };
        const uint32_t set{ reflection.get_decoration( uniformBuffer.id, spv::DecorationDescriptorSet ) };
        const uint32_t binding{ reflection.get_decoration( uniformBuffer.id, spv::DecorationBinding ) };
        const auto& spirvType = reflection.get_type_from_variable( uniformBuffer.id );
        
        SShaderResource shaderResource{};
        shaderResource.type   = EShaderResourceType::BufferUniform;
        shaderResource.stages = stageBits;
        
        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type = reflection.get_type( uniformBuffer.type_id );
        const bool                      isPushConstant = reflection.get_storage_class(
                uniformBuffer.id
        ) == spv::StorageClassPushConstant;
        const bool                      isBlock        = reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBlock
        ) || reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBufferBlock
        );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId     = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID(
                uniformBuffer.base_type_id
        ) : SPIRV_CROSS_NAMESPACE::ID( uniformBuffer.id );
        shaderResource.name = !uniformBuffer.name.empty() ? uniformBuffer.name : reflection.get_fallback_name(
                fallbackId
        );
        //
        
        shaderResource.size      = bufferSize;
        shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
        shaderResource.set       = set;
        shaderResource.binding   = binding;
//        shaderResource.mode = (shaderResource.set == 0 && shaderResource.binding == 0)? EShaderResourceMode::Dynamic : EShaderResourceMode::Static;
        
        m_resources.push_back( shaderResource );
    }
    for ( const auto& storageBuffer : resources.storage_buffers )
    {
        const auto& spirvType = reflection.get_type_from_variable( storageBuffer.id );
        const uint32_t bufferSize{ static_cast<uint32_t>( reflection.get_declared_struct_size( spirvType )) };
        const uint32_t set{ reflection.get_decoration( storageBuffer.id, spv::DecorationDescriptorSet ) };
        const uint32_t binding{ reflection.get_decoration( storageBuffer.id, spv::DecorationBinding ) };
        
        SShaderResource shaderResource{};
        shaderResource.type   = EShaderResourceType::BufferStorage;
        shaderResource.stages = stageBits;
        
        //see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
        const auto& type = reflection.get_type( storageBuffer.type_id );
        const bool                      isPushConstant = reflection.get_storage_class(
                storageBuffer.id
        ) == spv::StorageClassPushConstant;
        const bool                      isBlock        = reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBlock
        ) || reflection.get_decoration_bitset( type.self ).get(
                spv::DecorationBufferBlock
        );
        const SPIRV_CROSS_NAMESPACE::ID fallbackId     = !isPushConstant && isBlock ? SPIRV_CROSS_NAMESPACE::ID(
                storageBuffer.base_type_id
        ) : SPIRV_CROSS_NAMESPACE::ID( storageBuffer.id );
        shaderResource.name = !storageBuffer.name.empty() ? storageBuffer.name : reflection.get_fallback_name(
                fallbackId
        );
        //
        
        shaderResource.size       = bufferSize;
        shaderResource.arraySize  = !spirvType.array.empty() ? spirvType.array[0] : 1;
        shaderResource.set        = set;
        shaderResource.binding    = binding;
        shaderResource.qualifiers = 3;
        m_resources.push_back( shaderResource );
    }
    
    m_shaderHandle = CreateShaderModule( sprv );
}

const std::vector<BalVulkan::SShaderResource>& BalVulkan::CShader::GetShaderResources() const
{
    return m_resources;
}

const VkShaderModule& BalVulkan::CShader::GetShaderModule() const
{
    return m_shaderHandle;
}

BalVulkan::CShader* BalVulkan::CShader::CreateNew( const CDevice* pDevice )
{
    return new CShader{ pDevice };
}

VkShaderModule BalVulkan::CShader::CreateShaderModule( const std::vector<uint32_t>& data ) const
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = data.size() * sizeof( uint32_t );
    createInfo.pCode    = data.data();
    
    VkShaderModule shaderModule;
    CheckVkResult(
            vkCreateShaderModule( GetDevice()->GetVkDevice(), &createInfo, nullptr, &shaderModule ),
            "failed to create shader module!"
    );
    
    return shaderModule;
}

shaderc_include_result* BalVulkan::CFileIncluder::GetInclude( const char* requestedSource, shaderc_include_type type, const char* requestingSource, size_t includeDepth )
{
    (void) includeDepth;
    std::filesystem::path src{ requestingSource };
    src.remove_filename();
    const std::filesystem::path dest{ src.remove_filename().string() + requestedSource };
    const std::string           fullPath = ( type == shaderc_include_type_relative )
                                           ? absolute( dest ).string()
                                           : requestedSource;
    if ( fullPath.empty())
    {
        return nullptr;
    }
    
    std::ifstream newFileInfo( fullPath, std::ios::in | std::ios::binary );
    if ( !newFileInfo.is_open())
    {
        return nullptr;
    }
    const std::string newFileInfoContent(( std::istreambuf_iterator<char>( newFileInfo )),
                                         std::istreambuf_iterator<char>());
    newFileInfo.close();
    
    m_includedFiles.insert( fullPath );
    
    auto pFilePath{ new char[fullPath.size()] };
    auto pData{ new char[newFileInfoContent.size()] };
    memcpy( pFilePath, fullPath.data(), fullPath.size());
    memcpy( pData, newFileInfoContent.data(), newFileInfoContent.size());
    return new shaderc_include_result{ pFilePath, fullPath.size(), pData, newFileInfoContent.size(), nullptr };
}

void BalVulkan::CFileIncluder::ReleaseInclude( shaderc_include_result* include_result )
{
    delete[] include_result->source_name;
    delete[] include_result->content;
    delete include_result;
}

const std::unordered_set<std::string>& BalVulkan::CFileIncluder::FilePathTrace() const
{
    return m_includedFiles;
}
const std::vector<BalVulkan::EVertexComponent::Enum>& BalVulkan::CShader::GetVertexComponents() const
{
    return m_vertexComponents;
}
