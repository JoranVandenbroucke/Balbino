#include "pch.h"
#include "Interface.h"

#include "Windows/AssetBrowser.h"
#include "Windows/GameView.h"
#include "Windows/MainScreen.h"
#include "Windows/SceneHierarchy.h"

#include <Buffer.h>
#include <CommandPool.h>
#include <Device.h>
#include <FrameBuffer.h>
#include <ImageResource.h>
#include <ImageView.h>
#include <Queue.h>
#include <Sampler.h>

#include <imgui.h>
#include <ImGuizmo.h>
#include <SDL_vulkan.h>

#include <backends/imgui_impl_sdl.h>

// glsl_shader.vert, compiled with:
// # glslangValidator -V -x -o glsl_shader.vert.u32 glsl_shader.vert
/*
#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec4 aColor;
layout(push_constant) uniform uPushConstant { vec2 uScale; vec2 uTranslate; } pc;

out gl_PerVertex { vec4 gl_Position; };
layout(location = 0) out struct { vec4 Color; vec2 UV; } Out;

void main()
{
	Out.Color = aColor;
	Out.UV = aUV;
	gl_Position = vec4(aPos * pc.uScale + pc.uTranslate, 0, 1);
}
*/
static uint32_t g_glslShaderVertSpv[] =
{
	0x07230203, 0x00010000, 0x00080001, 0x0000002e, 0x00000000, 0x00020011, 0x00000001, 0x0006000b,
	0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001,
	0x000a000f, 0x00000000, 0x00000004, 0x6e69616d, 0x00000000, 0x0000000b, 0x0000000f, 0x00000015,
	0x0000001b, 0x0000001c, 0x00030003, 0x00000002, 0x000001c2, 0x00040005, 0x00000004, 0x6e69616d,
	0x00000000, 0x00030005, 0x00000009, 0x00000000, 0x00050006, 0x00000009, 0x00000000, 0x6f6c6f43,
	0x00000072, 0x00040006, 0x00000009, 0x00000001, 0x00005655, 0x00030005, 0x0000000b, 0x0074754f,
	0x00040005, 0x0000000f, 0x6c6f4361, 0x0000726f, 0x00030005, 0x00000015, 0x00565561, 0x00060005,
	0x00000019, 0x505f6c67, 0x65567265, 0x78657472, 0x00000000, 0x00060006, 0x00000019, 0x00000000,
	0x505f6c67, 0x7469736f, 0x006e6f69, 0x00030005, 0x0000001b, 0x00000000, 0x00040005, 0x0000001c,
	0x736f5061, 0x00000000, 0x00060005, 0x0000001e, 0x73755075, 0x6e6f4368, 0x6e617473, 0x00000074,
	0x00050006, 0x0000001e, 0x00000000, 0x61635375, 0x0000656c, 0x00060006, 0x0000001e, 0x00000001,
	0x61725475, 0x616c736e, 0x00006574, 0x00030005, 0x00000020, 0x00006370, 0x00040047, 0x0000000b,
	0x0000001e, 0x00000000, 0x00040047, 0x0000000f, 0x0000001e, 0x00000002, 0x00040047, 0x00000015,
	0x0000001e, 0x00000001, 0x00050048, 0x00000019, 0x00000000, 0x0000000b, 0x00000000, 0x00030047,
	0x00000019, 0x00000002, 0x00040047, 0x0000001c, 0x0000001e, 0x00000000, 0x00050048, 0x0000001e,
	0x00000000, 0x00000023, 0x00000000, 0x00050048, 0x0000001e, 0x00000001, 0x00000023, 0x00000008,
	0x00030047, 0x0000001e, 0x00000002, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002,
	0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000004, 0x00040017,
	0x00000008, 0x00000006, 0x00000002, 0x0004001e, 0x00000009, 0x00000007, 0x00000008, 0x00040020,
	0x0000000a, 0x00000003, 0x00000009, 0x0004003b, 0x0000000a, 0x0000000b, 0x00000003, 0x00040015,
	0x0000000c, 0x00000020, 0x00000001, 0x0004002b, 0x0000000c, 0x0000000d, 0x00000000, 0x00040020,
	0x0000000e, 0x00000001, 0x00000007, 0x0004003b, 0x0000000e, 0x0000000f, 0x00000001, 0x00040020,
	0x00000011, 0x00000003, 0x00000007, 0x0004002b, 0x0000000c, 0x00000013, 0x00000001, 0x00040020,
	0x00000014, 0x00000001, 0x00000008, 0x0004003b, 0x00000014, 0x00000015, 0x00000001, 0x00040020,
	0x00000017, 0x00000003, 0x00000008, 0x0003001e, 0x00000019, 0x00000007, 0x00040020, 0x0000001a,
	0x00000003, 0x00000019, 0x0004003b, 0x0000001a, 0x0000001b, 0x00000003, 0x0004003b, 0x00000014,
	0x0000001c, 0x00000001, 0x0004001e, 0x0000001e, 0x00000008, 0x00000008, 0x00040020, 0x0000001f,
	0x00000009, 0x0000001e, 0x0004003b, 0x0000001f, 0x00000020, 0x00000009, 0x00040020, 0x00000021,
	0x00000009, 0x00000008, 0x0004002b, 0x00000006, 0x00000028, 0x00000000, 0x0004002b, 0x00000006,
	0x00000029, 0x3f800000, 0x00050036, 0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8,
	0x00000005, 0x0004003d, 0x00000007, 0x00000010, 0x0000000f, 0x00050041, 0x00000011, 0x00000012,
	0x0000000b, 0x0000000d, 0x0003003e, 0x00000012, 0x00000010, 0x0004003d, 0x00000008, 0x00000016,
	0x00000015, 0x00050041, 0x00000017, 0x00000018, 0x0000000b, 0x00000013, 0x0003003e, 0x00000018,
	0x00000016, 0x0004003d, 0x00000008, 0x0000001d, 0x0000001c, 0x00050041, 0x00000021, 0x00000022,
	0x00000020, 0x0000000d, 0x0004003d, 0x00000008, 0x00000023, 0x00000022, 0x00050085, 0x00000008,
	0x00000024, 0x0000001d, 0x00000023, 0x00050041, 0x00000021, 0x00000025, 0x00000020, 0x00000013,
	0x0004003d, 0x00000008, 0x00000026, 0x00000025, 0x00050081, 0x00000008, 0x00000027, 0x00000024,
	0x00000026, 0x00050051, 0x00000006, 0x0000002a, 0x00000027, 0x00000000, 0x00050051, 0x00000006,
	0x0000002b, 0x00000027, 0x00000001, 0x00070050, 0x00000007, 0x0000002c, 0x0000002a, 0x0000002b,
	0x00000028, 0x00000029, 0x00050041, 0x00000011, 0x0000002d, 0x0000001b, 0x0000000d, 0x0003003e,
	0x0000002d, 0x0000002c, 0x000100fd, 0x00010038
};

