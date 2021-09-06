#include "pch.h"
#include "TextureManager.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "../Renderer/Renderer.h"
#include "../Renderer/Texture.h"

Balbino::CTexture* Balbino::CTextureManager::AddTexture(Balbino::CTexture* pTexture, const std::string& filePath)
{
	return GetInstance().IAddTexture(pTexture, filePath);
}

void Balbino::CTextureManager::Initialize( CRenderer* pRenderer )
{
	m_pRenderer = pRenderer;
}
void Balbino::CTextureManager::Cleanup()
{
	for ( const auto texture : m_textures )
	{
		delete texture.second;
	}
	m_textures.clear();

	m_pRenderer = nullptr;
}

Balbino::CTextureManager::CTextureManager()
	: m_pRenderer{ nullptr }
	, m_textures{}
{}

Balbino::CTextureManager::~CTextureManager()
{
	if (m_pRenderer != nullptr || !m_textures.empty())
		std::cerr << "Texture Manager not cleared" << std::endl;
};

Balbino::CTexture* Balbino::CTextureManager::IAddTexture(Balbino::CTexture* pTexture, const std::string& filePath)
{
	const std::filesystem::path path(std::filesystem::absolute(filePath));
	const uint32_t hash = static_cast<uint32_t>(std::hash<std::string>{}(path.relative_path().string()));
	if (m_textures.find(hash) != m_textures.end())
		return m_textures[hash];

	std::ifstream fileChecker{path};
	if (!fileChecker.is_open())
	{
		throw std::runtime_error(path.string() + " does not exist");
	}
	fileChecker.close();

	SDL_Surface* pSurface = IMG_Load(path.string().c_str());
	if (pTexture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
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

	pTexture->Initialize(pSurface, device, pCallbacks, physicalDevice, queue, commandPool);
	SDL_free(pSurface);
	return m_textures[hash] = pTexture;
}
