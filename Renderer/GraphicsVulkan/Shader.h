#pragma once

#include <filesystem>
#include <unordered_set>
#include <shaderc/shaderc.hpp>
#include "Base.h"
#include "Common.h"

#include <vulkan/vulkan.hpp>

namespace BalVulkan
{
	enum class EShaderResourceType
	{
		None,
		Input,
		InputAttachment,
		Output,
		Image,
		ImageSampler,
		ImageStorage,
		Sampler,
		BufferUniform,
		BufferStorage,
		PushConstant,
		SpecializationConstant,
	};
	struct SHaderResourceQualifiers
	{
		enum : uint32_t
		{
			None = 0,
			NonReadable = 1,
			NonWritable = 2,
		};
	};
	enum class EShaderResourceMode
	{
		Static,
		Dynamic,
		UpdateAfterBind
	};
	struct SShaderResource
	{
		VkShaderStageFlags stages;
		EShaderResourceType type;
		EShaderResourceMode mode;
		uint32_t set;
		uint32_t binding;
		uint32_t location;
		uint32_t inputAttachmentIndex;
		uint32_t vecSize;
		uint32_t columns;
		uint32_t arraySize;
		uint32_t offset;
		uint32_t size;
		uint32_t constantId;
		uint32_t qualifiers;
		std::string name;
	};

	enum class EShaderType : uint8_t
	{
		Vertex,
		Geometry,
		Fragment,
		Count
	};

	class CShader final : public CDeviceObject
	{
	public:
		explicit CShader( const CDevice* const device );
		CShader( const CShader& other );
		CShader( CShader&& ) = default;
		CShader& operator=( CShader&& ) = default;
		~CShader() override;

		void Initialize( const void* pShaderCode, size_t shaderCodeSize, BalVulkan::EShaderStage stage, const char* path );
		const std::vector<SShaderResource>& GetShaderResources()const;
		const VkShaderModule& GetShaderModule()const;
		static CShader* CreateNew( const CDevice* pDevice );
	private:
		VkShaderModule m_shaderHandle;
		std::vector<SShaderResource> m_resources;

		VkShaderModule CreateShaderModule( const std::vector<uint32_t>& data ) const;
	};

	class CFileIncluder final : public shaderc::CompileOptions::IncluderInterface
	{
	public:
		explicit CFileIncluder()
		{
		}

		~CFileIncluder() override;

		// Resolves a requested source file of a given type from a requesting
		// source into a shaderc_include_result whose contents will remain valid
		// until it's released.
		shaderc_include_result* GetInclude( const char* requestedSource,
											shaderc_include_type type,
											const char* requestingSource,
											size_t includeDepth ) override;
		// Releases an include result.
		void ReleaseInclude( shaderc_include_result* include_result ) override;

		// Returns a reference to the member storing the set of included files.
		const std::unordered_set<std::string>& FilePathTrace() const
		{
			return m_includedFiles;
		}

	private:
		// The full path and content of a source file.
		struct SFileInfo
		{
			const std::string fullPath;
			std::vector<char> contents;
		};

		// The set of full paths of included files.
		std::unordered_set<std::string> m_includedFiles;
	};
}
