#include "pch.h"
#include "ResourceManager.h"

#include "FileParcer.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/UBOStructs.h"

#include <fstream>

#include "IScene.h"

void CResourceManager::Initialize( const ISystem* pRenderer )
{
	m_pSystem = pRenderer;
}

void CResourceManager::Cleanup()
{
	for ( const auto& [fst, snd] : m_loadedMeshMap )
	{
		snd->Cleanup();
		delete snd;
	}
	for ( const auto& [fst, snd] : m_loadedMaterialMap )
	{
		snd->Cleanup();
		delete snd;
	}
	for ( const auto& [fst, snd] : m_loadedShaderMap )
	{
		//snd->Cleanup();
		snd->Release();
	}
	for ( const auto& [fst, snd] : m_loadedTextureMap )
	{
		snd->Cleanup();
		delete snd;
	}
	m_files.clear();
	m_loadedMeshMap.clear();
	m_loadedMaterialMap.clear();
	m_loadedShaderMap.clear();
	m_loadedTextureMap.clear();
}

Balbino::CTexture* CResourceManager::LoadTexture( const std::string_view assetPath )
{
	std::ifstream file( assetPath.data(), std::ios::in | std::ios::binary );
	if ( !file.is_open() )
		return nullptr;

	uint64_t uuid;
	BinaryReadWrite::Read( file, uuid );
	//BinaryReadWrite::Write( file, CUuid() );
	if ( m_loadedTextureMap.contains( uuid ) )
		return m_loadedTextureMap.at( uuid );
	uint8_t type;
	BinaryReadWrite::Read( file, type );
	//BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Image ) );
	if ( static_cast< EFileTypes >( type ) == EFileTypes::Image )
	{
		const auto pTextureComponent = new Balbino::CTexture{ g_pDevice, uuid };
		uint32_t width{};
		uint32_t height{};
		uint32_t mip{};
		uint32_t faces{};
		uint32_t layers{};
		uint32_t layout{};
		uint32_t viewType{};
		uint32_t format{};
		uint64_t size{};

		BinaryReadWrite::Read( file, width );
		//BinaryReadWrite::Write( file, pSurface->w );
		BinaryReadWrite::Read( file, height );
		//BinaryReadWrite::Write( file, pSurface->h );
		BinaryReadWrite::Read( file, mip );
		//BinaryReadWrite::Write( file, 1 );
		BinaryReadWrite::Read( file, faces );
		//BinaryReadWrite::Write( file, 1 );
		BinaryReadWrite::Read( file, layers );
		//BinaryReadWrite::Write( file, 1 );
		BinaryReadWrite::Read( file, layout );
		//BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EImageLayout::Undefined ) );
		BinaryReadWrite::Read( file, viewType );
		//BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EImageViewType::View2D ) );
		BinaryReadWrite::Read( file, format );
		//BinaryReadWrite::Write( file, static_cast< uint32_t >( BalVulkan::EFormat::R8G8B8A8Srgb ) );
		BinaryReadWrite::Read( file, size );
		//BinaryReadWrite::Write( file, static_cast< uint64_t >( pSurface->h ) * pSurface->pitch );
		void* pImageData = malloc( size );
		BinaryReadWrite::Read( file, pImageData, size );
		//BinaryReadWrite::Write( file, static_cast< uint8_t* >( pSurface->pixels ), static_cast< uint64_t >( pSurface->h ) * pSurface->pitch );
		pTextureComponent->Initialize( pImageData, width, height, mip, layers, faces, size, BalVulkan::EImageLayout( layout ), BalVulkan::EImageViewType( viewType ), BalVulkan::EFormat( format ), g_pCommandPool, g_pQueue );
		free( pImageData );
		m_loadedTextureMap[uuid] = pTextureComponent;
		file.close();
		return pTextureComponent;
	}
	file.close();
	return nullptr;
}

