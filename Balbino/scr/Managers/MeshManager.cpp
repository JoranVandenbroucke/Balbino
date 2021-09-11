#include "pch.h"
#include "MeshManager.h"

#include <filesystem>
#include <fstream>

#include "../Renderer/Mesh.h"
#include "../Renderer/Renderer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Balbino::CMesh* Balbino::CMeshManager::AddMesh(CMesh* pTexture, const std::string& filePath)
{
	return GetInstance().IAddMesh(pTexture, filePath.c_str());
}

Balbino::CMesh* Balbino::CMeshManager::AddMesh(CMesh* pTexture, const char* filePath)
{
	return GetInstance().IAddMesh(pTexture, filePath);
}

void Balbino::CMeshManager::Initialize(CRenderer* pRenderer)
{
	SetRenderer(pRenderer);
}

void Balbino::CMeshManager::Cleanup(const VkDevice& device, const VkAllocationCallbacks* pAllocator)
{
	for (const auto mesh : m_meshes)
	{
		mesh.second->Cleanup(device, pAllocator);
		delete mesh.second;
	}
	m_meshes.clear();

	m_pRenderer = nullptr;
}

void Balbino::CMeshManager::SetRenderer(CRenderer* pRenderer)
{
	m_pRenderer = pRenderer;
}

Balbino::CMeshManager::CMeshManager()
	: m_pRenderer{nullptr}
{
}

Balbino::CMeshManager::~CMeshManager()
{
	if (m_pRenderer != nullptr || !m_meshes.empty())
		std::cerr << "Texture Manager not cleared" << std::endl;
}

Balbino::CMesh* Balbino::CMeshManager::IAddMesh(CMesh* pMesh, const char* filePath)
{
	const std::filesystem::path path(std::filesystem::absolute(filePath));
	const uint32_t hash = static_cast<uint32_t>(std::hash<std::string>{}(path.relative_path().string()));
	if (m_meshes.find(hash) != m_meshes.end())
		return m_meshes[hash];

	std::ifstream fileChecker{path};
	if (!fileChecker.is_open())
	{
		throw std::runtime_error(path.string() + " does not exist");
	}
	fileChecker.close();


	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(filePath,
		aiProcess_Triangulate );

	// If the import failed, report it
	if (!scene)
	{
		throw importer.GetErrorString();
	}
	// Now we can access the file's contents.
	if (!scene->HasMeshes())
		throw std::runtime_error("No mesh inside file");

	//todo: support multiple meshes
	const aiMesh* pAiMesh = scene->mMeshes[0];

	if (!pAiMesh->HasTextureCoords(0))
		importer.ApplyPostProcessing(aiProcess_GenUVCoords);
	if (!pAiMesh->HasNormals())
		importer.ApplyPostProcessing(aiProcess_GenNormals| aiProcess_FixInfacingNormals);
	if (!pAiMesh->HasTangentsAndBitangents())
		importer.ApplyPostProcessing(aiProcess_CalcTangentSpace);
	importer.ApplyPostProcessing(aiProcess_JoinIdenticalVertices);

	std::vector<SVertex>vertices(pAiMesh->mNumVertices, SVertex{{0,0,0},{1,1,1,1},{0,0}});
	for (uint32_t i{}; i < pAiMesh->mNumVertices; ++i)
	{
		vertices[i].position.x = pAiMesh->mVertices[i].x;
		vertices[i].position.y = pAiMesh->mVertices[i].y;
		vertices[i].position.z = pAiMesh->mVertices[i].z;

		if (pAiMesh->HasVertexColors(0) && pAiMesh->mColors[0] != nullptr)
		{
			vertices[i].colour.r = pAiMesh->mColors[0][i].r;
			vertices[i].colour.g = pAiMesh->mColors[0][i].g;
			vertices[i].colour.b = pAiMesh->mColors[0][i].b;
			vertices[i].colour.a = pAiMesh->mColors[0][i].a;
		}
		if (pAiMesh->HasTextureCoords(0) && pAiMesh->mTextureCoords[0] != nullptr)
		{
			vertices[i].uv.x = pAiMesh->mTextureCoords[0][i].x;
			vertices[i].uv.y = pAiMesh->mTextureCoords[0][i].y;
		}
		if (pAiMesh->HasNormals() && pAiMesh->mNormals != nullptr)
		{
			vertices[i].normal.x = pAiMesh->mNormals[i].x;
			vertices[i].normal.y = pAiMesh->mNormals[i].y;
			vertices[i].normal.z = pAiMesh->mNormals[i].z;
		}
		if (pAiMesh->HasTangentsAndBitangents() && pAiMesh->mTangents != nullptr && pAiMesh->mBitangents != nullptr)
		{
			vertices[i].tangent.x = pAiMesh->mTangents[i].x;
			vertices[i].tangent.y = pAiMesh->mTangents[i].y;
			vertices[i].tangent.z = pAiMesh->mTangents[i].z;

			vertices[i].binormal.x = pAiMesh->mBitangents[i].x;
			vertices[i].binormal.y = pAiMesh->mBitangents[i].y;
			vertices[i].binormal.z = pAiMesh->mBitangents[i].z;
		}
	}
	std::vector<uint16_t> indices(pAiMesh->mNumFaces * 3, 0);
	for (uint32_t i{}; i < pAiMesh->mNumFaces; ++i)
	{
		indices[i * 3 + 0] = (uint16_t) pAiMesh->mFaces[i].mIndices[0];
		indices[i * 3 + 1] = (uint16_t) pAiMesh->mFaces[i].mIndices[1];
		indices[i * 3 + 2] = (uint16_t) pAiMesh->mFaces[i].mIndices[2];
	}

	VkDevice device;
	VkAllocationCallbacks* pCallbacks{nullptr};
	VkPhysicalDevice physicalDevice;
	VkQueue queue;
	VkCommandPool commandPool;

	m_pRenderer->GetDevice(device);
	m_pRenderer->GetAllocationCallbacks(pCallbacks);
	m_pRenderer->GetPhysicalDevice(physicalDevice);
	m_pRenderer->GetQueue(queue);
	m_pRenderer->GetCommandPool(commandPool);
	pMesh->Initialize(device, commandPool, queue, physicalDevice, pCallbacks, vertices, indices);
	return pMesh;
}
