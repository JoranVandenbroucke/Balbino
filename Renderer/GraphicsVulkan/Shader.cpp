#include "pch.h"
#include "Shader.h"

#include <fstream>
#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>
#include <spirv_reflect.hpp>

#include "Device.h"
#include "Funtions.h"


BalVulkan::CShader::~CShader()
{
	vkDestroyShaderModule( GetDevice()->GetVkDevice(), m_shaderHandle, nullptr );
	m_resources.clear();
}


BalVulkan::CShader::CShader( const CDevice* const device )
	: CDeviceObject{ device }
	, m_shaderHandle{ nullptr }
{
}

BalVulkan::CShader::CShader( const CShader& other )
	: CDeviceObject{ other.GetDevice() }
	, m_shaderHandle{ other.m_shaderHandle }
	, m_resources{ other.m_resources }
{
}

VkShaderModule BalVulkan::CShader::CreateShaderModule( const std::vector<uint32_t>& data ) const
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = data.size() * sizeof( uint32_t ) / sizeof( uint8_t );
	createInfo.pCode = data.data();

	VkShaderModule shaderModule;
	CheckVkResult( vkCreateShaderModule( GetDevice()->GetVkDevice(), &createInfo, nullptr, &shaderModule ), "failed to create shader module!" );
	//if ( vkCreateShaderModule( GetDevice()->GetVkDevice(), &createInfo, nullptr, &shaderModule ) != VK_SUCCESS )
	//{
	//	throw std::runtime_error( "failed to create shader module!" );
	//}

	return shaderModule;
}

BalVulkan::CFileIncluder::~CFileIncluder() = default;

shaderc_include_result* BalVulkan::CFileIncluder::GetInclude( const char* requestedSource, shaderc_include_type type, const char* requestingSource, size_t includeDepth )
{
	( void ) includeDepth;
	std::filesystem::path src{ requestingSource };
	src.remove_filename();
	const std::filesystem::path dest{ src.remove_filename().string() + requestedSource };
	//const std::string fullPath = ( type == shaderc_include_type_relative ) ? std::filesystem::relative( dest, src ).string() : std::filesystem::absolute( dest ).string();
	const std::string fullPath = ( type == shaderc_include_type_relative ) ? absolute( dest ).string() : requestedSource;
	if ( fullPath.empty() )
		return nullptr;

	// In principle, several threads could be resolving includes at the same
	// time.  Protect the included_files.

	// Read the file and save its full path and contents into stable addresses.
	//SFileInfo* newFileInfo = new SFileInfo{ full_path, {} };
	//if ( !shaderc_util::ReadFile( full_path, &( newFileInfo->contents ) ) ) {
	//return nullptr;
	//}

	std::ifstream newFileInfo( fullPath, std::ios::in | std::ios::binary );
	if ( !newFileInfo.is_open() )
		return nullptr;
	const std::string newFileInfoContent( ( std::istreambuf_iterator<char>( newFileInfo ) ), std::istreambuf_iterator<char>() );
	newFileInfo.close();

	m_includedFiles.insert( fullPath );

	char* pFilePath{ new char[fullPath.size()] };
	char* pData{ new char[newFileInfoContent.size()] };
	memcpy( pFilePath, fullPath.data(), fullPath.size() );
	memcpy( pData, newFileInfoContent.data(), newFileInfoContent.size() );
	return new shaderc_include_result{ pFilePath, fullPath.size(), pData, newFileInfoContent.size(), nullptr };
}

void BalVulkan::CFileIncluder::ReleaseInclude( shaderc_include_result * include_result )
{
	delete[] include_result->source_name;
	delete[] include_result->content;
	delete include_result;
}

