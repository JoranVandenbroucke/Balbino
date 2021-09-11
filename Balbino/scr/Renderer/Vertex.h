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
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 binormal;

		static VkVertexInputBindingDescription GetBindingDescription()
		{
			const VkVertexInputBindingDescription bindingDescription{
				.binding = 0,
				.stride = sizeof(SVertex),
				.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
			};
			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 6> GetAttributeDescriptions()
		{
			std::array<VkVertexInputAttributeDescription, 6> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(SVertex, position);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(SVertex, colour);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(SVertex, uv);

			attributeDescriptions[3].binding = 0;
			attributeDescriptions[3].location = 3;
			attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[3].offset = offsetof(SVertex, normal);

			attributeDescriptions[4].binding = 0;
			attributeDescriptions[4].location = 4;
			attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[4].offset = offsetof(SVertex, tangent);

			attributeDescriptions[5].binding = 0;
			attributeDescriptions[5].location = 5;
			attributeDescriptions[5].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[5].offset = offsetof(SVertex, binormal);

			return attributeDescriptions;
		}

		SVertex(const glm::vec3& position, const glm::vec4& colour, const glm::vec2& uv, const glm::vec3& normal = glm::vec3{}, const glm::vec3& tangent = glm::vec3{}, const glm::vec3& binormal = glm::vec3{});
	};

	inline SVertex::SVertex(const glm::vec3& position, const glm::vec4& colour, const glm::vec2& uv, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec3& binormal)
		:position{position}
		, colour{colour}
		, uv{uv}
		, normal{normal}
		, tangent{tangent}
		, binormal{binormal}
	{
	}
}
