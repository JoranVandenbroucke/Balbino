#pragma once
#include <array>
#include <glm.hpp>
#include <vulkan/vulkan.h>

namespace Balbino
{
	struct SVertex
	{
		glm::vec3 position;
		glm::vec4 colour;
		glm::vec2 uv;
		//glm::vec3 normal;
		//glm::vec3 tangent;
		//glm::vec3 binormal;
        static VkVertexInputBindingDescription GetBindingDescription()
        {
			const VkVertexInputBindingDescription bindingDescription{
                .binding = 0,
                .stride = sizeof(SVertex),
                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
            };
            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(SVertex, position);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(SVertex, colour);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(SVertex, uv);

            return attributeDescriptions;
        }
	};
}
