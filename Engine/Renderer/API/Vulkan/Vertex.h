#pragma once
#pragma warning(push)
#pragma warning(disable:4201)

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

namespace FawnVision
{
    struct EVertexComponent
    {
        enum Enum : uint8_t
        {
            Position,
            Color,
            UV,
            Normal,
            Tangent
        };
    };
    
    struct InstanceBatch
    {
        InstanceBatch() = default;
        explicit InstanceBatch( const glm::mat4& trans )
                : transform{ trans }
        {
        }
        glm::mat4 transform;
        
        static VkVertexInputBindingDescription GetBindingDescription();
        
        static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
    };
    
    struct SVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 uv;
        glm::vec3 normal;
        glm::vec4 tangent;
        
        static VkVertexInputBindingDescription GetBindingDescription();
        
        static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions( uint32_t binding, const std::vector<EVertexComponent::Enum>& components );
        
        explicit SVertex( const glm::vec3& position = glm::vec3{ 0, 0, 0 }, const glm::vec4& color = glm::vec4{
                1, 1, 1, 1
        }, const glm::vec2& uv = glm::vec2{ 0, 0 }, const glm::vec3& normal = glm::vec3{
                0, 1, 0
        }, const glm::vec4& tangent = glm::vec4{
                1, 0, 0, 1
        } );
    };
}
#pragma warning(pop)
