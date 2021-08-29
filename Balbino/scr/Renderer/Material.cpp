#include "pch.h"
#include "Material.h"

#include <fstream>


Balbino::EShaderType Balbino::operator|(Balbino::EShaderType lhs, Balbino::EShaderType rhs )
{
	return static_cast<Balbino::EShaderType>( static_cast<uint8_t>( lhs ) | static_cast<uint8_t>( rhs ) );
}

Balbino::EShaderType Balbino::operator|=(Balbino::EShaderType lhs, Balbino::EShaderType rhs )
{
	return static_cast<Balbino::EShaderType>( static_cast<uint8_t>( lhs ) | static_cast<uint8_t>( rhs ) );
}

bool Balbino::operator&(Balbino::EShaderType lhs, Balbino::EShaderType rhs )
{
	return ( static_cast<uint8_t>( lhs ) & static_cast<uint8_t>( rhs ) );
}

Balbino::CMaterial::CMaterial()
	: m_firstIndex{ 0 }
	, m_lastIndex{ 0 }
	, m_shaderType{}
{
}

void Balbino::CMaterial::Load( const char* filePath )
{
	if ( m_shaderType != static_cast<EShaderType>( 0 ) )
	{
		m_fullShader = ReadFile( filePath );
		m_shaderType = EShaderType::eCompute | EShaderType::eFragment | EShaderType::eGeometry | EShaderType::eTessellation | EShaderType::eVertex;
	}
}

void Balbino::CMaterial::LoadVertexShader( const char* filePath )
{
	if ( m_shaderType > EShaderType::eTessellation )
		return;
	m_vertexShader = ReadFile( filePath );
	m_shaderType |= EShaderType::eVertex;
}

void Balbino::CMaterial::LoadFragmentShader( const char* filePath )
{
	if ( m_shaderType > EShaderType::eTessellation )
		return;
	m_fragmentShader = ReadFile( filePath );
	m_shaderType |= EShaderType::eFragment;
}

void Balbino::CMaterial::LoadGeometryShader( const char* filePath )
{
	if ( m_shaderType > EShaderType::eTessellation )
		return;
	m_geometryShader = ReadFile( filePath );
	m_shaderType |= EShaderType::eGeometry;
}

void Balbino::CMaterial::LoadComputeShader( const char* filePath )
{
	if ( m_shaderType > EShaderType::eTessellation )
		return;
	m_computeShader = ReadFile( filePath );
	m_shaderType |= EShaderType::eCompute;
}

void Balbino::CMaterial::LoadTessellationShader( const char* filePath )
{
	if ( m_shaderType > EShaderType::eTessellation )
		return;
	m_tessellationShader = ReadFile( filePath );
	m_shaderType |= EShaderType::eTessellation;
}


void Balbino::CMaterial::Compile( VkDevice& device )
{
	VkPipelineShaderStageCreateInfo shaderStages[5];
	if ( m_vertexShader.size() )
	{
		auto shader = CreateShaderModule( m_vertexShader, device );

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = shader;
		vertShaderStageInfo.pName = "main";
		shaderStages[0] = vertShaderStageInfo;

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		//todo: get swapchain size
		viewport.width = static_cast<float>( 64 );
		viewport.height = static_cast<float>( 64 );
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		//todo: get swapchain size
		scissor.offset = VkOffset2D{ 0, 0 };
		scissor.extent = VkExtent2D{64,64};

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;
		
		vkDestroyShaderModule( device, shader, nullptr );
	}
	if ( m_fragmentShader.size() )
	{
		auto shader = CreateShaderModule( m_fragmentShader, device );

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = shader;
		fragShaderStageInfo.pName = "main";
		shaderStages[1] = fragShaderStageInfo;
		vkDestroyShaderModule( device, shader, nullptr );
	}
	if ( m_geometryShader.size() )
	{
		auto shader = CreateShaderModule( m_geometryShader, device );

		VkPipelineShaderStageCreateInfo geomShaderStageInfo{};
		geomShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		geomShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
		geomShaderStageInfo.module = shader;
		geomShaderStageInfo.pName = "main";
		shaderStages[2] = geomShaderStageInfo;
		vkDestroyShaderModule( device, shader, nullptr );
	}
	if ( m_computeShader.size() )
	{
		auto shader = CreateShaderModule( m_computeShader, device );

		VkPipelineShaderStageCreateInfo compShaderStageInfo{};
		compShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		compShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		compShaderStageInfo.module = shader;
		compShaderStageInfo.pName = "main";
		shaderStages[3] = compShaderStageInfo;
		vkDestroyShaderModule( device, shader, nullptr );
	}
	if ( m_tessellationShader.size() )
	{
		auto shader = CreateShaderModule( m_computeShader, device );

		VkPipelineShaderStageCreateInfo tessShaderStageInfo{};
		tessShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		tessShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		tessShaderStageInfo.module = shader;
		tessShaderStageInfo.pName = "main";
		shaderStages[4] = tessShaderStageInfo;
		vkDestroyShaderModule( device, shader, nullptr );
	}
	if ( m_fullShader.size() )
	{
		m_shaderList.push_back( std::make_pair( CreateShaderModule( m_fullShader, device ), EShaderType::eCompute | EShaderType::eFragment | EShaderType::eGeometry | EShaderType::eTessellation | EShaderType::eVertex ) );
	}
}

void Balbino::CMaterial::Destroy()
{
	m_vertexShader.clear();
	m_fragmentShader.clear();
	m_geometryShader.clear();
	m_computeShader.clear();
	m_tessellationShader.clear();
	m_shaderList.clear();
	m_vertexBuffer.clear();
}


VkShaderModule Balbino::CMaterial::CreateShaderModule( const std::vector<char>& code, VkDevice& device )
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>( code.data() );
	VkShaderModule shaderModule;
	if (vkCreateShaderModule( device, &createInfo, nullptr, &shaderModule ) != VK_SUCCESS )
	{
		throw std::runtime_error( "failed to create shader module!" );
	}
	return shaderModule;
}

//https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Shader_modules
std::vector<char> Balbino::CMaterial::ReadFile( const std::string& filename )
{
	std::ifstream file( filename, std::ios::ate | std::ios::binary );

	if ( !file.is_open() )
	{
		throw std::runtime_error( "failed to open file!" );
	}
	const size_t fileSize = static_cast<size_t>( file.tellg() );
	std::vector<char> buffer( fileSize );
	file.seekg( 0 );
	file.read( buffer.data(), fileSize );
	file.close();

	return buffer;
}