// glsl_shader.frag, compiled with:
// # glslangValidator -V -x -o glsl_shader.frag.u32 glsl_shader.frag
/*
#version 450 core
layout(location = 0) out vec4 fColor;
layout(set=0, binding=0) uniform sampler2D sTexture;
layout(location = 0) in struct { vec4 Color; vec2 UV; } In;
void main()
{
	fColor = In.Color * texture(sTexture, In.UV.st);
}
*/
static uint32_t g_glslShaderFragSpv[] =
{
	0x07230203, 0x00010000, 0x00080001, 0x0000001e, 0x00000000, 0x00020011, 0x00000001, 0x0006000b,
	0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001,
	0x0007000f, 0x00000004, 0x00000004, 0x6e69616d, 0x00000000, 0x00000009, 0x0000000d, 0x00030010,
	0x00000004, 0x00000007, 0x00030003, 0x00000002, 0x000001c2, 0x00040005, 0x00000004, 0x6e69616d,
	0x00000000, 0x00040005, 0x00000009, 0x6c6f4366, 0x0000726f, 0x00030005, 0x0000000b, 0x00000000,
	0x00050006, 0x0000000b, 0x00000000, 0x6f6c6f43, 0x00000072, 0x00040006, 0x0000000b, 0x00000001,
	0x00005655, 0x00030005, 0x0000000d, 0x00006e49, 0x00050005, 0x00000016, 0x78655473, 0x65727574,
	0x00000000, 0x00040047, 0x00000009, 0x0000001e, 0x00000000, 0x00040047, 0x0000000d, 0x0000001e,
	0x00000000, 0x00040047, 0x00000016, 0x00000022, 0x00000000, 0x00040047, 0x00000016, 0x00000021,
	0x00000000, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016, 0x00000006,
	0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000004, 0x00040020, 0x00000008, 0x00000003,
	0x00000007, 0x0004003b, 0x00000008, 0x00000009, 0x00000003, 0x00040017, 0x0000000a, 0x00000006,
	0x00000002, 0x0004001e, 0x0000000b, 0x00000007, 0x0000000a, 0x00040020, 0x0000000c, 0x00000001,
	0x0000000b, 0x0004003b, 0x0000000c, 0x0000000d, 0x00000001, 0x00040015, 0x0000000e, 0x00000020,
	0x00000001, 0x0004002b, 0x0000000e, 0x0000000f, 0x00000000, 0x00040020, 0x00000010, 0x00000001,
	0x00000007, 0x00090019, 0x00000013, 0x00000006, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
	0x00000001, 0x00000000, 0x0003001b, 0x00000014, 0x00000013, 0x00040020, 0x00000015, 0x00000000,
	0x00000014, 0x0004003b, 0x00000015, 0x00000016, 0x00000000, 0x0004002b, 0x0000000e, 0x00000018,
	0x00000001, 0x00040020, 0x00000019, 0x00000001, 0x0000000a, 0x00050036, 0x00000002, 0x00000004,
	0x00000000, 0x00000003, 0x000200f8, 0x00000005, 0x00050041, 0x00000010, 0x00000011, 0x0000000d,
	0x0000000f, 0x0004003d, 0x00000007, 0x00000012, 0x00000011, 0x0004003d, 0x00000014, 0x00000017,
	0x00000016, 0x00050041, 0x00000019, 0x0000001a, 0x0000000d, 0x00000018, 0x0004003d, 0x0000000a,
	0x0000001b, 0x0000001a, 0x00050057, 0x00000007, 0x0000001c, 0x00000017, 0x0000001b, 0x00050085,
	0x00000007, 0x0000001d, 0x00000012, 0x0000001c, 0x0003003e, 0x00000009, 0x0000001d, 0x000100fd,
	0x00010038
};

