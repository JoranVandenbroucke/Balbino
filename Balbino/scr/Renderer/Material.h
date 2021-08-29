#pragma once
#include <glm.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace Balbino
{
	struct SVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec3 uv;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 binormal;
		
		static VkVertexInputBindingDescription GetBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription;
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof( SVertex );
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}
		static std::array<VkVertexInputAttributeDescription, 6> GetAttributeDescriptions()
		{
			std::array<VkVertexInputAttributeDescription, 6> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof( SVertex, position );

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
			attributeDescriptions[1].offset = offsetof( SVertex, color );

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof( SVertex, uv );

			attributeDescriptions[3].binding = 0;
			attributeDescriptions[3].location = 2;
			attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[3].offset = offsetof( SVertex, normal );

			attributeDescriptions[4].binding = 0;
			attributeDescriptions[4].location = 2;
			attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[4].offset = offsetof( SVertex, tangent );

			attributeDescriptions[5].binding = 0;
			attributeDescriptions[5].location = 2;
			attributeDescriptions[5].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[5].offset = offsetof( SVertex, binormal );

			return attributeDescriptions;
		}
	};

	enum class EShaderType : uint8_t
	{
		eVertex = 1,
		eFragment = 2,
		eGeometry = 4,
		eCompute = 8,
		eTessellation = 16

	};
	EShaderType operator|( EShaderType lhs, EShaderType rhs );
	EShaderType operator|=( EShaderType lhs, EShaderType rhs );
	bool operator&( Balbino::EShaderType lhs, Balbino::EShaderType rhs );

	class CMaterial
	{
	public:
		CMaterial();

		//https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Shader_modules
		void Load( const char* filePath );	//Asumes that All shaders will be in this one file
		void LoadVertexShader( const char* filePath );	//Asumes that there will be still other shaders
		void LoadFragmentShader( const char* filePath );	//Asumes that there will be still other shaders
		void LoadGeometryShader( const char* filePath );	//Asumes that there will be still other shaders
		void LoadComputeShader( const char* filePath );	//Asumes that there will be still other shaders
		void LoadTessellationShader( const char* filePath );	//Asumes that there will be still other shaders
		void Compile( VkDevice& device );
		void Destroy();
		uint32_t m_firstIndex;
		uint32_t m_lastIndex;
		std::vector<SVertex> m_vertexBuffer{};
	private:
		std::vector<char> m_fullShader{};
		std::vector<char> m_vertexShader{};
		std::vector<char> m_fragmentShader{};
		std::vector<char> m_geometryShader{};
		std::vector<char> m_computeShader{};
		std::vector<char> m_tessellationShader{};
		std::vector < std::pair<VkShaderModule, EShaderType>> m_shaderList{};
		Balbino::EShaderType m_shaderType;
		VkShaderModule CreateShaderModule( const std::vector<char>& code, VkDevice& device );

		static std::vector<char> ReadFile( const std::string& filename );
	};

}