BalVulkan::CShaderPipeline* CResourceManager::LoadShader( std::string_view assetPath )
{
	std::ifstream file( assetPath.data(), std::ios::in | std::ios::binary );
	if ( !file.is_open() )
		return nullptr;

	uint64_t uuid;
	BinaryReadWrite::Read( file, uuid );
	if ( m_loadedShaderMap.contains( uuid ) )
		return m_loadedShaderMap.at( uuid );
	uint8_t type;
	BinaryReadWrite::Read( file, type );
	//BinaryReadWrite::Write( file, static_cast< uint8_t >( EFileTypes::Image ) );
	if ( static_cast< EFileTypes >( type ) == EFileTypes::Shader )
	{
		uint64_t size1;
		uint64_t size2;
		uint64_t size3;
		BinaryReadWrite::Read( file, size1 );
		BinaryReadWrite::Read( file, size2 );
		BinaryReadWrite::Read( file, size3 );
		BinaryReadWrite::MoveCursor( file, static_cast< int >( size1 ) );

		void* pData = malloc( sizeof uint32_t * size2 );
		BinaryReadWrite::Read( file, pData, sizeof uint32_t * size2 );
		std::vector<uint32_t> vertexData( size2, 0 );
		vertexData.assign( ( uint32_t* ) pData, ( uint32_t* ) pData + size2 );
		free( pData );

		pData = malloc( sizeof uint32_t * size3 );
		std::vector<uint32_t> fragmentData( size3, 0 );
		BinaryReadWrite::Read( file, pData, sizeof uint32_t * size3 );
		fragmentData.assign( ( uint32_t* ) pData, ( uint32_t* ) pData + size3 );
		free( pData );

		BalVulkan::CShader* pVertexShader = new BalVulkan::CShader{ g_pDevice };
		BalVulkan::CShader* pFragmentShader = new BalVulkan::CShader{ g_pDevice };
		BalVulkan::CShaderPipeline* pPipeline = BalVulkan::CShaderPipeline::CreateNew( g_pDevice );
		pVertexShader->Initialize( vertexData.data(), size2, BalVulkan::EShaderStage::VertexShader );
		pFragmentShader->Initialize( fragmentData.data(), size3, BalVulkan::EShaderStage::FragmentShader );
		pPipeline->Initialize( { pVertexShader, nullptr, pFragmentShader }, *g_pFrameBuffer, { BalVulkan::EVertexComponent::Position, BalVulkan::EVertexComponent::Color, BalVulkan::EVertexComponent::UV, BalVulkan::EVertexComponent::Normal, BalVulkan::EVertexComponent::Tangent }, 1, g_pSwapChain );
		pVertexShader->Release();
		pFragmentShader->Release();

		file.close();
		m_loadedShaderMap[uuid] = pPipeline;
		return pPipeline;
	}
	file.close();
	return nullptr;
}