BalEditor::CInterface::CInterface()
	: m_pMain{ nullptr }
	, m_pGameView{ nullptr }
	, m_pAssetBrowser{ nullptr }
	, m_pSceneHierarchy{ nullptr }
	, m_descriptorPool{ nullptr }
	, m_descriptorSetLayout{ nullptr }
	, m_descriptorSet{ nullptr }
	, m_pipelineCache{ nullptr }
	, m_pipelineLayout{ nullptr }
	, m_pipeline{ nullptr }
	, m_shaderModuleVert{ nullptr }
	, m_shaderModuleFrag{ nullptr }
	, m_pDevice{ nullptr }
	, m_pResource{ nullptr }
	, m_pView{ nullptr }
	, m_pSampler{ nullptr }
	, m_pVertexBuffer{ nullptr }
	, m_pIndexBuffer{ nullptr }
	, m_pWindow{ nullptr }
	, m_vertexCount{ 0 }
	, m_indexCount{ 0 }
{
}


void BalEditor::CInterface::Initialize( SDL_Window* pWindow, const int32_t w, const int32_t h, const BalVulkan::CDevice* pDevice, const BalVulkan::CQueue* pQueue, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CFrameBuffer* pFrameBuffer )
{
	m_pWindow = pWindow;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForVulkan( pWindow );
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	io.DisplaySize = ImVec2( static_cast<float>( w ), static_cast<float>( h ) );
	io.DisplayFramebufferScale = ImVec2( 1.0f, 1.0f );
	//{
	//	io.KeyMap[ImGuiKey_Tab] = SDL_GetScancodeFromKey( SDLK_TAB );
	//	io.KeyMap[ImGuiKey_LeftArrow] = SDL_GetScancodeFromKey( SDLK_LEFT );
	//	io.KeyMap[ImGuiKey_RightArrow] = SDL_GetScancodeFromKey( SDLK_RIGHT );
	//	io.KeyMap[ImGuiKey_UpArrow] = SDL_GetScancodeFromKey( SDLK_UP );
	//	io.KeyMap[ImGuiKey_DownArrow] = SDL_GetScancodeFromKey( SDLK_DOWN );
	//	io.KeyMap[ImGuiKey_Home] = SDL_GetScancodeFromKey( SDLK_HOME );
	//	io.KeyMap[ImGuiKey_End] = SDL_GetScancodeFromKey( SDLK_END );
	//	io.KeyMap[ImGuiKey_Delete] = SDL_GetScancodeFromKey( SDLK_DELETE );
	//	io.KeyMap[ImGuiKey_Backspace] = SDL_GetScancodeFromKey( SDLK_BACKSPACE );
	//	io.KeyMap[ImGuiKey_Enter] = SDL_GetScancodeFromKey( SDLK_RETURN );
	//	io.KeyMap[ImGuiKey_Escape] = SDL_GetScancodeFromKey( SDLK_ESCAPE );
	//	io.KeyMap[ImGuiKey_Space] = SDL_GetScancodeFromKey( SDLK_SPACE );
	//	io.KeyMap[ImGuiKey_A] = SDLK_a;
	//	io.KeyMap[ImGuiKey_C] = SDLK_c;
	//	io.KeyMap[ImGuiKey_V] = SDLK_v;
	//	io.KeyMap[ImGuiKey_X] = SDLK_x;
	//	io.KeyMap[ImGuiKey_Y] = SDLK_y;
	//	io.KeyMap[ImGuiKey_Z] = SDLK_z;
	//}
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	SetImGuiStyle();

	// Create font texture
	unsigned char* fontData;
	int texWidth, texHeight;
	io.Fonts->GetTexDataAsRGBA32( &fontData, &texWidth, &texHeight );
	const uint64_t uploadSize = texWidth * texHeight * 4 * sizeof( char );

	BalVulkan::CBuffer* pStagingBuffer = BalVulkan::CBuffer::CreateNew( pDevice, pCommandPool, pQueue );
	pStagingBuffer->Initialize( uploadSize, BalVulkan::EBufferUsageFlagBits::TransferSrcBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
	pStagingBuffer->UpdateData( fontData, uploadSize );

	m_pResource = BalVulkan::CImageResource::CreateNew( pDevice );
	m_pResource->Initialize( texWidth, texHeight, 1, 1, BalVulkan::EImageLayout::Undefined, BalVulkan::EFormat::R8G8B8A8Unorm, BalVulkan::EImageUsageFlagBits::TransferDstBit | BalVulkan::EImageUsageFlagBits::SampledBit );
	m_pResource->TransitionImageLayout( 1u, pStagingBuffer, BalVulkan::EImageLayout::TransferDstOptimal );
	pStagingBuffer->CopyBufferToImage( m_pResource );
	m_pResource->TransitionImageLayout( 1u, pStagingBuffer, BalVulkan::EImageLayout::ShaderReadOnlyOptimal );

	pStagingBuffer->Release();
	m_pSampler = BalVulkan::CSampler::CreateNew( pDevice );
	m_pSampler->Initialize( m_pResource->GetMipCount() );
	m_pView = BalVulkan::CImageView::CreateNew( *m_pResource, BalVulkan::EImageViewType::View2D, 0u, m_pResource->GetMipCount(), 0u, m_pResource->GetLayerCount() );

	m_pVertexBuffer = BalVulkan::CBuffer::CreateNew( pDevice, pCommandPool, pQueue );
	m_pIndexBuffer = BalVulkan::CBuffer::CreateNew( pDevice, pCommandPool, pQueue );

	// Descriptor pool
	std::vector poolSizes{
		VkDescriptorPoolSize{
			.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = 1,
		}
	};
	VkDescriptorPoolCreateInfo descriptorPoolInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.maxSets = 1,
		.poolSizeCount = static_cast<uint32_t>( poolSizes.size() ),
		.pPoolSizes = poolSizes.data(),
	};
	vkCreateDescriptorPool( pDevice->GetVkDevice(), &descriptorPoolInfo, nullptr, &m_descriptorPool );

	// Descriptor set layout
	std::vector setLayoutBindings = {
		VkDescriptorSetLayoutBinding{
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
		}
	};
	VkDescriptorSetLayoutCreateInfo descriptorLayout{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount = static_cast<uint32_t>( setLayoutBindings.size() ),
		.pBindings = setLayoutBindings.data(),
	};
	vkCreateDescriptorSetLayout( pDevice->GetVkDevice(), &descriptorLayout, nullptr, &m_descriptorSetLayout );

	// Descriptor set
	VkDescriptorSetAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = m_descriptorPool,
		.descriptorSetCount = 1,
		.pSetLayouts = &m_descriptorSetLayout,
	};
	vkAllocateDescriptorSets( pDevice->GetVkDevice(), &allocInfo, &m_descriptorSet );
	VkDescriptorImageInfo fontDescriptor{
		.sampler = m_pSampler->GetSampler(),
		.imageView = m_pView->GetImageView(),
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
	};
	std::vector writeDescriptorSets
	{
		VkWriteDescriptorSet{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = m_descriptorSet,
			.dstBinding = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.pImageInfo = &fontDescriptor,
		}
	};
	vkUpdateDescriptorSets( pDevice->GetVkDevice(), static_cast<uint32_t>( writeDescriptorSets.size() ), writeDescriptorSets.data(), 0, nullptr );

	// Pipeline cache
	VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
	pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	vkCreatePipelineCache( pDevice->GetVkDevice(), &pipelineCacheCreateInfo, nullptr, &m_pipelineCache );

	// Pipeline layout
	// Push constants for UI rendering parameters
	VkPushConstantRange pushConstantRange{
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
		.offset = 0,
		.size = sizeof( float ) * 4,
	};
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 1,
		.pSetLayouts = &m_descriptorSetLayout,
		.pushConstantRangeCount = 1,
		.pPushConstantRanges = &pushConstantRange,
	};
	vkCreatePipelineLayout( pDevice->GetVkDevice(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout );

	// Setup graphics pipeline for UI rendering
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.flags = 0,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE,
	};
	VkPipelineRasterizationStateCreateInfo rasterizationState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.flags = 0,
		.depthClampEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_NONE,
		.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
		.lineWidth = 1.0f,
	};
	// Enable blending
	VkPipelineColorBlendAttachmentState blendAttachmentState{
		.blendEnable = VK_TRUE,
		.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
		.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		.colorBlendOp = VK_BLEND_OP_ADD,
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
		.alphaBlendOp = VK_BLEND_OP_ADD,
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
	};
	VkPipelineColorBlendStateCreateInfo colorBlendState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.attachmentCount = 1,
		.pAttachments = &blendAttachmentState,
	};

	VkPipelineDepthStencilStateCreateInfo depthStencilState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable = VK_FALSE,
		.depthWriteEnable = VK_FALSE,
		.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
		.back{ .compareOp = VK_COMPARE_OP_ALWAYS, },
	};

	VkPipelineViewportStateCreateInfo viewportState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.flags = 0,
		.viewportCount = 1,
		.scissorCount = 1,
	};
	VkPipelineMultisampleStateCreateInfo multisampleState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.flags = 0,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
	};

	std::vector dynamicStateEnables = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamicState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.flags = 0,
		.dynamicStateCount = static_cast<uint32_t>( dynamicStateEnables.size() ),
		.pDynamicStates = dynamicStateEnables.data(),
	};

	std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};

	VkGraphicsPipelineCreateInfo pipelineCreateInfo{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.flags = 0,
		.stageCount = static_cast<uint32_t>( shaderStages.size() ),
		.pStages = shaderStages.data(),
		.pInputAssemblyState = &inputAssemblyState,
		.pViewportState = &viewportState,
		.pRasterizationState = &rasterizationState,
		.pMultisampleState = &multisampleState,
		.pDepthStencilState = &depthStencilState,
		.pColorBlendState = &colorBlendState,
		.pDynamicState = &dynamicState,
		.layout = m_pipelineLayout,
		.renderPass = pFrameBuffer->GetRenderPass(),
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = -1,
	};

	// Vertex bindings an attributes based on ImGui vertex definition
	std::vector vertexInputBindings{
		VkVertexInputBindingDescription{
			.binding = 0,
			.stride = sizeof( ImDrawVert ),
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
		}
	};
	std::vector vertexInputAttributes{
		VkVertexInputAttributeDescription{
			// Location 0: Position
			.location = 0,
			.binding = 0,
			.format = VK_FORMAT_R32G32_SFLOAT,
			.offset = offsetof( ImDrawVert, pos ),
		},
		VkVertexInputAttributeDescription{
			// Location 1: UV
			.location = 1,
			.binding = 0,
			.format = VK_FORMAT_R32G32_SFLOAT,
			.offset = offsetof( ImDrawVert, uv ),
		},
		VkVertexInputAttributeDescription{
			// Location 0: Color
			.location = 2,
			.binding = 0,
			.format = VK_FORMAT_R8G8B8A8_UNORM,
			.offset = offsetof( ImDrawVert, col ),
		}
	};
	VkPipelineVertexInputStateCreateInfo vertexInputState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = static_cast<uint32_t>( vertexInputBindings.size() ),
		.pVertexBindingDescriptions = vertexInputBindings.data(),
		.vertexAttributeDescriptionCount = static_cast<uint32_t>( vertexInputAttributes.size() ),
		.pVertexAttributeDescriptions = vertexInputAttributes.data(),

	};
	pipelineCreateInfo.pVertexInputState = &vertexInputState;

	VkShaderModuleCreateInfo vertInfo = {};
	vertInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vertInfo.codeSize = sizeof( g_glslShaderVertSpv );
	vertInfo.pCode = static_cast<uint32_t*>( g_glslShaderVertSpv );
	vkCreateShaderModule( pDevice->GetVkDevice(), &vertInfo, nullptr, &m_shaderModuleVert );

	VkShaderModuleCreateInfo fragInfo = {};
	fragInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	fragInfo.codeSize = sizeof( g_glslShaderFragSpv );
	fragInfo.pCode = static_cast<uint32_t*>( g_glslShaderFragSpv );
	vkCreateShaderModule( pDevice->GetVkDevice(), &fragInfo, nullptr, &m_shaderModuleFrag );

	shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStages[0].module = m_shaderModuleVert;
	shaderStages[0].pName = "main";
	shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStages[1].module = m_shaderModuleFrag;
	shaderStages[1].pName = "main";

	vkCreateGraphicsPipelines( pDevice->GetVkDevice(), m_pipelineCache, 1, &pipelineCreateInfo, nullptr, &m_pipeline );

	m_pDevice = pDevice;
	m_pMain = new CMainScreen{};
	m_pGameView = new CGameView{};
	m_pAssetBrowser = new CAssetBrowser{};
	m_pSceneHierarchy = new CSceneHierarchy{};
}

