#pragma once
#include <glm.hpp>
#include <vulkan/vulkan.hpp>

namespace BalVulkan
{
	enum class EVertexComponent : uint8_t
	{
		Position,
		Color,
		UV,
		Normal,
		Tangent,
		Bitangent,
	};
	struct SVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 uv;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;

		static VkVertexInputBindingDescription GetBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions( uint32_t binding, const std::vector<EVertexComponent>& components );

		explicit SVertex( const glm::vec3& position = glm::vec3{ 0,0,0 }, const glm::vec4& color = glm::vec4{ 1,1,1,1 }, const glm::vec2& uv = glm::vec2{ 0, 0 },
		                  const glm::vec3& normal = glm::vec3{ 0,1,0 }, const glm::vec3& tangent = glm::vec3{ 1,0,0 }, const glm::vec3& bitangent = glm::vec3{ 0,0,1 } );
	};
}