Balbino::CMaterial* CResourceManager::LoadMaterial( std::string_view assetPath )
{
	std::ifstream file( assetPath.data(), std::ios::in | std::ios::binary );
	if ( !file.is_open() )
		return nullptr;

	uint64_t uuid;
	BinaryReadWrite::Read( file, uuid );
	if ( m_loadedMaterialMap.contains( uuid ) )
		return m_loadedMaterialMap.at( uuid );
	uint8_t type;
	BinaryReadWrite::Read( file, type );
	if ( static_cast< EFileTypes >( type ) == EFileTypes::Material )
	{
		uint64_t shaderId;
		bool hasOnlyShaderResources;
		std::vector<BalVulkan::SShaderResource> shaderResources;
		std::vector<BalVulkan::SDescriptorSet> descriptorSets;
		BinaryReadWrite::Read( file, shaderId );
		BinaryReadWrite::Read( file, shaderResources );
		BinaryReadWrite::IsAtEnd( file, hasOnlyShaderResources );
		//if ( !hasOnlyShaderResources )
		//	BinaryReadWrite::Read( file, descriptorSets );
		//else
		{
			BalVulkan::CBuffer* pUBO = m_pSystem->GetCurrentActiveScene()->GetModelBuffer();
			pUBO->Initialize( sizeof SModelObject, BalVulkan::EBufferUsageFlagBits::UniformBufferBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
			descriptorSets.emplace_back( BalVulkan::SDescriptorSet::EType::Buffer, pUBO );

			pUBO = m_pSystem->GetCurrentActiveScene()->GetShadingBuffer();
			pUBO->Initialize( sizeof SLightObject, BalVulkan::EBufferUsageFlagBits::UniformBufferBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit );
			descriptorSets.emplace_back( BalVulkan::SDescriptorSet::EType::Buffer, pUBO );
		}
		file.close();
		Balbino::CMaterial* pMaterial = new Balbino::CMaterial{ shaderId, g_pCommandPool };
		pMaterial->Initialize( GetShader( shaderId, true ), descriptorSets, g_pDevice );
		m_loadedMaterialMap[uuid] = pMaterial;
		return pMaterial;
	}
	file.close();
	return nullptr;
}

Balbino::IMesh* CResourceManager::LoadModel( std::string_view assetPath )
{
	std::ifstream file( assetPath.data(), std::ios::in | std::ios::binary );
	if ( !file.is_open() )
		return nullptr;

	uint64_t uuid;
	BinaryReadWrite::Read( file, uuid );
	if ( m_loadedMeshMap.contains( uuid ) )
		return m_loadedMeshMap.at( uuid );
	uint8_t type;
	BinaryReadWrite::Read( file, type );
	if ( static_cast< EFileTypes >( type ) == EFileTypes::Model )
	{
		uint64_t indicesSize;
		uint64_t verticesSize;
		uint64_t metadataSize;
		std::vector<uint32_t> indices;
		std::vector<BalVulkan::SVertex> vertices;
		std::vector<Balbino::SMeshMetadata> metadatas;
		BinaryReadWrite::Read( file, indicesSize );
		BinaryReadWrite::Read( file, verticesSize );
		BinaryReadWrite::Read( file, metadataSize );

		void* pData = malloc( indicesSize * sizeof uint32_t );
		BinaryReadWrite::Read( file, pData, indicesSize * sizeof( uint32_t ) );
		indices.assign( ( uint32_t* ) pData, ( uint32_t* ) pData + indicesSize );
		free( pData );

		pData = malloc( verticesSize * sizeof BalVulkan::SVertex );
		BinaryReadWrite::Read( file, pData, verticesSize * sizeof( BalVulkan::SVertex ) );
		vertices.assign( ( BalVulkan::SVertex* ) pData, ( BalVulkan::SVertex* ) pData + verticesSize );
		free( pData );

		pData = malloc( metadataSize * sizeof uint32_t );
		BinaryReadWrite::Read( file, pData, metadataSize * sizeof( Balbino::SMeshMetadata ) );
		metadatas.assign( ( Balbino::SMeshMetadata* ) pData, ( Balbino::SMeshMetadata* ) pData + metadataSize );
		free( pData );
		Balbino::CMesh* pMesh = new Balbino::CMesh{ vertices, indices, metadatas, uuid };
		pMesh->Initialize( g_pDevice, g_pCommandPool, g_pQueue );
		return m_loadedMeshMap[uuid] = pMesh;
	}
	file.close();

	return nullptr;
}

bool CResourceManager::BindMaterial( uint64_t id )
{
	if ( m_loadedMaterialMap.contains( id ) )
	{
		m_loadedMaterialMap[id]->Bind();
		return true;
	}
	if ( !m_loadedMaterialMap.empty() )
	{
		m_loadedMaterialMap.begin()->second->Bind();
		return true;
	}
	return false;
}

Balbino::CTexture* CResourceManager::GetTexture( CUuid getMeshId, bool tryToCreateWhenNotFound )
{
	if ( m_loadedTextureMap.contains( ( uint64_t ) getMeshId ) )
		return m_loadedTextureMap[( uint64_t ) getMeshId];
	if ( tryToCreateWhenNotFound )
	{
		FindAllFiles();
		return LoadTexture( std::find( m_files.begin(), m_files.end(), getMeshId )->path.string() );
	}
	return nullptr;
}

BalVulkan::CShaderPipeline* CResourceManager::GetShader( CUuid getMeshId, bool tryToCreateWhenNotFound )
{
	if ( m_loadedShaderMap.contains( ( uint64_t ) getMeshId ) )
		return m_loadedShaderMap[( uint64_t ) getMeshId];
	if ( tryToCreateWhenNotFound )
	{
		FindAllFiles();
		return LoadShader( std::find( m_files.begin(), m_files.end(), getMeshId )->path.string() );
	}
	return nullptr;
}

Balbino::CMaterial* CResourceManager::GetMaterial( CUuid getMeshId, bool tryToCreateWhenNotFound )
{
	if ( m_loadedMaterialMap.contains( ( uint64_t ) getMeshId ) )
		return m_loadedMaterialMap[( uint64_t ) getMeshId];
	if ( tryToCreateWhenNotFound )
	{
		FindAllFiles();
		return LoadMaterial( std::find( m_files.begin(), m_files.end(), getMeshId )->path.string() );
	}
	return nullptr;
}

Balbino::IMesh* CResourceManager::GetModel( CUuid getMeshId, bool tryToCreateWhenNotFound )
{
	if ( m_loadedMeshMap.contains( ( uint64_t ) getMeshId ) )
		return m_loadedMeshMap[( uint64_t ) getMeshId];
	if ( tryToCreateWhenNotFound )
	{
		FindAllFiles();
		return LoadModel( std::find( m_files.begin(), m_files.end(), getMeshId )->path.string() );
	}
	return nullptr;
}

const std::map<uint64_t, Balbino::CMaterial*>& CResourceManager::GetAllLoadedMaterials() const
{
	return m_loadedMaterialMap;
}

void CResourceManager::ReloadAll( BalVulkan::CCommandPool* commandPool, BalVulkan::CQueue* queue )
{
	g_pCommandPool = commandPool;
	g_pQueue = queue;
	for ( const auto& mesh : m_loadedMeshMap )
	{
		mesh.second->Cleanup();
		mesh.second->Initialize( g_pDevice, commandPool, queue );
	}
	for ( const auto& shader : m_loadedShaderMap )
	{
		
		shader.second->Release();
	}
	m_loadedShaderMap.clear();
	for ( const auto& material : m_loadedMaterialMap )
	{
		std::ifstream file( std::find( m_files.begin(), m_files.end(), material.first )->path.string(), std::ios::in | std::ios::binary );
		if ( !file.is_open() )
			continue;
		uint64_t uuid;
		uint8_t type;
		uint64_t shaderId;
		bool hasOnlyShaderResources;
		std::vector<BalVulkan::SShaderResource> shaderResources;
		std::vector<BalVulkan::SDescriptorSet> descriptorSets;
		BinaryReadWrite::Read( file, uuid );
		BinaryReadWrite::Read( file, type );
		BinaryReadWrite::Read( file, shaderId );
		BinaryReadWrite::Read( file, shaderResources );
		BinaryReadWrite::IsAtEnd( file, hasOnlyShaderResources );
		{
			BalVulkan::CBuffer* pUBO = m_pSystem->GetCurrentActiveScene()->GetModelBuffer();
			pUBO->Initialize( sizeof SModelObject, BalVulkan::EBufferUsageFlagBits::UniformBufferBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
			descriptorSets.emplace_back( BalVulkan::SDescriptorSet::EType::Buffer, pUBO );

			pUBO = m_pSystem->GetCurrentActiveScene()->GetShadingBuffer();
			pUBO->Initialize( sizeof SLightObject, BalVulkan::EBufferUsageFlagBits::UniformBufferBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
			descriptorSets.emplace_back( BalVulkan::SDescriptorSet::EType::Buffer, pUBO );
		}
		file.close();
		material.second->Cleanup();
		delete material.second;
		m_loadedMaterialMap[uuid] = new Balbino::CMaterial{shaderId, g_pCommandPool};
		m_loadedMaterialMap[uuid]->Initialize( GetShader( shaderId, true ), descriptorSets, g_pDevice );
	}
}

void CResourceManager::FindAllFiles()
{
	auto it = m_files.begin();
	while ( it != m_files.end() )
	{
		if ( !std::filesystem::exists( it->path ) )
		{
			//Remove
			free( it->pData );
			it = m_files.erase( it );
		}
		else
		{
			++it;
		}
	}

	for ( auto file = std::filesystem::recursive_directory_iterator( "..\\Data" );
		  file != std::filesystem::recursive_directory_iterator();
		  ++file )
	{
		if ( file->path().extension() != ".basset" )
			continue;
		auto currentFileLastWriteTime = std::filesystem::last_write_time( *file );

		// File creation
		auto filetIt = std::ranges::find_if( m_files, [&file]( const SFile& filePair )->bool
		{
			return filePair.path == file->path();
		} );
		if ( filetIt == m_files.end() )
		{
			//Create
			m_files.push_back( GetData( relative( file->path() ) ) );
			m_files.back().lastWrittenTime = currentFileLastWriteTime;
			m_files.back().depth = file.depth() + 1;
		}
		else
		{
			if ( filetIt->lastWrittenTime != currentFileLastWriteTime )
			{
				//Modifi
				( *filetIt ) = GetData( relative( file->path() ) );
				filetIt->lastWrittenTime = currentFileLastWriteTime;
				filetIt->depth = file.depth() + 1;
			}
		}
	}
}

SFile CResourceManager::GetData( const std::filesystem::path& path ) const
{
	SFile file{};
	std::ifstream fileStream( path, std::ios::in | std::ios::binary );
	if ( fileStream.is_open() )
	{
		uint8_t value;
		BinaryReadWrite::Read( fileStream, file.uuid );
		BinaryReadWrite::Read( fileStream, value );
		BinaryReadWrite::MoveCursorToEnd( fileStream );
		BinaryReadWrite::GetCursorPosition( fileStream, file.size );
		BinaryReadWrite::MoveCursorToStart( fileStream );

		file.type = static_cast< EFileTypes >( value );
		file.pData = malloc( file.size );
		BinaryReadWrite::GetData( fileStream, ( char* ) file.pData, file.size );
		file.path = path;
		file.fileName = path.filename().string();
		fileStream.close();
	}
	return file;
}
