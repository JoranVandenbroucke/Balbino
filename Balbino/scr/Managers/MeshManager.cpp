#include "pch.h"
#include "MeshManager.h"

#include <filesystem>
#include <fstream>

#include "../Renderer/Renderer.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/MeshMetadata.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Balbino::CMeshManager::Initialize(CRenderer* pRenderer)
{
	SetRenderer(pRenderer);
}

void Balbino::CMeshManager::Cleanup(const VkDevice& device, const VkAllocationCallbacks* pAllocator)
{
	for (auto& mesh : m_meshes)
	{
		mesh.second.Cleanup(device, pAllocator);
	}
	m_meshes.clear();
	m_pRenderer = nullptr;
}

void Balbino::CMeshManager::SetRenderer(CRenderer* pRenderer)
{
	m_pRenderer = pRenderer;
}

void Balbino::CMeshManager::Draw(const VkCommandBuffer& commandBuffer, const VkDescriptorSet* descriptorSet)
{
	for (auto& mesh : m_meshes)
	{
		mesh.second.Draw(commandBuffer, descriptorSet);
	}
}

Balbino::CMeshManager::CMeshManager()
	: m_pRenderer{ nullptr }
{
}

Balbino::CMeshManager::~CMeshManager()
{
	if (m_pRenderer != nullptr || !m_meshes.empty())
		std::cerr << "Texture Manager not cleared" << std::endl;
}

Balbino::CMesh* Balbino::CMeshManager::AddMesh( const char* filePath)
{
	const std::filesystem::path path(std::filesystem::absolute(filePath));
	const uint32_t hash = static_cast<uint32_t>(std::hash<std::string>{}(path.relative_path().string()));
	if (m_meshes.find(hash) != m_meshes.end())
		return &m_meshes[hash];

	std::ifstream fileChecker{ path };
	if (!fileChecker.is_open())
	{
		throw std::runtime_error(path.string() + " does not exist");
	}
	fileChecker.close();


	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath,
		aiProcess_Triangulate);

	// If the import failed, report it
	if (!scene)
	{
		throw importer.GetErrorString();
	}
	// Now we can access the file's contents.
	if (!scene->HasMeshes())
		throw std::runtime_error("No mesh inside file");

	const uint32_t nrOfMeshes{scene->mNumMeshes };
	std::vector<SVertex> vertices;
	std::vector<SMeshMetadata> metadata(nrOfMeshes);
	std::vector<uint32_t> indices;
	for (uint32_t i{}; i < nrOfMeshes; ++i)
	{
		const aiMesh* pAiMesh{ scene->mMeshes[i] };
		if (!pAiMesh->HasTextureCoords(0))
			importer.ApplyPostProcessing(aiProcess_GenUVCoords);
		if (!pAiMesh->HasNormals())
			importer.ApplyPostProcessing(aiProcess_GenNormals | aiProcess_FixInfacingNormals);
		if (!pAiMesh->HasTangentsAndBitangents())
			importer.ApplyPostProcessing(aiProcess_CalcTangentSpace);
		importer.ApplyPostProcessing(aiProcess_JoinIdenticalVertices);

		metadata[i].indexCount = pAiMesh->mNumFaces * 3;
		metadata[i].indexStart = (uint32_t)indices.size();
		metadata[i].shaderId = i;
		const uint32_t prevSize {(uint32_t)vertices.size() };
		vertices.resize(prevSize + pAiMesh->mNumVertices, SVertex{ {0,0,0},{1,1,1,1},{0,0} });
		indices.resize(metadata[i].indexStart + metadata[i].indexCount, 0);
		for (uint32_t j{}; j < pAiMesh->mNumVertices; ++j) {
			vertices[prevSize+j].position.x = pAiMesh->mVertices[j].x;
			vertices[prevSize+j].position.y = pAiMesh->mVertices[j].y;
			vertices[prevSize+j].position.z = pAiMesh->mVertices[j].z;

			if (pAiMesh->HasVertexColors(0) && pAiMesh->mColors[0] != nullptr)
			{
				vertices[prevSize+j].colour.r = pAiMesh->mColors[0][j].r;
				vertices[prevSize+j].colour.g = pAiMesh->mColors[0][j].g;
				vertices[prevSize+j].colour.b = pAiMesh->mColors[0][j].b;
				vertices[prevSize+j].colour.a = pAiMesh->mColors[0][j].a;
			}
			if (pAiMesh->HasTextureCoords(0) && pAiMesh->mTextureCoords[0] != nullptr)
			{
				vertices[prevSize+j].uv.x = pAiMesh->mTextureCoords[0][j].x;
				vertices[prevSize+j].uv.y = pAiMesh->mTextureCoords[0][j].y;
			}
			if (pAiMesh->HasNormals() && pAiMesh->mNormals != nullptr)
			{
				vertices[prevSize+j].normal.x = pAiMesh->mNormals[j].x;
				vertices[prevSize+j].normal.y = pAiMesh->mNormals[j].y;
				vertices[prevSize+j].normal.z = pAiMesh->mNormals[j].z;
			}
			if (pAiMesh->HasTangentsAndBitangents() && pAiMesh->mTangents != nullptr && pAiMesh->mBitangents != nullptr)
			{
				vertices[prevSize+j].tangent.x = pAiMesh->mTangents[j].x;
				vertices[prevSize+j].tangent.y = pAiMesh->mTangents[j].y;
				vertices[prevSize+j].tangent.z = pAiMesh->mTangents[j].z;

				vertices[prevSize+j].binormal.x = pAiMesh->mBitangents[j].x;
				vertices[prevSize+j].binormal.y = pAiMesh->mBitangents[j].y;
				vertices[prevSize+j].binormal.z = pAiMesh->mBitangents[j].z;
			}
		}
		for (uint32_t j{}; j < pAiMesh->mNumFaces; ++j)
		{
			indices[metadata[i].indexStart + j * 3 + 0] = (prevSize + pAiMesh->mFaces[j].mIndices[0]);
			indices[metadata[i].indexStart + j * 3 + 1] = (prevSize + pAiMesh->mFaces[j].mIndices[1]);
			indices[metadata[i].indexStart + j * 3 + 2] = (prevSize + pAiMesh->mFaces[j].mIndices[2]);
		}
	}
	VkDevice device;
	VkAllocationCallbacks* pCallbacks{ nullptr };
	VkPhysicalDevice physicalDevice;
	VkQueue queue;
	VkCommandPool commandPool;

	m_pRenderer->GetDevice(device);
	m_pRenderer->GetAllocationCallbacks(pCallbacks);
	m_pRenderer->GetPhysicalDevice(physicalDevice);
	m_pRenderer->GetQueue(queue);
	m_pRenderer->GetCommandPool(commandPool);
	const auto& [fst, snd] = m_meshes.try_emplace(hash);
	if (!snd)
		throw std::runtime_error("Could not emplace mesh");
	fst->second.Initialize(device, commandPool, queue, physicalDevice, pCallbacks, vertices, indices, metadata);
	return &fst->second;
}
