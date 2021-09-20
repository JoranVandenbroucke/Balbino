#include "pch.h"
#include "Application.h"

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include <chrono>
#include <iostream>
#include <vector>

#include "input/InputHandler.h"
#include "Managers/CameraManager.h"
#include "Managers/MeshManager.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"
#include "Renderer/Renderer.h"

#ifdef BL_EDITOR
#include <scr/Interface.h>
#endif


Balbino::Application::Application()
	: m_pWindow{nullptr}
	, m_manager{}
	, m_pRenderer{DBG_NEW CRenderer{}}
	, m_pInputHandler{DBG_NEW CInputHandler{}}
	, m_pCameraManager{DBG_NEW CCameraManager{}}
	, m_pTextureManager{DBG_NEW CTextureManager{}}
	, m_pShaderManager{DBG_NEW CShaderManager{}}
	, m_pMeshManager{ DBG_NEW CMeshManager {}}
#ifdef BL_EDITOR
	, m_pInterface{ DBG_NEW CInterface{} }
#endif
{
}

Balbino::Application::~Application()
{
}

void Balbino::Application::Initialize()
{
#ifdef BL_EDITOR
	m_pRenderer->SetInterface(m_pInterface);
#endif

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	SDL_DisplayMode current;
	if (SDL_GetDesktopDisplayMode(0, &current) != 0)
	{
		throw std::runtime_error(std::string("Could not get display mode for video display 0: ") + SDL_GetError());
	}

	constexpr SDL_WindowFlags flags{ SDL_WINDOW_VULKAN /*| SDL_WINDOW_FULLSCREEN*/ };
	m_pWindow = SDL_CreateWindow(
		"Balbino Editor",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		current.w / 2,
		current.h / 2,
		flags
	);
	if (m_pWindow == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	/*
	 * if you download Vulkan from https://vulkan.lunarg.com/home/welcome
	 * you get code for Open SDL
	 *
	 * this is code direct form it
	 */

	 // Get WSI extensions from SDL (we can add more if we like - we just can't remove these)
	unsigned int extenstionCount;
	if (!SDL_Vulkan_GetInstanceExtensions(m_pWindow, &extenstionCount, nullptr))
	{
		throw std::runtime_error(std::string("Could not get the number of required m_Instance extensions from SDL."));
	}
	const char** extensions{ DBG_NEW const char* [extenstionCount] };
	if (!SDL_Vulkan_GetInstanceExtensions(m_pWindow, &extenstionCount, extensions))
	{
		throw std::runtime_error(std::string("Could not get the names of required m_Instance extensions from SDL."));
	}

	m_pInputHandler->Initialize();
	m_manager.SetInputHandler(m_pInputHandler);
	m_manager.SetCameraManager(m_pCameraManager);
	m_manager.SetMeshManager(m_pMeshManager);
	m_manager.SetShaderManager(m_pShaderManager);
	m_manager.SetTextureManager(m_pTextureManager);
	m_pRenderer->Setup(m_pWindow, extensions, extenstionCount);
	m_pCameraManager->SetRenderer(m_pRenderer);
	delete[] extensions;
}

void Balbino::Application::LoadGame() const
{
	std::cout << "Load Engine\n";
	CManager::GetCameraManager()->AddCamera({0,1,0});
}

void Balbino::Application::Cleanup()
{
	m_pCameraManager->Cleanup();
	m_pInputHandler->Cleanup();
	m_pRenderer->Cleanup();

	delete m_pCameraManager;
	delete m_pInputHandler;
	delete m_pTextureManager;
	delete m_pShaderManager;
	delete m_pMeshManager;
	delete m_pRenderer;

#ifdef BL_EDITOR
	delete m_pInterface;
	m_pInterface = nullptr;
#endif

	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();

	m_pRenderer = nullptr;
	m_pWindow = nullptr;
}

void Balbino::Application::Run()
{
	LoadGame();
	bool isRunning{ true };
	auto start{ std::chrono::system_clock::now() };
	while (isRunning)
	{
		auto end{ std::chrono::system_clock::now() };
		const float deltaTime{ std::chrono::duration<float>(end - start).count() };
		start = end;

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
#ifdef BL_EDITOR
			m_pInterface->HandleEvents(e);
#endif

			switch (e.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;

			default:
				// Do nothing.
				break;
			}
			m_pInputHandler->ProcessEvents(e);
		}
		m_pCameraManager->Update(deltaTime);
		m_pInputHandler->Update();
		m_pRenderer->Draw(m_pWindow);
	}
	Cleanup();
}
