#pragma once
#include <fstream>

#include "CommandPool.h"
#include "FileParcer.h"
#include "Components/TextureComponent.h"

inline BalVulkan::CDevice* g_pDevice{nullptr};
inline BalVulkan::CCommandPool* g_pCommandPool{ nullptr };
inline BalVulkan::CQueue* g_pQueue{ nullptr };

inline void ImportTexture( const std::string_view value, Balbino::CTextureComponent*& pTexture, const VkDescriptorPool descriptorPool = VK_NULL_HANDLE, const VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE )
{
	if ( pTexture )
		return;

	std::ifstream file{ ( value.data() ), std::ios::in | std::ios::binary };
	if ( !file.is_open() )
		return;
	uint8_t fileType;
	BinaryReadWrite::Read( file, fileType );
	if ( ( EFileTypes )fileType != EFileTypes::Image )
		return;

	uint32_t width{};
	uint32_t height{};
	uint32_t mip{};
	uint32_t faces{};
	uint32_t layers{};
	uint32_t layout{};
	uint32_t type{};
	uint32_t format{};
	uint64_t size{};

	BinaryReadWrite::Read( file, width );
	BinaryReadWrite::Read( file, height );
	BinaryReadWrite::Read( file, mip );
	BinaryReadWrite::Read( file, faces );
	BinaryReadWrite::Read( file, layers );
	BinaryReadWrite::Read( file, layout );
	BinaryReadWrite::Read( file, type );
	BinaryReadWrite::Read( file, format );
	BinaryReadWrite::Read( file, size );
	void* pData = malloc( size );
	BinaryReadWrite::Read( file, pData, size );
	//file.read( reinterpret_cast< char* >( pData ), size );
	pTexture = new Balbino::CTextureComponent{ g_pDevice };
	pTexture->Initialize( pData, width, height, mip, layers, faces, size,static_cast<BalVulkan::EImageLayout>( layout ), static_cast<BalVulkan::EImageViewType>( type ), static_cast<BalVulkan::EFormat>( format ), g_pCommandPool, g_pQueue, descriptorPool, descriptorSetLayout );
	free( pData );
	file.close();
}
