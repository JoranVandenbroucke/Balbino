#pragma once

#include <filesystem>
#include <unordered_set>
#include <shaderc/shaderc.hpp>
#include <vulkan/vulkan.hpp>
#include "Base.h"
#include "Common.h"
#include "Vertex.h"


namespace FawnVision
{
    struct EShaderResourceType
    {
        enum Enum : uint8_t
        {
            None,
            Input,
            InputAttachment,
            Output,
            Image,
            ImageSampler,
            ImageStorage,
            Sampler,
            BufferUniform,
            BufferStorage,
            PushConstant,
            SpecializationConstant,
        };
    };
    
    struct SShaderResourceQualifiers
    {
        enum Enum : uint32_t
        {
            None        = 0,
            NonReadable = 1,
            NonWritable = 2,
        };
    };
    
    struct EShaderResourceMode
    {
        enum Enum : uint8_t
        {
            Static,
            Dynamic,
            UpdateAfterBind
        };
    };
    
    struct SShaderResource
    {
        VkShaderStageFlags        stages;
        EShaderResourceType::Enum type;
        EShaderResourceMode::Enum mode;
        uint32_t                  set;
        uint32_t                  binding;
        uint32_t                  location;
        uint32_t                  inputAttachmentIndex;
        uint32_t                  vecSize;
        uint32_t                  columns;
        uint32_t                  arraySize;
        uint32_t                  offset;
        uint32_t                  size;
        uint32_t                  constantId;
        uint32_t                  qualifiers;
        uint64_t                  resourceID;
        std::string               name;
    };
    
    class CShader final : public CDeviceObject
    {
    public:
        explicit CShader( const CDevice* pDevice );
        CShader( const CShader& other );
        ~CShader() override;
        
        void Initialize( const void* pShaderCode, size_t shaderCodeSize, shader_stage stage );
        const std::vector<SShaderResource>& GetShaderResources() const;
        const VkShaderModule& GetShaderModule() const;
        const std::vector<EVertexComponent::Enum>& GetVertexComponents() const;
    private:
        VkShaderModule               m_shaderHandle;
        std::vector<SShaderResource> m_resources;
        std::vector<EVertexComponent::Enum> m_vertexComponents;
        
        VkShaderModule CreateShaderModule( const std::vector<uint32_t>& data ) const;
    };
    
    class CFileIncluder final : public shaderc::CompileOptions::IncluderInterface
    {
    public:
        CFileIncluder() = default;
        ~CFileIncluder() override = default;
        
        // Resolves a requested source file of a given type from a requesting
        // source into a shaderc_include_result whose contents will remain valid
        // until it's released.
        shaderc_include_result* GetInclude( const char* requestedSource, shaderc_include_type type, const char* requestingSource, size_t includeDepth ) override;
        // Releases an include result.
        void ReleaseInclude( shaderc_include_result* includeResult ) override;
        
        // Returns a reference to the member storing the set of included files.
        [[nodiscard]] const std::unordered_set<std::string>& FilePathTrace() const;
    
    private:
        // The full path and content of a source file.
        struct SFileInfo
        {
            const std::string fullPath;
            std::vector<char> contents;
        };
        
        // The set of full paths of included files.
        std::unordered_set<std::string> m_includedFiles;
    };
}
