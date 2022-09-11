#include "Vertex.h"

VkVertexInputBindingDescription BalVulkan::SVertex::GetBindingDescription()
{
    constexpr VkVertexInputBindingDescription bindingDescription{
            .binding = 0,
            .stride = sizeof( SVertex ),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };
    return bindingDescription;
}

VkVertexInputBindingDescription BalVulkan::InstanceBatch::GetBindingDescription()
{
    constexpr VkVertexInputBindingDescription bindingDescription{
            .binding = 1,
            .stride = sizeof( InstanceBatch ),
            .inputRate = VK_VERTEX_INPUT_RATE_INSTANCE,
    };
    return bindingDescription;
}

std::vector<VkVertexInputAttributeDescription> BalVulkan::SVertex::GetAttributeDescriptions( uint32_t binding, const std::vector<EVertexComponent>& components )
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
    uint32_t                                       location{};
    for ( const EVertexComponent                   component : components )
    {
        switch ( component )
        {
            case EVertexComponent::Position:
                attributeDescriptions.emplace_back(
                        location,
                        binding,
                        VK_FORMAT_R32G32B32_SFLOAT,
                        static_cast<uint32_t>(offsetof( SVertex, position ))
                );
                break;
            case EVertexComponent::UV:
                attributeDescriptions.emplace_back(
                        location,
                        binding,
                        VK_FORMAT_R32G32_SFLOAT,
                        static_cast<uint32_t>(offsetof( SVertex, uv ))
                );
                break;
            case EVertexComponent::Color:
                attributeDescriptions.emplace_back(
                        location,
                        binding,
                        VK_FORMAT_R32G32B32A32_SFLOAT,
                        static_cast<uint32_t>(offsetof( SVertex, color ))
                );
                break;
            case EVertexComponent::Normal:
                attributeDescriptions.emplace_back(
                        location,
                        binding,
                        VK_FORMAT_R32G32B32_SFLOAT,
                        static_cast<uint32_t>(offsetof( SVertex, normal ))
                );
                break;
            case EVertexComponent::Tangent:
                attributeDescriptions.emplace_back(
                        location,
                        binding,
                        VK_FORMAT_R32G32B32_SFLOAT,
                        static_cast<uint32_t>(offsetof( SVertex, tangent ))
                );
                break;
            default:
                break;
        }
        ++location;
    }
    return attributeDescriptions;
}

std::vector<VkVertexInputAttributeDescription> BalVulkan::InstanceBatch::GetAttributeDescriptions()
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
    uint32_t                                       location{ 5 };

    attributeDescriptions.emplace_back(
            location,
            1,
            VK_FORMAT_R32G32B32A32_SFLOAT,
            0u
    );
    ++location;
    attributeDescriptions.emplace_back(
            location,
            1,
            VK_FORMAT_R32G32B32A32_SFLOAT,
            (uint32_t) sizeof( glm::vec4 )
    );
    ++location;
    attributeDescriptions.emplace_back(
            location,
            1,
            VK_FORMAT_R32G32B32A32_SFLOAT,
            (uint32_t) sizeof( glm::vec4 ) * 2u
    );
    ++location;
    attributeDescriptions.emplace_back(
            location,
            1,
            VK_FORMAT_R32G32B32A32_SFLOAT,
            (uint32_t) sizeof( glm::vec4 ) * 3u
    );
    return attributeDescriptions;
}

BalVulkan::SVertex::SVertex( const glm::vec3& position, const glm::vec4& color, const glm::vec2& uv,
                             const glm::vec3& normal, const glm::vec4& tangent )
        : position{ position },
          color{ color },
          uv{ uv },
          normal{ normal },
          tangent{ tangent }
{
}
