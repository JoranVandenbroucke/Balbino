#include "pch.h"
#include "Shader.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iostream>

#include "Vertex.h"
#include "../Managers/ShaderManager.h"

Balbino::CShader::CShader()
	: m_pipelineLayout{m_pipelineLayout}
	, m_pipeline{m_pipeline}
{
}

Balbino::CShader::~CShader()
{
	if (m_pipeline != VK_NULL_HANDLE || m_pipelineLayout != VK_NULL_HANDLE)
		std::cerr << "index buffer was not destroyed" << std::endl;
}

void* Balbino::CShader::operator new(size_t size)
{
	if (CShader* pTexture = static_cast<CShader*>(::operator new(size)))
	{
		CShaderManager::AddShader(pTexture);

		return pTexture;
	}
	return nullptr;
}

void* Balbino::CShader::operator new(size_t size, int b, const char* f, int l)
{
	if (CShader* pTexture = static_cast<CShader*>(::operator new(size, b, f, l)))
	{
		CShaderManager::AddShader(pTexture);

		return pTexture;
	}
	return nullptr;
}

void Balbino::CShader::operator delete(void* ptr)
{
	::operator delete(ptr);
}

void Balbino::CShader::operator delete(void* ptr, int b, const char* f, int l)
{
	::operator delete(ptr, b, f, l);
}

void Balbino::CShader::Initialize(const VkDevice& device, const VkExtent2D& swapChainExtent, const VkRenderPass& renderPass, VkDescriptorSetLayout& descriptorSetLayout, const VkAllocationCallbacks* callbacks)
{
	const std::string vertShaderCode{ReadFile(R"(../Resources/Shaders/vertex.spv)")};
	const std::string fragShaderCode{ReadFile(R"(../Resources/Shaders/fragment.spv)")};


	const VkShaderModule& vertShaderModule{CreateShaderModule(vertShaderCode, device, callbacks)};
	const VkShaderModule& fragShaderModule{CreateShaderModule(fragShaderCode, device, callbacks)};

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription = SVertex::GetBindingDescription();
	auto attributeDescriptions = SVertex::GetAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, callbacks, &m_pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = m_pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.pDepthStencilState = &depthStencil;

	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, callbacks, &m_pipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

void Balbino::CShader::Cleanup(const VkDevice& device, const VkAllocationCallbacks* callbacks)
{
	vkDestroyPipelineLayout(device, m_pipelineLayout, callbacks);
	vkDestroyPipeline(device, m_pipeline, callbacks);

	m_pipeline = VK_NULL_HANDLE;
	m_pipelineLayout = VK_NULL_HANDLE;
}

void Balbino::CShader::Bind(const VkCommandBuffer& commandBuffer) const
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
}

const VkPipelineLayout& Balbino::CShader::GetPipelineLayout() const
{
	return m_pipelineLayout;
}

VkPipeline Balbino::CShader::GetPipeline() const
{
	return m_pipeline;
}

VkShaderModule Balbino::CShader::CreateShaderModule(const std::string& code, const VkDevice& device, const VkAllocationCallbacks* callbacks)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.c_str());

	VkShaderModule shaderModule;
	const VkResult result{vkCreateShaderModule(device, &createInfo, callbacks, &shaderModule)};
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create shader module!" + std::to_string(result));
	}
	return shaderModule;
}

//https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Shader_modules
std::string Balbino::CShader::ReadFile(const std::string& filename)
{
	try
	{
		std::ifstream file(filename, std::ios::binary);
		if (!file.is_open())
		{
			throw std::runtime_error{"failed to open file! " + std::filesystem::absolute(filename).string()};
		}
		const std::string text = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		return text;
	}
	catch (const std::ios_base::failure& f)
	{
		std::cerr << "Caught error: " << f.what() << f.code().message() << '\n';
	}
	return {};
}