void BalEditor::CInterface::Draw( BalVulkan::CCommandPool* pCommandPool )
{
	//ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	m_pMain->Draw();
	m_pGameView->Draw();
	m_pAssetBrowser->Draw();
	m_pSceneHierarchy->Draw();

	ImGui::Render();
	UpdateBuffers();
	FrameRender( pCommandPool );
}

void BalEditor::CInterface::Cleanup() const
{
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	vkDestroyShaderModule( m_pDevice->GetVkDevice(), m_shaderModuleVert, nullptr );
	vkDestroyShaderModule( m_pDevice->GetVkDevice(), m_shaderModuleFrag, nullptr );
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();
	m_pResource->Release();
	m_pView->Release();
	m_pSampler->Release();
	vkDestroyPipelineCache( m_pDevice->GetVkDevice(), m_pipelineCache, nullptr );
	vkDestroyPipeline( m_pDevice->GetVkDevice(), m_pipeline, nullptr );
	vkDestroyPipelineLayout( m_pDevice->GetVkDevice(), m_pipelineLayout, nullptr );
	vkDestroyDescriptorPool( m_pDevice->GetVkDevice(), m_descriptorPool, nullptr );
	vkDestroyDescriptorSetLayout( m_pDevice->GetVkDevice(), m_descriptorSetLayout, nullptr );

	delete m_pMain;
	delete m_pGameView;
	delete m_pAssetBrowser;
	delete m_pSceneHierarchy;
}

