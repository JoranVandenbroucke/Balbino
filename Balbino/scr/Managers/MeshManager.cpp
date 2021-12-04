#include "pch.h"
#include "MeshManager.h"

#include "../Renderer/MeshMetadata.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

void Balbino::CMeshManager::Initialize( const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue )
{
	for ( auto& mesh : m_meshes )
	{
		mesh.second.Initialize( pDevice, pCommandPool, pQueue );
	}
}

void Balbino::CMeshManager::Cleanup()
{
	for ( auto& mesh : m_meshes )
	{
		mesh.second.Cleanup();
	}
	m_meshes.clear();
}

void Balbino::CMeshManager::Draw( BalVulkan::CCommandPool* pCommandPool )
{
	for ( auto& mesh : m_meshes )
	{
		mesh.second.Draw(pCommandPool);
	}
}

Balbino::CMeshManager::CMeshManager()
= default;

Balbino::CMeshManager::~CMeshManager()
{
	if ( !m_meshes.empty() )
		std::cerr << "Texture Manager not cleared" << std::endl;
}

Balbino::CMesh* Balbino::CMeshManager::AddMesh( const char* filePath )
{
	const std::filesystem::path path( std::filesystem::absolute( filePath ) );
	const uint32_t hash = static_cast<uint32_t>( std::hash<std::string>{}( path.relative_path().string() ) );
	if ( m_meshes.find( hash ) != m_meshes.end() )
		return &m_meshes.at( hash );

	std::ifstream fileChecker{ path };
	if ( !fileChecker.is_open() )
	{
		throw std::runtime_error( path.string() + " does not exist" );
	}
	fileChecker.close();


	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( filePath, static_cast<unsigned>( aiProcess_Triangulate | aiProcess_RemoveRedundantMaterials | aiProcess_FlipUVs | aiProcess_GenBoundingBoxes | aiProcess_ValidateDataStructure ) );

	// If the import failed, report it
	if ( !scene )
	{
		throw importer.GetErrorString();
	}
	// Now we can access the file's contents.
	if ( !scene->HasMeshes() )
		throw std::runtime_error( "No mesh inside file" );

	const uint32_t nrOfMeshes{ scene->mNumMeshes };
	std::vector<BalVulkan::SVertex> vertices;
	std::vector<SMeshMetadata> metadata( nrOfMeshes );
	std::vector<uint32_t> indices;
	for ( uint32_t i{}; i < nrOfMeshes; ++i )
	{
		const aiMesh* pAiMesh{ scene->mMeshes[i] };
		if ( !pAiMesh->HasTextureCoords( 0 ) )
			importer.ApplyPostProcessing( aiProcess_GenUVCoords );
		if ( !pAiMesh->HasNormals() )
			importer.ApplyPostProcessing( aiProcess_GenNormals | aiProcess_FixInfacingNormals );
		if ( !pAiMesh->HasTangentsAndBitangents() )
			importer.ApplyPostProcessing( aiProcess_CalcTangentSpace );
		importer.ApplyPostProcessing( aiProcess_JoinIdenticalVertices );

		metadata[i].indexCount = pAiMesh->mNumFaces * 3;
		metadata[i].indexStart = ( uint32_t ) indices.size();
		metadata[i].materialId = i;
		const uint32_t prevSize{ ( uint32_t ) vertices.size() };
		vertices.resize( prevSize + pAiMesh->mNumVertices, BalVulkan::SVertex{ { 0, 0, 0 }, { 1, 1, 1, 1 }, { 0, 0 } } );
		indices.resize( metadata[i].indexStart + metadata[i].indexCount, 0 );
		for ( uint32_t j{}; j < pAiMesh->mNumVertices; ++j )
		{
			vertices[prevSize + j].position.x = pAiMesh->mVertices[j].x;
			vertices[prevSize + j].position.y = pAiMesh->mVertices[j].y;
			vertices[prevSize + j].position.z = pAiMesh->mVertices[j].z;

			if ( pAiMesh->HasVertexColors( 0 ) && pAiMesh->mColors[0] != nullptr )
			{
				vertices[prevSize + j].color.r = pAiMesh->mColors[0][j].r;
				vertices[prevSize + j].color.g = pAiMesh->mColors[0][j].g;
				vertices[prevSize + j].color.b = pAiMesh->mColors[0][j].b;
				vertices[prevSize + j].color.a = pAiMesh->mColors[0][j].a;
			}
			if ( pAiMesh->HasTextureCoords( 0 ) && pAiMesh->mTextureCoords[0] != nullptr )
			{
				vertices[prevSize + j].uv.x = pAiMesh->mTextureCoords[0][j].x;
				vertices[prevSize + j].uv.y = pAiMesh->mTextureCoords[0][j].y;
			}
			if ( pAiMesh->HasNormals() && pAiMesh->mNormals != nullptr )
			{
				vertices[prevSize + j].normal.x = pAiMesh->mNormals[j].x;
				vertices[prevSize + j].normal.y = pAiMesh->mNormals[j].y;
				vertices[prevSize + j].normal.z = pAiMesh->mNormals[j].z;
			}
			if ( pAiMesh->HasTangentsAndBitangents() && pAiMesh->mTangents != nullptr && pAiMesh->mBitangents != nullptr )
			{
				vertices[prevSize + j].tangent.x = pAiMesh->mTangents[j].x;
				vertices[prevSize + j].tangent.y = pAiMesh->mTangents[j].y;
				vertices[prevSize + j].tangent.z = pAiMesh->mTangents[j].z;

				vertices[prevSize + j].bitangent.x = pAiMesh->mBitangents[j].x;
				vertices[prevSize + j].bitangent.y = pAiMesh->mBitangents[j].y;
				vertices[prevSize + j].bitangent.z = pAiMesh->mBitangents[j].z;
			}
		}
		for ( uint32_t j{}; j < pAiMesh->mNumFaces; ++j )
		{
			indices[metadata[i].indexStart + j * 3 + 0] = ( prevSize + pAiMesh->mFaces[j].mIndices[0] );
			indices[metadata[i].indexStart + j * 3 + 1] = ( prevSize + pAiMesh->mFaces[j].mIndices[1] );
			indices[metadata[i].indexStart + j * 3 + 2] = ( prevSize + pAiMesh->mFaces[j].mIndices[2] );
		}
	}

	const auto& [fst, snd] = m_meshes.try_emplace( hash, vertices, indices, metadata );
	if ( !snd )
		throw std::runtime_error( "Could not emplace mesh" );
	return &fst->second;
}