void BalVulkan::CShader::Initialize( const void* pShaderCode, size_t shaderCodeSize, BalVulkan::EShaderStage stage, const char* path )
{
	VkShaderStageFlagBits stageBits{};
	shaderc_shader_kind stageKind{};
	switch ( stage )
	{
		case EShaderStage::VertexShader:
			stageBits = VK_SHADER_STAGE_VERTEX_BIT;
			stageKind = shaderc_vertex_shader;
			break;
		case EShaderStage::GeometryShader:
			stageBits = VK_SHADER_STAGE_GEOMETRY_BIT;
			stageKind = shaderc_geometry_shader;
			break;
		case EShaderStage::FragmentShader:
			stageBits = VK_SHADER_STAGE_FRAGMENT_BIT;
			stageKind = shaderc_fragment_shader;
			break;
	}

	const shaderc::Compiler compiler;
	shaderc::CompileOptions options;
	options.SetTargetEnvironment( shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2 );
	options.SetOptimizationLevel( shaderc_optimization_level_performance );
	options.SetIncluder( std::make_unique<CFileIncluder>() );
	const shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv( ( const char* ) pShaderCode, shaderCodeSize, stageKind, path, options );
	if ( result.GetCompilationStatus() != shaderc_compilation_status_success )
	{
		//handle errors
		std::cout << result.GetErrorMessage();
		assert( false );
	}
	const std::vector vertexSPRV( result.cbegin(), result.cend() );

	spirv_cross::CompilerReflection reflection{ vertexSPRV };
	const spirv_cross::ShaderResources& resources{ reflection.get_shader_resources() };
	//std::cout << reflection.compile();

	for ( const auto& stageInput : resources.stage_inputs )
	{
		const auto& bufferType{ reflection.get_type( stageInput.base_type_id ) };
		const auto& spirvType = reflection.get_type_from_variable( stageInput.id );

		SShaderResource shaderResource{};
		shaderResource.type = EShaderResourceType::Input;
		shaderResource.stages = stageBits;

		//see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
		const auto& type = reflection.get_type( stageInput.type_id );
		const bool isPushConstant = reflection.get_storage_class( stageInput.id ) == spv::StorageClassPushConstant;
		const bool isBlock = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) ||
			reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
		const spirv_cross::ID fallbackId = !isPushConstant && isBlock ? spirv_cross::ID( stageInput.base_type_id ) : spirv_cross::ID( stageInput.id );
		shaderResource.name = !stageInput.name.empty() ? stageInput.name : reflection.get_fallback_name( fallbackId );
		//

		shaderResource.vecSize = bufferType.vecsize;
		shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
		shaderResource.location = reflection.get_decoration( stageInput.id, spv::DecorationLocation );

		m_resources.push_back( shaderResource );
	}
	for ( const auto& inputAttachment : resources.subpass_inputs )
	{
		const auto& spirvType = reflection.get_type_from_variable( inputAttachment.id );

		SShaderResource shaderResource{};
		shaderResource.type = EShaderResourceType::InputAttachment;
		shaderResource.stages = stageBits;

		//see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
		const auto& type = reflection.get_type( inputAttachment.type_id );
		const bool isPushConstant = reflection.get_storage_class( inputAttachment.id ) == spv::StorageClassPushConstant;
		const bool isBlock = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) ||
			reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
		const spirv_cross::ID fallbackId = !isPushConstant && isBlock ? spirv_cross::ID( inputAttachment.base_type_id ) : spirv_cross::ID( inputAttachment.id );
		shaderResource.name = !inputAttachment.name.empty() ? inputAttachment.name : reflection.get_fallback_name( fallbackId );
		//

		shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
		shaderResource.inputAttachmentIndex = reflection.get_decoration( inputAttachment.id, spv::DecorationInputAttachmentIndex );
		shaderResource.set = reflection.get_decoration( inputAttachment.id, spv::DecorationDescriptorSet );
		shaderResource.binding = reflection.get_decoration( inputAttachment.id, spv::DecorationBinding );

		m_resources.push_back( shaderResource );
	}
	for ( const auto& output : resources.stage_outputs )
	{
		const auto& bufferType{ reflection.get_type( output.base_type_id ) };
		const auto& spirvType = reflection.get_type_from_variable( output.id );

		SShaderResource shaderResource{};
		shaderResource.type = EShaderResourceType::Output;
		shaderResource.stages = stageBits;

		//see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
		const auto& type = reflection.get_type( output.type_id );
		const bool isPushConstant = reflection.get_storage_class( output.id ) == spv::StorageClassPushConstant;
		const bool isBlock = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) ||
			reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
		const spirv_cross::ID fallbackId = !isPushConstant && isBlock ? spirv_cross::ID( output.base_type_id ) : spirv_cross::ID( output.id );
		shaderResource.name = !output.name.empty() ? output.name : reflection.get_fallback_name( fallbackId );
		//

		shaderResource.vecSize = bufferType.vecsize;
		shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
		shaderResource.location = reflection.get_decoration( output.id, spv::DecorationLocation );

		m_resources.push_back( shaderResource );
	}
	for ( const auto& image : resources.separate_images )
	{
		const auto& spirvType = reflection.get_type_from_variable( image.id );
		SShaderResource shaderResource{};
		shaderResource.type = EShaderResourceType::Image;
		shaderResource.stages = stageBits;
		//shaderResource.name = stageInput.name;
		shaderResource.name = reflection.get_name( image.id );

		shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
		shaderResource.set = reflection.get_decoration( image.id, spv::DecorationDescriptorSet );
		shaderResource.binding = reflection.get_decoration( image.id, spv::DecorationBinding );

		m_resources.push_back( shaderResource );
	}
	for ( const auto& imageSampler : resources.sampled_images )
	{
		const auto& spirvType = reflection.get_type_from_variable( imageSampler.id );
		SShaderResource shaderResource{};
		shaderResource.type = EShaderResourceType::ImageSampler;
		shaderResource.stages = stageBits;

		//see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
		const auto& type = reflection.get_type( imageSampler.type_id );
		const bool isPushConstant = reflection.get_storage_class( imageSampler.id ) == spv::StorageClassPushConstant;
		const bool isBlock = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) ||
			reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
		const spirv_cross::ID fallbackId = !isPushConstant && isBlock ? spirv_cross::ID( imageSampler.base_type_id ) : spirv_cross::ID( imageSampler.id );
		shaderResource.name = !imageSampler.name.empty() ? imageSampler.name : reflection.get_fallback_name( fallbackId );
		//

		shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
		shaderResource.set = reflection.get_decoration( imageSampler.id, spv::DecorationDescriptorSet );
		shaderResource.binding = reflection.get_decoration( imageSampler.id, spv::DecorationBinding );

		m_resources.push_back( shaderResource );
	}
	for ( const auto& imageStorage : resources.storage_images )
	{
		const auto& spirvType = reflection.get_type_from_variable( imageStorage.id );
		SShaderResource shaderResource{};
		shaderResource.type = EShaderResourceType::ImageStorage;
		shaderResource.stages = stageBits;

		//see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
		const auto& type = reflection.get_type( imageStorage.type_id );
		const bool isPushConstant = reflection.get_storage_class( imageStorage.id ) == spv::StorageClassPushConstant;
		const bool isBlock = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) ||
			reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
		const spirv_cross::ID fallbackId = !isPushConstant && isBlock ? spirv_cross::ID( imageStorage.base_type_id ) : spirv_cross::ID( imageStorage.id );
		shaderResource.name = !imageStorage.name.empty() ? imageStorage.name : reflection.get_fallback_name( fallbackId );
		//

		shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
		shaderResource.set = reflection.get_decoration( imageStorage.id, spv::DecorationDescriptorSet );
		shaderResource.binding = reflection.get_decoration( imageStorage.id, spv::DecorationBinding );
		shaderResource.qualifiers = 3;

		m_resources.push_back( shaderResource );
	}
	for ( const auto& sampler : resources.separate_samplers )
	{
		const auto& spirvType = reflection.get_type_from_variable( sampler.id );
		SShaderResource shaderResource{};
		shaderResource.type = EShaderResourceType::Sampler;
		shaderResource.stages = stageBits;

		//see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
		const auto& type = reflection.get_type( sampler.type_id );
		const bool isPushConstant = reflection.get_storage_class( sampler.id ) == spv::StorageClassPushConstant;
		const bool isBlock = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) ||
			reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
		const spirv_cross::ID fallbackId = !isPushConstant && isBlock ? spirv_cross::ID( sampler.base_type_id ) : spirv_cross::ID( sampler.id );
		shaderResource.name = !sampler.name.empty() ? sampler.name : reflection.get_fallback_name( fallbackId );
		//

		shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
		shaderResource.set = reflection.get_decoration( sampler.id, spv::DecorationDescriptorSet );
		shaderResource.binding = reflection.get_decoration( sampler.id, spv::DecorationBinding );

		m_resources.push_back( shaderResource );
	}
	for ( const auto& uniformBuffer : resources.uniform_buffers )
	{
		const auto& bufferType{ reflection.get_type( uniformBuffer.base_type_id ) };
		const uint32_t bufferSize{ static_cast< uint32_t >( reflection.get_declared_struct_size( bufferType ) ) };
		const uint32_t set{ reflection.get_decoration( uniformBuffer.id, spv::DecorationDescriptorSet ) };
		const uint32_t binding{ reflection.get_decoration( uniformBuffer.id, spv::DecorationBinding ) };
		const auto& spirvType = reflection.get_type_from_variable( uniformBuffer.id );

		SShaderResource shaderResource{};
		shaderResource.type = EShaderResourceType::BufferUniform;
		shaderResource.stages = stageBits;

		//see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
		const auto& type = reflection.get_type( uniformBuffer.type_id );
		const bool isPushConstant = reflection.get_storage_class( uniformBuffer.id ) == spv::StorageClassPushConstant;
		const bool isBlock = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) ||
			reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
		const spirv_cross::ID fallbackId = !isPushConstant && isBlock ? spirv_cross::ID( uniformBuffer.base_type_id ) : spirv_cross::ID( uniformBuffer.id );
		shaderResource.name = !uniformBuffer.name.empty() ? uniformBuffer.name : reflection.get_fallback_name( fallbackId );
		//

		shaderResource.size = bufferSize;
		shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
		shaderResource.set = set;
		shaderResource.binding = binding;

		m_resources.push_back( shaderResource );
	}
	for ( const auto& storageBuffer : resources.storage_buffers )
	{
		const auto& spirvType = reflection.get_type_from_variable( storageBuffer.id );
		const uint32_t bufferSize{ static_cast< uint32_t >( reflection.get_declared_struct_size( spirvType ) ) };
		const uint32_t set{ reflection.get_decoration( storageBuffer.id, spv::DecorationDescriptorSet ) };
		const uint32_t binding{ reflection.get_decoration( storageBuffer.id, spv::DecorationBinding ) };

		SShaderResource shaderResource{};
		shaderResource.type = EShaderResourceType::Sampler;
		shaderResource.stages = stageBits;

		//see spirv_reflect.cpp emit_resources(const char *tag, const SmallVector<Resource> &resources) at line 552
		const auto& type = reflection.get_type( storageBuffer.type_id );
		const bool isPushConstant = reflection.get_storage_class( storageBuffer.id ) == spv::StorageClassPushConstant;
		const bool isBlock = reflection.get_decoration_bitset( type.self ).get( spv::DecorationBlock ) ||
			reflection.get_decoration_bitset( type.self ).get( spv::DecorationBufferBlock );
		const spirv_cross::ID fallbackId = !isPushConstant && isBlock ? spirv_cross::ID( storageBuffer.base_type_id ) : spirv_cross::ID( storageBuffer.id );
		shaderResource.name = !storageBuffer.name.empty() ? storageBuffer.name : reflection.get_fallback_name( fallbackId );
		//

		shaderResource.size = bufferSize;
		shaderResource.arraySize = !spirvType.array.empty() ? spirvType.array[0] : 1;
		shaderResource.set = set;
		shaderResource.binding = binding;
		shaderResource.qualifiers = 3;
		m_resources.push_back( shaderResource );
	}

	//const std::string source{ reflection.compile() };
	m_shaderHandle = CreateShaderModule( vertexSPRV );
}

const std::vector<BalVulkan::SShaderResource>& BalVulkan::CShader::GetShaderResources() const
{
	return m_resources;
}

const VkShaderModule& BalVulkan::CShader::GetShaderModule() const
{
	return m_shaderHandle;
}

BalVulkan::CShader* BalVulkan::CShader::CreateNew( const CDevice * pDevice )
{
	return new CShader{ pDevice };
}