void BalEditor::CInterface::ProcessEvent( SDL_Event e ) const
{
	ImGui_ImplSDL2_ProcessEvent( &e );
	switch ( e.type )
	{
		case SDL_DROPFILE:
			char* droppedFileDir = e.drop.file;
			assert( ImportFile( droppedFileDir ) );
		// Shows directory of dropped file
			SDL_free( droppedFileDir ); // Free dropped_filedir memory
	}
}

void BalEditor::CInterface::SetContext( IScene* pScene ) const
{
	m_pMain->SetContext( pScene );
	m_pSceneHierarchy->SetContext( pScene );
}

void BalEditor::CInterface::Resize( const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue )
{
	m_pVertexBuffer->Release();
	m_pIndexBuffer->Release();

	m_pVertexBuffer = BalVulkan::CBuffer::CreateNew( m_pDevice, pCommandPool, pQueue );
	m_pIndexBuffer = BalVulkan::CBuffer::CreateNew( m_pDevice, pCommandPool, pQueue );
}

void BalEditor::CInterface::UpdateBuffers()
{
	const ImDrawData* imDrawData = ImGui::GetDrawData();
	// Note: Alignment is done inside buffer creation
	const VkDeviceSize vertexBufferSize = imDrawData->TotalVtxCount * sizeof( ImDrawVert );
	const VkDeviceSize indexBufferSize = imDrawData->TotalIdxCount * sizeof( ImDrawIdx );

	if ( ( vertexBufferSize == 0 ) || ( indexBufferSize == 0 ) )
	{
		return;
	}

	// Update buffers only if vertex or index count has been changed compared to current buffer size

	// Vertex buffer
	if ( ( m_pVertexBuffer->GetBuffer() == VK_NULL_HANDLE ) || ( m_vertexCount != imDrawData->TotalVtxCount ) )
	{
		m_pVertexBuffer->Unmapped();
		m_pVertexBuffer->Rebuild( vertexBufferSize, BalVulkan::EBufferUsageFlagBits::VertexBufferBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit );
		m_vertexCount = imDrawData->TotalVtxCount;
		m_pVertexBuffer->Map();
	}

	// Index buffer
	if ( ( m_pIndexBuffer->GetBuffer() == VK_NULL_HANDLE ) || ( m_indexCount < imDrawData->TotalIdxCount ) )
	{
		m_pIndexBuffer->Rebuild( indexBufferSize, BalVulkan::EBufferUsageFlagBits::IndexBufferBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit );
		m_indexCount = imDrawData->TotalIdxCount;
		m_pIndexBuffer->Map();
	}

	// Upload data
	auto vtxDst = static_cast<ImDrawVert*>( m_pVertexBuffer->GetMapped() );
	auto idxDst = static_cast<ImDrawIdx*>( m_pIndexBuffer->GetMapped() );

	for ( int n = 0; n < imDrawData->CmdListsCount; n++ )
	{
		const ImDrawList* cmd_list = imDrawData->CmdLists[n];
		memcpy( vtxDst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof( ImDrawVert ) );
		memcpy( idxDst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof( ImDrawIdx ) );
		vtxDst += cmd_list->VtxBuffer.Size;
		idxDst += cmd_list->IdxBuffer.Size;
	}

	// Flush to make writes visible to GPU
	m_pVertexBuffer->Flush();
	m_pIndexBuffer->Flush();

	//for ( int n = 0; n < imDrawData->CmdListsCount; n++ ) {
	//	const ImDrawList* cmd_list = imDrawData->CmdLists[n];
	//	m_pIndexBuffer->UpdateData( cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof( ImDrawVert ) );
	//	memcpy( idxDst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof( ImDrawIdx ) );
	//}
}

void BalEditor::CInterface::FrameRender( BalVulkan::CCommandPool* pCommandPool ) const
{
	const ImGuiIO& io = ImGui::GetIO();

	vkCmdBindDescriptorSets( pCommandPool->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descriptorSet, 0, nullptr );
	vkCmdBindPipeline( pCommandPool->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline );

	const VkViewport viewport{
		.width = ImGui::GetIO().DisplaySize.x,
		.height = ImGui::GetIO().DisplaySize.y,
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};
	vkCmdSetViewport( pCommandPool->GetCommandBuffer(), 0, 1, &viewport );

	// UI scale and translate via push constants
	const float pushConstBlock[4]{ 2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y, -1.f, -1.f };
	vkCmdPushConstants( pCommandPool->GetCommandBuffer(), m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( float ) * 4, &pushConstBlock );

	// Render commands
	const ImDrawData* imDrawData = ImGui::GetDrawData();

	if ( imDrawData->CmdListsCount > 0 )
	{
		int32_t indexOffset = 0;
		int32_t vertexOffset = 0;
		constexpr VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers( pCommandPool->GetCommandBuffer(), 0, 1, &m_pVertexBuffer->GetBuffer(), offsets );
		vkCmdBindIndexBuffer( pCommandPool->GetCommandBuffer(), m_pIndexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16 );

		for ( int32_t i = 0; i < imDrawData->CmdListsCount; i++ )
		{
			const ImDrawList* cmdList = imDrawData->CmdLists[i];
			for ( int32_t j = 0; j < cmdList->CmdBuffer.Size; j++ )
			{
				const ImDrawCmd* pcmd = &cmdList->CmdBuffer[j];
				VkRect2D scissorRect{};
				scissorRect.offset.x = std::max( static_cast<int32_t>( pcmd->ClipRect.x ), 0 );
				scissorRect.offset.y = std::max( static_cast<int32_t>( pcmd->ClipRect.y ), 0 );
				scissorRect.extent.width = static_cast<uint32_t>( pcmd->ClipRect.z - pcmd->ClipRect.x );
				scissorRect.extent.height = static_cast<uint32_t>( pcmd->ClipRect.w - pcmd->ClipRect.y );
				vkCmdSetScissor( pCommandPool->GetCommandBuffer(), 0, 1, &scissorRect );
				vkCmdDrawIndexed( pCommandPool->GetCommandBuffer(), pcmd->ElemCount, 1, indexOffset, vertexOffset, 0 );
				indexOffset += pcmd->ElemCount;
			}
			vertexOffset += cmdList->VtxBuffer.Size;
		}
	}
}


void BalEditor::CInterface::SetImGuiStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4( 1.000f, 1.000f, 1.000f, 1.000f );
	colors[ImGuiCol_TextDisabled] = ImVec4( 0.500f, 0.500f, 0.500f, 1.000f );
	colors[ImGuiCol_WindowBg] = ImVec4( 0.180f, 0.180f, 0.180f, 1.000f );
	colors[ImGuiCol_ChildBg] = ImVec4( 0.280f, 0.280f, 0.280f, 0.000f );
	colors[ImGuiCol_PopupBg] = ImVec4( 0.313f, 0.313f, 0.313f, 1.000f );
	colors[ImGuiCol_Border] = ImVec4( 0.266f, 0.266f, 0.266f, 1.000f );
	colors[ImGuiCol_BorderShadow] = ImVec4( 0.000f, 0.000f, 0.000f, 0.000f );
	colors[ImGuiCol_FrameBg] = ImVec4( 0.160f, 0.160f, 0.160f, 1.000f );
	colors[ImGuiCol_FrameBgHovered] = ImVec4( 0.200f, 0.200f, 0.200f, 1.000f );
	colors[ImGuiCol_FrameBgActive] = ImVec4( 0.280f, 0.280f, 0.280f, 1.000f );
	colors[ImGuiCol_TitleBg] = ImVec4( 0.148f, 0.148f, 0.148f, 1.000f );
	colors[ImGuiCol_TitleBgActive] = ImVec4( 0.148f, 0.148f, 0.148f, 1.000f );
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4( 0.148f, 0.148f, 0.148f, 1.000f );
	colors[ImGuiCol_MenuBarBg] = ImVec4( 0.195f, 0.195f, 0.195f, 1.000f );
	colors[ImGuiCol_ScrollbarBg] = ImVec4( 0.160f, 0.160f, 0.160f, 1.000f );
	colors[ImGuiCol_ScrollbarGrab] = ImVec4( 0.277f, 0.277f, 0.277f, 1.000f );
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4( 0.300f, 0.300f, 0.300f, 1.000f );
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_CheckMark] = ImVec4( 1.000f, 1.000f, 1.000f, 1.000f );
	colors[ImGuiCol_SliderGrab] = ImVec4( 0.391f, 0.391f, 0.391f, 1.000f );
	colors[ImGuiCol_SliderGrabActive] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_Button] = ImVec4( 1.000f, 1.000f, 1.000f, 0.000f );
	colors[ImGuiCol_ButtonHovered] = ImVec4( 1.000f, 1.000f, 1.000f, 0.156f );
	colors[ImGuiCol_ButtonActive] = ImVec4( 1.000f, 1.000f, 1.000f, 0.391f );
	colors[ImGuiCol_Header] = ImVec4( 0.313f, 0.313f, 0.313f, 1.000f );
	colors[ImGuiCol_HeaderHovered] = ImVec4( 0.469f, 0.469f, 0.469f, 1.000f );
	colors[ImGuiCol_HeaderActive] = ImVec4( 0.469f, 0.469f, 0.469f, 1.000f );
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4( 0.391f, 0.391f, 0.391f, 1.000f );
	colors[ImGuiCol_SeparatorActive] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_ResizeGrip] = ImVec4( 1.000f, 1.000f, 1.000f, 0.250f );
	colors[ImGuiCol_ResizeGripHovered] = ImVec4( 1.000f, 1.000f, 1.000f, 0.670f );
	colors[ImGuiCol_ResizeGripActive] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_Tab] = ImVec4( 0.098f, 0.098f, 0.098f, 1.000f );
	colors[ImGuiCol_TabHovered] = ImVec4( 0.352f, 0.352f, 0.352f, 1.000f );
	colors[ImGuiCol_TabActive] = ImVec4( 0.195f, 0.195f, 0.195f, 1.000f );
	colors[ImGuiCol_TabUnfocused] = ImVec4( 0.098f, 0.098f, 0.098f, 1.000f );
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4( 0.195f, 0.195f, 0.195f, 1.000f );
	colors[ImGuiCol_DockingPreview] = ImVec4( 0.038f, 0.420f, 0.000f, 0.781f );
	colors[ImGuiCol_DockingEmptyBg] = ImVec4( 0.180f, 0.180f, 0.180f, 1.000f );
	colors[ImGuiCol_PlotLines] = ImVec4( 0.469f, 0.469f, 0.469f, 1.000f );
	colors[ImGuiCol_PlotLinesHovered] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_PlotHistogram] = ImVec4( 0.586f, 0.586f, 0.586f, 1.000f );
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_TextSelectedBg] = ImVec4( 1.000f, 1.000f, 1.000f, 0.156f );
	colors[ImGuiCol_DragDropTarget] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_NavHighlight] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4( 0.000f, 0.000f, 0.000f, 0.586f );
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4( 0.000f, 0.000f, 0.000f, 0.586f );

	style->WindowPadding = ImVec2( 2.0f, 2.0f );
	style->FramePadding = ImVec2( 2.0f, 2.0f );
	style->CellPadding = ImVec2( 2.0f, 2.0f );
	style->ItemSpacing = ImVec2( 4.0f, 4.0f );
	style->ItemInnerSpacing = ImVec2( 4.0f, 4.0f );
	style->TouchExtraPadding = ImVec2( 0.0f, 0.0f );
	style->IndentSpacing = 22.0f;
	style->ScrollbarSize = 12.0f;
	style->GrabMinSize = 12.0f;

	style->WindowBorderSize = 0.0f;
	style->ChildBorderSize = 1.0f;
	style->PopupBorderSize = 1.0f;
	style->FrameBorderSize = 1.0f;
	style->TabBorderSize = 1.0f;

	style->WindowRounding = 0.0f;
	style->ChildRounding = 4.0f;
	style->FrameRounding = 4.0f;
	style->PopupRounding = 4.0f;
	style->ScrollbarSize = 4.0f;
	style->GrabRounding = 4.0f;
	style->LogSliderDeadzone = 4.0f;
	style->TabRounding = 4.0f;
}
